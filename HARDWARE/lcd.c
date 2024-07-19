#include "lcd.h"
#include "lcdfont.h"
#include "stdlib.h"
#include "main.h"
#include "lvgl.h"
#define BUFF_TOTAL 339488
#define panle_y 412
#define panel_x 412*2
#define BUFF_N  64
#define use_qspi 0

#if use_qspi
static uint8_t frame_cache[panle_y][panel_x]__attribute__((section(".ARM.__at_0x24000000")));
#else
//static uint8_t frame_cache[BUFF_TOTAL]__attribute__((section(".ARM.__at_0x24000000")));
#endif
extern QSPI_HandleTypeDef QSPI_Handler;
extern QSPI_CommandTypeDef QSPI_CmdInitStructure;
extern MDMA_HandleTypeDef hmdma;


uint16_t LCD_RGB233toRGB565(uint8_t color)
{
  uint8_t  blue[] = {0,11,21,31}; //��ɫ2-5λ��ɫ���ұ�
  uint16_t color16 = 0;
  //        =====Green=====     ===============Red==============
  color16=(color & 0x1C)<<6|(color&0xC0)<<5|(color&0xE0)<<8;
  //        =====Green=====    =======Blue======
  color16|=(color&0x1C)<<3|blue[color&0x03];
  return color16;	
}
/******************************************************************************
      ����˵������ָ�����������ɫ
      ������ݣ�xs,ys   ��ʼ����
                xe,ye   ��ֹ����
								color       Ҫ������ɫ
      ����ֵ��  ��
******************************************************************************/
#if use_qspi
void LCD_Fill1(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint32_t color)
{
	uint8_t Color[2] = {color >> 8, color & 0xff}; 
	
	for(uint32_t y = 0;y<412;y++)
	{
        // ��� frame_cache ����
		for (uint32_t i = 0; i < 412; i ++)
		{
			frame_cache[y][i*2] = Color[0];
			frame_cache[y][i*2 + 1] = Color[1];
		}
	}
	
    LCD_Address_Set(xs, ys, xe, ye);
    LCD_WR_REG(0x2C);

	for(uint32_t a=0;a<412;a++)  //����ˢ��
	{
		LCD_4LineTransmit_DATA(0x3C, panel_x, &frame_cache[a][0]);
	}
	
}

#else

//void LCD_Fill1(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,uint32_t color)
//{
//	uint8_t Color[2] = {color >> 8, color & 0xff}; 
//	
//	 // ��� frame_cache ����
//	for (uint32_t i = 0; i < 169744; i ++)
//	{
//		frame_cache[i*2] = Color[0];
//		frame_cache[i*2 + 1] = Color[1];
//	}
//	
//    LCD_Address_Set(xs, ys, xe, ye);
//    LCD_WR_REG(0x2C);

//	for(uint32_t a=0;a<206;a++)  //����ˢ��
//	{
//		LCD_4LineTransmit_DATA_DMA(0x3C, 412*4, &frame_cache[a*412*4]);
//	}
//	
//}

