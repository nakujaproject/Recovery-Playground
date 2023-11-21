import asyncio
import websockets
import io
from picamera2 import Picamera2
from picamera2.encoders import JpegEncoder
from picamera2.outputs import FileOutput

uri = "ws://192.168.100.9:8080"

async def main():
    
    async with websockets.connect(uri) as websocket:
         with Picamera2() as camera:
            # Adjust camera settings if needed
            camera.resolution = (640, 480)
            # Capture and send images indefinitely
            while True:
                # Capture image
                image_stream = io.BytesIO()
                camera.start_recording(JpegEncoder(), image_stream)
                image_data = image_stream.getvalue()
                print(image_data)
                # Send image to the client
                await websocket.send(image_data)
                await asyncio.sleep(0.1)

asyncio.run(main())
