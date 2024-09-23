// ####################
//      Created by                                                   
// ╔═╗╔╦╗╔═╗╔╦╗╔╗╔╔═╗╔╦╗  
// ╠═╣ ║ ║ ║║║║║║║╠╣  ║   
// ╩ ╩ ╩ ╚═╝╩ ╩╝╚╝╚   ╩   
// ####################   

/* 
 * FLASH SETTINGS
 * Board: LOLIN D32
 * Flash Frequency: 80MHz
 * Partition Scheme: Minimal SPIFFS.
*/

/* 
 * This software is specifically designed to train and enhance the AI of your Pwnagotchi,
 * a device known for its ability to passively detect and deauthenticate Wi-Fi networks. 
 * By simulating networks with randomized SSIDs and passwords, this tool provides a controlled 
 * environment for the Pwnagotchi's AI to practice identifying and targeting vulnerable access points. 
 * As the AI learns from each interaction, it becomes more effective at capturing handshakes and 
 * executing penetration strategies. 
 * Ultimately, this software improves the Pwnagotchi’s ability to autonomously scan, deauth, and 
 * gather valuable network data, refining its performance in real-world Wi-Fi security assessments.
*/

#include <WiFi.h>
#include <esp_wifi.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>

// Wi-Fi configuration
unsigned long previousMillis = 0;
const long networkInterval = 60 * 1000; // Changes network every 60 seconds
const long beaconInterval = 5 * 1000; // Broadcasts beacon every 5 seconds
unsigned long previousBeaconMillis = 0;
unsigned long lastPrintMillis = 0; // For controlling serial print frequency
const long printInterval = 10 * 1000; // 10 seconds in milliseconds

// New variable for channel change timing
unsigned long previousChannelMillis = 0;
const long channelChangeInterval = 10 * 1000; // Change channel every 10 seconds

String currentSSID = "";
int currentChannel = 1;
String pwnagotchiName = "";
String prevPwnagotchiName = ""; // To store the previous Pwnagotchi name

// Broadcast MAC address in beacon frames
uint8_t broadcastMac[6]; // This will store the ESP32's MAC address

// Initialize the display
TFT_eSPI tft = TFT_eSPI(); 

// Define the necessary structures for packet analysis
typedef struct {
  unsigned frame_ctrl:16;
  unsigned duration_id:16;
  uint8_t addr1[6];
  uint8_t addr2[6];
  uint8_t addr3[6];
  unsigned sequence_ctrl:16;
  uint8_t addr4[6];
} wifi_ieee80211_mac_hdr_t;

typedef struct {
  wifi_ieee80211_mac_hdr_t hdr;
  uint8_t payload[0];
} wifi_ieee80211_packet_t;

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the display
  tft.init();
  tft.setRotation(1);

  // Initialize Wi-Fi in AP mode
  WiFi.mode(WIFI_AP);
  createRandomNetwork();

  // Get the ESP32's MAC address
  esp_read_mac(broadcastMac, ESP_MAC_WIFI_STA); // Get the MAC address for the Wi-Fi STA interface

  // Initialize Wi-Fi in promiscuous mode
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&pwnSnifferCallback);
}

void loop() {
  unsigned long currentMillis = millis();

  // Create a new network every 60 seconds
  if (currentMillis - previousMillis >= networkInterval) {
    previousMillis = currentMillis;
    createRandomNetwork();
    delay(100); // Small delay to prevent watchdog from triggering
  }

  // Send beacon frames every 5 seconds
  if (currentMillis - previousBeaconMillis >= beaconInterval) {
    previousBeaconMillis = currentMillis;
    sendBeaconFrame();
    delay(100); // Small delay to prevent watchdog from triggering
  }

  // Change channel every 10 seconds
  if (currentMillis - previousChannelMillis >= channelChangeInterval) {
    previousChannelMillis = currentMillis;
    changeChannel(); // Change the channel
    delay(100); // Small delay to prevent watchdog from triggering
  }

  yield(); // Allow other tasks to run
}

void changeChannel() {
  // Set a new random channel
  currentChannel = random(1, 12); // Generate a random Wi-Fi channel between 1 and 11
  WiFi.softAPConfig(WiFi.softAPIP(), WiFi.softAPIP(), currentChannel); // Apply the new channel
  Serial.println("Changed channel to: " + String(currentChannel));
  displayNetworkInfo(currentSSID, currentChannel, pwnagotchiName);
}

void createRandomNetwork() {
  // Creates new random SSID
  currentSSID = "PT_" + generateRandomSSID(8); // Add prefix and generate a random SSID with a length of 8
  String randomPassword = generateRandomPassword(8); // Generate a random password with a length of 8
  currentChannel = random(1, 12); // Generate a random Wi-Fi channel between 1 and 11
  WiFi.softAP(currentSSID.c_str(), randomPassword.c_str(), currentChannel); // Create the network on the random channel
  Serial.println("Created network: " + currentSSID + " with password: " + randomPassword + " on channel: " + String(currentChannel));
  displayNetworkInfo(currentSSID, currentChannel, pwnagotchiName);
}

String generateRandomSSID(int length) {
  String ssid = "";
  const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  for (int i = 0; i < length; i++) {
    ssid += charset[random(0, sizeof(charset) - 1)];
  }
  return ssid;
}
// Creates new random password
String generateRandomPassword(int length) {
  String password = "";
  const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
  for (int i = 0; i < length; i++) {
    password += charset[random(0, sizeof(charset) - 1)];
  }
  return password;
}

