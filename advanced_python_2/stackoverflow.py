from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter, FileType
from collections import defaultdict
import io
import json
import logging
import sys
import string

DEFAULT_DATASET_PATH = "resources/wikipedia_sample"

logger = logging.getLogger("my_example")


class StackoverflowAnalyzer:
    def __init__(self, posts_xml):
        pass

    def parse_file(self, xml_filepath):
        pass

    def parse_xml_line(self, xml_line):
        pass


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
    build_parser.add_argument(
        "-o", "--output", default="inverted.index", dest="output",
        help="path to output to export Inverted Index",
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
    query_parser.add_argument(
        "--index", default="inverted.index",
        dest="index", help="path to file with inverted index",
    )
    query_parser.set_defaults(
        dataset=DEFAULT_DATASET_PATH,
        callback=process_query_arguments,
    )


def process_build_arguments(build_arguments):
    logger.debug("start loading documents...")
    documents = load_documents(build_arguments.dataset)
    logger.debug("loading documents is complete")
    logger.debug("start building inverted index...")
    inverted_index = build_inverted_index(documents)
    logger.debug("inverted index is ready")
    # TODO: add dump
    logger.debug("start dump to " + build_arguments.output)
    inverted_index.dump(build_arguments.output)


def process_query_arguments(query_arguments):
    inverted_index = InvertedIndex.load(query_arguments.index)
    for query_line in query_arguments.query_file:
        query = query_line.rstrip("\n").lower().split()
        document_ids = inverted_index.query(query)
        print(','.join(map(str, document_ids)))
        logger.debug("the answer to query %s is %s", query, document_ids)


def process_queries_old(dataset, query_file):
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
        print(f"{document_ids}")
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
