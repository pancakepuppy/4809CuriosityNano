/*
 * SSD1306.h
 *
 * Created: 4/3/2019 12:20:55 PM
 *  Author: labrat
 */ 


#ifndef SSD1306_H_
#define SSD1306_H_
// Notes
/*
Reset conditions:
o	Contrast = 0x7F
o	RAM contents to display
o	Normal (Non-Inverted) display
o	Display Off
	Addressing Mode = Page Addressing Mode
o	Page Addressing Mode
o		Lower Column Start Address = 0x0
o		Higher Column Start Address = 0x0
	Horz/Vert Addressing Mode
o		Column Start Address = 0
o		Column End Address = 127
x		Page Start Address = 0
x		Page End Address = 7 (Set to 3 for 128x32 OLED?)
o	Display Start Line = 0x00
o	Column Address 0 mapped to SEG0 (Experiment with this)
x	MUX ratio = 64 (Set to 32 (0x1F) for 128x32 OLED?)
	COM Output Scan Direction scans from COM0 to COM[N-1]
	Display Offset = 0x00
	Alternative COM pin configuration
	Disable COM Left/Right remap
	Display clock divide ratio = 0 (ratio 1)
	Oscillator frequency = 0x8
	Phase 1 period 2*DCLK
	Phase 2 period 2*DCLK
	VCOMH deselect level = .77*Vcc
*/
// Defines
#define SSD1306_ADDR ( 0x3C << 1 ) // SSD1306 at 0x3C 7-bit slave addr
#define SSD1306_ADDR_R ( SSD1306_ADDR | TWI_READ )
#define SSD1306_ADDR_W ( SSD1306_ADDR | TWI_WRITE )

#define SSD1306_CTRL_CMD_SINGLE 0x80
#define SSD1306_CTRL_CMD_STREAM 0x00
#define SSD1306_CTRL_DATA_STREAM 0x40

#define SSD1306_CMD_SET_CONTRAST_CONTROL 0x81 // Double byte command, send 8 bits for contrast after this command
#define SSD1306_CMD_OUTPUT_RAM 0xA4
#define SSD1306_CMD_OUTPUT_ALL 0xA5
#define SSD1306_CMD_DISPLAY_INVERSE 0xA7
#define SSD1306_CMD_DISPLAY_NORMAL 0xA6
#define SSD1306_CMD_DISPLAY_ON 0xAF
#define SSD1306_CMD_DISPLAY_OFF 0xAE

#define SSD1306_CMD_SET_MEMORY_ADDR_MODE 0x20
#define SSD1306_CMD_SET_COLUMN_RANGE 0x21 // Use in Horz/Vert mode, follow with 0x00 and 0x127 for 128 column display
#define SSD1306_CMD_SET_PAGE_RANGE 0x22 // Use in Horz/Vert mode, follow with 0x00 and 0x03 for 32 row display
#define SSD1306_CMD_SET_PAGE_START 0xB0 // OR with PAGE0-PAGE3

#define SSD1306_CMD_SET_DISPLAY_START_LINE 0x40
#define SSD1306_CMD_SET_SEGMENT_REMAP 0xA1
#define SSD1306_CMD_SET_MUX_RATIO 0xA8 // Follow with 0x3F for 64 MUX
#define SSD1306_CMD_SET_COM_SCAN_MODE 0xC8
#define SSD1306_CMD_SET_DISPLAY_OFFSET 0xD3 // Follow with 0x00
#define SSD1306_CMD_SET_COM_PIN_MAP 0xDA

#define SSD1306_CMD_SET_DISPLAY_CLK_DIV 0xD5 // Follow with 0x80
#define SSD1306_CMD_SET_PRECHARGE 0xD9 // Follow with 0x22
#define SSD1306_CMD_SET_VCOMH_DESELECT 0xDB // Follow with 0x30

#define SSD1306_CMD_SET_CHARGE_PUMP 0x8D // Follow with 0x14

#define SSD1306_CMD_NOP 0xE3

// Data structures
uint8_t init_seq[] = {SSD1306_CTRL_CMD_STREAM,
	SSD1306_CMD_DISPLAY_OFF,
	SSD1306_CMD_SET_DISPLAY_CLK_DIV, 0x80,
	SSD1306_CMD_SET_MUX_RATIO, 0x1F,
	SSD1306_CMD_SET_DISPLAY_OFFSET, 0x00,
	SSD1306_CMD_SET_DISPLAY_START_LINE | 0x0,
	SSD1306_CMD_SET_MEMORY_ADDR_MODE, 0x02,
	SSD1306_CMD_SET_SEGMENT_REMAP | 0x01,
	SSD1306_CMD_SET_COM_SCAN_MODE,
	SSD1306_CMD_SET_COM_PIN_MAP, 0x02,
	SSD1306_CMD_SET_CONTRAST_CONTROL, 0x7F,
	SSD1306_CMD_SET_PRECHARGE, 0xF1,
	SSD1306_CMD_SET_VCOMH_DESELECT, 0x40,
	SSD1306_CMD_SET_CHARGE_PUMP, 0x14,
	SSD1306_CMD_OUTPUT_RAM,
	SSD1306_CMD_DISPLAY_NORMAL,
SSD1306_CMD_DISPLAY_ON};

uint8_t blank_seq[] = {SSD1306_CTRL_DATA_STREAM,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00};

uint8_t dither_seq[] = {SSD1306_CTRL_DATA_STREAM,
	0xaa, 0x55, 0xaa, 0x55,
	0xaa, 0x55, 0xaa, 0x55,
	0xaa, 0x55, 0xaa, 0x55,
	0xaa, 0x55, 0xaa, 0x55};

#endif /* SSD1306_H_ */