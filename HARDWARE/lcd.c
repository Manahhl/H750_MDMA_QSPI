#include "lcd.h"
#include "stdlib.h"
#include "main.h"

#define BUFF_TOTAL 339488
#define panle_y 412
#define panel_x 412*2

extern QSPI_HandleTypeDef QSPI_Handler;
extern QSPI_CommandTypeDef QSPI_CmdInitStructure;
extern MDMA_HandleTypeDef hmdma;

extern lv_disp_drv_t *disp_drv;
//volatile uint8_t QSPI_TX;
void HAL_QSPI_TxCpltCallback(QSPI_HandleTypeDef *hqspi)
{
	lv_disp_flush_ready(&disp_drv);
	disp_enable_update();
	
//	while(QSPI_TX == 0)
//		QSPI_TX = 1;
}

void LCD_Fill(uint16_t xs,uint16_t ys,uint16_t xe,uint16_t ye,lv_color_t *color)
{
	disp_disable_update();
	uint32_t send_size = ((xe- xs+1) * (ye - ys+1) * sizeof(lv_color_t));
	LCD_Address_Set(xs,ys,xe,ye);
	LCD_WR_REG(0x2c);
	SCB_CleanDCache_by_Addr((uint32_t *)0x24000000, 512*1024);
	LCD_4LineTransmit_DATA_DMA(0x3c,send_size,(uint8_t *)color);
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

    a.var = obj; // ��������

    a.exec_cb = (lv_anim_exec_xcb_t)lv_obj_set_y; // y���ƶ�
    a.time = lv_anim_speed_to_time(50, 0, 120);   // 1s�Ķ���ʱ��
    a.start_value = 0;                            // ��ʼ����
    a.end_value = 120;                            // ��������

    a.act_time = -500; // ��һ��������ʱ����ʱ500ms

    a.path_cb = lv_anim_path_bounce; // ģ�ⵯ����������
                                     // a.ready_cb = ready_call_back;  // �Զ����ӡ���� �ɿ���

    a.playback_time = 1;    // �����������ط�
    a.playback_delay = 200; // �ط���ʱʱ��

    a.repeat_cnt = 10000; // �ظ���������
    a.repeat_delay = 200; // ÿ���ظ���������ʱʱ��
    lv_anim_start(&a);    // ���ú�����ʼ����

    b.var = obj1; // ��������

    b.exec_cb = (lv_anim_exec_xcb_t)lv_obj_set_y; // y���ƶ�
    b.time = lv_anim_speed_to_time(20, 0, 120);   // 1s�Ķ���ʱ��
    b.start_value = 0;                            // ��ʼ����
    b.end_value = 120;                            // ��������

    b.act_time = -500; // ��һ��������ʱ����ʱ500ms

    b.path_cb = lv_anim_path_bounce; // ģ�ⵯ����������
    // a.ready_cb = ready_call_back;  // �Զ����ӡ���� �ɿ���

    b.playback_time = 1;    // �����������ط�
    b.playback_delay = 200; // �ط���ʱʱ��

    b.repeat_cnt = 10000;   // �ظ���������
    b.repeat_delay = 200; // ÿ���ظ���������ʱʱ��
    lv_anim_start(&b);    // ���ú�����ʼ����
}


