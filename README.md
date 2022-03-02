# vaccimon-device
Software for the vaccine carrier monitor device.

## Setup
1. Clone this repository.
2. Install [Adruino IDE](https://www.arduino.cc/en/software).
3. Go to `Preferences > Additional Boards Manager URLs` and add the link below:
    * https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
4. Open the Boards Manager and install `esp32`.
5. Set `DOIT ESP32 DEVKIT V1` as your board.


## Requirements
### Software
* Adrduino
* Board:
    * ESP32
* Libraries:
    * DallasTemperature
    * BH1750
    * Adafruit MPU6050
    * OneWire
* VSCodium (optional)

### Hardware
* DOIT ESP32 DEVKIT V1
* DS18B20 Temperature Sensor
* GY-302 BH1750 Light Intensity Sensor
* GY-521 MPU-6050 Gyro Sensor
