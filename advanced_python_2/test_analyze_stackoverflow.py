# import logging
# from argparse import Namespace
from textwrap import dedent

import pytest
# from unittest.mock import patch, call, Mock
from analyze_stackoverflow import StackoverflowAnalyzer, setup_logging


def test_stop_words_correctness(small_stop_words, get_loggers):
    analyzer = StackoverflowAnalyzer("", small_stop_words, *get_loggers)
    assert analyzer.stop_words == ['is', 'than']


def test_stop_words_correctness_with_empty_line(tmpdir, get_loggers):
    another_stop_words_file = tmpdir.join("dataset.txt")
    another_stop_words_file.write(
        dedent("""
        is
        \n
        than
        \n
        a
            """)
    )
    analyzer = StackoverflowAnalyzer("", another_stop_words_file, *get_loggers)
    assert set(analyzer.stop_words) == {'a', 'is', 'than'}


def test_stop_words_correctness_empty_file(tmpdir, get_loggers):
    stop_words_file = tmpdir.join("dataset.txt")
    stop_words_file.write(
        dedent("""
        \n
        \n
            """)
    )
    analyzer = StackoverflowAnalyzer("", stop_words_file, *get_loggers)
    assert analyzer.stop_words == []


def test_stop_words_encoding(tmpdir, get_loggers):
    stop_words_file_2 = tmpdir.join("dataset.txt", encoding="koi8-r")
    stop_words_file_2.write(
        dedent("""
            \n
            is
            \n
            the
            at
            a
            but
                """)
    )
    analyzer = StackoverflowAnalyzer("", stop_words_file_2, *get_loggers)
    assert analyzer.stop_words == ["is", 'the', 'at', 'a', 'but']


def test_posts_parse_correctness(small_posts, small_stop_words, get_loggers):
    analyzer = StackoverflowAnalyzer(small_posts, small_stop_words, *get_loggers)
    posts_dict = analyzer.year_to_words_map

    assert len(posts_dict) == 2
    assert posts_dict.get('2025') is not None
    assert len(posts_dict['2025']) == 6
    assert posts_dict.get('2026') is not None
    assert len(posts_dict['2026']) == 3

    assert posts_dict.get('2010') is None
    assert posts_dict.get('20') is None
    assert posts_dict.get('2018') is None


def test_posts_words_score_correctness(small_posts, small_stop_words, get_loggers):
    analyzer = StackoverflowAnalyzer(small_posts, small_stop_words, *get_loggers)
    posts_dict = analyzer.year_to_words_map

    assert posts_dict['2025']['seo'] == 15
    assert posts_dict['2025']['better'] == 10
    assert posts_dict['2025']['done'] == 10
    assert posts_dict['2025']['with'] == 10
    assert posts_dict['2025']['repetition'] == 10
    assert posts_dict['2025']['what'] == 5

    assert posts_dict['2026']['better'] == 20
    assert posts_dict['2026']['python'] == 20
    assert posts_dict['2026']['javascript'] == 20


@pytest.fixture
def small_stop_words(tmpdir):
    stop_words_file = tmpdir.join("stop_words.txt")
    stop_words_file.write(
        dedent("""
            is
            than
                """)
    )
    return stop_words_file


@pytest.fixture
def small_posts(tmpdir):
    stackoverflow_posts_file = tmpdir.join("sample.xml")
    stackoverflow_posts_file.write(
        """<row Id="1" PostTypeId="1"  CreationDate="2025-11-15T20:09:58.970" Score="10" ViewCount="100" Body="I would like to declare a variable 'XMLOutput'" OwnerUserId="508703" LastEditorUserId="13302" LastEditDate="2010-11-15T20:43:01.363" LastActivityDate="2013-10-18T07:48:48.510" Title="Is SEO better better better done with repetition?" FavoriteCount="2" />
        <row Id="2" PostTypeId="1"  CreationDate="2025-11-15T20:09:58.970" Score="5" ViewCount="105" Body=" declare " OwnerUserId="508703" LastEditorUserId="13302" LastEditDate="2010-11-15T20:43:01.363" LastActivityDate="2013-10-18T07:48:48.510" Title="What is SEO?"  FavoriteCount="2" />
        <row Id="3" PostTypeId="1" CreationDate="2026-11-15T20:09:58.970" Score="20" ViewCount="13" Body="variable 'XMLOutput'" OwnerUserId="508703" LastEditorUserId="13302" LastEditDate="2010-11-15T20:43:01.363" LastActivityDate="2013-10-18T07:48:48.510" Title="Is Python better than Javascript?" FavoriteCount="2" />"""
    )
    return stackoverflow_posts_file


@pytest.fixture
def small_query_file(tmpdir):
    file = tmpdir.join("queries.txt")
    file.write("2025,2025,2\n2025,2026,4")
    return file


