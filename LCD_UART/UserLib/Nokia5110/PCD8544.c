/*******************************************************************************
 * File				:	PCD8544.c
 * Description:	Library for NOKIA 5110 LCD driver, PCD8544
 * Datum			:	2018.10.05
 * Version		:	1.0
 * Author			:	Aytac Dilek
 * Modified By:	Harsimranjit Sindhar
 * Platform		:	STM8 STVD
 * CPU				:	STM8L152C6
 * IDE				:	ST Visual Develop
 * Compiler		:	COSMIC 
 * Module			:	PCD8544 LCD driver for NOKIA 5110
 * Function		:	LCD Driver
 * Pin Definitions	:	
							PB0  = LED (BL Pin)
 * 						PB1 = Reset (RST Pin, NSS)
 * 						PB2 = ChipSelect (CE Pin)
 * 						PB3 = Mode (DC Pin)
 * 						PB5 = Clock (Clk Pin) - SCLK
 * 						PB6 = Data (Din Pin ) - MOSI
*******************************************************************************/



/* Includes *******************************************************************/
#include "stm8l15x.h"
#include "stm8l15x_conf.h"
#include "pcd8544.h"
#include "font_6x8.h"
#include "delay.h"



/* Internal Functions *********************************************************/
/** Hardware Control and Configuration Functions */
void nokia5110_spi_config(void);
void nokia5110_spi_writeByte(PCD8544_MODE_TypeDef mode, unsigned char data);



/*******************************************************************************
* Function Name	: 	nokia5110_init
* Description	: 	Initialize LCD
* Input			: 	None
* Output		: 	None
* Return		: 	None
* Attention		: 	None
*******************************************************************************/
void nokia5110_init(void){
 
	/* Configure spi pins */
	nokia5110_spi_config();

	/* Set pin initial state */
	NOKIA5110_LED_ON(); 		// Turn back light off
	NOKIA5110_DC_HIGH(); 		// Mode = command;
	NOKIA5110_DIN_HIGH(); 	// Set In at high level;
	NOKIA5110_CLK_HIGH(); 	// Set CLK high;
	NOKIA5110_CE_HIGH(); 		// Unselect chip;

	/* Reset the LCD to a known state */
	NOKIA5110_RST_LOW();		// Set LCD reset = 0;
	delay_ms(10);		        // Keep reset pin low for 10 ms
	NOKIA5110_RST_HIGH();		// LCD_RST = 1;

	/* Configure LCD module */
	nokia5110_spi_writeByte(PCD8544_MODE_Command, 0x21);		// Extended instruction set selected
	nokia5110_spi_writeByte(PCD8544_MODE_Command, 0xC4); 		// Set LCD voltage (defined by experimentation...)
	nokia5110_spi_writeByte(PCD8544_MODE_Command, 0x14);		// Set Bias for 1/48
	nokia5110_spi_writeByte(PCD8544_MODE_Command, 0x04);		// Set temperature control (TC2)
	nokia5110_spi_writeByte(PCD8544_MODE_Command, 0x20);		// Revert to standard instruction set
	nokia5110_clear(); 									// Clear display (still off)
	nokia5110_spi_writeByte(PCD8544_MODE_Command, 0x0c);		// Set display on in "normal" mode (not inversed)
}



/*******************************************************************************
* Function Name	: 	nokia5110_clear
* Description	: 	Clear display. Write 0 in all memory location.
* Input			: 	None
* Output		: 	None
* Return		: 	None
* Attention		: 	None
*******************************************************************************/
void nokia5110_clear(void){
	unsigned char i, j;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 84; j++) {
			nokia5110_spi_writeByte(PCD8544_MODE_Data, 0x00);
		}
	}
}
 

/*******************************************************************************
* Function Name	: 	nokia5110_setPosition
* Description	: 	Set memory current location for characters (set coordinates).
* Input			: 	X => Column (range from 0 to 13)
* 					    Y => Row (range from 0 to 5)
* Output		: 	None
* Return		: 	None
* Attention		: 	Applies only for Fonts with a 6 pixels width.
*******************************************************************************/
void nokia5110_gotoXY(unsigned char x, unsigned char y){
	
	nokia5110_spi_writeByte(PCD8544_MODE_Command, 0x40 | y); // Row
	nokia5110_spi_writeByte(PCD8544_MODE_Command, 0x80 | x); // Coloumn
}



/*******************************************************************************
* Function Name	: 	nokia5110_writeChar
* Description	: 	Write character to LCD at current position
* Input			: 	c => char to write
* Output		: 	None
* Return		: 	None
* Attention		: 	None
*******************************************************************************/
void nokia5110_writeChar(unsigned char c){
	unsigned char line;
	unsigned char ch = 0;

	c = c - 32;

	for (line = 0; line < 6; line++) {
		ch = font6_8[c][line];
		nokia5110_spi_writeByte(PCD8544_MODE_Data, ch);
	}
}



/*******************************************************************************
* Function Name	: 	nokia5110_writeCharInv
* Description	: 	Write character to LCD in inverse video at current location
* Input			: 	c => char to write
* Output		: 	None
* Return		: 	None
* Attention		: 	None
*******************************************************************************/
void nokia5110_writeCharInv(unsigned char c) {
	unsigned char line;
	unsigned char ch = 0;

	c = c - 32;

	for (line = 0; line < 6; line++) {
		ch = ~font6_8[c][line];
		nokia5110_spi_writeByte(PCD8544_MODE_Data, ch);
	}
}



