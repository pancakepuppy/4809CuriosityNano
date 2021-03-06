/*
 * SSD1306.h
 *
 * Created: 4/3/2019 12:20:55 PM
 *  Author: PancakePuppy (John White)
 */ 


#ifndef SSD1306_H_
#define SSD1306_H_
#include <stdint.h>

// Function Prototypes
void SSD1306_Init();
void SSD1306_SetPage(uint8_t page);
void SSD1306_SetColumn(uint8_t addr);
void SSD1306_Fill(uint8_t data);
void SSD1306_PageWrite(uint8_t *data, uint8_t length, uint8_t page, uint8_t coloffset);
void SSD1306_SetAddrMode(uint8_t addrmode);
void SSD1306_SetColumnRange(uint8_t colfrom, uint8_t colto);
void SSD1306_SetPageRange(uint8_t pagefrom, uint8_t pageto);
void SSD1306_DrawImage(uint8_t *data, uint8_t width, uint8_t height, uint8_t offset);

// Display Address Defines
#define SSD1306_ADDR ( 0x3C << 1 ) // SSD1306 at 0x3C 7-bit slave addr
#define SSD1306_ADDR_R ( SSD1306_ADDR | TWI_READ )
#define SSD1306_ADDR_W ( SSD1306_ADDR | TWI_WRITE )

// Display Control Byte Defines
#define SSD1306_CTRL_CMD_SINGLE 0x80
#define SSD1306_CTRL_CMD_STREAM 0x00
#define SSD1306_CTRL_DATA_STREAM 0x40

// Fundamental Command Table
#define SSD1306_CMD_SET_CONTRAST_CONTROL 0x81 // Double byte command, send 8 bits for contrast after this command
#define SSD1306_CMD_DISPLAY_RAM_CONTENTS 0xA4
#define SSD1306_CMD_ENTIRE_DISPLAY_ON 0xA5
#define SSD1306_CMD_DISPLAY_INVERSE 0xA7
#define SSD1306_CMD_DISPLAY_NORMAL 0xA6
#define SSD1306_CMD_DISPLAY_ON 0xAF
#define SSD1306_CMD_DISPLAY_OFF 0xAE

// Addressing Setting Command Table
#define SSD1306_CMD_SET_LOWER_COLUMN_ADDR 0x00 // OR with low nibble
#define SSD1306_CMD_SET_UPPER_COLUMN_ADDR 0x10 // OR with low nibble
#define SSD1306_CMD_SET_MEMORY_ADDR_MODE 0x20 // Follow with mode bits
#define SSD1306_CMD_SET_COLUMN_RANGE 0x21 // Use in Horz/Vert mode, follow with 0x00 and 0x127 for 128 column display
#define SSD1306_CMD_SET_PAGE_RANGE 0x22 // Use in Horz/Vert mode, follow with 0x00 and 0x03 for 32 row display
#define SSD1306_CMD_SET_PAGE_START 0xB0 // OR with 0-7 for page number

// Hardware Configuration Command Table
#define SSD1306_CMD_SET_DISPLAY_RAM_START_LINE 0x40 // OR with 6 bits to set display RAM start line
#define SSD1306_CMD_SET_SEGMENT_NO_REMAP 0xA0
#define SSD1306_CMD_SET_SEGMENT_REMAP 0xA1
#define SSD1306_CMD_SET_MUX_RATIO 0xA8 // Follow with 0x3F for 64 MUX
#define SSD1306_CMD_SET_COM_SCAN_REMAPPED 0xC8
#define SSD1306_CMD_SET_COM_SCAN_NORMAL 0xC0
#define SSD1306_CMD_SET_DISPLAY_OFFSET 0xD3 // Vertical shift by COM, follow with 6 bits for shift
#define SSD1306_CMD_SET_COM_PIN_MAP 0xDA
#define SSD1306_CMD_SET_CHARGE_PUMP 0x8D // Follow with 0x14

// Timing & Driving Scheme Command Table
#define SSD1306_CMD_SET_DISPLAY_CLK_DIV 0xD5 // Follow with 0x80
#define SSD1306_CMD_SET_PRECHARGE 0xD9 // Follow with 0x22
#define SSD1306_CMD_SET_VCOMH_DESELECT 0xDB // Follow with 0x30
#define SSD1306_CMD_NOP 0xE3

// Address Mode Defines
#define SSD1306_ADDRMODE_HORZ 0x00
#define SSD1306_ADDRMODE_VERT 0x01
#define SSD1306_ADDRMODE_PAGE 0x02
#define SSD1306_ADDRMODE_INVALID 0x03

// Scrolling Command Table
#define SSD1306_CMD_CONT_HORZ_SCROLL_RIGHT_SETUP  0x26
#define SSD1306_CMD_CONT_HORZ_SCROLL_LEFT_SETUP 0x27

#endif /* SSD1306_H_ */