#endif

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
	SCB_CleanDCache_by_Addr((uint32_t *)0x24000000, 100*1024);
	LCD_4LineTransmit_DATA_DMA(0x3c,send_size,(uint8_t *)color);
}
/******************************************************************************
      ����˵������ʾ���ִ�
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ��ִ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ� ��ѡ 16 24 32
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t*s,uint16_t fc,uint16_t bc,uint8_t sizey)
{
	while(*s!=0)
	{
		if(sizey==12) LCD_ShowChinese12x12(x,y,s,fc,bc,sizey);
		else if(sizey==16) LCD_ShowChinese16x16(x,y,s,fc,bc,sizey);
		else if(sizey==24) LCD_ShowChinese24x24(x,y,s,fc,bc,sizey);
		else if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc,sizey);
		else return;
		s+=2;
		x+=sizey;
	}
}

/******************************************************************************
      ����˵������ʾ����12x12����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese12x12(uint16_t x,uint16_t y,uint8_t*s,uint16_t fc,uint16_t bc,uint8_t sizey)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//������Ŀ
	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint8_t FontColor[2]={fc>>8,fc&0xff};
	uint8_t BackColor[2]={bc>>8,bc&0xff};	
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	                         
	HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if((tfont12[k].Index[0]==*(s))&&(tfont12[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			LCD_WR_REG(0x2C);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(tfont12[k].Msk[i]&(0x01<<j))LCD_4LineTransmit_DATA(0x3C,2,FontColor);
				  else LCD_4LineTransmit_DATA(0x3C,2,BackColor);
					m++;
					if(m%sizey==0)
					{
						m=0;
						break;
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/******************************************************************************
      ����˵������ʾ����16x16����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t*s,uint16_t fc,uint16_t bc,uint8_t sizey)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//������Ŀ
	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint8_t FontColor[2]={fc>>8,fc&0xff};
	uint8_t BackColor[2]={bc>>8,bc&0xff};
  TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			LCD_WR_REG(0x2C);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(tfont16[k].Msk[i]&(0x01<<j))LCD_4LineTransmit_DATA(0x3C,2,FontColor);
				  else LCD_4LineTransmit_DATA(0x3C,2,BackColor);
					m++;
					if(m%sizey==0)
					{
						m=0;
						break;
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 


/******************************************************************************
      ����˵������ʾ����24x24����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese24x24(uint16_t x,uint16_t y,uint8_t*s,uint16_t fc,uint16_t bc,uint8_t sizey)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//������Ŀ
	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint8_t FontColor[2]={fc>>8,fc&0xff};
	uint8_t BackColor[2]={bc>>8,bc&0xff};
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			LCD_WR_REG(0x2C);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(tfont24[k].Msk[i]&(0x01<<j))LCD_4LineTransmit_DATA(0x3C,2,FontColor);
				  else LCD_4LineTransmit_DATA(0x3C,2,BackColor);
					m++;
					if(m%sizey==0)
					{
						m=0;
						break;
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/******************************************************************************
      ����˵������ʾ����32x32����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese32x32(uint16_t x,uint16_t y,uint8_t*s,uint16_t fc,uint16_t bc,uint8_t sizey)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//������Ŀ
	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint8_t FontColor[2]={fc>>8,fc&0xff};
	uint8_t BackColor[2]={bc>>8,bc&0xff};
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			LCD_WR_REG(0x2C);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(tfont32[k].Msk[i]&(0x01<<j))LCD_4LineTransmit_DATA(0x3C,2,FontColor);
				  else LCD_4LineTransmit_DATA(0x3C,2,BackColor);
					m++;
					if(m%sizey==0)
					{
						m=0;
						break;
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
}

/******************************************************************************
      ����˵������ʾ�����ַ�
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ���ַ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey)
{
	uint8_t temp,sizex,t,m=0;
	uint16_t i,TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint8_t FontColor[2]={fc>>8,fc&0xff};
	uint8_t BackColor[2]={bc>>8,bc&0xff};
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //�õ�ƫ�ƺ��ֵ
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //���ù��λ�� 
	LCD_WR_REG(0x2C);
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==16)temp=ascii_1608[num][i];		 //����8x16����
		else if(sizey==24)temp=ascii_2412[num][i];		 //����12x24����
		else if(sizey==32)temp=ascii_3216[num][i];		 //����16x32����
		else return;
		for(t=0;t<8;t++)
		{
				if(temp&(0x01<<t))LCD_4LineTransmit_DATA(0x3C,2,FontColor);
				else LCD_4LineTransmit_DATA(0x3C,2,BackColor);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
		}
	}   	 	  
}


/******************************************************************************
      ����˵������ʾ�ַ���
      ������ݣ�x,y��ʾ����
                *p Ҫ��ʾ���ַ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey);
		x+=sizey/2;
		p++;
	}  
}


/******************************************************************************
      ����˵������ʾ����
      ������ݣ�m������nָ��
      ����ֵ��  ��
******************************************************************************/
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;
	return result;
}


/******************************************************************************
      ����˵������ʾ��������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ��������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowIntNum(uint16_t x,uint16_t y,uint16_t num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	uint8_t sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey);
	}
} 


/******************************************************************************
      ����˵������ʾ��λС������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾС������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowFloatNum1(uint16_t x,uint16_t y,float num,uint8_t len,uint16_t fc,uint16_t bc,uint8_t sizey)
{         	
	uint8_t t,temp,sizex;
	uint16_t num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey);
	}
}

/******************************************************************************
      ����˵������ʾͼƬ
      ������ݣ�x,y�������
                length ͼƬ����
                width  ͼƬ���
                pic[]  ͼƬ����    
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[])
{
	uint16_t i,j;
	uint32_t k=0;
  uint8_t Color[2];
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	LCD_WR_REG(0x2C);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			Color[0]=pic[k*2];
			Color[1]=pic[k*2+1];
			LCD_4LineTransmit_DATA(0x3C,2,Color);
			k++;
		}
	}
}

void LCD_ShowPicture232(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[])
{
	uint16_t i,j;
	uint32_t k=0;
	uint16_t color;
  uint8_t Color[2];
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	LCD_WR_REG(0x2C);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			color=LCD_RGB233toRGB565(pic[k]);
			Color[0]=color>>8;
			Color[1]=color&0xff;
			LCD_4LineTransmit_DATA(0x3C,2,Color);
			k++;
		}
	}
}