/*******************************************************************************
* Function Name	: 	nokia5110_writeString
* Description	: 	Write string to LCD at current position.
* Input			: 	s => string pointer
* Output		: 	None
* Return		: 	None
* Attention		: 	String must be null terminated.
*******************************************************************************/
void nokia5110_writeString(unsigned char *s){
	unsigned char ch;

	while (*s != '\0') {
		ch = *s;
		nokia5110_writeChar(ch);
		s++;
	}
}



/*******************************************************************************
* Function Name	: 	nokia5110_setContrast
* Description	: 	Set LCD Contrast
* Input			: 	contrast =>
* Output		: 	None
* Return		: 	None
* Attention		: 	None
*******************************************************************************/
void nokia5110_setContrast(unsigned char contrast){
	/*  LCD Extended Commands. */
	nokia5110_spi_writeByte(PCD8544_MODE_Command, 0x21);

	/* Set LCD Vop (Contrast). */
	nokia5110_spi_writeByte(PCD8544_MODE_Command, 0x80 | contrast);

	/*  LCD Standard Commands, horizontal addressing mode. */
	nokia5110_spi_writeByte(PCD8544_MODE_Command, 0x20);
}



/* Internal Functions *********************************************************/
/*******************************************************************************
* Function Name	: 	nokia5110_spi_config
* Description	: 	Configure SPI pins (Hardware SPI)
* Input			: 	None
* Output		: 	None
* Return		: 	None
* Attention	: 	
*						PB0  : LED control
* 					PB1 : RESET
* 					PB2 : ChipSelect
* 					PB3 : DC (Mode)
* 					PB5 : SPI-1_CLK
* 					PB6 : SPI-1_MOSI
* 					PB7 : SPI-1_MISO
*******************************************************************************/
void nokia5110_spi_config(void){
	int index;
	/* --------------------------------------------------- */
	/* Enable SPI clock */
  CLK_PeripheralClockConfig(PCD8544_SPI_CLOCK, ENABLE);
  /* Enable DMA clock */
  //CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
	
	/* Enable LED Pin */
	GPIO_Init(PCD8544_LED_PORT, PCD8544_LED_PIN, GPIO_Mode_Out_PP_High_Fast);
	/* Enable Reset Pin */
	GPIO_Init(PCD8544_RST_PORT, PCD8544_RST_PIN, GPIO_Mode_Out_PP_High_Fast);
	/* Enable CE Pin */
	GPIO_Init(PCD8544_CE_PORT, PCD8544_CE_PIN, GPIO_Mode_Out_PP_High_Fast);
	/* Enable DC Pin */
	GPIO_Init(PCD8544_DC_PORT, PCD8544_DC_PIN, GPIO_Mode_Out_PP_High_Fast);
	
	/* Set the MOSI and SCK at high level */
  GPIO_ExternalPullUpConfig(GPIOB, PCD8544_MOSI_PIN | PCD8544_CLK_PIN, ENABLE);
	
	 /* SPI configuration -------------------------------------------*/
  SPI_DeInit(SPI1);
  SPI_Init(PCD8544_SPI_PER, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_4, SPI_Mode_Master,
           SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_Direction_Tx,
           SPI_NSS_Soft, (uint8_t)0x07);	
	
	/* DMA on Tx configuration -------------------------------------------*/
	//SPI_DMACmd(PCD8544_SPI_PER, SPI_DMAReq_TX, ENABLE);

  /* Insert Delay to ensure Synchro w/ SPI */
  //for (index = 0; index < 0xFF; index++);

  /* Enable SPI */
  SPI_Cmd(PCD8544_SPI_PER, ENABLE);
}



/*******************************************************************************
* Function Name	: 	nokia5110_spi_writeByte
* Description	: 	Write byte to the module.
* Input			: 	mode => 0 if command, 1 if data
* 					data => data to write
* Output		: 	None
* Return		: 	None
* Attention		: 	None
*******************************************************************************/
void nokia5110_spi_writeByte(PCD8544_MODE_TypeDef mode, unsigned char data){
	
	NOKIA5110_CE_LOW();			// SPI_CS = 0;
	
	if(mode == PCD8544_MODE_Command){
		NOKIA5110_DC_LOW();
	}
	else{
		NOKIA5110_DC_HIGH();
	}

	/* Loop while DR register in not empty */
	while (SPI_GetFlagStatus(PCD8544_SPI_PER, SPI_FLAG_TXE) == RESET);
	/* Send a Byte through the SPI peripheral */
	SPI_SendData(PCD8544_SPI_PER, data);
	/* Be sure that the character goes to the shift register */
	while (SPI_GetFlagStatus(PCD8544_SPI_PER, SPI_FLAG_TXE) == RESET);
	/* Wait until entire byte has been read (which we discard anyway) */
  //	while( SPI_I2S_GetFlagStatus(PCD8544_SPI_PER, SPI_I2S_FLAG_RXNE) != SET );
   
	NOKIA5110_CE_HIGH();	// SPI_CS = 1;

	
}