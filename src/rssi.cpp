#include "rssi.h"
#include "utils.h"

// Based on https://github.com/TenoTrash/ESP32_ESPNOW_RSSI

const uint8_t ourAddress[] = SEND_MAC;
int avg_rssi = 0;
int count = 0;

void promiscuous_rx_cb(void *buf, wifi_promiscuous_pkt_type_t type) {
  // All espnow traffic uses action frames which are a subtype of the mgmnt frames so filter out everything else.
  if (type != WIFI_PKT_MGMT)
    return;

  const wifi_promiscuous_pkt_t *ppkt = (wifi_promiscuous_pkt_t *)buf;
  const wifi_ieee80211_packet_t *ipkt = (wifi_ieee80211_packet_t *)ppkt->payload;
  const wifi_ieee80211_mac_hdr_t *hdr = &ipkt->hdr;

  int rssi = ppkt->rx_ctrl.rssi;
  if(equalMacs(hdr->addr1, ourAddress)) {
          // A moving average where the history doesn't have such importance
          avg_rssi = (avg_rssi*3 + rssi*5) >> 3;
          count++;
          if (count % RSSI_PKTS == 0)
                  {
                  Serial.print("RSSI: ");
                  Serial.println(avg_rssi);
                  count = 1;
                  }
  }
}
