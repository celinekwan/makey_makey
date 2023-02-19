// Request permission to access the serial port
async function requestSerialPort() {

    let port;
    port = await navigator.serial.requestPort();

    document.getElementById("message").innerHTML = "Perfect reading conditions!";

    await port.open({ baudRate: 9600 });
    
    // Create a reader to read data from the serial port
    const reader = port.readable.getReader();
  
    // Read data from the serial port
    while (true) {
      const { value, done } = await reader.read();
      
      // Convert the received data to a string
      const textDecoder = new TextDecoder();
      let state = textDecoder.decode(value);

      // Update Warning Message
        if (state.localeCompare("b") == 0) {
            document.getElementById("message").innerHTML = "Warning: Light levels are too bright for reading!";
        } else if (state.localeCompare("d") == 0) {
            document.getElementById("message").innerHTML = "Warning: Light levels are too dim for reading!";
        } else if (state.localeCompare("c") == 0) {
            document.getElementById("message").innerHTML = "Warning: Book is too close for reading!";
        } else if (state.localeCompare("n") == 0) {
            document.getElementById("message").innerHTML = "Perfect reading conditions!";
        }
    }
}

document.addEventListener('DOMContentLoaded', () => {
    document.getElementById("startButton").addEventListener('click', () => {
        document.getElementById("startButton").remove();
        requestSerialPort();
    });
});
