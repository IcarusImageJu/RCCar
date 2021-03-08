#include "WiFi.h"
#include <esp_now.h>

const int UP = 5;
const int DOWN = 13;
const int LEFT = 12;
const int RIGHT = 14;

// MCU MAC ADDRESS: AC:67:B2:1D:05:F4
// Receiver MCU MAC ADDRESS: 8C:AA:B5:8B:DB:B0

const long interval = 100; 
unsigned long previousMillis;

// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0x8C, 0xAA, 0xB5, 0x8B, 0xDB, 0xB0};

typedef struct struct_message {
    int up;
    int right;
    int down;
    int left;
    long interval;
} struct_message;

struct_message ButtonsReading;
String success;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  if (status == ESP_NOW_SEND_SUCCESS){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Set ESP-NOW Role
  // esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // Register for a callback function that will be called when data is received
  // esp_now_register_recv_cb(OnDataRecv);
  
  // initialize the pushbutton pin as an input:
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());
}

void getReadings(){
  // read the state of the pushbutton value:

  ButtonsReading.up = digitalRead(UP);
  ButtonsReading.right = digitalRead(RIGHT);
  ButtonsReading.down = digitalRead(DOWN);
  ButtonsReading.left = digitalRead(LEFT);
  ButtonsReading.interval = interval;
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated
    previousMillis = currentMillis;
    getReadings();
    esp_now_send(broadcastAddress, (uint8_t *) &ButtonsReading, sizeof(ButtonsReading));
    Serial.print(ButtonsReading.up);
    Serial.print(ButtonsReading.right);
    Serial.print(ButtonsReading.down);
    Serial.println(ButtonsReading.left);
    delay(1);
  }
}
