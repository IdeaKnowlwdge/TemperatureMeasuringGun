#ifndef __PWR_CTL_H__
#define __PWR_CTL_H__


#include "stm32f1xx.h"

typedef enum {
	PWR_OLED = 0,	//OLED
	PWR_INFRARED,	//红外测温传感器
	
	PWR_DEV_NUMBER,
}_PWR_DEV;

struct pin_ctl
{
	GPIO_TypeDef* GPIOx;		//GPIO口,比如GPIOB 
	uint32_t Periph_Clock;
	uint32_t pin;
#define PWR_ENABLE	0
#define PWR_DISABLE	1
	uint32_t status;
	void (*clk_enable)(void);
	void (*clk_disable)(void);
	void (*other)(void);
};

struct power_control;

struct pwr_fops
{
	void (*init)(struct power_control* pwr_ctl);	
	void (*enable)(struct power_control* pwr_ctl,_PWR_DEV dev);
	void (*disable)(struct power_control* pwr_ctl,_PWR_DEV dev);
	int (*get_status)(struct power_control* pwr_ctl,_PWR_DEV dev);
	void (*delay_ms)(uint16_t nms);
	void (*delay_us)(uint32_t nus);
};

struct power_control{
	int dev_number;
	struct pin_ctl* port_info;
	struct pwr_fops* ops;
};

extern struct power_control board_pwr_ctl;

void board_power_ctl(_PWR_DEV dev,uint8_t status);
int get_power_status(_PWR_DEV dev);

void power_ctl_register(void);
void power_ctl_unregister(void);

void power_pin_ctl(void);

#endif
