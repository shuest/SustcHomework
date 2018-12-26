
#include "led.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define D4 1 
#define D5 2

//为了可移植性， 将LED的引脚，端口，始终独立出来
#define D4_GPIO_PIN GPIO_Pin_8
#define D5_GPIO_PIN GPIO_Pin_9
#define D4_GPIO_PORT GPIOB
#define D5_GPIO_PORT GPIOB
#define D4_GPIO_ACC RCC_APB2Periph_GPIOB
#define D5_GPIO_ACC RCC_APB2Periph_GPIOB

/***********************************************************
* 名称: leds_init()
* 功能: 初始化led的GPIO时钟，并初始化为输出模式
* 参数: 无
* 返回: 无
* 修改:
* 注释: 1) 在使用stm32的led前，必须先调用这个初始化函数
***********************************************************/

void leds_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* 初始化GPIO时钟 */
  RCC_APB2PeriphClockCmd(D4_GPIO_ACC | D5_GPIO_ACC, ENABLE);

  /* 设置LED的引脚为推挽输出 */
  GPIO_InitStructure.GPIO_Pin = D4_GPIO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(D4_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = D5_GPIO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(D5_GPIO_PORT, &GPIO_InitStructure);
  
  //D4和D5默认关闭
  D4_off();
  D5_off();
}

/***********************************************************
* 名称: D4_on()
* 功能: 点亮D4
* 参数: 无
* 返回: 无
* 修改:
* 注释:
***********************************************************/
void D4_off(void)
{
  GPIO_SetBits(D4_GPIO_PORT, D4_GPIO_PIN);
}

/***********************************************************
* 名称: D5_on()
* 功能: 点亮D5
* 参数: 无
* 返回: 无
* 修改:
* 注释:
***********************************************************/
void D5_off(void)
{
  GPIO_SetBits(D5_GPIO_PORT, D5_GPIO_PIN);
}

/***********************************************************
* 名称: D4_off()
* 功能: 熄灭D4
* 参数: 无
* 返回: 无
* 修改:
* 注释:
***********************************************************/
void D4_on(void)
{
  GPIO_ResetBits(D4_GPIO_PORT, D4_GPIO_PIN);
}

/***********************************************************
* 名称: D5_off()
* 功能: 熄灭D5
* 参数: 无
* 返回: 无
* 修改:
* 注释:
***********************************************************/
void D5_on(void)
{
  GPIO_ResetBits(D5_GPIO_PORT, D5_GPIO_PIN);
}

/***********************************************************
* 名称: D4_toggle()
* 功能: 反转D4
* 参数: 无
* 返回: 无
* 修改:
* 注释:
***********************************************************/
void D4_toggle(void)
{
  GPIO_WriteBit(D4_GPIO_PORT, D4_GPIO_PIN, !GPIO_ReadOutputDataBit(D4_GPIO_PORT, D4_GPIO_PIN));
}

/***********************************************************
* 名称: D5_toggle()
* 功能: 反转D5
* 参数: 无
* 返回: 无
* 修改:
* 注释:
***********************************************************/
void D5_toggle(void)
{
  GPIO_WriteBit(D5_GPIO_PORT, D5_GPIO_PIN, !GPIO_ReadOutputDataBit(D5_GPIO_PORT, D5_GPIO_PIN));
}
