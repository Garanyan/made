from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter, FileType
from collections import defaultdict
import io
import json
import logging
import sys
import string

DEFAULT_DATASET_PATH = "resources/wikipedia_sample"

# logger = logging.getLogger(__name__)
logger = logging.getLogger("my_example")


class InvertedIndex:
    def __init__(self, word_to_docs_mapping):
        # make a high-level copy
        self.word_to_docs_mapping = {
            word: doc_ids
            for word, doc_ids in word_to_docs_mapping.items()
        }

    def query(self, words):
        # список документов в которых есть все указанные слова
        result = set()

        for word in words:
            if word in self.word_to_docs_mapping.keys():
                if not result:
                    result = self.word_to_docs_mapping[word]
                else:
                    result = result.intersection(self.word_to_docs_mapping[word])

        return list(result)

    def dump(self, filepath, storage_policy=None):
        storage_policy = storage_policy or JsonStoragePolicy()
        assert isinstance(storage_policy, StoragePolicy), (
            "you provided wrong argument ..."
        )

        with open(filepath, "w") as fout:
            storage_policy.dump(self.word_to_docs_mapping, fout)

    def __eq__(self, other):
        return self.word_to_docs_mapping == other.word_to_docs_mapping

    @classmethod
    def load(cls, filepath, storage_policy=None):
        storage_policy = storage_policy or JsonStoragePolicy()
        assert isinstance(storage_policy, StoragePolicy), (
            "you provided wrong argument ..."
        )
        word_to_docs_mapping = {}
        with open(filepath, 'r') as f:
            word_to_docs_mapping = storage_policy.load(f)
        return cls(word_to_docs_mapping)


class StoragePolicy:
    def dump(self, word_to_docs_mapping, index_fio):
        pass

    def load(self, index_fio):
        pass


class JsonStoragePolicy(StoragePolicy):
    def dump(self, word_to_docs_mapping, index_fio):
        serializable_word_to_docs_mapping = {
            word: list(doc_ids)
            for word, doc_ids in word_to_docs_mapping.items()
        }
        dump = json.dumps(serializable_word_to_docs_mapping)
        index_fio.write(dump)

    def load(self, index_fio):
        json_load = json.load(index_fio)
        word_to_docs_mapping = {
            word: set(json_load[word])
            for word in json_load
        }
        return word_to_docs_mapping


def load_documents(filepath):
    documents = {}
    with open(filepath, encoding='utf-8') as fin:
        for line in fin:
            line = line.rstrip("\n")
            if line:
                doc_id, content = line.split("\t", 1)
                documents[int(doc_id)] = content
    return documents


def build_inverted_index(documents):
    word_to_docs_mapping = defaultdict(set)
    for doc_id, content in documents.items():
        content.translate(str.maketrans('', '', string.punctuation))
        words = content.split()
        for word in words:
            word_to_docs_mapping[word.lower()].add(int(doc_id))
    return InvertedIndex(word_to_docs_mapping)


def setup_parser(parser):
    """
    parser.add_argument(
        "-d", "--dataset", default=DEFAULT_DATASET_PATH, dest="dataset",
        help="path to dataset to build Inverted Index",
        # "-d", "--dataset", required=True, dest="dataset",
        # "-i", "--input", required=True, dest="dataset",
        # metavar="FANCY_PATH",
        # help="path to dataset to build Inverted Index, default path is %(default)s",
    )
    parser.add_argument(
        "-q", "--query", nargs="+", required=True,
        help="list words to look for documents",
    )
    """
    subparsers = parser.add_subparsers(help="choose command to run")

    build_parser = subparsers.add_parser(
        "build", help="load dataset, build II and save to file",
        formatter_class=ArgumentDefaultsHelpFormatter,
    )
    build_parser.add_argument(
        "-d", "--dataset", default=DEFAULT_DATASET_PATH, dest="dataset",
        help="path to dataset to build Inverted Index",
    )
    build_parser.set_defaults(callback=process_build_arguments)

    query_parser = subparsers.add_parser("query", help="query Inverted Index")
    query_group = query_parser.add_mutually_exclusive_group(required=False)
    query_group.add_argument(
        "--query-file-cp1251", type=FileType("r", encoding="cp1251"),
        # default=open("../resources/queries.txt"),
        # default=sys.stdin,
        default=io.TextIOWrapper(sys.stdin.buffer, encoding="cp1251"),
        dest="query_file", help="collection of queries to run against Inverted Index",
    )
    query_group.add_argument(
        "--query-file-utf8", type=FileType("r", encoding="utf-8"),
        default=sys.stdin,
        # default=open("../resources/queries.txt"),
        dest="query_file", help="collection of queries to run against Inverted Index",
    )
    query_parser.set_defaults(
        dataset=DEFAULT_DATASET_PATH,
        callback=process_query_arguments,
    )


def process_build_arguments(build_arguments):
    print("start loading documents...")
    documents = load_documents(build_arguments.dataset)
    print("loading documents is complete")
    print("start building inverted index...")
    inverted_index = build_inverted_index(documents)
    print("inverted index is ready for queries")
    # TODO: add dump
    inverted_index.dump("inverted.index")


def process_query_arguments(query_arguments):
    return process_queries(query_arguments.dataset, query_arguments.query_file)


def process_queries(dataset, query_file):
    # inverted_index = InvertedIndex.load("inverted.index")
    # document_ids = inverted_index.query(["two", "words"])
    # print(f"the answer is {document_ids}")

    # print("start loading documents...", file=sys.stderr)
    logger.info("start loading documents...")
    documents = load_documents(dataset)
    logger.info("loading documents is complete")
    logger.info("start building inverted index...")
    inverted_index = build_inverted_index(documents)
    logger.info("inverted index is ready for queries")

    for query_line in query_file:
        query = query_line.rstrip("\n").lower().split()
        document_ids = inverted_index.query(query)
        print(f"the answer to query {query} is {document_ids}")
        logger.debug("the answer to query %s is %s", query, document_ids)


def setup_logging(arguments):
    logging.basicConfig(
        filename='inverted_index_class.log', level=logging.DEBUG,
        format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S",
    )


def main():
    """It is Python CLI interface for Inverted Index Application

    If you don't understand how argparse work see documentation via:
    - https://...
    see: Markdown
    """
    # print(sys.argv)
    parser = ArgumentParser(
        prog="inverted-index",
        description="Inverted Index Application: build, query, dump and load",
        formatter_class=ArgumentDefaultsHelpFormatter,
    )
    setup_parser(parser)

    arguments = parser.parse_args()
    setup_logging(arguments)
    logger.info("Application called with arguments %s", arguments)

    arguments.callback(arguments)


if __name__ == "__main__":
    main()
