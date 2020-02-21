#!/usr/bin/env python
import asyncio
import websockets
import subprocess

# Demonstrating injection if shell=True
stuff = 'injection\ninnocence" | grep "t'
print(subprocess.check_output(f'echo "{stuff}"', shell=True))

async def wait_and_execute(websocket, path):
    while True:
        code = await websocket.recv()
        print(f'< {code}')

        #result = subprocess.check_output(['python', '-c', code], text=True)
        try:
            result = subprocess.check_output(
                ['docker', 'container', 'run', '--rm', 'executor', 'python', '-c', code],
                text=True,
                stderr=subprocess.STDOUT
            )
        except subprocess.CalledProcessError as error:
            result = f'{error.output}\n\nExited with status code {error.returncode}'

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
