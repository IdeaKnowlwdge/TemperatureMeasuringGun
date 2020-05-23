#include "pwr_ctl.h"
#include "include.h"

#define SOF_PWR_OLED_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define SOF_PWR_OLED_CLK_DISABLE() __HAL_RCC_GPIOA_CLK_DISABLE()

#define SOF_PWR_INFRARED_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()
#define SOF_PWR_INFRARED_CLK_DISABLE() __HAL_RCC_GPIOC_CLK_DISABLE()


void pwr_oled_clk_enable(void)
{
	SOF_PWR_OLED_CLK_ENABLE();
}
void pwr_oled_clk_disable(void)
{
	SOF_PWR_OLED_CLK_DISABLE();
}
void pwr_infrared_clk_enable(void)
{
	SOF_PWR_INFRARED_CLK_ENABLE();
}
void pwr_infrared_clk_disable(void)
{
	SOF_PWR_INFRARED_CLK_DISABLE();
}

void pwr_infrared_af_setup(void)
{
	
}

struct pin_ctl port_info[] = {
	[PWR_OLED] = {
		.GPIOx = GPIOA,
		.Periph_Clock = 0,
		.pin = GPIO_PIN_5,
		.clk_enable = pwr_oled_clk_enable,
		.clk_disable = pwr_oled_clk_disable,
	},
	[PWR_INFRARED] = {
		.GPIOx = GPIOC,
		.Periph_Clock = 0,
		.pin = GPIO_PIN_13,
		.clk_enable = pwr_infrared_clk_enable,
		.clk_disable = pwr_infrared_clk_disable,
		.other = pwr_infrared_af_setup,
	},
};

void pwr_init(struct power_control* pwr_ctl)
{
	int dev = 0;
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	for(dev = 0;dev < pwr_ctl->dev_number;dev++)
	{
		if(pwr_ctl->port_info[dev].clk_enable)
			pwr_ctl->port_info[dev].clk_enable();
		
		if(pwr_ctl->port_info[dev].other)
			pwr_ctl->port_info[dev].other();
		
		GPIO_InitStruct.Pin = pwr_ctl->port_info[dev].pin;	
		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  
		GPIO_InitStruct.Pull  = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(pwr_ctl->port_info[dev].GPIOx, &GPIO_InitStruct);
		
		pwr_ctl->ops->disable(pwr_ctl,(_PWR_DEV)dev);

	}
}

void pwr_enable(struct power_control* pwr_ctl,_PWR_DEV dev)
{
	HAL_GPIO_WritePin(pwr_ctl->port_info[dev].GPIOx, pwr_ctl->port_info[dev].pin, GPIO_PIN_RESET);
	pwr_ctl->port_info[dev].status = PWR_ENABLE;
}
void pwr_disable(struct power_control* pwr_ctl,_PWR_DEV dev)
{
	HAL_GPIO_WritePin(pwr_ctl->port_info[dev].GPIOx, pwr_ctl->port_info[dev].pin, GPIO_PIN_SET);
	pwr_ctl->port_info[dev].status = PWR_DISABLE;
}
int pwr_get_status(struct power_control* pwr_ctl,_PWR_DEV dev)
{
	return pwr_ctl->port_info[dev].status;
}

void pwr_delay_ms(uint16_t nms)
{
	delay_ms(nms);
}
void pwr_delay_us(uint32_t nus)
{
	delay_us(nus);
}

struct pwr_fops board_ops = {
	.init = pwr_init,
	.enable = pwr_enable,
	.disable = pwr_disable,
	.get_status = pwr_get_status,
	.delay_ms = pwr_delay_ms,
	.delay_us = pwr_delay_us,
};

struct power_control board_pwr_ctl ={
	.dev_number = PWR_DEV_NUMBER,
	.port_info = port_info,
	.ops = &board_ops,
};

void board_power_ctl(_PWR_DEV dev,uint8_t status)
{
	struct power_control* pwr_ctl = &board_pwr_ctl;
	
	if(status == PWR_ENABLE)
	{
		pwr_ctl->ops->enable(pwr_ctl,(_PWR_DEV)dev);
	}
	else if(status == PWR_DISABLE)
	{
		pwr_ctl->ops->disable(pwr_ctl,(_PWR_DEV)dev);
	}
}

int get_power_status(_PWR_DEV dev)
{
	struct power_control* pwr_ctl = &board_pwr_ctl;
	
	return pwr_ctl->ops->get_status(pwr_ctl,dev);
}

void power_ctl_register(void)
{
	int dev = 0;
	struct power_control* pwr_ctl = &board_pwr_ctl;
	
	pwr_ctl->ops->init(pwr_ctl);
	
	for(dev = 0;dev < pwr_ctl->port_info[dev].status;dev++)
	{
		pwr_ctl->ops->enable(pwr_ctl,(_PWR_DEV)dev);
	}
}

void power_ctl_unregister(void)
{
	int dev = 0;
	struct power_control* pwr_ctl = &board_pwr_ctl;
	
	for(dev = 0;dev < pwr_ctl->port_info[dev].status;dev++)
	{
		pwr_ctl->ops->disable(pwr_ctl,(_PWR_DEV)dev);
	}
}

void power_pin_ctl(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
														   
    GPIO_InitStruct.Pin = GPIO_PIN_5;	
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
}

