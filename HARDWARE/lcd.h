#ifndef _LCD_H_
#define _LCD_H_
#include "lvgl.h"
#include "lcd_init.h"
void LCD_Fill1(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint32_t color);
void LCD_Fill(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,lv_color_t *color);
void obj_add_anim();
//void lcdqspi_draw_line(uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1,uint16_t *pixel);
void lcdqspi_draw_line(uint16_t x0, uint16_t x1, uint32_t y,uint16_t y0,uint16_t y1 ,uint16_t *pixel);
void lcd_fill(uint16_t xs,uint16_t xe,uint16_t ys,uint16_t ye); 
void lcd_buf_fill(uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1,uint16_t *pixel);
void LCD_DRAW_LINE_DMA();
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x011F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


#endif





