#include <esp_now.h>
#include <WiFi.h>

const long interval = 100; 
unsigned long previousMillis;

// Motor A
int motor1Pin1 = 27; 
int motor1Pin2 = 26;

int motor2Pin1 = 16; 
int motor2Pin2 = 17;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

typedef struct struct_message {
    int up;
    int right;
    int down;
    int left;
    long interval;
} struct_message;

struct_message buttons;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&buttons, incomingData, sizeof(buttons));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print(buttons.up);
  // Serial.print(buttons.right);
  // Serial.print(buttons.down);
  // Serial.println(buttons.left);
  // Serial.println(buttons.interval);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);

  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated
    previousMillis = currentMillis;
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, LOW);
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, LOW); 
    if(buttons.up == 0 && buttons.down == 1) {
      Serial.println("go up");
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, HIGH); 
    }
    if(buttons.right == 0 && buttons.left == 1) {
      Serial.println("go right");
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, HIGH); 
    }
    if(buttons.down == 0 && buttons.up == 1) {
      Serial.println("go down");
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW); 
    }
    if(buttons.left == 0 && buttons.right == 1) {
      Serial.println("go left");
      digitalWrite(motor2Pin1, HIGH);
      digitalWrite(motor2Pin2, LOW); 
    }
    Serial.print(".");
    delay(1);
  }
}
