
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HSS_UART_STM8_H
#define __HSS_UART_STM8_H

void UART1_Init(uint32_t baudrate);
bool UART1_Check_Message(unsigned char *message);
void UART1_Handle_IT(void);

#define PUTCHAR_PROTOTYPE char putchar (char c)
#define GETCHAR_PROTOTYPE char getchar (void)

#endif /* __HSS_UART_STM8_H */