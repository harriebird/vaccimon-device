# vaccimon-device

Software for the vaccine carrier monitoring device. The device is powered by an
ESP32 Development Board connected with sensors to collect temperature and motion.
The device information is shown to the health worker using an OLED display,
which minimizes the need for the vaccine carrier to be opened for temperature
monitoring. The collected sensor data is also exposed to other devices via BLE
(Bluetooth Low Energy).

![Device Diagram](img/device-diagram.png)

## Setup
1. Clone this repository.
2. Install [Adruino IDE](https://www.arduino.cc/en/software).
3. Go to `Preferences > Additional Boards Manager URLs` and add the link below:
    * https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
4. Open the Boards Manager and install `esp32`.
5. Set `DOIT ESP32 DEVKIT V1` as your board.


## Requirements
### Software
* Arduino IDE
* Board:
    * ESP32
* Libraries:
    * DallasTemperature v3.9.0 by Miles Burton
    * MPU6050_light v1.2.1 by rfetick
    * OneWire v2.3.7 by Paul Stoffegen
    * Adafruit SSD1306 v2.5.7 by Adafruit
* VSCodium (optional)

### Hardware
* DOIT ESP32 DEVKIT V1
* DS18B20 Temperature Sensor
* GY-521 MPU-6050 Gyro Sensor
* SSD1306 OLED Display Module
