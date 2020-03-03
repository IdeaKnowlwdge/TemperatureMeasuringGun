#ifndef __UART_DRIVER_H_
#define __UART_DRIVER_H_


#include "stm32f1xx.h"
#include <stdio.h>


//串口波特率
#define DEBUG_USART_BAUDRATE                    115200

//引脚定义
/*******************************************************/
#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK_ENABLE()                __HAL_RCC_USART1_CLK_ENABLE();

#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USART_RX_PIN                      GPIO_PIN_10

#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USART_TX_PIN                      GPIO_PIN_9

#define DEBUG_USART_IRQHandler                  USART1_IRQHandler
#define DEBUG_USART_IRQ                 		    USART1_IRQn
/************************************************************/


extern UART_HandleTypeDef UartHandle;



void Usart_SendString(uint8_t *str);
void USARTx_IintConfig(void);
int fputc(int ch, FILE *f);
void NVIC_Configuration(void);


#endif /* __USART1_H */


