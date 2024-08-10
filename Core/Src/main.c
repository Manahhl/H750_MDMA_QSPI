//QSPI_CLK   PB2
//QSPI_DATA0 PD11
//QSPI_DATA1 PD12
//QSPI_DATA2 PE2
//QSPI_DATA3 PD13
//LCD_RES    PC1   
//QSPI_NCS   PB6

/* SRAM MAP
ITCM  0x00000000~0x0000FFFF(0x10000, 64K) - 紧密耦合内存 - CPU直接访问(指令) - 速度400MHZ
DTCM  0x20000000~0x2001FFFF(0x20000,128K) - 紧密耦合内存 - CPU直接访问(数据) - 速度400MHZ

D1域, 带宽是64bit, AXI总线, D3域中的BDMA主控不能访问, 其它都可以访问此RAM区 - 速度200MHZ
SRAMA 0x24000000~0x2407FFFF(0x80000,512K) - 用途不限，可以用于用户应用数据存储或者LCD显存

D2域, 带宽是32bit, AHB总线, D3域中的BDMA主控不能访问这三块SRAM, 其它都可以访问这几个RAM区 - 速度200MHZ
SRAM1 0x30000000~0x3001FFFF(0x20000,128K) - 用途不限，可用于D2域中的DMA缓冲，也可以当D1域断电后用于运行程序代码
SRAM2 0x30020000~0x3003FFFF(0x20000,128K) - 用途不限，可用于D2域中的DMA缓冲，也可以用于用户数据存取
SRAM3 0x30040000~0x30047FFF(0x08000, 32K) - 用途不限，主要用于以太网和USB的缓冲

D3域, 带宽是32bit，AHB总线, 大部分主控都能访这块SRAM区。
SRAM4 0x38000000~0x3800FFFF(0x10000, 64K) - 用途不限，可以用于D3域中的DMA缓冲，也可以当D1和D2域进入DStandby待机方式后，继续保存用户数据

备份RAM区, D3域, 带宽是32bit, AHB总线, 大部分主控都能访问这块SRAM区
SRAMB 0x38800000~0x38800FFF(0x01000,  4K) - 用途不限，主要用于系统进入低功耗模式后，继续保存数据（Vbat引脚外接电池）

DTCM和ITCM不支持DMA1，DMA2和BDMA，仅支持MDMA
AXI SRAM，SRAM1，SRAM2，SRAM3不支持BDMA，支持MDMA，DMA1和DMA2。
SRAM4支持所有DMA，即MDMA，DMA1，DMA2和BDMA

DTCM是400MHz的，而其它的RAM都是200MHz，推荐工程的主RAM空间采用TCM，而其它需要大RAM或者DMA的场合，使用剩余RAM空间
*/

#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "lcd.h"
#include "lvgl.h"
#include "tim.h"

extern void lv_demo_stress(void);
extern void lv_demo_music(void);
extern uint8_t lvgl_task;

void CPU_CACHE_Enable(void)
{
    /* Enable I-Cache */
    SCB_EnableICache();

    /* Enable D-Cache */
    SCB_EnableDCache();
}

#if USE_TICK
volatile uint16_t ms_cnt_1 = 0; // 计时变量1
volatile uint16_t ms_cnt_2 = 0; // 计时变量2
#endif

int main(void)
{    
	mpu_init();
	CPU_CACHE_Enable();	
	HAL_Init();	
	SystemClock_Config();
	
	MX_GPIO_Init();
	QSPI_GPIOInit();
	QSPI_Init();
	MX_USART1_UART_Init();
	
	#if USE_TICK ==0
	 MX_TIM7_Init();
	#endif
	
	
	lv_init();
	lv_port_disp_init();

	//obj_add_anim();
	//lv_demo_stress();
	lv_demo_music();
    
  while (1)
  {
	 #if USE_TICK 
	  if (ms_cnt_2 >= 5)
      {
         ms_cnt_2 = 0;      // 计时清零
         lv_task_handler(); // LVGL任务处理
      }
     if (ms_cnt_1 >= 500) // 判断是否计时到500ms
     {
         ms_cnt_1 = 0; // 计时清零
     }
	 #else
	  if(lvgl_task >= 5)
	  {
		  lv_task_handler();
		  lvgl_task = 0;
	  }
	 // printf("sizeof lv_color_t = %d\r\n",sizeof(lv_color_t));   2
	 // printf("(SRAM1_SIZE / (412 * sizeof(lv_color_t))) = %d\r\n",(SRAM1_SIZE / (412 * sizeof(lv_color_t)))); 159
	 #endif
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 64;
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
  
	
#if 1   
	
	//high speed io compensation
	__HAL_RCC_CSI_ENABLE() ;
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    HAL_EnableCompensationCell();
	
	//D2 area sram clock enable
	__HAL_RCC_D2SRAM1_CLK_ENABLE();
	__HAL_RCC_D2SRAM2_CLK_ENABLE();
	__HAL_RCC_D2SRAM3_CLK_ENABLE();
	
#endif
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */


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
