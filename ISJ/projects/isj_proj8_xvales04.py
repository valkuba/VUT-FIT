#!/usr/bin/env python3

import asyncio
import aiohttp

async def get_url(session, url):
    """Creating asynchronious task"""
    try:
        async with session.get(url) as response:
            return response.status, url
    except aiohttp.ClientConnectorError as e:
        return 'aiohttp.ClientConnectorError', url

async def get_urls(urls):
    """Asynchronious calling of task, returns result as tuple (status, website)"""
    async with aiohttp.ClientSession() as session:
        tasks = [get_url(session, url) for url in urls]
        results = await asyncio.gather(*tasks)
        return results

if __name__ == '__main__':

    urls = ['https://www.fit.vutbr.cz', 'https://www.szn.cz', 'https://office.com']
    res = asyncio.run(get_urls(urls))
    print(res)

#[(200, 'https://www.fit.vutbr.cz'), ('aiohttp.ClientConnectorError', 'https://www.szn.cz'), (200, 'https://office.com')]