import asyncio
import picamera
import websockets
import io

async def handle_client(websocket, path):
    print("Client connected")

    try:
        with picamera.PiCamera() as camera:
            # Adjust camera settings if needed
            camera.resolution = (640, 480)

            # Capture and send images indefinitely
            while True:
                # Capture image
                image_stream = io.BytesIO()
                camera.capture(image_stream, format='jpeg')
                image_data = image_stream.getvalue()

                # Send image to the client
                await websocket.send(image_data)
                await asyncio.sleep(0.1)  # Adjust sleep time as needed

    except websockets.exceptions.ConnectionClosedOK:
        print("Client disconnected")

async def main():
    server = await websockets.serve(handle_client, "192.168.100.9", 8080)

    print("WebSocket server started")
    await server.wait_closed()

asyncio.run(main())
