import asyncio
import base64
import logging
import time
from collections import defaultdict

import aiohttp
from aiohttp import web


logging.basicConfig(format='%(asctime)s %(message)s')
log = logging.getLogger()
log.setLevel(logging.INFO)


async def fetch(session, url):
    log.info('Starting download from %s', url)

    content = None
    async with session.get(url) as response:
        if response.status == 200:
            log.info('Response received from %s, size %s', response.url, response.headers.get('Content-Length'))
        else:
            log.error('Bad %s, status %d', response.url, response.status)
        content = await response.read()

    return content


async def async_scraper_simple(context, username, urls):
    images = []
    async with aiohttp.ClientSession() as session:
        tasks = []
        for url in urls:
            tasks.append(asyncio.create_task(fetch(session, url)))

        try:
            context[username].extend(tasks)
            images = await asyncio.gather(*tasks)
            del context[username]
        except asyncio.CancelledError:
            log.info('Tasks were cancelled for %s', username)

    return images


async def handler_for_staff(request):
    start_time = time.monotonic()

    username = request.match_info['username']
    stuff = await request.json()
    context = request.app['context']
    images = await async_scraper_simple(context, username, stuff['urls'])

    log.info('Downloaded %d images for %s in %d ms', len(images), username, (time.monotonic() - start_time) * 1000)

    return web.json_response({'images': [base64.b64encode(image).decode('utf-8') for image in images]})


async def handler_for_cancel(request):
    username = request.match_info['username']

    context = request.app['context']
    tasks_to_cancel = context.get(username, [])
    for task in tasks_to_cancel:
        task.cancel()

    cancelled = filter(lambda t: not t.done(), tasks_to_cancel)
    while list(cancelled):
        await asyncio.sleep(0.1)
        cancelled = filter(lambda t: not t.done(), tasks_to_cancel)

    if tasks_to_cancel:
        del context[username]

    log.info('Cancelled %d images for %s', len(tasks_to_cancel), username)
    return web.json_response({'cancelled': len(tasks_to_cancel)})


def async_scraper_server():
    app = web.Application()
    app.add_routes([web.post('/scrap_my_staff/{username}', handler_for_staff)])
    app.add_routes([web.post('/cancel_my_staff/{username}', handler_for_cancel)])

    app['context'] = defaultdict(list)
    web.run_app(app)


if __name__ == '__main__':
    async_scraper_server()
