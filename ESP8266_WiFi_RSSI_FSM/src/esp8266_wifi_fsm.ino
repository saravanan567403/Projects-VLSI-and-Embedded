

#include <ESP8266WiFi.h>
typedef enum {
  STATE_RESET,
  STATE_IDLE,
  STATE_WIFI_SCAN,
  STATE_PROCESS,
  STATE_DISPLAY
} system_state_t;

system_state_t current_state = STATE_RESET;

// Wi-Fi scan result count
int network_count = 0;

// Timer variable (used instead of delay)
unsigned long prev_time = 0;
const unsigned long idle_interval = 3000; 
void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);   // Station mode only
  WiFi.disconnect();    
}
void loop() {

  switch (current_state) {
    case STATE_RESET:
      Serial.println("\n[RESET] System Initializing...");
      current_state = STATE_IDLE;
      break;
    case STATE_IDLE:
      if (millis() - prev_time >= idle_interval) {
        prev_time = millis();
        Serial.println("\n[IDLE] Triggering Wi-Fi Scan...");
        current_state = STATE_WIFI_SCAN;
      }
      break;
    case STATE_WIFI_SCAN:
      Serial.println("[SCAN] Scanning networks...");
      network_count = WiFi.scanNetworks();
      current_state = STATE_PROCESS;
      break;
    case STATE_PROCESS:
      Serial.println("[PROCESS] Processing scan data...");
      current_state = STATE_DISPLAY;
      break;

    
    case STATE_DISPLAY:
      if (network_count == 0) {
        Serial.println("[DISPLAY] No networks found.");
      } else {
        Serial.println("[DISPLAY] Networks found:");
        for (int i = 0; i < network_count; i++) {
          Serial.print("  ");
          Serial.print(i + 1);
          Serial.print(". ");
          Serial.print(WiFi.SSID(i));
          Serial.print(" | RSSI: ");
          Serial.print(WiFi.RSSI(i));
          Serial.println(" dBm");
        }
      }
      Serial.println("[DISPLAY] Scan complete.\n");
      current_state = STATE_IDLE;
      break;

  
    default:
      current_state = STATE_RESET;
      break;
  }
}
