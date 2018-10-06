 
/*******************************************************************************
 * File				:	HSS_UART_stm8.c
 * Description:	Wrapper for STM8 UART driver, 
 * Datum			:	2018.10.05
 * Version		:	1.0
 * Author			:	Harsimranjit Sindhar
 * Platform		:	STM8 STVD
 * CPU				:	STM8L152C6
 * IDE				:	ST Visual Develop
 * Compiler		:	COSMIC 
 * Pin Def  	:	PC2  = RX
 * 						: PC3  = TX
 * NOTE				: UART1_Handle_IT() called from Interrupt routine.
 * RX_FLAG  = it is SET in interrrupt routine i.e stm8l15x.c
*******************************************************************************/




/* Includes ------------------------------------------------------------------*/
#include "stm8l15x_conf.h" /* important */
#include "hss_uart_stm8.h"
#include "string.h"



/* Private variables ---------------------------------------------------------*/
#define END_FLAG '$'

short uartCounter = 0;
short BUFFER_COUNT= 0;
const uint8_t  BUFFER_SIZE = 32;
uint8_t  BUFFER_RX_Read[32];
bool  RX_FLAG = 0; 


 
//------------------------------------------------------------------------------
// Function Name : UART1_Init(uint32_t baudrate) 
// Description   : Setup UART1, and receive pin on interrupt Mode
// UART Pins     : PORTC: pin2-->TX, pin3-->RX
//------------------------------------------------------------------------------

void UART1_Init(uint32_t baudrate)
{
	CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
	
  USART_Init(USART1,  baudrate, USART_WordLength_8b, USART_StopBits_1,
                       USART_Parity_No, (USART_Mode_TypeDef) (USART_Mode_Tx|USART_Mode_Rx));
  USART_Cmd(USART1, ENABLE);
	
	/* Set PC3--->TX & PC2--->RX */
  GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);

  /* Enable the USART Receive interrupt: this interrupt is generated when the USART
     receive data register is not empty */
  USART_ITConfig(USART1, USART_IT_RXNE , ENABLE);
	
	/* Enable general interrupts */
  enableInterrupts();
}


//------------------------------------------------------------------------------
// Function Name :  UART1_CHECK_IT()
// Description   :  When UART interrupt complete, User can check for message
// Input         :  
//------------------------------------------------------------------------------

bool UART1_Check_Message(unsigned char *message)
{
	  int i;
		bool result = 0;
	  USART_FLAG_TypeDef TX_COM_FLAG; // Transmission complete flag
		
		if(RX_FLAG == 1)
		{
			 /* Clear the RX Flag */
			 RX_FLAG = 0;
			 
			 //printf("\r\n Data: %s \r\n", BUFFER_RX_Read);
			 for(i =0; i <= strlen(BUFFER_RX_Read); i++)
			 {
				 /* Reply Back on uart terminal */
				 USART_SendData8(USART1, BUFFER_RX_Read[i]);
				 
				 TX_COM_FLAG = RESET;
				 while(TX_COM_FLAG == RESET)
				 {
				   TX_COM_FLAG = USART_GetFlagStatus(USART1,  USART_FLAG_TC);
				 }
		   }
			 
			 /* Pass the address of store message */			  
			 strcpy(message, BUFFER_RX_Read);
			 result = 1;
		}
		
		return result;
}


//------------------------------------------------------------------------------
// Function Name :  UART1_Handle_IT()
// Description   :  when uart interupt occurs, data is processed here
// Input         :  
//------------------------------------------------------------------------------
void UART1_Handle_IT()
{
	 uint8_t recv;
	 recv = USART_ReceiveData8(USART1);
	 
   if(recv != END_FLAG) BUFFER_RX_Read[BUFFER_COUNT++] = recv;
	 	 
	 if((recv == END_FLAG) || (BUFFER_COUNT >= (BUFFER_SIZE-1))) 
	 {
	   RX_FLAG = 1;
	  		
		 /* add null for string termination */
		 BUFFER_RX_Read[BUFFER_COUNT] = '\0';	 
		 
		 BUFFER_COUNT = 0;
   
	 }else RX_FLAG = 0;
}


//------------------------------------------------------------------------------
// Description   :  Redirect printf to UART1
//------------------------------------------------------------------------------
PUTCHAR_PROTOTYPE
{
  /* Write a character to the USART */
  USART_SendData8(USART1, c);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

  return (c);
}

GETCHAR_PROTOTYPE
{
  int c = 0;
  /* Loop until the Read data register flag is SET */
  while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    c = USART_ReceiveData8(USART1);
    return (c);
}


