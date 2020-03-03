#include "Usart_Driver.h"
#include "include.h"


UART_HandleTypeDef UartHandle;
//extern uint8_t ucTemp;  

 /**
  * @brief  DEBUG_USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */  
void USARTx_IintConfig(void)
{ 
  
  UartHandle.Instance          = DEBUG_USART;             //选择串口
  
  UartHandle.Init.BaudRate     = DEBUG_USART_BAUDRATE;    //波特率  115200
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;      //8个数据位  
  UartHandle.Init.StopBits     = UART_STOPBITS_1;         //1个停止位
  UartHandle.Init.Parity       = UART_PARITY_NONE;        //没有校验位
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;     //没有硬件流控
  UartHandle.Init.Mode         = UART_MODE_TX_RX;         //工作模式
  
  HAL_UART_Init(&UartHandle);                             //初始化串口
    
 /*使能串口接收中断 */
  __HAL_UART_ENABLE_IT(&UartHandle,UART_IT_RXNE);  
}


/**
  * @brief UART MSP 初始化 
  * @param huart: UART handle
  * @retval 无
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  DEBUG_USART_CLK_ENABLE();             //打开串口时钟
	
	DEBUG_USART_RX_GPIO_CLK_ENABLE();     //打开接收引脚时钟
  DEBUG_USART_TX_GPIO_CLK_ENABLE();     //打开发送引脚时钟
  
/**USART1 GPIO Configuration    
  PA9     ------> USART1_TX
  PA10    ------> USART1_RX 
  */
  /* 配置Tx引脚为复用功能  */
  GPIO_InitStruct.Pin = DEBUG_USART_TX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed =  GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* 配置Rx引脚为复用功能 */
  GPIO_InitStruct.Pin = DEBUG_USART_RX_PIN;
  GPIO_InitStruct.Mode=GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！	
  HAL_GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct); 
 
  HAL_NVIC_SetPriority(DEBUG_USART_IRQ ,0,1);	//抢占优先级0，子优先级1
  HAL_NVIC_EnableIRQ(DEBUG_USART_IRQ );		    //使能USART1中断通道  
}


/*****************  发送字符串 **********************/
void Usart_SendString(uint8_t *str)
{
	unsigned int k=0;
  do 
  {
      HAL_UART_Transmit(&UartHandle,(uint8_t *)(str + k) ,1,1000);
      k++;
  } while(*(str + k)!='\0');
  
}

//重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口DEBUG_USART */
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);	
	
	return (ch);
}


/*******************************************
**	函数名 : NVIC_Configuration
**	功  能 : 选择中断的组别							
**	参  数 : 无	
**  返回值 : 无
********************************************/
void NVIC_Configuration(void)
{
	
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级

}


