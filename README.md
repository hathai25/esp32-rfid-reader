<p align="center">
  <a href="" rel="noopener">
 <img width=200px height=200px src="https://cdn-icons-png.freepik.com/512/5200/5200104.png" alt="Project logo"></a>
</p>

<h3 align="center">RFID Reader Project</h3>

<div align="center">

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/kylelobo/The-Documentation-Compendium.svg)](https://github.com/hathai25/esp32-rfid-reader/pulls)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](/LICENSE)

</div>

---

<p align="center"> 
A project involves creating a device that uses an ESP32 microcontroller to read RFID tags, allowing for wireless identification and access control applications.
    <br> 
</p>

## 📝 Table of Contents

- [Introduction](#introduction)
- [Getting Started](#getting-started)
- [Bill of Material](#material)
- [Hardware Schematic](#schematic)
- [Software Concept](#software)
- [Acknowledgments](#acknowledgement)

## 🧐 Introduction <a name = "introduction"></a>

The project includes source code for an RFID Reader with the following requirements:

#### 📃 Functional requirement

- Read RFID Card: The ESP32 must be able to read data from RFID cards using an attached RFID reader module, capturing the unique identifier (UID) and other stored information from the card.
- Display Card Info on OLED Screen: Upon successfully reading an RFID card, the ESP32 should display the card's information, such as the UID and any additional data, on an OLED screen connected to the microcontroller.
- Activate Buzzer and LED on RFID Tap: When an RFID card is tapped and read, the ESP32 should trigger a buzzer and light up an LED to provide audible and visual feedback, indicating a successful read operation.

#### 📃 Non-functional requirement

- Debouncing: Introducing a debounce time interval during which repeated taps of the same card are ignored to prevent duplicate reads.
- State Management: Maintaining a record of recently read card UIDs and ensuring that any repeated taps within a predefined time window are disregarded.

## 🏁 Getting Started <a name = "getting-started"></a>

(Dành cho người sử dụng ko biết gì về IT, kèm ảnh minh họa sản phẩm, và link video)

## ⛏️ Bill of Material <a name = "material"></a>

- [ArduinoIDE](https://www.arduino.cc/en/software): For programming the device
- [Altium](https://www.altium.com/): For designing schematic
- ESP32 Microcontroller x1
- LED x1
- Oled SSD1306 x1
- Buzzer x1

## 📖 Hardware Schematic <a name = "schematic"></a>

![image](https://github.com/hathai25/esp32-rfid-reader/assets/74005327/93767119-3dca-4666-ac78-f9533a0ed0d5)

This schematic outlines the interconnections between an ESP32 microcontroller and various peripheral components, including an RFID-RC522 module, an OLED display, an LED, and a Buzzer. The connections are specified via General-Purpose Input/Output (GPIO) pins, designated for specific signal transmissions that enable communication between the ESP32 and the peripherals. [Software Concept](#software) below is a detailed breakdown of the connections.

## 💻 Software Concept <a name = "sofware"></a>

By utilizing the GPIOs of the ESP32 and SPI & I²C interface support, the project is able to control the following components:

#### ESP32 and RFID-RC522 Module

- GPIO2: This reset line is connected from the ESP32 to the RST pin on the RFID-RC522. It is used to reset the RFID module.
- GPIO5: The SS pin on the RFID-RC522 is connected to GPIO5 on the ESP32. This pin enables the ESP32 to select the RFID-RC522 module for communication over the SPI interface.

#### ESP32 and OLED Display

- GPIO21: The SDA line of the I²C interface for the OLED display is connected to GPIO21. This line is used for data transfer between the ESP32 and the OLED.
- GPIO22: The SCL line of the I²C interface links GPIO22 on the ESP32 with the corresponding SCL input on the OLED. This line provides the clock signal that synchronizes data transmission.

#### ESP32 and LED

- GPIO13: An LED is connected to GPIO13 on the ESP32. This setup allows the ESP32 to control the LED, turning it on or off by setting GPIO13 high or low, respectively.

#### ESP32 and Buzzer

- GPIO4: The Buzzer is connected to GPIO4 on the ESP32. By toggling GPIO4 high and low at various frequencies, the ESP32 can control the buzzer to generate different tones.


### Initialization

- Initialize the U8X8 display, LED pin, buzzer pin, and RFID module.
- Retrieve the index along with the three most recent IDs from the Preferences library.
- Display the three most recent IDs on the screen.

### Main Loop

- Check if the newly scanned card matches the previously scanned card; if it does, do not process the new card scan.
- Activate the buzzer and turn on the LED light to signal a successful scan.
- Update the storage with the three most recent card IDs and increment the counter by one.
- Save the updated values and the counter increment back to the Preferences library - which uses a portion of the on-board non-volatile memory (NVS) of the ESP32 to store data.

## ✍️ Authors <a name = "authors"></a>

- [@hathai25](https://github.com/hathai25)
- [@Ha-Hieu-Thanh](https://github.com/Ha-Hieu-Thanh)
- [@ntanh2201](https://github.com/ntanh2201)
