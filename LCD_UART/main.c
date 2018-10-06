/*******************************************************************************
 * File				:	Nokia 51100 LCD with Uart communication
 * Description:	Example application for sending data from uart and display on LCD, 
 * Datum			:	2018.10.05
 * Version		:	1.0
 * Author			:	Harsimranjit Sindhar
 * Platform		:	STM8 STVD
 * CPU				:	STM8L152C6
 * IDE				:	ST Visual Develop
 * Compiler		:	COSMIC 
 
 Pin out for UART1
 * Pin Def  	:	PC2  = RX
 * 						: PC3  = TX
 * Note				: '$' is terminating char when data sent from PC
 
 Pin out for NOKIA 5110 LCD - PCD8544
 *						: PB0 = LED (BL Pin)
 * 						: PB1 = Reset (RST Pin, NSS)
 * 						: PB2 = ChipSelect (CE Pin)
 * 						: PB3 = Mode (DC Pin)
 * 						: PB5 = Clock (Clk Pin) - SCLK
 * 						: PB6 = Data (Din Pin ) - MOSI
 *  
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "stm8l15x_conf.h"
#include "pcd8544.h"
#include "delay.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


 
void main() {
	int n, col, row;
	char str[32];
	row = col = 1;
	
	/* System Clock */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1); // 16Mhz
	
	/* UART1 Setup */
	UART1_Init(115200);
	/* Start Program */
  printf("\n\rUSART STM8L DEMO \r\n");
	
	/* NOKIA 5110 PCD8544 LCD Init */
	nokia5110_init();
	
	/* Set LCD Contrast */
	nokia5110_setContrast(55);
	
	n   = 0;
	col = 1; //LCD Coloum
	row = 1; //LCD ROW
	
	nokia5110_clear();		
  nokia5110_gotoXY(col, row);
	nokia5110_writeString("Welcome!");
	
	while(1)
	{
		
		/* Check Uart message if any */
		if(UART1_Check_Message(str))
		{		  
		 nokia5110_clear();		
     nokia5110_gotoXY(col, row);
		 nokia5110_writeString(str);
	  }
		
		/* increment and display counter on LCD */
		n += 1;
		sprintf(str, "%d", n); 
		nokia5110_gotoXY(30, 4);
		nokia5110_writeString(str);
		delay_ms(1000);		
	}
}
