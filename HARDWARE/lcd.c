#include "main.h"

#define BUFF_TOTAL 339488
#define panle_Y 412
#define panle_X 412
#define panel_X_DATA 412*2
#define panel_x_y 412*412
//static uint8_t frame_cache[panle_Y][panel_X_DATA]__attribute__((section (".RAM_D1"))) ;
//static uint8_t frame_cache[panle_Y][panel_X_DATA] __attribute__((section(".ARM.__at_0x24000000"))); 
//static uint8_t frame_cache[BUFF_TOTAL]__attribute__((section (".RAM_D1"))) ;
__attribute__((section (".RAM_D1"))) static uint16_t frame_cache[panel_x_y];

extern QSPI_HandleTypeDef QSPI_Handler;
extern QSPI_CommandTypeDef QSPI_CmdInitStructure;
extern MDMA_HandleTypeDef hmdma;


extern lv_disp_drv_t *disp_drv;
void HAL_QSPI_TxCpltCallback(QSPI_HandleTypeDef *hqspi)
{
	lv_disp_flush_ready(&disp_drv);
	//disp_enable_update();
}

//方式1
void LCD_Fill(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,lv_color_t *color)
{
	disp_disable_update();
	uint32_t send_size = ((xe- xs+1) * (ye - ys+1) * sizeof(lv_color_t));
	LCD_Address_Set(xs,ys,xe,ye);
	LCD_WR_REG(0x2c);
	SCB_CleanDCache_by_Addr((uint32_t *)0x24000000, 512*1024);
	LCD_4LineTransmit_DATA_DMA(0x3c,send_size,(uint8_t *)color);
}

//void lcdqspi_draw_line(uint16_t x0, uint16_t x1, uint32_t y,uint16_t y0,uint16_t y1 ,uint16_t *pixel)
//{
//    if ((x1 < panle_X)&&(y < panle_Y)&&(x1 >= x0))
//    {
//        uint32_t  oft = x0 * 2;
//        uint16_t *dat = pixel;
//        for (uint32_t x = x0; x <= x1; x++)
//        {
//            frame_cache[y][oft + 1] = *dat & 0xff;
//            frame_cache[y][oft + 0] = (*dat & 0xff00) >> 8;
//            oft += 2;
//            dat++;
//        }
//    }
//}

//方式2 
//void lcd_fill(uint16_t xs,uint16_t xe,uint16_t ys,uint16_t ye)
//{
//	disp_disable_update();
//	LCD_Address_Set(xs,ys,xe,ye);
//	LCD_WR_REG(0x2c); 
//	//SCB_CleanDCache_by_Addr((uint32_t *)0x24000000, 512*1024);
//	for(uint32_t i = 0;i<ye;i++ )
//	{
//		LCD_4LineTransmit_DATA_DMA(0x3c,panel_X_DATA,&frame_cache[i][0]);
//	} 
//	//lv_disp_flush_ready(&disp_drv);
//	//disp_enable_update();
//}

//void lcd_buf_fill(uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1,uint16_t *pixel)
//{
//	uint32_t width = x1- x0+1;
//	uint32_t height = y1-y0+1;
//	for(uint32_t y = 0;y<height;y++)
//	{
//		 for(uint32_t x=0;x<width;x++)
//		{
//			uint32_t arr_x = x0 + x;
//			uint32_t arr_y = y0 +y;
//			
//			frame_cache[arr_y][(arr_x*2) + 0] = *pixel & 0xff;
//			frame_cache[arr_y][(arr_x*2) + 1] = (*pixel & 0xff00) >> 8;
//			pixel++;
//		}
//	}
//}

//方式3
 void lcd_buf_fill(uint32_t x0, uint32_t x1, uint32_t y0, uint32_t y1,uint16_t *pixel)
{
	uint32_t area_size ;
	uint32_t remain_size ;
	uint16_t index;
	
	area_size  = ((x1 - x0 +1)*(y1 - y0 +1)*sizeof(uint16_t));
	if(area_size > panel_x_y)
	{
		remain_size = area_size - panel_x_y;
		area_size = panel_x_y;
	}  
	for(index = 0;index <= area_size;index++)
	{
		frame_cache[index] = *pixel;
		pixel++;
	}
	LCD_Address_Set(x0,y0,x1,y1);
	LCD_WR_REG(0x2c);
	LCD_4LineTransmit_DATA_DMA(0x3c,area_size,(uint8_t *)&frame_cache[0]);
}
 
void obj_add_anim()
{
    lv_anim_t a;
    memset(&a, 0, sizeof(lv_anim_t));

    lv_anim_t b;
    memset(&b, 0, sizeof(lv_anim_t));

    lv_obj_t *screen = lv_scr_act();
    lv_obj_t *obj = lv_obj_create(screen);
    lv_obj_t *obj1 = lv_obj_create(screen);
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 50, 50);
    lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE), 0);

    lv_obj_set_pos(obj1, 170, 0);
    lv_obj_set_size(obj1, 50, 50);
    lv_obj_set_style_bg_color(obj1, lv_palette_main(LV_PALETTE_PINK), 0);

    a.var = obj; // 动画对象

    a.exec_cb = (lv_anim_exec_xcb_t)lv_obj_set_y; // y轴移动
    a.time = lv_anim_speed_to_time(50, 0, 120);   // 1s的动画时间
    a.start_value = 0;                            // 开始坐标
    a.end_value = 120;                            // 结束坐标

    a.act_time = -500; // 第一次做动画时，延时500ms

    a.path_cb = lv_anim_path_bounce; // 模拟弹性物体下落
                                     // a.ready_cb = ready_call_back;  // 自定义打印函数 可开启

    a.playback_time = 1;    // 不开启动画回放
    a.playback_delay = 200; // 回放延时时间

    a.repeat_cnt = 10000; // 重复动画次数
    a.repeat_delay = 200; // 每次重复动画的延时时间
    lv_anim_start(&a);    // 调用函数开始动画

    b.var = obj1; // 动画对象

    b.exec_cb = (lv_anim_exec_xcb_t)lv_obj_set_y; // y轴移动
    b.time = lv_anim_speed_to_time(20, 0, 120);   // 1s的动画时间
    b.start_value = 0;                            // 开始坐标
    b.end_value = 120;                            // 结束坐标

    b.act_time = -500; // 第一次做动画时，延时500ms

    b.path_cb = lv_anim_path_bounce; // 模拟弹性物体下落
    // a.ready_cb = ready_call_back;  // 自定义打印函数 可开启

    b.playback_time = 1;    // 不开启动画回放
    b.playback_delay = 200; // 回放延时时间

    b.repeat_cnt = 10000;   // 重复动画次数
    b.repeat_delay = 200; // 每次重复动画的延时时间
    lv_anim_start(&b);    // 调用函数开始动画
}


