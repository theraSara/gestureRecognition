# Gesture Recognition with Arduino Nano

This project implements gesture recognition using an Arduino Nano, an onboard LSM9DS1 IMU sensor, and a TensorFlow Lite micro model. The system detects and classifies gestures such as "Punch" and "Flex" based on accelerometer and gyroscope data and activates LEDs as output indicators.

---

## Features

- **Real-Time Gesture Recognition**: Recognizes two gestures: "Punch" and "Flex."
- **TensorFlow Lite Integration**: Utilizes a pre-trained TensorFlow Lite model for gesture classification.
- **LED Indicators**: Visual feedback using RED and BLUE LEDs for classified gestures.
- **Low Power Usage**: Designed for microcontroller-based low-power systems.

---

## Hardware Requirements

- Arduino Nano (or similar microcontroller)
- LSM9DS1 IMU sensor (built-in for Arduino Nano 33 BLE Sense)
- Two LEDs (Red and Blue)
- Resistors for LEDs
- Breadboard and jumper wires

---

## Software Requirements

- Arduino IDE (with TensorFlow Lite and LSM9DS1 libraries installed)
- TensorFlow Lite Micro model for gesture classification (`model.h` included in the code)

---

## Setup Instructions

### 1. Install Libraries
- Open the Arduino IDE.
- Go to **Sketch > Include Library > Manage Libraries**.
- Install the following libraries:
  - `Arduino_LSM9DS1`
  - `TensorFlowLite`

### 2. Hardware Connections
- Connect the Red LED to pin `23` (with a resistor).
- Connect the Blue LED to pin `22` (with a resistor).
- Connect an additional LED to pin `25` for power status.

### 3. Model Deployment
- Include your TensorFlow Lite Micro model header file (`model.h`) in the project. This file contains the serialized TensorFlow model for gesture classification.

### 4. Upload Code
- Connect the Arduino Nano to your computer.
- Open the provided code in the Arduino IDE.
- Select the correct **Board** and **Port** under **Tools**.
- Upload the code to the microcontroller.

---

## How It Works

1. **Idle State**:
   - The system waits for significant acceleration to begin data collection.

2. **Data Collection**:
   - Captures IMU data (acceleration and gyroscope readings) for 119 samples.

3. **Gesture Classification**:
   - Processes the collected data through the TensorFlow Lite model for classification.

4. **LED Indication**:
   - Turns on the RED LED for "Punch."
   - Turns on the BLUE LED for "Flex."

---

## Code Highlights

### TensorFlow Lite Integration
- Loads the pre-trained gesture recognition model and allocates tensors for inference.

### Gesture Data Normalization
- Acceleration and gyroscope data are normalized to match the model's input range.

### Confidence Scores
- Outputs confidence scores for "Punch" and "Flex" gestures.

---

## Example Output

After gesture classification, the Arduino Serial Monitor will display confidence scores:

```Punch: 0.85 Flex: 0.15```


The corresponding LED will light up based on the classification.

---

## Customization

### Add New Gestures
- Train a new TensorFlow model with additional gestures.
- Convert the model to TensorFlow Lite Micro and replace `model.h`.

### Adjust Sensitivity
- Modify the `accelerationThreshold` to change the sensitivity for triggering data collection.

---

## Troubleshooting

1. **IMU Initialization Failed**:
   - Ensure the IMU sensor is connected and functioning properly.

2. **Model Loading Error**:
   - Verify the TensorFlow Lite model version matches the `TFLITE_SCHEMA_VERSION` in the code.

3. **LEDs Not Working**:
   - Check the connections and ensure the correct pins are used.

---

## References

- [Arduino_LSM9DS1 Library](https://www.arduino.cc/en/Reference/ArduinoLSM9DS1)
- [TensorFlow Lite for Microcontrollers](https://www.tensorflow.org/lite/microcontrollers)

---

## License

This project is open-source and available for modification and distribution under the MIT License.
