#ifndef _LCD_INIT_H_
#define _LCD_INIT_H_

#include "QSPI.h"

/*
//SPD2010 不支持单独控制一个像素点
//详细介绍请参考 SPD2010 datasheet
//P15  QSPI Timing 时序说明 
//P48  14.1.18 Set Column(2AH) 寄存器介绍
*/


#define LCD_W 412
#define LCD_H 412


#define LCD_RES_Set() HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);
#define LCD_RES_Clr() HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);


void LCD_WR_REG(uint8_t reg);
void LCD_Transmit_Byte(uint8_t reg,uint8_t dat);
void LCD_Transmit_DATA(uint8_t reg,uint32_t len,uint8_t *dat);
void LCD_4LineTransmit_Byte(uint8_t reg,uint8_t dat);
void LCD_4LineTransmit_DATA(uint8_t reg,uint32_t len,uint8_t *dat);
void LCD_4LineTransmit_DATA_DMA(uint8_t reg,uint32_t len,uint8_t *dat);
void LCD_Address_Set(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye);

void LCD_Init(void);

#endif



