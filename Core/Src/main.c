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

void SystemClock_Config(void);
static void MPU_Initialize(void);
static void MPU_Config(void);

#define MDMA_QSPI 0

static void CPU_CACHE_Enable(void)
{
    /* 使能 I-Cache */
    SCB_EnableICache();

    /* 使能 D-Cache */
    SCB_EnableDCache();
	//SCB_DisableDCache();
}

int main(void)
{

  HAL_Init();
	
  MPU_Config();
  CPU_CACHE_Enable();	
  SystemClock_Config();
	
  MX_GPIO_Init();
	
	QSPI_GPIOInit();
	QSPI_Init();
	
  MX_USART1_UART_Init();
	
	LCD_Init();
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	
  while (1)
  {
		#if MDMA_QSPI
		LCD_ShowPicture232(92,80,240,128,gImage_2);
		LCD_ShowString(104,220,"1.45 TFT_LCD TEST",RED,WHITE,24);
		LCD_ShowString(92,252,"RESOLUTION:412x412",RED,WHITE,24);
		LCD_ShowString(104,284,"DRIVER IC:SPD2010",RED,WHITE,24);
		LCD_ShowString(120,314,"Interface:QSPI",RED,WHITE,24); 
		HAL_Delay(1000);HAL_Delay(500);
		LCD_Fill(0,0,LCD_W,LCD_H,WHITE);	
    LCD_Fill(0,0,40,LCD_H,RED);
		LCD_Fill(40,0,80,LCD_H,GREEN);
		LCD_Fill(80,0,120,LCD_H,BLUE);
		LCD_Fill(120,0,160,LCD_H,YELLOW);
		LCD_Fill(180,0,200,LCD_H,WHITE);
		LCD_Fill(200,0,240,LCD_H,LIGHTBLUE);
		LCD_Fill(240,0,280,LCD_H,BLACK);
		LCD_Fill(280,0,320,LCD_H,RED);
		LCD_Fill(320,0,360,LCD_H,GREEN);
		LCD_Fill(360,0,412,LCD_H,BLUE);
		HAL_Delay(1000);HAL_Delay(500);
		LCD_Fill(0,0,204,204,RED);
		LCD_Fill(204,0,LCD_W,204,GREEN);
		LCD_Fill(0,204,204,LCD_H,BLUE);
		LCD_Fill(204,204,LCD_W,LCD_H,WHITE);
		HAL_Delay(1000);HAL_Delay(500);
		LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
		LCD_ShowString(100,40,"ZHONGJINGYUAN",BLACK,WHITE,32);
		LCD_ShowPicture232(86,86,240,240,gImage_1);
		LCD_ShowString(104,360,"color depth 24bit",BLACK,WHITE,24);
		HAL_Delay(1000);HAL_Delay(500);	
		LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
		#else
		//HAL_Delay(1000);HAL_Delay(500);
		LCD_Fill1(0,0,LCD_W,LCD_H,WHITE);
		HAL_Delay(1000);
		LCD_Fill1(0,0,LCD_W,LCD_H,GREEN);
		HAL_Delay(1000);
		LCD_Fill1(0,0,LCD_W,LCD_H,RED);
		HAL_Delay(1000);				
//		LCD_Fill1(0,0,40,LCD_H,RED);
//		LCD_Fill1(40,0,80,LCD_H,GREEN);
//		LCD_Fill1(80,0,120,LCD_H,BLUE);
//		LCD_Fill1(120,0,160,LCD_H,YELLOW);
//		LCD_Fill1(180,0,200,LCD_H,WHITE);
//		LCD_Fill1(200,0,240,LCD_H,LIGHTBLUE);
//		LCD_Fill1(240,0,280,LCD_H,BLACK);
//		LCD_Fill1(280,0,320,LCD_H,RED);
//		LCD_Fill1(320,0,360,LCD_H,GREEN);
//		LCD_Fill1(360,0,412,LCD_H,BLUE);
//		HAL_Delay(1000);HAL_Delay(500);
//		LCD_Fill1(0,0,204,204,RED);
//		LCD_Fill1(204,0,LCD_W,204,GREEN);
//		LCD_Fill1(0,204,204,LCD_H,BLUE);
//		LCD_Fill1(204,204,LCD_W,LCD_H,WHITE);
//		HAL_Delay(1000);HAL_Delay(500);
		LCD_Fill1(0,0,LCD_W,LCD_H,YELLOW);
		HAL_Delay(1000);
		LCD_Fill1(0,0,LCD_W,LCD_H,BLUE);
		HAL_Delay(1000);
		#endif
		printf("uart test !!");
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
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
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
   /*
      使用IO的高速模式，要使能IO补偿，即调用下面三个函数 
      （1）使能CSI clock
      （2）使能SYSCFG clock
      （3）使能I/O补偿单元， 设置SYSCFG_CCCSR寄存器的bit0
    */
    __HAL_RCC_CSI_ENABLE();

    __HAL_RCC_SYSCFG_CLK_ENABLE();

    HAL_EnableCompensationCell();

       /* AXI SRAM的时钟是上电自动使能的，而D2域的SRAM1，SRAM2和SRAM3要单独使能 */	
    #if 0
    __HAL_RCC_D2SRAM1_CLK_ENABLE();
    __HAL_RCC_D2SRAM2_CLK_ENABLE();
    __HAL_RCC_D2SRAM3_CLK_ENABLE();

    __HAL_RCC_BKPRAM_CLKAM_ENABLE();       
    __HAL_RCC_D3SRAM1_CLKAM_ENABLE();
    #endif 
  
  
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* MPU Configuration */
static void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct;

    /* 禁止 MPU */
    HAL_MPU_Disable();

#if 0
    /* Configure the MPU attributes as Device not cacheable 
     for ETH DMA descriptors */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x30040000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Configure the MPU attributes as Cacheable write through 
     for LwIP RAM heap which contains the Tx buffers */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x30044000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
#endif

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

//    /* 配置显存IO空间的属性为Write through */
//	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
//	MPU_InitStruct.BaseAddress      = 0x30000000;
//	MPU_InitStruct.Size             = MPU_REGION_SIZE_256KB;
//	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
//	MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
//	MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;     /* 不要CASHE */
//	MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
//	MPU_InitStruct.Number           = MPU_REGION_NUMBER3;
//	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
//	MPU_InitStruct.SubRegionDisable = 0x00;
//	MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
//	HAL_MPU_ConfigRegion(&MPU_InitStruct);

//    /* 配置FMC IO空间的属性为Write through */
//	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
//	MPU_InitStruct.BaseAddress      = 0x60000000;
//	MPU_InitStruct.Size             = ARM_MPU_REGION_SIZE_32B;
//	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
//	MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
//	MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;     /* 不要CASHE */
//	MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
//	MPU_InitStruct.Number           = MPU_REGION_NUMBER4;
//	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
//	MPU_InitStruct.SubRegionDisable = 0x00;
//	MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
//	HAL_MPU_ConfigRegion(&MPU_InitStruct);

//	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
//	MPU_InitStruct.BaseAddress      = 0x20000000;
//	MPU_InitStruct.Size             = ARM_MPU_REGION_SIZE_128KB;
//	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
//	MPU_InitStruct.IsBufferable     = MPU_ACCESS_NOT_BUFFERABLE;
//	MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;     /* 不要CASHE */
//	MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
//	MPU_InitStruct.Number           = MPU_REGION_NUMBER6;
//	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
//	MPU_InitStruct.SubRegionDisable = 0x00;
//	MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
//	HAL_MPU_ConfigRegion(&MPU_InitStruct);
//    
//    /*使能 MPU */
//    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
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
