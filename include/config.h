#ifndef CONFIG_H
#define CONFIG_H
///////////////////////////////////
//  Set here your parameters

#define BOARD1_MAC {0x30, 0xC6, 0xF7, 0x1F, 0xD7, 0x40}  // replace with your board's address
#define BOARD2_MAC {0x30, 0xC6, 0xF7, 0x2F, 0x38, 0x58}  // replace with your board's address

#define WIFI_CHAN  13 // 12-13 only legal in US in lower power mode, do not use 14
#define BAUD_RATE  57600
#define TX_PIN     1 // default UART0 is pin 1 (shared by USB)
#define RX_PIN     3 // default UART0 is pin 3 (shared by USB)
#define SER_PARAMS SERIAL_8N1 // SERIAL_8N1: start/stop bits, no parity

// Print estimations on the RSSI value
#define RSSI 0
#define RSSI_PKTS 10

#define BUFFER_SIZE 250 // max of 250 bytes
//#define DEBUG // for additional serial messages (may interfere with other messages)
// #define LED_BUILTIN whatever


///////////////////////////////////

#ifndef LED_BUILTIN
#define LED_BUILTIN 2  // some boards don't have an LED or have it connected elsewhere
#endif

#ifndef RSSI
#define RSSI 0
#endif

#ifndef BOARD
#define BOARD=1
#endif

#if BOARD==1
#pragma message "Compiling for BOARD 1!"
#define RECV_MAC BOARD2_MAC
#define SEND_MAC BOARD1_MAC
//#define BLINK_ON_SEND
//#define BLINK_ON_SEND_SUCCESS
//#define BLINK_ON_RECV
#else
#pragma message "Compiling for BOARD 2!"
#define RECV_MAC BOARD1_MAC
#define SEND_MAC BOARD2_MAC
//#define BLINK_ON_SEND
//#define BLINK_ON_SEND_SUCCESS
//#define BLINK_ON_RECV
#endif

#endif
