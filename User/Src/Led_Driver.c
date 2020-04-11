#include "Led_Driver.h"


/***********************************************
**	函数名 : Led_Init
**	功  能 : 初始化GPIOA口的8/11/12引脚
**  参  数 : 无
**	返回值 : 无
************************************************/


void Led_Init(void)
{

    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef  GPIO_InitStruct;

    /*开启RGBLED相关的GPIO外设时钟*/
    __HAL_RCC_GPIOA_CLK_ENABLE();


    /*选择要控制的GPIO引脚*/
    GPIO_InitStruct.Pin = REDLED_GPIO_PIN|GREENLED_GPIO_PIN|BLUELED_GPIO_PIN;

    /*设置引脚的输出类型为推挽输出*/
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;

    /*设置引脚为上拉模式*/
    GPIO_InitStruct.Pull  = GPIO_PULLUP;

    /*设置引脚速率为高速 */
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
    HAL_GPIO_Init(RGBLED_GPIO_PORT, &GPIO_InitStruct);

    /*关闭RGB灯*/
    All_LedOff();

}


/*******************************************
**	函数名 : Led_Ctl
**	功  能 : 控制LED三色灯
**	参  数 : 参数1、灯的类型  参数2、灯的状态
**  返回值 : 无
********************************************/
void Led_Ctl(LED_Type Led_Type, uint8_t Led_State)
{

    switch(Led_Type)
    {
    case Red:

        if(Led_State == ON)      //红灯的控制开关
        {
            RED_LED(ON);          //红灯开
        } else
            RED_LED(OFF);         //红灯关

        break;

    case Green:

        if(Led_State == ON)      //红灯的控制开关
        {
            GREEN_LED(ON) ;           //绿灯开
        } else
            GREEN_LED(OFF);           //绿灯关

        break;

    case Blue:

        if(Led_State == ON)      //蓝灯的控制开关
        {
            BLUE_LED(ON);           //蓝灯开
        } else
            BLUE_LED(OFF);         //蓝灯关

        break;
    }

}


/*******************************************
**	函数名 : All_Led_On
**	功  能 : 打开所有的灯	，即白灯
**	参  数 : 无
**  返回值 : 无
********************************************/
void  All_LedOn(void)
{
    RED_LED(ON);           //红灯开
    GREEN_LED(ON);           //绿灯开
    BLUE_LED(ON);           //蓝灯开
}

/*******************************************
**	函数名 : All_Led_Off
**	功  能 : 关闭所有的灯	，即灭灯
**	参  数 : 无
**  返回值 : 无
********************************************/
void  All_LedOff(void)
{
    RED_LED(OFF);           //红灯关
    GREEN_LED(OFF);           //绿灯关
    BLUE_LED(OFF);           //蓝灯关
}




