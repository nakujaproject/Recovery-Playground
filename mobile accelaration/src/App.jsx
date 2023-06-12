import { useState, useEffect } from 'react'

function App() {
  let [accelerationX, setAccelerationX] = useState(0)
  let [accelerationY, setAccelerationY] = useState(0)
  let [accelerationZ, setAccelerationZ] = useState(0)

  useEffect(() => {
    // Check if the DeviceMotion API is supported by the browser
    if (window.DeviceMotionEvent) {
      // Start listening for accelerometer data
      window.addEventListener('devicemotion', handleMotionEvent, true);
    }else{
      alert('DeviceMotionEvent is not supported');
    }

    return () => {
      // Clean up: remove the event listener when the component unmounts
      window.removeEventListener('devicemotion', handleMotionEvent, false);
    }
  }, []);

  // Event handler for accelerometer data
  function handleMotionEvent(event) {
    // Get the acceleration values
    setAccelerationX(event.accelerationIncludingGravity.x)
    setAccelerationY(event.accelerationIncludingGravity.y)
    setAccelerationZ(event.accelerationIncludingGravity.z)

    // Do something with the acceleration values
    console.log('Acceleration X: ' + accelerationX);
    console.log('Acceleration Y: ' + accelerationY);
    console.log('Acceleration Z: ' + accelerationZ);
  }

  return (
    <div>
      <h1 className='text-center'>Accelerometer App</h1>
      <p>Acceleration X: {accelerationX}</p>
      <p>Acceleration Y: {accelerationY}</p>
      <p>Acceleration Z: {accelerationZ}</p>
    </div>
  )
}

export default App
