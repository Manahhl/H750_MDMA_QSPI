#ifndef _LCD_H_
#define _LCD_H_
#include "lvgl.h"
#include "lcd_init.h"
void LCD_Fill1(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint32_t color);
void LCD_Fill(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,lv_color_t *color);
void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t*s,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ���ִ�
void LCD_ShowChinese12x12(uint16_t x,uint16_t y,uint8_t*s,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ����12x12����
void LCD_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t*s,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ����16x16����
void LCD_ShowChinese24x24(uint16_t x,uint16_t y,uint8_t*s,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ����24x24����
void LCD_ShowChinese32x32(uint16_t x,uint16_t y,uint8_t*s,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ����32x32����

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾһ���ַ�
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ�ַ���
uint32_t mypow(uint8_t m,uint8_t n);//����
void LCD_ShowIntNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ��������
void LCD_ShowFloatNum1(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey);//��ʾ��λС������

void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[]);//��ʾͼƬ


//������ɫ
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
#define BROWN 			     0XBC40 //��ɫ
#define BRRED 			     0XFC07 //�غ�ɫ
#define GRAY  			     0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			     0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


#endif





