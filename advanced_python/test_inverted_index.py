import logging
from argparse import Namespace
from textwrap import dedent

import pytest
from unittest.mock import patch, call, Mock
import time

import inverted_index
from inverted_index import process_query_arguments, load_documents, build_inverted_index, InvertedIndex, JsonZipStoragePolicy, JsonStoragePolicy, do_busy_work, \
    do_busy_work_full_import

DATASET_BIG_FPATH = "resources/wikipedia_sample"
DATASET_SMALL_FPATH = "resources/wikipedia_sample_small"


def test_can_query(tmpdir):
    dataset_fio = tmpdir.join("dataset.txt")
    dataset_fio.write(
        dedent("""\
        1\thappy cat wow
        2\thappy cat good
        3\tgood cat audi
        4\t audi and bmw
        """)
    )
    documents = load_documents(dataset_fio)
    inverted_index = build_inverted_index(documents)
    document_ids = inverted_index.query(["happy", "good"])
    assert document_ids == [2]
    assert inverted_index.query(["happy", "good", "cat"]) == [2]
    assert inverted_index.query(["cat"]) == [1, 2, 3]
    assert inverted_index.query(["cat", "audi"]) == [3]
    assert inverted_index.query(["cat", "audi", 'cat']) == [3]
    assert inverted_index.query(["cat", "audi", 'audi']) == [3]
    assert inverted_index.query(["audi", 'bmw']) == [4]
    assert inverted_index.query(["audi", 'bmw', 'cat']) == list()


def test_can_load_documents(tmpdir):
    dataset_fio = tmpdir.join("dataset.txt")
    dataset_fio.write(dedent("""\
        1\thappy cat
        2\thappy cat good
        3\tgood cat
    """))
    documents = load_documents(dataset_fio)
    etalon_documents = {
        1: "happy cat",
        2: "happy cat good",
        3: "good cat",
    }
    assert etalon_documents == documents, (
        "load_documents incorrectly loaded dataset"
    )


@pytest.fixture
def wikipedia_documents():
    documents = load_documents("resources/wikipedia_sample")
    return documents


@pytest.fixture
def small_sample_wikipedia_documents():
    documents = load_documents("resources/wikipedia_sample_small")
    return documents


def test_can_load_wikipedia_sample(wikipedia_documents):
    assert len(wikipedia_documents) == 4100, (
        "you incorrectly loaded Wikipedia sample"
    )


def test_can_build_and_query_inverted_index(wikipedia_documents):
    wikipedia_inverted_index = build_inverted_index(wikipedia_documents)
    doc_ids = wikipedia_inverted_index.query(["wikipedia"])
    assert isinstance(doc_ids, list), "inverted index query should return list"


@pytest.fixture
def wikipedia_inverted_index(wikipedia_documents):
    wikipedia_inverted_index = build_inverted_index(wikipedia_documents)
    return wikipedia_inverted_index


@pytest.fixture
def small_wikipedia_inverted_index(small_sample_wikipedia_documents):
    wikipedia_inverted_index = build_inverted_index(small_sample_wikipedia_documents)
    return wikipedia_inverted_index


def test_can_dump_and_load_inverted_index(tmpdir, wikipedia_inverted_index):
    index_fio = tmpdir.join("index.dump")
    wikipedia_inverted_index.dump(index_fio)
    loaded_inverted_index = InvertedIndex.load(index_fio)
    assert wikipedia_inverted_index == loaded_inverted_index, (
        "load should return the same inverted index"
    )


def test_check_compression_good():
    json_inverted_index = InvertedIndex.load("inverted_index/inverted_json.index", JsonStoragePolicy())
    compressed_inverted_index = InvertedIndex.load("inverted_index/inverted_json_zip.index", JsonZipStoragePolicy())
    assert json_inverted_index.query(["two", "words"]) == compressed_inverted_index.query(
        ["two", "words"]), "compressin give another answer"

    assert json_inverted_index.get_size() == compressed_inverted_index.get_size(), "compressed file has diff num of records"


@pytest.mark.xfail
def test_can_dump_and_load_inverted_index_with_array_policy(tmpdir, small_wikipedia_inverted_index):
    # do some magic here
    pass


@pytest.mark.skip
def test_can_dump_and_load_big_inverted_index_with_array_policy(tmpdir, wikipedia_inverted_index):
    # do some magic here
    pass


# @pytest.mark.parametrize(
#     ("filepath",),
#     [
#         ("resources/wikipedia.sample",),
#         ("resources/small_wikipedia.sample",),
#     ],
#     ids=["small dataset", "big dataset"],
# )
# def test_can_dump_and_load_inverted_index_with_array_policy_parametrized(filepath, tmpdir):
#     # some code here
#
#     inverted_index.dump(index_fio, storage_policy=ArrayStoragePolicy)
#     InvertedIndex.load(index_fio, storage_policy=ArrayStoragePolicy)
#
#     assert etalon_inverted_index == loaded_inverted_index, (
#         "load should return the same inverted index"
#     )

@pytest.mark.parametrize(
    "dataset_fpath",
    [
        pytest.param(DATASET_BIG_FPATH, marks=[pytest.mark.slow], id="big test suite"),
        pytest.param(DATASET_SMALL_FPATH, id="small test suite"),
    ],
)
def test_process_query_arguments_print_to_stdout(capsys, caplog, dataset_fpath):
    caplog.set_level("DEBUG")
    with open("resources/queries.txt") as queries_fin:
        query_arguments = Namespace(
            dataset=dataset_fpath,
            query_file=queries_fin,
        )
        process_query_arguments(query_arguments)
        captured = capsys.readouterr()

        # from pdb import set_trace; set_trace()

        assert "start loading" not in captured.out
        assert "start loading" in caplog.text

        assert any("start loading" in log_message for log_message in caplog.messages)

        for log_record in caplog.records:
            assert log_record.levelno < logging._nameToLevel["WARNING"], "TBD"

        assert "['two', 'words']" in captured.out
        assert "['two', 'words']" not in captured.err
        assert ("my_example", 20, "the answer to query ['even', 'more', 'words'] is []") in caplog.record_tuples


from inverted_index import sleep

@patch('time.sleep')
def test_sleep_1(sleep_mock):
    do_busy_work()

@patch('inverted_index.sleep')
def test_sleep_2(full_sleep_mock):
    do_busy_work_full_import(5)

@patch()
def test_do_busy_work_nested(full_sleep_mock, sleep_mock):
    do_busy_work_nested()
    assert full_sleep_mock.call_count == 1
    assert sleep_mock.mock.call_count == 1

    assert full_sleep_mock.call_args == call(5)
    assert sleep_mock.call_args == call(4)


