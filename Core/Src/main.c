//QSPI_CLK   PB2
//QSPI_DATA0 PD11
//QSPI_DATA1 PD12
//QSPI_DATA2 PE2
//QSPI_DATA3 PD13
//LCD_RES    PC1   
//QSPI_BK1   PB6

#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "lcd.h"
#include "pic.h"
#include "lvgl.h"

void SystemClock_Config(void);
static void MPU_Initialize(void);
static void MPU_Config(void);

static void obj_add_anim()
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

    lv_obj_set_pos(obj1, 120, 0);
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

extern void lv_demo_stress(void);
extern void lv_demo_music(void);

void CPU_CACHE_Enable(void)
{
    /* Enable I-Cache */
    SCB_EnableICache();

    /* Enable D-Cache */
    SCB_EnableDCache();
}


#define MDMA_QSPI 1

int main(void)
{    
	MPU_Config();
	
	HAL_Init();
	
	CPU_CACHE_Enable();
	
	SystemClock_Config();

	MX_GPIO_Init();
	QSPI_GPIOInit();
	QSPI_Init();
	MX_USART1_UART_Init();
	lv_init();
	lv_port_disp_init();
	//LCD_Init();
	//LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	//obj_add_anim();
	//lv_demo_stress();
	lv_demo_music();
  while (1)
  {
	  
//		#if MDMA_QSPI
//		LCD_ShowPicture232(92,80,240,128,gImage_2);
//		LCD_ShowString(104,220,"1.45 TFT_LCD TEST",RED,WHITE,24);
//		LCD_ShowString(92,252,"RESOLUTION:412x412",RED,WHITE,24);
//		LCD_ShowString(104,284,"DRIVER IC:SPD2010",RED,WHITE,24);
//		LCD_ShowString(120,314,"Interface:QSPI",RED,WHITE,24); 
//		HAL_Delay(1000);HAL_Delay(500);
//		LCD_Fill1(0,0,LCD_W,LCD_H,WHITE);
//		HAL_Delay(1000);	  
//        LCD_Fill1(0,0,40,LCD_H,RED);
//	  HAL_Delay(1000);
//		LCD_Fill1(40,0,80,LCD_H,GREEN);HAL_Delay(1000);
//		LCD_Fill1(80,0,120,LCD_H,0X00fF); HAL_Delay(1000);
//		LCD_Fill1(120,0,160,LCD_H,YELLOW);HAL_Delay(1000);
//		LCD_Fill1(180,0,200,LCD_H,WHITE);HAL_Delay(1000);
//		LCD_Fill1(200,0,240,LCD_H,BLUE);HAL_Delay(1000);
//		LCD_Fill1(240,0,280,LCD_H,BLACK);HAL_Delay(1000);
//		LCD_Fill1(280,0,320,LCD_H,RED); HAL_Delay(1000);
//		LCD_Fill1(320,0,360,LCD_H,GREEN);HAL_Delay(1000);
//		LCD_Fill1(360,0,412,LCD_H,BLUE);
//		HAL_Delay(1000);HAL_Delay(500);
//		LCD_Fill1(0,0,204,204,RED); HAL_Delay(1000);
//		LCD_Fill1(204,0,LCD_W,204,GREEN);HAL_Delay(1000);
//		LCD_Fill1(0,204,204,LCD_H,BLUE); HAL_Delay(1000);
//		LCD_Fill1(204,204,LCD_W,LCD_H,WHITE);HAL_Delay(1000);
//		HAL_Delay(1000);HAL_Delay(500);
//		LCD_Fill1(0,0,LCD_W,LCD_H,WHITE);
//		LCD_ShowString(100,40,"ZHONGJINGYUAN",BLACK,WHITE,24);
//		LCD_ShowPicture232(86,86,240,240,gImage_1);
//		LCD_ShowString(104,360,"color depth 24bit",BLACK,WHITE,24);
//		HAL_Delay(1000);HAL_Delay(500);
//		LCD_Fill1(0,0,412,412,BLUE);HAL_Delay(1000);
//		LCD_Fill1(0,0,LCD_W,LCD_H,WHITE);HAL_Delay(1000);
//		#else
//		//HAL_Delay(1000);HAL_Delay(500);
//		LCD_Fill1(0,0,LCD_W,LCD_H,WHITE);
//		HAL_Delay(1000);
//		LCD_Fill1(0,0,LCD_W,LCD_H,GREEN);
//		HAL_Delay(1000);
//		LCD_Fill1(0,0,LCD_W,LCD_H,RED);
//		HAL_Delay(1000);				
//		LCD_Fill1(0,0,LCD_W,LCD_H,YELLOW);
//		HAL_Delay(1000); 
//		LCD_Fill1(0,0,LCD_W,LCD_H,LGRAY);
//		HAL_Delay(1000);
//		
//		LCD_Fill1(0,0,LCD_W,LCD_H,WHITE);
//		HAL_Delay(1000);
//		LCD_Fill1(0,0,LCD_W,LCD_H,GREEN);
//		HAL_Delay(1000);
//		LCD_Fill1(0,0,LCD_W,LCD_H,RED);
//		HAL_Delay(1000);				
//		LCD_Fill1(0,0,LCD_W,LCD_H,YELLOW);
//		HAL_Delay(1000); 
//		LCD_Fill1(0,0,LCD_W,LCD_H,LGRAY);
//		HAL_Delay(1000);
//		#endif
//		printf("uart test !!");

	HAL_Delay(5);
	lv_task_handler();
  }
	
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
      /*activate CSI clock mondatory for I/O Compensation Cell*/
    __HAL_RCC_CSI_ENABLE() ;

    /* Enable SYSCFG clock mondatory for I/O Compensation Cell */
    __HAL_RCC_SYSCFG_CLK_ENABLE() ;

    /* Enables the I/O Compensation Cell */
    HAL_EnableCompensationCell();
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* MPU Configuration */
static void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct;

    /* 禁止 MPU */
    HAL_MPU_Disable();
	
    /* 配置AXI SRAM的MPU属性为Write through */
    MPU_InitStruct.Enable 			= MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress 		= 0x24000000;
    MPU_InitStruct.Size 			= MPU_REGION_SIZE_512KB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;     /* 不要CACHE */
	MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number 			= MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField 	= MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec 		= MPU_INSTRUCTION_ACCESS_ENABLE;
    HAL_MPU_ConfigRegion(&MPU_InitStruct);
   
    /*使能 MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
