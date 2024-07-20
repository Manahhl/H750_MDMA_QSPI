#include "lcd.h"
#include "stdlib.h"
#include "main.h"
#include "lvgl.h"

#define BUFF_TOTAL 339488
#define panle_y 412
#define panel_x 412*2

#define use_qspi 0

extern QSPI_HandleTypeDef QSPI_Handler;
extern QSPI_CommandTypeDef QSPI_CmdInitStructure;
extern MDMA_HandleTypeDef hmdma;


extern void LV_ATTRIBUTE_FLUSH_READY lv_disp_flush_ready(lv_disp_drv_t * disp_drv);
extern lv_disp_drv_t *disp_drv;
void HAL_QSPI_TxCpltCallback(QSPI_HandleTypeDef *hqspi)
{
	lv_disp_flush_ready(&disp_drv);
}

void LCD_Fill(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,lv_color_t *color)
{
	
	uint32_t send_size = ((xe- xs + 1) * (ye - ys+1) * sizeof(lv_color_t));
	LCD_Address_Set(xs,ys,xe,ye);
	LCD_WR_REG(0x2c);
	__disable_fault_irq();
	SCB_CleanDCache_by_Addr((uint32_t *)0x24000000, 100*1024);
	LCD_4LineTransmit_DATA_DMA(0x3c,send_size,(uint8_t *)color);
	__enable_fault_irq();
}
