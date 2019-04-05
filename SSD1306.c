/*
 * SSD1306.c
 *
 * Created: 4/3/2019 12:20:44 PM
 *  Author: PancakePuppy (John White)
 */ 
#include "SSD1306.h"
#include "TWI.h"
#define F_CPU 3333333UL
#include <util/delay.h>

// Initialization sequence for 128x32 OLED
uint8_t init_seq[] = {SSD1306_CTRL_CMD_STREAM,
	SSD1306_CMD_DISPLAY_OFF, // Display OFF (sleep mode)
	SSD1306_CMD_SET_DISPLAY_CLK_DIV, 0x80, // Set Display Clock Divide Ratio/Oscillator Frequency; Divide Ratio = 1, Oscillator Frequency = 0x8 (0x0 to 0xF)
	SSD1306_CMD_SET_MUX_RATIO, 0x1F, // Set Multiplex Ratio; 32MUX
	SSD1306_CMD_SET_DISPLAY_OFFSET, 0x00, // Set vertical shift by COM; Offset = 0
	SSD1306_CMD_SET_DISPLAY_RAM_START_LINE | 0x0, // Set display RAM start line register; Start line = 0
	SSD1306_CMD_SET_MEMORY_ADDR_MODE, 0x02, // Set memory addressing mode; 0 for Horizontal, 1 for Vertical, 2 for Page Mode
	SSD1306_CMD_SET_SEGMENT_REMAP | 0x01, // Set segment re-map; Column address 127 is mapped to SEG0
	SSD1306_CMD_SET_COM_SCAN_REMAPPED, // Set COM output scan direction; Remapped mode, scan from COM31 to COM0
	SSD1306_CMD_SET_COM_PIN_MAP, 0x02, // Set COM pins hardware configuration; Sequential COM pin configuration + Disable COM left/right remap
	SSD1306_CMD_SET_CONTRAST_CONTROL, 0x7F, // Set contrast control; Contrast = 0x7F (Unsure if this actually does anything)
	SSD1306_CMD_SET_PRECHARGE, 0xF1, // Set pre-charge period; Phase 1 = 1*DCLK, Phase 2 = 15*DCLK
	SSD1306_CMD_SET_VCOMH_DESELECT, 0x20, // Set Vcomh deselect level; Level = 0.77*Vcc
	SSD1306_CMD_SET_CHARGE_PUMP, 0x14, // Charge pump setting; Enable charge pump during display on
	SSD1306_CMD_DISPLAY_RAM_CONTENTS, // Display RAM contents
	SSD1306_CMD_DISPLAY_NORMAL, // Display normal (non-inverted)
	SSD1306_CMD_DISPLAY_ON // Display ON
};


// SSD1306_Init() sends the initialization sequence to the display. 
void SSD1306_Init()
{
	TWIM_Communicate(SSD1306_ADDR_W, init_seq, sizeof(init_seq));
}

void SSD1306_SetPage(uint8_t page)
{
	if(TWIM_AddressPhase(SSD1306_ADDR_W))
		return; // Return if address phase fails
	TWIM_TransmitData(SSD1306_CTRL_CMD_SINGLE);
	TWIM_TransmitData(SSD1306_CMD_SET_PAGE_START + page);
	TWIM_EndTransaction();
}

void SSD1306_SetColumn(uint8_t addr)
{
	if(TWIM_AddressPhase(SSD1306_ADDR_W))
		return; // Return if address phase fails
	TWIM_TransmitData(SSD1306_CTRL_CMD_STREAM);
	TWIM_TransmitData(SSD1306_CMD_SET_LOWER_COLUMN_ADDR | (addr & 0x0f)); // Lower nibble
	TWIM_TransmitData(SSD1306_CMD_SET_UPPER_COLUMN_ADDR | (addr >> 4)); // Upper nibble
	TWIM_EndTransaction();
}

void SSD1306_Fill(uint8_t data)
{
	uint8_t page;
	uint8_t col;
	for(page = 0; page < 4; page++)
	{
		SSD1306_SetPage(page);
		SSD1306_SetColumn(0);
		if(TWIM_AddressPhase(SSD1306_ADDR_W))
			return;
		TWIM_TransmitData(SSD1306_CTRL_DATA_STREAM);
		for(col = 0; col < 128; col++)
		{
			TWIM_TransmitData(data);
		}
		TWIM_EndTransaction();
	}
}

// SSD1306_PageWrite writes length bytes of *data onto display page page starting at column coloffset
// Use this function in Page addressing mode
void SSD1306_PageWrite(uint8_t *data, uint8_t length, uint8_t page, uint8_t coloffset)
{
	if((length + coloffset) > 128 || page > 3)
		return;
	SSD1306_SetPage(page);
	SSD1306_SetColumn(coloffset);
	if(TWIM_AddressPhase(SSD1306_ADDR_W)) // Begin TWI transaction
		return; // Return if address phase fails
	TWIM_TransmitData(SSD1306_CTRL_DATA_STREAM); // Tell display to expect data stream
	for(uint8_t i=0; i < length; i++)
	{
		TWIM_TransmitData(*(data+i));
	}
	TWIM_EndTransaction(); // End the TWI transaction
}

// SSD1306_SetAddrMode sets the addressing mode
// Use SSD1306_ADDRMODE_* defines as arguments
void SSD1306_SetAddrMode(uint8_t addrmode)
{
	if(addrmode > 2)
		return; // Only values up to 2 are acceptable as address modes. This check probably optimizes away
	if(TWIM_AddressPhase(SSD1306_ADDR_W))
		return; // Address phase failed
	TWIM_TransmitData(SSD1306_CTRL_CMD_STREAM);
	TWIM_TransmitData(SSD1306_CMD_SET_MEMORY_ADDR_MODE);
	TWIM_TransmitData(addrmode);
	TWIM_EndTransaction();	
}

void SSD1306_SetColumnRange(uint8_t colfrom, uint8_t colto)
{
	if(colfrom > 127 || colto > 127)
		return;
	if(TWIM_AddressPhase(SSD1306_ADDR_W))
		return; // Address phase failed
	TWIM_TransmitData(SSD1306_CTRL_CMD_STREAM);
	TWIM_TransmitData(SSD1306_CMD_SET_COLUMN_RANGE);
	TWIM_TransmitData(colfrom);
	TWIM_TransmitData(colto);
	TWIM_EndTransaction();
}

void SSD1306_SetPageRange(uint8_t pagefrom, uint8_t pageto)
{
	if(pagefrom > 7 || pageto > 7)
		return;
	if(TWIM_AddressPhase(SSD1306_ADDR_W))
		return; // Address phase failed
	TWIM_TransmitData(SSD1306_CTRL_CMD_STREAM);
	TWIM_TransmitData(SSD1306_CMD_SET_PAGE_RANGE);
	TWIM_TransmitData(pagefrom);
	TWIM_TransmitData(pageto);
	TWIM_EndTransaction();
}

void SSD1306_DrawImage(uint8_t *data, uint8_t width, uint8_t height, uint8_t offset)
{
	if((width+offset) > 128 || height > 32)
		return;
	SSD1306_PageWrite(data, width, 0, offset);
	if(width > 8)
		SSD1306_PageWrite((data+width), width, 1, offset);
	if(width > 16)
		SSD1306_PageWrite((data+2*width), width, 2, offset);
	if(width > 24)
		SSD1306_PageWrite((data+3*width), width, 3, offset);
	return;
}