import asyncio
import logging
import time

import aiohttp


logging.basicConfig(format='%(asctime)s %(message)s')
log = logging.getLogger()
log.setLevel(logging.INFO)


URLS = [
    'https://docs.python.org/3/_static/py.png',
    'https://www.python.org/static/img/psf-logo@2x.png'
]


async def fetch(session, url):
    log.info('Starting download from %s', url)

    content = None
    async with session.get(url) as response:
        if response.status == 200:
            log.info('Response received from %s, size %s', response.url, response.headers['Content-Length'])
        else:
            log.error('Bad %s, status %d', response.url, response.status)
        content = await response.read()

    return content


async def async_scraper_simple(urls=URLS):
    images = []
    start_time = time.monotonic()
    async with aiohttp.ClientSession() as session:
        tasks = []
        for url in urls:
            tasks.append(asyncio.create_task(fetch(session, url)))
        images = await asyncio.gather(*tasks)

    log.info('Downloaded in %d ms', (time.monotonic() - start_time) * 1000)
    return images


if __name__ == '__main__':
    asyncio.run(async_scraper_simple())