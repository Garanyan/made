from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter
from collections import defaultdict
import logging
import re
import json
import xml.etree.ElementTree as ET


# logger = logging.getLogger("analyzer")
# warn_logger = logging.getLogger("warn")


class StackoverflowAnalyzer:
    def __init__(self, posts_xml_file, stop_words_file, logger_all, logger_warn):
        self.logger = logger_all
        self.warn_logger = logger_warn

        self.stop_words = []
        self.year_to_words_map = defaultdict(lambda: defaultdict(int))
        self.load_stop_words(stop_words_file)
        self.parse_posts_file(posts_xml_file)

        self.logger.info("Analyzer inited.")
        self.logger.info('process XML dataset, ready to serve queries')

    def load_stop_words(self, stop_words_file):
        try:
            self.logger.info("Start load stop words from %s", stop_words_file)
            with open(stop_words_file, 'r', encoding="koi8-r") as file:
                self.stop_words = [line.rstrip() for line in file if line.rstrip()]
                self.logger.info("Finish load stop words.")
        except FileNotFoundError:
            self.logger.warning("There is no stop words file at %s", stop_words_file)
            self.warn_logger.warning("There is no stop words file at %s", stop_words_file)

    def parse_posts_file(self, xml_filepath):
        self.logger.info("process XML dataset")
        try:
            with open(xml_filepath, 'r', encoding="utf-8") as file:
                for line in file:
                    self.parse_xml_line(line)
                self.logger.info("Finish load posts.")
                for year in self.year_to_words_map:
                    self.year_to_words_map[year] = \
                        defaultdict(int, sorted(
                            self.year_to_words_map[year].items(),
                            reverse=True,
                            key=lambda item: item[1])
                                    )

        except FileNotFoundError:
            self.logger.warning("There is no xml file at %s", xml_filepath)
            self.warn_logger.warning("There is no xml file at %s", xml_filepath)

    def parse_xml_line(self, line):
        #self.logger.debug("parse line %s", line)
        root = ET.fromstring(line.rstrip())
        if root.attrib['PostTypeId'] == "1" and int(root.attrib['Score']) != 0:
            post_date = root.attrib['CreationDate']
            title_words = re.findall("\\w+", root.attrib['Title'].lower())  # ?
            for word in set(title_words):
                if word not in self.stop_words:
                    self.year_to_words_map[post_date[0:4]][word] += int(root.attrib['Score'])

    def process_queries(self, queries_file):
        with open(queries_file, 'r', encoding="utf-8") as file:
            for line in file:
                if line.rstrip():
                    start_year, end_year, top_n = line.rstrip().split(',')
                    self.logger.debug("got query \"%s\"", line.rstrip())
                    self.process_query(int(start_year), int(end_year), int(top_n))

        self.logger.info('finish processing queries')

    def process_query(self, start_year, end_year, top_n):
        top_words_candidate = defaultdict(int)
        for year in range(start_year, end_year + 1):
            for word, score in list(self.year_to_words_map[str(year)].items()):
                top_words_candidate[word] += score
        # local_top_n = min(top_n + 1, len(top_words_candidate))
        top_words = [[k, v]
                     for k, v in sorted(sorted(top_words_candidate.items(),

                                               key=lambda item: item[0]))
                     ]
        top_words = sorted(top_words, reverse=True, key=lambda item: item[1])
        top_words = top_words[:min(top_n, len(top_words_candidate))]
        #print(top_words)

        if len(top_words) < top_n:
            self.logger.warning(
                'not enough data to answer, found %s words out of %s for period "%s,%s"',
                len(top_words), top_n, start_year, end_year
            )
            self.warn_logger.warning(
                'not enough data to answer, found %s words out of %s for period "%s,%s"',
                len(top_words), top_n, start_year, end_year
            )
        self.logger.info('{"start": %s, "end": %s, "top": %s}', start_year, end_year, top_words)
        print(json.dumps({"start": start_year, "end": end_year, "top": top_words}))
        return {"start": start_year, "end": end_year, "top": top_words}
        # print(f'{{"start": {start_year}, "end": {end_year}, "top": {top_words}}}')


def setup_parser(parser, logger, warn_logger):
    """
    python3 analyze_stackoverflow.py \
            --questions /path/to/dataset/questions.xml \
            --stop-words /path/to/stop_words_in_koi8r.txt \
            --queries /path/to/quries.csv

    """

    parser.add_argument(
        "--questions", default="resources/small_posts.xml", dest="questions",
        help="path to Stackoverflow database",
    )

    parser.add_argument(
        "--stop-words", default="resources/small_stop_words.txt", dest="stop",
        help="path to stop words. which will be excluded",
    )

    parser.add_argument(
        "--queries", default="resources/queries.csv", dest="query_file",
        help="collection of queries to run against Stackoverflow [start_year,end_year,top_N]",
    )

    parser.set_defaults(
        logger_all=logger,
        logger_warn=warn_logger,
        callback=process_query_arguments,
    )


def process_query_arguments(query_arguments):
    analyzer = StackoverflowAnalyzer(query_arguments.questions,
                                     query_arguments.stop,
                                     logger_all=query_arguments.logger_all,
                                     logger_warn=query_arguments.logger_warn)
    analyzer.process_queries(query_arguments.query_file)


def setup_logger(logger_name, log_file, level=logging.INFO):
    log_setup = logging.getLogger(logger_name)
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s',
                                  datefmt='%Y-%m-%d %H:%M:%S')
    fileHandler = logging.FileHandler(log_file, mode='a')
    fileHandler.setFormatter(formatter)
    streamHandler = logging.StreamHandler()
    streamHandler.setFormatter(formatter)
    log_setup.setLevel(level)
    log_setup.addHandler(fileHandler)
    # log_setup.addHandler(streamHandler)

    return log_setup


def setup_logging():
    logger = setup_logger("log", "analyze_stackoverflow.log", logging.DEBUG)
    warn_logger = setup_logger("warn", "analyze_stackoverflow.warn", logging.WARNING)
    # logging.basicConfig(
    #     filename='analyze_stackoverflow.log', level=logging.DEBUG,
    #     format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
    #     datefmt="%Y-%m-%d %H:%M:%S",
    # )
    return logger, warn_logger


def main():
    """It is Python CLI interface for Inverted Index Application

    If you don't understand how argparse work see documentation via:
    - https://...
    see: Markdown
    """
    # print(sys.argv)
    logger, warn_logger = setup_logging()

    parser = ArgumentParser(
        prog="inverted-index",
        description="Inverted Index Application: build, query, dump and load",
        formatter_class=ArgumentDefaultsHelpFormatter,
    )
    setup_parser(parser, logger, warn_logger)

    arguments = parser.parse_args()
    logger.info("Application called with arguments %s", arguments)

    arguments.callback(arguments)


if __name__ == "__main__":
    main()
