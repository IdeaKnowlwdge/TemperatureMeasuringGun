#include "Key_Driver.h"
#include "SysTick_Driver.h"

/***********************************************
**	函数名 : Led_Init
**	功  能 : 初始化GPIOB口的12/13/14/15引脚为浮空输入
**  参  数 : 无
**	返回值 :	 无
************************************************/
void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启按键GPIOB口的时钟*/
    ALLKEY_GPIO_CLK_ENABLE();

    GPIO_InitStructure.Pin = KEY1_PIN|KEY2_PIN|KEY3_PIN|KEY4_PIN;  //选择按键的引脚
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;                  //设置引脚为输入模式
    GPIO_InitStructure.Pull = GPIO_NOPULL;       							//设置引脚不上拉也不下拉
    HAL_GPIO_Init(ALLKEY_GPIO_PORT, &GPIO_InitStructure);     //使用上面的结构体初始化按键

}


/***********************************************
**	函数名 : Key_IsAnyButtonPress
**	功  能 : 扫描上下左右按键是否有按下
**  参  数 : 按键类型指针变量
**	返回值 :	 按键是否被按下
************************************************/
unsigned char Key_IsAnyButtonPress(pen_key_type_t keyType)
{

    /*检测按键1是否按下 */
    if(HAL_GPIO_ReadPin(ALLKEY_GPIO_PORT, KEY1_PIN) == KEY_ON )
    {
        //消抖
        delay_ms(30);
        /*再次检测按键1是否按下 */
        if(HAL_GPIO_ReadPin(ALLKEY_GPIO_PORT, KEY1_PIN) == KEY_ON )
        {
//		/*等待按键释放 */
//		while(HAL_GPIO_ReadPin(ALLKEY_GPIO_PORT,KEY1_PIN) == KEY_ON);

            *keyType = EN_KEY_TYPE_UP;     //上按键
            return 	KEY_ON;
        }
    }

    /*检测按键2是否按下 */
    if(HAL_GPIO_ReadPin(ALLKEY_GPIO_PORT, KEY2_PIN) == KEY_ON )
    {
        //消抖
        delay_ms(30);
        /*再次检测按键2是否按下 */
        if(HAL_GPIO_ReadPin(ALLKEY_GPIO_PORT, KEY2_PIN) == KEY_ON )
        {
            /*等待按键释放 */
            while(HAL_GPIO_ReadPin(ALLKEY_GPIO_PORT,KEY2_PIN) == KEY_ON);

            *keyType = EN_KEY_TYPE_RIGHT;   //右按键
            return 	KEY_ON;
        }
    }

    /*检测按键3是否按下 */
    if(HAL_GPIO_ReadPin(ALLKEY_GPIO_PORT, KEY3_PIN) == KEY_ON )
    {
        //消抖
        delay_ms(30);
        /*再次检测按键3是否按下 */
        if(HAL_GPIO_ReadPin(ALLKEY_GPIO_PORT, KEY3_PIN) == KEY_ON )
        {
            /*等待按键释放 */
            while(HAL_GPIO_ReadPin(ALLKEY_GPIO_PORT,KEY3_PIN) == KEY_ON);

            *keyType = EN_KEY_TYPE_LEFT;     //左安建
            return 	KEY_ON;
        }
    }

    /*检测按键3是否按下 */
    if(HAL_GPIO_ReadPin(ALLKEY_GPIO_PORT, KEY4_PIN) == KEY_ON )
    {
        //消抖
        delay_ms(30);
        /*再次检测按键3是否按下 */
        if(HAL_GPIO_ReadPin(ALLKEY_GPIO_PORT, KEY4_PIN) == KEY_ON )
        {
            /*等待按键释放 */
            while(HAL_GPIO_ReadPin(ALLKEY_GPIO_PORT,KEY4_PIN) == KEY_ON);

            *keyType = EN_KEY_TYPE_DOWN;     //左安建
            return 	KEY_ON;
        }
    }


    return KEY_OFF;
}