void sendBeaconFrame() {
  uint8_t ssidLength = currentSSID.length();
  uint8_t beaconPacket[128] = {
      // Beacon Frame
      0x80, 0x00, // Frame Control
      0x00, 0x00, // Duration
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // Destination: Broadcast
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // Source: Broadcast
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // BSSID: Broadcast
      0x00, 0x00, // Sequence Number
      // Fixed Parameters
      0x00, 0x00, 0x00, 0x00, // Timestamp
      0x64, 0x00, // Beacon Interval
      0x01, 0x04, // Capability Info
      // SSID Parameter Set
      0x00, ssidLength // Tag: SSID parameter set, Tag length
  };

  // Copy SSID into the packet
  for (int i = 0; i < ssidLength; i++) {
    beaconPacket[38 + i] = currentSSID[i];
  }

  // Append supported rates and DS parameter set
  beaconPacket[38 + ssidLength] = 0x01;
  beaconPacket[39 + ssidLength] = 0x08;
  beaconPacket[40 + ssidLength] = 0x82;
  beaconPacket[41 + ssidLength] = 0x84;
  beaconPacket[42 + ssidLength] = 0x8b;
  beaconPacket[43 + ssidLength] = 0x96;
  beaconPacket[44 + ssidLength] = 0x0c;
  beaconPacket[45 + ssidLength] = 0x12;
  beaconPacket[46 + ssidLength] = 0x18;
  beaconPacket[47 + ssidLength] = 0x24;
  beaconPacket[48 + ssidLength] = 0x03;
  beaconPacket[49 + ssidLength] = 0x01;
  beaconPacket[50 + ssidLength] = currentChannel;

  // Generate and set a random MAC address for this beacon frame
  uint8_t fakeMac[6];
  for (int i = 0; i < 6; i++) {
    fakeMac[i] = random(0, 256);
  }
  
  // Set the random MAC address as the source and BSSID addresses
  memcpy(beaconPacket + 6, fakeMac, 6); // Source
  memcpy(beaconPacket + 12, fakeMac, 6); // BSSID

  // Convert MAC address to a string
  String macAddress = String(fakeMac[0], HEX) + ":" + String(fakeMac[1], HEX) + ":" + String(fakeMac[2], HEX) + ":" + String(fakeMac[3], HEX) + ":" + String(fakeMac[4], HEX) + ":" + String(fakeMac[5], HEX);
  macAddress.toUpperCase(); // Ensure uppercase for MAC address
  
  // Broadcast the beacon frame on the AP interface
  esp_err_t result = esp_wifi_80211_tx(WIFI_IF_AP, beaconPacket, 51 + ssidLength, false);

  if (result == ESP_OK) {
    Serial.println("Beacon frame sent with SSID: " + currentSSID + " and random MAC address: " + macAddress);
  } else {
    Serial.println("Error sending beacon frame");
  }
}

void getMAC(char* addr, const uint8_t* payload, int pos) {
  snprintf(addr, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
           payload[pos], payload[pos + 1], payload[pos + 2],
           payload[pos + 3], payload[pos + 4], payload[pos + 5]);
}

// Callback function for promiscuous mode
void pwnSnifferCallback(void* buf, wifi_promiscuous_pkt_type_t type) {
  wifi_promiscuous_pkt_t *snifferPacket = (wifi_promiscuous_pkt_t*)buf;
  int len = snifferPacket->rx_ctrl.sig_len;

  if (type == WIFI_PKT_MGMT) {
    len -= 4; // Adjust length if necessary
    if (snifferPacket->payload[0] == 0x80) { // Beacon frame
      String jsonPayload = "";
      for (int i = 0; i < len; i++) {
        if (isPrintable(snifferPacket->payload[i + 38])) {
          jsonPayload += (char)snifferPacket->payload[i + 38];
        }
      }

      //Serial.println("JSON Payload: " + jsonPayload); // Print raw JSON payload for debugging

      DynamicJsonDocument doc(2048); // Increased buffer size
      DeserializationError error = deserializeJson(doc, jsonPayload);
      if (!error) {
        String name = doc["name"].as<String>();

        char srcMac[18];
        getMAC(srcMac, snifferPacket->payload, 10);

        if (pwnagotchiName != name) {
          prevPwnagotchiName = pwnagotchiName;
          pwnagotchiName = name;
        }
        if (millis() - lastPrintMillis >= printInterval) {
          //Serial.println("MAC: " + String(srcMac) + " Pwnagotchi Name: " + name);
          lastPrintMillis = millis();
        }
        displayNetworkInfo(currentSSID, currentChannel, pwnagotchiName);
      } else {
        Serial.println("Failed to parse JSON: " + String(error.c_str()));
      }
    }
  }
}

void displayNetworkInfo(String ssid, int channel, String pwnagotchiName) {
  tft.fillScreen(TFT_BLACK);  // Clear the screen

  // Set font and text color
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);

  // Display title: "Pwny-Trainer"
  tft.setCursor(10, 10);
  tft.print("Pwny-Trainer");

  // Draw a horizontal green line below the title
  tft.drawLine(10, 35, tft.width() - 10, 35, TFT_GREEN);

  // Display SSID info
  tft.setTextSize(1);  // Adjust size for other text
  tft.setCursor(10, 50);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("SSID: ");
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.print(ssid);

  // Display Channel info
  tft.setCursor(10, 70);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("Channel: ");
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.print(channel);

  // Display Pwnagotchi name
  tft.setCursor(10, 90);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print("Pwnagotchi: ");
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.print(pwnagotchiName);

  // Optional: Display previous Pwnagotchi name (if needed)
  if (prevPwnagotchiName != "") {
    tft.setCursor(10, 110);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.print("Prev Pwny: ");
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.print(prevPwnagotchiName);
  }
}