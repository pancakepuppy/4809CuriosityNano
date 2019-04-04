/*
 * SSD1306.c
 *
 * Created: 4/3/2019 12:20:44 PM
 *  Author: PancakePuppy (John White)
 */ 
#include "SSD1306.h"
#include "TWI.h"

// Initialization sequence for 128x32 OLED
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

// SSD1306_Init() sends the initialization sequence to the display. 
void SSD1306_Init()
{
	TWIM_Communicate(SSD1306_ADDR_W, init_seq, sizeof(init_seq));
	return;
}

void SSD1306_SetPage(uint8_t page)
{
	if(page > 7) // Page can only be 0 through 7
		return;
	if(TWIM_AddressPhase(SSD1306_ADDR_W))
		return; // Return if address phase fails
	TWIM_TransmitData(SSD1306_CTRL_CMD_SINGLE);
	TWIM_TransmitData(SSD1306_CMD_SET_PAGE_START + page);
	TWIM_EndTransaction();
	return;
}