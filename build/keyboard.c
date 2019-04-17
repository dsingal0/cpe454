#include "keyboard.h"
#include "asm.h"
static char scancodeMap[0xFE] = {0};
static char scancodeMapShift[0xFE] = {0};
static void initScanCodes(void)
{
    scancodeMap[0x0D] = '\t';
    scancodeMap[0x0E] = '`';
    scancodeMap[0x15] = 'q';
    scancodeMap[0x16] = '1';
    scancodeMap[0x1A] = 'z';
    scancodeMap[0x1B] = 's';
    scancodeMap[0x1C] = 'a';
    scancodeMap[0x1D] = 'w';
    scancodeMap[0x1E] = '2';
    scancodeMap[0x21] = 'c';
    scancodeMap[0x22] = 'x';
    scancodeMap[0x23] = 'd';
    scancodeMap[0x24] = 'e';
    scancodeMap[0x25] = '4';
    scancodeMap[0x26] = '3';
    scancodeMap[0x29] = ' ';
    scancodeMap[0x2A] = 'v';
    scancodeMap[0x2B] = 'f';
    scancodeMap[0x2C] = 't';
    scancodeMap[0x2D] = 'r';
    scancodeMap[0x2E] = '5';
    scancodeMap[0x31] = 'n';
    scancodeMap[0x32] = 'b';
    scancodeMap[0x33] = 'h';
    scancodeMap[0x34] = 'g';
    scancodeMap[0x35] = 'y';
    scancodeMap[0x36] = '6';
    scancodeMap[0x3A] = 'm';
    scancodeMap[0x3B] = 'j';
    scancodeMap[0x3C] = 'u';
    scancodeMap[0x3D] = '7';
    scancodeMap[0x3E] = '8';
    scancodeMap[0x41] = ',';
    scancodeMap[0x42] = 'k';
    scancodeMap[0x43] = 'i';
    scancodeMap[0x44] = 'o';
    scancodeMap[0x45] = '0';
    scancodeMap[0x46] = '9';
    scancodeMap[0x49] = '.';
    scancodeMap[0x4A] = '/';
    scancodeMap[0x4B] = 'l';
    scancodeMap[0x4C] = ';';
    scancodeMap[0x4D] = 'p';
    scancodeMap[0x4E] = '-';
    scancodeMap[0x52] = '\'';
    scancodeMap[0x54] = '[';
    scancodeMap[0x55] = '=';
    scancodeMap[0x5A] = '\n';
    scancodeMap[0x5B] = ']';
    scancodeMap[0x5D] = '\\';

    scancodeMapShift[0x0D] = '\t';
    scancodeMapShift[0x0E] = '~';
    scancodeMapShift[0x15] = 'Q';
    scancodeMapShift[0x16] = '!';
    scancodeMapShift[0x1A] = 'Z';
    scancodeMapShift[0x1B] = 'S';
    scancodeMapShift[0x1C] = 'A';
    scancodeMapShift[0x1D] = 'W';
    scancodeMapShift[0x1E] = '@';
    scancodeMapShift[0x21] = 'C';
    scancodeMapShift[0x22] = 'X';
    scancodeMapShift[0x23] = 'D';
    scancodeMapShift[0x24] = 'E';
    scancodeMapShift[0x25] = '$';
    scancodeMapShift[0x26] = '#';
    scancodeMapShift[0x29] = ' ';
    scancodeMapShift[0x2A] = 'V';
    scancodeMapShift[0x2B] = 'F';
    scancodeMapShift[0x2C] = 'T';
    scancodeMapShift[0x2D] = 'R';
    scancodeMapShift[0x2E] = '%';
    scancodeMapShift[0x31] = 'N';
    scancodeMapShift[0x32] = 'B';
    scancodeMapShift[0x33] = 'H';
    scancodeMapShift[0x34] = 'G';
    scancodeMapShift[0x35] = 'Y';
    scancodeMapShift[0x36] = '^';
    scancodeMapShift[0x3A] = 'M';
    scancodeMapShift[0x3B] = 'J';
    scancodeMapShift[0x3C] = 'U';
    scancodeMapShift[0x3D] = '&';
    scancodeMapShift[0x3E] = '*';
    scancodeMapShift[0x41] = '<';
    scancodeMapShift[0x42] = 'K';
    scancodeMapShift[0x43] = 'I';
    scancodeMapShift[0x44] = 'O';
    scancodeMapShift[0x45] = ')';
    scancodeMapShift[0x46] = '(';
    scancodeMapShift[0x49] = '>';
    scancodeMapShift[0x4A] = '?';
    scancodeMapShift[0x4B] = 'L';
    scancodeMapShift[0x4C] = ':';
    scancodeMapShift[0x4D] = 'P';
    scancodeMapShift[0x4E] = '_';
    scancodeMapShift[0x52] = '"';
    scancodeMapShift[0x54] = '{';
    scancodeMapShift[0x55] = '+';
    scancodeMapShift[0x5A] = '\n';
    scancodeMapShift[0x5B] = '}';
    scancodeMapShift[0x5D] = '|';
}

void initKeyboard(void)
{
    initScanCodes();
    // disable ports
    outb(PS2_CMD,DISABLE_PORT_1);
    outb(PS2_CMD,DISABLE_PORT_2);
    // get current configuration
    outb(PS2_CMD,CMD_GET_CONFIGURATION);
    
}