#include <stdint.h>
#ifndef KEYBOARD_H
#define KEYBOARD_H
#define PS2_DATA 0x60
#define PS2_CMD 0x64
#define PS2_STATUS PS2_CMD
#define PS2_STATUS_OUTPUT 1
#define PS2_STATUS_INPUT (1 << 1)
#define PS2_STATUS_OUTPUT 1
#define ENABLE_PORT_1 0xAE
#define DISABLE_PORT_1 0xAD
#define DISABLE_PORT_2 0xA7
#define CMD_GET_CONFIGURATION 0x20
#define CMD_SET_CONFIGURATION 0x60
#define SET_CONFIG 0b10101100 // disables bit 0, 1, 4, 6
#define CONTROLLER_TEST 0xAA
// keyboard stuff
#define ACK 0xFA
#define RESEND 0xFE
#define RESET 0xFF
#define SCAN_CODE 0xF0
#define SCAN_CODE_2 2
#define ECHO 0xEE
void initKeyboard(void);
static uint8_t getData();
static void sendData(uint8_t data);
static void initScanCodes(void);
char pollKeyboard();
#endif