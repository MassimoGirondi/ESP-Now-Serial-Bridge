#include "config.h"
#include "rssi.h"
#include "utils.h"
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

const uint8_t peerAddress[] = RECV_MAC;
const uint8_t ourAddress[] = SEND_MAC;
// wait for double the time between bytes at this serial baud rate before
// sending a packet this *should* allow for complete packet forming when using
// packetized serial comms
const uint32_t timeout_micros = (int)(1.0 / BAUD_RATE * 1E6) * 20;

uint8_t buf_recv[BUFFER_SIZE];
uint8_t buf_send[BUFFER_SIZE];
uint8_t buf_size = 0;
uint32_t send_timeout = 0;

esp_now_peer_info_t peerInfo; // scope workaround for arduino-esp32 v2.0.1

#if defined(DEBUG) || defined(BLINK_ON_SEND_SUCCESS)
uint8_t led_status = 0;
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
#ifdef DEBUG
  if (status == ESP_NOW_SEND_SUCCESS) {
    Serial.println("Send success");
  } else {
    Serial.println("Send failed");
  }
#endif

#ifdef BLINK_ON_SEND_SUCCESS
  if (status == ESP_NOW_SEND_SUCCESS) {
    led_status = ~led_status;
    // this function happens too fast to register a blink
    // instead, we latch on/off as data is successfully sent
    digitalWrite(LED_BUILTIN, led_status);
    return;
  }
  // turn off the LED if send fails
  led_status = 0;
  digitalWrite(LED_BUILTIN, led_status);
#endif
}
#endif

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
#ifdef BLINK_ON_RECV
  digitalWrite(LED_BUILTIN, HIGH);
#endif
  memcpy(&buf_recv, incomingData, sizeof(buf_recv));
  Serial.write(buf_recv, len);
#ifdef BLINK_ON_RECV
  digitalWrite(LED_BUILTIN, LOW);
#endif
#ifdef DEBUG
  Serial.print("\n Bytes received: ");
  Serial.println(len);
  Serial.print("\n From");
  printMac(mac);
  Serial.println();
#endif
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(BAUD_RATE, SER_PARAMS, RX_PIN, TX_PIN);
  Serial.println(send_timeout);
  WiFi.mode(WIFI_STA);

#ifdef DEBUG
  Serial.print("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
#ifdef BOARD1
  Serial.println("This is board 1");
#else
  Serial.println("This is board 2");
#endif
  Serial.print("Peer address is ");
  printMac(peerAddress);
  Serial.println();

#endif

  if (esp_wifi_set_channel(WIFI_CHAN, WIFI_SECOND_CHAN_NONE) != ESP_OK) {
#ifdef DEBUG
    Serial.println("Error changing WiFi channel");
#endif
    return;
  }

  if (esp_now_init() != ESP_OK) {
#ifdef DEBUG
    Serial.println("Error initializing ESP-NOW");
#endif
    return;
  }

#if RSSI
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&promiscuous_rx_cb);
#endif

#if defined(DEBUG) || defined(BLINK_ON_SEND_SUCCESS)
  esp_now_register_send_cb(OnDataSent);
#endif

  // esp_now_peer_info_t peerInfo;  // scope workaround for arduino-esp32 v2.0.1
  memcpy(peerInfo.peer_addr, peerAddress, 6);
  peerInfo.channel = WIFI_CHAN;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
#ifdef DEBUG
    Serial.println("Failed to add peer");
#endif
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

  // read up to BUFFER_SIZE from serial port
  if (Serial.available()) {
    while (Serial.available() && buf_size < BUFFER_SIZE) {
      buf_send[buf_size] = Serial.read();
      send_timeout = micros() + timeout_micros;
      buf_size++;
    }
  }

  // send buffer contents when full or timeout has elapsed
  if (buf_size == BUFFER_SIZE || (buf_size > 0 && micros() >= send_timeout)) {
#ifdef BLINK_ON_SEND
    digitalWrite(LED_BUILTIN, HIGH);
#endif
    esp_err_t result =
        esp_now_send(peerAddress, (uint8_t *)&buf_send, buf_size);
    buf_size = 0;
#ifdef DEBUG
    if (result == ESP_OK) {
      Serial.println("Sent!");
    } else {
      Serial.println("Send error");
    }
#endif
#ifdef BLINK_ON_SEND
    digitalWrite(LED_BUILTIN, LOW);
#endif
  }
}
