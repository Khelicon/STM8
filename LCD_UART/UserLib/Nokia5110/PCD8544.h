/*******************************************************************************
* File  		:	PCD8544.h
* Description	: 	Library for NOKIA 5110 LCD driver, PCD8544
* Author		: 	Aytac Dilek 
* Modified by	:	Harsimranjit Sindhar
* Note			: 	GNU General Public License, version 3 (GPL-3.0)
*******************************************************************************/
#ifndef __PCD8544_H
#define __PCD8544_H


/* Includes ********************************************************************/
#include "stm8l15x.h"
#include "stm8l15x_conf.h"



/* Defines *********************************************************************/
#define PCD8544_SPI_PER			SPI1
#define PCD8544_SPI_PORT		GPIOB
#define PCD8544_SPI_CLOCK		CLK_Peripheral_SPI1
#define PCD8544_GPIO_PORT		GPIOB
#define PCD8544_GPIO_CLOCK	RCC_APB2Periph_GPIOB

#define PCD8544_LED_PORT 		GPIOB
#define PCD8544_LED_PIN 		GPIO_Pin_0

#define PCD8544_RST_PORT 		GPIOB
#define PCD8544_RST_PIN 		GPIO_Pin_1

#define PCD8544_CE_PORT			GPIOB
#define PCD8544_CE_PIN 			GPIO_Pin_2

#define PCD8544_DC_PORT 		GPIOB
#define PCD8544_DC_PIN 			GPIO_Pin_3

#define PCD8544_CLK_PORT		GPIOB
#define PCD8544_CLK_PIN			GPIO_Pin_5
#define PCD8544_MOSI_PORT		GPIOB
#define PCD8544_MOSI_PIN		GPIO_Pin_6
#define PCD8544_MISO_PORT		GPIOB
#define PCD8544_MISO_PIN		GPIO_Pin_7


#define PCD8544_WIDTH			  84
#define PCD8544_HEIGHT			48



/* Macros **********************************************************************/
#define NOKIA5110_LED_ON()		GPIO_SetBits(PCD8544_LED_PORT, PCD8544_LED_PIN);
#define NOKIA5110_LED_OFF()		GPIO_ResetBits(PCD8544_LED_PORT, PCD8544_LED_PIN);
#define NOKIA5110_RST_LOW()		GPIO_ResetBits(PCD8544_RST_PORT, PCD8544_RST_PIN)
#define NOKIA5110_RST_HIGH()	GPIO_SetBits(PCD8544_RST_PORT, PCD8544_RST_PIN)
#define NOKIA5110_DC_LOW()		GPIO_ResetBits(PCD8544_DC_PORT, PCD8544_DC_PIN)
#define NOKIA5110_DC_HIGH()		GPIO_SetBits(PCD8544_DC_PORT, PCD8544_DC_PIN)
#define NOKIA5110_DIN_LOW()		GPIO_ResetBits(PCD8544_MOSI_PORT, PCD8544_MOSI_PIN)
#define NOKIA5110_DIN_HIGH()	GPIO_SetBits(PCD8544_MOSI_PORT, PCD8544_MOSI_PIN)
#define NOKIA5110_CLK_LOW()		GPIO_ResetBits(PCD8544_CLK_PORT, PCD8544_CLK_PIN)
#define NOKIA5110_CLK_HIGH()	GPIO_SetBits(PCD8544_CLK_PORT, PCD8544_CLK_PIN)
#define NOKIA5110_CE_LOW()		GPIO_ResetBits(PCD8544_CE_PORT, PCD8544_CE_PIN)
#define NOKIA5110_CE_HIGH()		GPIO_SetBits(PCD8544_CE_PORT, PCD8544_CE_PIN)



/* Enumarations ****************************************************************/
typedef enum{
	PCD8544_MODE_Command = 0,
	PCD8544_MODE_Data = 1
}PCD8544_MODE_TypeDef;



/* Global Functions ************************************************************/
void nokia5110_init(void);
void nokia5110_writeChar(unsigned char c);
void nokia5110_writeCharInv(unsigned char c);
void nokia5110_writeString(unsigned char *s);
void nokia5110_clear(void);
void nokia5110_gotoXY(unsigned char x, unsigned char y);
void nokia5110_setContrast(unsigned char contrast);



//--------------------------------------------------------------
#endif // __PCD8544_H