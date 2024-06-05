<p align="center">
  <a href="" rel="noopener">
 <img width=200px height=200px src="https://github.com/phuthien007/Waiting-Queue/blob/master/client/public/resources/images/tf-logo.jpg" alt="Project logo"></a>
</p>

<h3 align="center">RFID Reader Project</h3>

<div align="center">

[![Status](https://img.shields.io/badge/status-active-success.svg)]()
[![GitHub Pull Requests](https://img.shields.io/github/issues-pr/kylelobo/The-Documentation-Compendium.svg)](https://github.com/phuthien007/Waiting-Queue/pulls)
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

## ⛏️ Bill of Material <a name = "material"></a>

- ArduinoIDE: For programming the device 
- ESP32 Microcontroller x1
- LED x1
- Oled SSD1306 x1
- Buzzer x1

## 📖 Hardware Schematic <a name = "schematic"></a>

## 💻 Software Concept <a name = "sofware"></a>

## ✍️ Authors <a name = "authors"></a>

- [@hathai25](https://github.com/hathai25)