@pytest.fixture
def dublicate_posts(tmpdir):
    stackoverflow_posts_file = tmpdir.join("sample.xml")
    stackoverflow_posts_file.write(
        """<row Id="1" PostTypeId="1"  CreationDate="2020-11-15T20:09:58.970" Score="7"  Title="Data data data Data, data?" FavoriteCount="2" />
        <row Id="2" PostTypeId="1"  CreationDate="2021-11-15T20:09:58.970" Score="5"    Title="What is your data?"  FavoriteCount="2" />
        <row Id="3" PostTypeId="1" CreationDate="2022-11-15T20:09:58.970" Score="8" Title="Is Python better than Data Sicence?" FavoriteCount="2" />
        <row Id="4" PostTypeId="1"  CreationDate="2023-11-15T20:09:58.970" Score="5"    Title="is data?"  FavoriteCount="2" />
        <row Id="5" PostTypeId="1"  CreationDate="2023-11-15T20:09:58.970" Score="4"    Title="is office?"  FavoriteCount="2" />
        <row Id="6" PostTypeId="1"  CreationDate="2023-11-15T20:09:58.970" Score="3"    Title="is SQL?"  FavoriteCount="2" />
        <row Id="7" PostTypeId="1"  CreationDate="2023-11-15T20:09:58.970" Score="1"    Title="is Java?"  FavoriteCount="2" />
        <row Id="8" PostTypeId="1"  CreationDate="2024-11-15T20:09:58.970" Score="5"    Title="Java?"  FavoriteCount="2" />
        <row Id="9" PostTypeId="1"  CreationDate="2024-11-15T20:09:58.970" Score="4"    Title="SQL?"  FavoriteCount="2" />
        <row Id="10" PostTypeId="1"  CreationDate="2025-11-15T20:09:58.970" Score="1"    Title="JAVA!!"  FavoriteCount="2" />"""
    )
    return stackoverflow_posts_file


@pytest.fixture
def get_loggers(tmpdir):
    return setup_logging()


def test_query_correctness(dublicate_posts, small_stop_words, get_loggers):
    analyzer = StackoverflowAnalyzer(dublicate_posts, small_stop_words, *get_loggers)
    json_result = analyzer.process_query(2020, 2022, 4)
    assert json_result['top'][0][0] == 'data'
    assert json_result['top'][0][1] == 20


def test_query_subtop_to_top(dublicate_posts, small_stop_words, get_loggers):
    analyzer = StackoverflowAnalyzer(dublicate_posts, small_stop_words, *get_loggers)
    json_result = analyzer.process_query(2023, 2024, 2)
    assert json_result['top'][0][0] == 'sql'
    assert json_result['top'][0][1] == 7
    assert json_result['top'][1][0] == 'java'
    assert json_result['top'][1][1] == 6


def test_query_sorted(dublicate_posts, small_stop_words, get_loggers):
    analyzer = StackoverflowAnalyzer(dublicate_posts, small_stop_words, *get_loggers)
    json_result = analyzer.process_query(2023, 2025, 1)
    assert json_result['top'][0][0] == 'java'
    assert json_result['top'][0][1] == 7


def test_query_small_dataset(small_posts, small_stop_words, get_loggers):
    analyzer = StackoverflowAnalyzer(small_posts, small_stop_words, *get_loggers)
    json_result = analyzer.process_query(2025, 2025, 2)
    assert json_result['top'][0][0] == 'seo'
    assert json_result['top'][0][1] == 15
    assert json_result['top'][1][0] == 'better'
    assert json_result['top'][1][1] == 10

    json_result = analyzer.process_query(2025, 2026, 4)
    assert json_result['top'][0][0] == 'better'
    assert json_result['top'][0][1] == 30
    assert json_result['top'][1][0] == 'javascript'
    assert json_result['top'][1][1] == 20
    assert json_result['top'][2][0] == 'python'
    assert json_result['top'][2][1] == 20
    assert json_result['top'][3][0] == 'seo'
    assert json_result['top'][3][1] == 15


def test_no_exeption_in_constructor(get_loggers):
    try:
        StackoverflowAnalyzer("", "", *get_loggers)
    except:
        pytest.fail("Unexpected Error ..")


def test_no_exeption_in_parse_posts_file(get_loggers):
    try:
        analyzer = StackoverflowAnalyzer("", "", *get_loggers)
        analyzer.parse_posts_file("somepath")
    except:
        pytest.fail("Unexpected Error ..")


def test_load_stop_words(small_stop_words, get_loggers):
    analyzer = StackoverflowAnalyzer("", "", *get_loggers)
    analyzer.load_stop_words(small_stop_words)
    assert len(analyzer.stop_words) == 2


def test_logger(caplog, small_posts, small_stop_words, small_query_file, get_loggers):
    analyzer = StackoverflowAnalyzer(small_posts, small_stop_words, *get_loggers)
    analyzer.process_queries(small_query_file)
    caplog.set_level("DEBUG")

    assert "got query" in caplog.text
    assert "got query \"2025,2025,2\"" in caplog.text
    assert "got query \"2025,2025,2\"" in caplog.text
    assert 'process XML dataset' in caplog.text
    assert 'ready to serve queries' in caplog.text
    assert 'finish processing queries' in caplog.text
    assert 'process XML dataset' in caplog.text


def test_output(capsys, small_posts, small_stop_words, small_query_file, get_loggers):
    analyzer = StackoverflowAnalyzer(small_posts, small_stop_words, *get_loggers)
    analyzer.process_queries(small_query_file)

    captured = capsys.readouterr()
    assert '"top": [["better", 30], ["javascript", 20], ["python", 20], ["seo", 15]]' in captured.out
    assert '"start": 2025, "end": 2025, "top": [["seo", 15], ["better", 10]]' in captured.out
    assert captured.out.count('\n') == 2
