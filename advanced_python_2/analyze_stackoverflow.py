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
    python3 analyze_stackoverflow.py \
            --questions /path/to/dataset/questions.xml \
            --stop-words /path/to/stop_words_in_koi8r.txt \
            --queries /path/to/quries.csv

    """

    parser.add_argument(
        "--questions", default="/path/to/dataset/questions.xml", dest="questions",
        help="path to stackowerflow database",
    )

    parser.add_argument(
        "--stop-words", default="/path/to/stop_words_in_koi8r.txt", dest="stop",
        help="path to stop words. which will be excluded",
    )

    parser.add_argument(
        "--queries", type=FileType("r", encoding="koi8-r"),
        default=io.TextIOWrapper(sys.stdin.buffer, encoding="koi8-r"),
        dest="query_file", help="collection of queries to run against Inverted Index",
    )

    parser.set_defaults(
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
