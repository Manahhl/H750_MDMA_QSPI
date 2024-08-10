/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "lvgl.h"
#include "mpu.h"
#include "lcd.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern void disp_enable_update(void);
extern void disp_disable_update(void);
void SystemClock_Config(void);
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
 #define USE_TICK 0
///////////////////////////////////// 
 
#define SRAM_ITCM_BASE      (0x00000000)
#define SRAM_ITCM_SIZE      (1024 * 64U)

#define SRAM_DTCM_BASE      (0x20000000)
#define SRAM_DTCM_SIZE      (1024 * 128U)

#define SRAM_AXI_BASE       (0x24000000)        /* LCD FRAME BUFFER */
#define SRAM_AXI_SIZE       (1024 * 512U)

#define SRAM1_BASE          (0x30000000)        /* LVGL DISPLAY ROW BUFFER */
#define SRAM1_SIZE          (1024 * 128U)

#define SRAM2_BASE          (0x30020000)    
#define SRAM2_SIZE          (1024 * 128U)

#define SRAM3_BASE          (0x30040000)
#define SRAM3_SIZE          (1024 * 32U)

#define SRAM4_BASE          (0x38000000)
#define SRAM4_SIZE          (1024 * 64U)

#define SRAM_BKP_BASE       (0x38800000)
#define SRAM_BKP_SIZE       (1024 * 4U)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
