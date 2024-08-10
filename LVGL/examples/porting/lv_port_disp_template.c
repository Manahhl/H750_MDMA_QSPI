/**
 * @file lv_port_disp_templ.c
 *
 */

/*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp_template.h"
#include <stdbool.h>
#include "lv_hal_disp.h"
#include "main.h"
/*********************
 *      DEFINES
 *********************/
#ifndef MY_DISP_HOR_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
    #define MY_DISP_HOR_RES    412
#endif

#ifndef MY_DISP_VER_RES
    #warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen height, default value 240 is used for now.
    #define MY_DISP_VER_RES    412
#endif

/**********************
 *      TYPEDEFS
 **********************/
//#define LV_VER_SIZE 79
//static lv_disp_draw_buf_t draw_buf_dsc_1;
////ALIGN_32BYTES(static lv_color_t buf_1[MY_DISP_HOR_RES * LV_VER_SIZE]__attribute__((section(".ARM.__at_0x24000000"))));
//__attribute__((section (".RAM_D2"))) static lv_color_t buf_1[MY_DISP_HOR_RES * LV_VER_SIZE];//__attribute__((section(".ARM.__at_0x24000000")));	


//开启Dcache不会影响画面数据	
//static lv_disp_draw_buf_t draw_buf_dsc_1;

//__attribute__((section (".RAM_D2"))) static lv_color_t buf_1[SRAM1_SIZE];

//#define BUF1_ROWS (SRAM1_SIZE/4 /(MY_DISP_HOR_RES * sizeof(lv_color_t)))


	
/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

extern void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/
lv_disp_drv_t disp_drv;
/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();


    /* Example for 1) */
    //lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 79);   /*Initialize the display buffer*/

    /* Example for 2) */
    static lv_disp_draw_buf_t draw_buf_dsc_2;
    __attribute__((section (".RAM_D2")))static lv_color_t buf_2_1[MY_DISP_HOR_RES * 79];                        /*A buffer for 10 rows*/
    __attribute__((section (".RAM_D2")))static lv_color_t buf_2_2[MY_DISP_HOR_RES * 79];                        /*An other buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, MY_DISP_HOR_RES * 79);   /*Initialize the display buffer*/

      /* Example for 3) also set disp_drv.full_refresh = 1 below*/
//    static lv_disp_draw_buf_t draw_buf_dsc_3;
//    static lv_color_t buf_3_1[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*A screen sized buffer*/
//    static lv_color_t buf_3_2[MY_DISP_HOR_RES * MY_DISP_VER_RES];            /*Another screen sized buffer*/
//    lv_disp_draw_buf_init(&draw_buf_dsc_3, buf_3_1, buf_3_2,
//                          MY_DISP_VER_RES * LV_VER_RES_MAX);   /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_2;

    /*Required for Example 3)*/
    //disp_drv.full_refresh = 1;

    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    //disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
	LCD_Init();
    /*You code here*/
}

volatile bool disp_flush_enabled = true;

/* Enable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_enable_update(void)
{
    disp_flush_enabled = true;
}

/* Disable updating the screen (the flushing process) when disp_flush() is called by LVGL
 */
void disp_disable_update(void)
{
    disp_flush_enabled = false;
}
 uint8_t QSPI_TX = 1;
/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
	//printf("disp_flush_enabled = %d",disp_flush_enabled);
    if(disp_flush_enabled) 
	{ 
		//方式1 使用AXI RAM 作为lvglbuffer，直接将数据通过MDMA发送至QSPI数据寄存器
		//SCB_InvalidateDCache_by_Addr((uint32_t *)0x24000000, 512*1024);
		//LCD_Fill(area->x1,area->y1,area->x2,area->y2,color_p);
		
		//方式2 使用RAM_D2 288KB 速度200mhz 作为lvglbuffer，AXI RAM 512KB作为显存buffer，按行发送
		//lcd_buf_fill(area->x1,area->x2,area->y1,area->y2,(uint16_t *)color_p); 
		//lcd_fill(area->x1,area->x2,area->y1,area->y2);
		
		//方式3 在方式2基础上实现帧信号同步，buffer缓存方式不变
		if(!QSPI_TX)
		{
			lcd_buf_fill(area->x1,area->x2,area->y1,area->y2,(uint16_t *)color_p);
		}
		
		
		//lv_disp_flush_ready(&disp_drv);
    }
	 
}

/*OPTIONAL: GPU INTERFACE*/

#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
