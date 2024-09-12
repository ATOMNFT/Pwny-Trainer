<div align="center">

# Pwnagotchi Trainer with ESP32

## 📷 Images coming soon! 📷

</div>

This Arduino sketch transforms an ESP32 into a Pwnagotchi trainer, helping Pwnagotchis improve their ability to detect, deauthenticate, and capture handshakes from Wi-Fi networks in a dynamic, ever-changing environment. 
<br>
The sketch as of now focuses mainly on association training (Still trains other areas, but minimally) but I'm working on deauth training and a few other features.

## Features

- **Channel Hopping**: Every 10 seconds, the ESP32 switches to a new Wi-Fi channel. This ensures that the Pwnagotchi gets exposure to networks on various channels, enhancing its training by scanning across a wider range of frequencies.

- **AP/SSID Hopping**: Changes network every 60 seconds. This simulates a constantly changing Wi-Fi environment by dynamically switching between different SSIDs. This keeps the Pwnagotchi actively scanning, analyzing new networks, and attempting to capture handshakes in a variety of conditions.

- **Beacon Broadcasting**: Broadcasts beacon every 5 seconds containg the randomly generated SSID, channel and MAC address for the pwnagotchi in training to discover/associate/deauth.
  
- **Promiscuous Mode (Sniffer) & Beacon Frame Interaction**: The ESP32 operates in promiscuous mode, capturing Wi-Fi management frames, particularly beacon frames sent by Pwnagotchis, extracts the Pwnagotchi's name from the JSON payload, and displays it. This helps identify specific Pwnagotchis in training.


## What the Pwnagotchi is Doing

- **Detection of Access Points**: The Pwnagotchi passively scans for networks. When it detects the SSIDs broadcast by this ESP32 trainer, it analyzes and interacts with the networks, learning how to deauthenticate and potentially capture handshakes.

- **Interaction with Beacon Frames**: The Pwnagotchi detects beacon frames sent by the ESP32, which contain SSID, channel info, and MAC addresses. This interaction helps the Pwnagotchi practice passive handshake capture, improving its ability to detect vulnerable networks.

- **Name Recognition**: If a Pwnagotchi sends out beacon frames with its name embedded in the metadata, the ESP32 recognizes and displays it on the screen, showing which Pwnagotchis are actively training.

## Benefits

- **Training Pwnagotchi AI**: The randomization and constantly changing network environment forces the Pwnagotchi to work harder in recognizing patterns and interacting with networks, enhancing its learning and automated penetration testing capabilities.

- **Real-World Simulation**: By generating random SSIDs, passwords, and beacon frames, the trainer creates a dynamic Wi-Fi environment similar to what Pwnagotchis would encounter in real-world scenarios.

## Getting Started

1. **Clone this repository**:
    ```bash
    git clone <https://github.com/ATOMNFT/Pwny-Trainer>
    ```
 --  **Or you can download the sketch above.**

2. **Upload the sketch** to your ESP32 device using the Arduino IDE.

3. **Run your Pwnagotchi** Run the pwnagotchi in AI mode alongside the ESP32 trainer to start improving its skills!

## License

This project is licensed under the GNU License. See the file for details.

---

Feel free to contribute or report any issues you find!

