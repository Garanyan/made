import requests
import time
import logging


logging.basicConfig(format='%(asctime)s %(message)s')
log = logging.getLogger()
log.setLevel(logging.INFO)


URLS = [
    'https://docs.python.org/3/_static/py.png',
    'https://www.python.org/static/img/psf-logo@2x.png'
]


def sync_scraper(urls=URLS):
    images = []
    start_time = time.monotonic()
    for url in urls:
        log.info('Starting download from %s', url)
        response = requests.get(url)
        if response.status_code == 200:
            log.info('Response received from %s, size %s', response.url, response.headers['Content-Length'])
            images.append(response.content)
        else:
            log.error('Bad %s, status %d', response.url, response.status_code)

    log.info('Downloaded in %d ms', (time.monotonic() - start_time) * 1000)
    return images


if __name__ == '__main__':
    sync_scraper()
