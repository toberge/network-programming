#!/usr/bin/env python
import asyncio
import websockets
from executor import run
import json

async def wait_and_execute(websocket, path):
    while True:
        data = await websocket.recv()
        request = json.loads(data)
        print(f'< {request["code"]}')

        result = run(request['code'], request['language'])
        
        await websocket.send(str(result))
        print(f"> {result}")

# Did not work, going straight to wait_and_execute instead
# async def connect(websocket, path):
#     while True:
#         thing = await websocket.recv()
#         await websocket.send(thing)

#     asyncio.get_event_loop().create_task(wait_and_execute(websocket))


start_server = websockets.serve(wait_and_execute, "localhost", 8764)

asyncio.get_event_loop().run_until_complete(start_server)
asyncio.get_event_loop().run_forever()
