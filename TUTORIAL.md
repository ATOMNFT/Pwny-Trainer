# Pwnagotchi Trainer with ESP32: Installation and Setup Guide

This guide walks you through the steps to install the Pwnagotchi Trainer Arduino sketch on an ESP32, enabling your Pwnagotchi to improve its ability to detect, deauthenticate, and capture handshakes from Wi-Fi networks.

## Table of Contents
- [Features Overview](#features-overview)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Step-by-Step Installation](#step-by-step-installation)
- [Running the Sketch](#running-the-sketch)
- [Using the Training_Block Plugin](#using-the-training_block-plugin)
- [Benefits](#benefits)
- [Conclusion](#conclusion)

## Features Overview
The Pwnagotchi Trainer simulates dynamic Wi-Fi environments by switching SSIDs, broadcasting beacons, and hopping channels to train a Pwnagotchi on recognizing, interacting with, and deauthenticating networks. Key features include:

- **Channel Hopping**: Switches Wi-Fi channels every 10 seconds.
- **AP/SSID Hopping**: Generates random networks every 60 seconds.
- **Beacon Broadcasting**: Broadcasts network beacons every 5 seconds for the Pwnagotchi to detect and interact with.
- **Promiscuous Mode**: Captures Wi-Fi frames and displays Pwnagotchi names from JSON payloads in beacon frames.

## Hardware Requirements
- ESP32 (e.g., TTGO T-Display, CYD variant 1 or 2)
- USB cable for flashing the ESP32
- Pwnagotchi device in AI mode (for interaction and training)

## Software Requirements
### Arduino IDE
- Download and install the latest version of the Arduino IDE: [Arduino IDE Download](https://www.arduino.cc/en/software)

### ESP32 Board Package
In Arduino IDE:
1. Go to **File > Preferences**.
2. In the Additional Boards Manager URLs, add the following URL: <br>
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

3. Go to **Tools > Board > Board Manager** and search for "ESP32."
4. Install the `esp32` package by Espressif Systems.

### TFT_eSPI Library (For devices with displays)
1. Go to **Sketch > Include Library > Manage Libraries**.
2. Search for `TFT_eSPI by Bodmer` and install it.

### Pwnagotchi Firmware
Ensure you have the latest Pwnagotchi firmware installed and configured. Follow the instructions [here](https://pwnagotchi.ai).

## Step-by-Step Installation

### 1. Clone the Repository
To get started, download or clone the Pwnagotchi Trainer sketch from GitHub:

```bash
git clone https://github.com/ATOMNFT/Pwny-Trainer
Alternatively, you can download the ZIP file from GitHub and extract it.

2. Open the Sketch in Arduino IDE
Launch Arduino IDE.
Open the downloaded sketch by going to File > Open and selecting the Pwny-Trainer.ino file.
3. Set Up Your ESP32 Board in Arduino IDE
Connect your ESP32 device to your computer via USB.
In Arduino IDE, go to Tools > Board > ESP32 Arduino and select the correct ESP32 board (e.g., TTGO T-Display or ESP32 Dev Module).
Set the correct port by going to Tools > Port and selecting the port your ESP32 is connected to.
4. Install Required Libraries
Ensure the following libraries are installed in the Arduino IDE:

WiFi.h (for Wi-Fi functionality)
TFT_eSPI (if your ESP32 has a display)
Go to Sketch > Include Library > Manage Libraries and search for any missing dependencies.

5. Upload the Sketch to ESP32
Verify the code by clicking the checkmark icon in the top left of Arduino IDE.
Once verified, click the right arrow icon to upload the sketch to your ESP32.
6. Monitor the Serial Output
Once uploaded, open the Serial Monitor (Tools > Serial Monitor) to monitor the ESP32's operation. You should see the ESP32 broadcasting SSIDs, hopping channels, and interacting with any nearby Pwnagotchis.

Running the Sketch
After flashing the sketch, your ESP32 will begin:

Broadcasting random SSIDs every 60 seconds.
Hopping between channels every 10 seconds.
Sending beacon frames every 5 seconds with random MAC addresses for the Pwnagotchi to discover.
Detecting Pwnagotchis by capturing beacon frames and displaying their names on the screen.
The Pwnagotchi should automatically start detecting the SSIDs and beacon frames and interacting with them, helping it improve its skills in capturing handshakes.

Using the Training_Block Plugin
The Training_Block plugin prevents the Pwnagotchi from uploading pcap files containing "PT_" in the filename, reducing unnecessary uploads to WPA-sec.

Upload the Plugin
Place the Training_Block plugin file in the Pwnagotchi's plugins directory:
bash
Copy code
/etc/pwnagotchi/plugins.d/
Enable the Plugin
Add the following line to your config.toml file:
toml
Copy code
main.plugins.Training_Block.enabled = true
Restart the Pwnagotchi.
After saving the changes, restart the Pwnagotchi for the plugin to take effect.

Benefits
Enhanced Training: By simulating real-world Wi-Fi environments, the trainer helps Pwnagotchis improve their ability to detect and capture handshakes.
Automation: The automated hopping and beacon broadcasting keep the Pwnagotchi engaged and continuously learning.
Improved Penetration Testing: Random SSID generation forces the Pwnagotchi to dynamically adjust, mimicking real-world Wi-Fi conditions.
Conclusion
Your ESP32 is now set up as a Pwnagotchi Trainer! It will continuously broadcast beacons, hop between networks and channels, and interact with any Pwnagotchis in range. By following the instructions in this guide, you’ll ensure that your Pwnagotchi gets the best possible training environment.

Enjoy enhancing your Pwnagotchi’s AI capabilities and penetration testing skills!