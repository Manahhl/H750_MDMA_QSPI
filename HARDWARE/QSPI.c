#include "QSPI.h"
#include "main.h"


QSPI_HandleTypeDef QSPI_Handler;
QSPI_CommandTypeDef QSPI_CmdInitStructure;
MDMA_HandleTypeDef hmdma;


void QSPI_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
	
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
	PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_D1HCLK;
	
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE(); 
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_QSPI_CLK_ENABLE();
	
	__HAL_RCC_QSPI_FORCE_RESET();
	__HAL_RCC_QSPI_RELEASE_RESET();
  
	GPIO_InitStructure.Pin=GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
	GPIO_InitStructure.Mode=GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull=GPIO_PULLUP;
	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Alternate=GPIO_AF9_QUADSPI;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_2;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull =GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF9_QUADSPI;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_6;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF10_QUADSPI;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
	GPIO_InitStructure.Pin = GPIO_PIN_2;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Alternate = GPIO_AF9_QUADSPI;
	HAL_GPIO_Init(GPIOE,&GPIO_InitStructure);
}

void QSPI_Init(void)
{
	
	QSPI_Handler.Instance=QUADSPI;
	QSPI_Handler.Init.ChipSelectHighTime=QSPI_CS_HIGH_TIME_8_CYCLE;  //ƬѡΪ����ʱ
	QSPI_Handler.Init.ClockMode=QSPI_CLOCK_MODE_3;					//����ʱ��ģʽ
	QSPI_Handler.Init.ClockPrescaler=10;							//����ʱ�ӷ�Ƶ��
	QSPI_Handler.Init.DualFlash=QSPI_DUALFLASH_DISABLE;				//����˫����ģʽ״̬
	QSPI_Handler.Init.FifoThreshold=32;								//����FIFO���ֽ���ֵ�����ڼ��ģʽʹ�ã�
	QSPI_Handler.Init.FlashID=QSPI_FLASH_ID_1;						//����ʹ�õ�����
	QSPI_Handler.Init.FlashSize=18;									//���������С
	QSPI_Handler.Init.SampleShifting=QSPI_SAMPLE_SHIFTING_NONE; 	//���ò�����λ
	HAL_QSPI_Init(&QSPI_Handler);
	
	HAL_NVIC_SetPriority(QUADSPI_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(QUADSPI_IRQn);
	
	__HAL_RCC_MDMA_CLK_ENABLE();
	
	hmdma.Instance = MDMA_Channel0;
	hmdma.Init.Request = MDMA_REQUEST_QUADSPI_FIFO_TH;
	hmdma.Init.TransferTriggerMode = MDMA_BUFFER_TRANSFER;
	hmdma.Init.Priority =  MDMA_PRIORITY_HIGH;
	hmdma.Init.Endianness =  MDMA_LITTLE_ENDIANNESS_PRESERVE;
	hmdma.Init.SourceInc = MDMA_SRC_INC_BYTE;
	hmdma.Init.DestinationInc =  MDMA_DEST_INC_DISABLE;
	hmdma.Init.SourceDataSize =  MDMA_SRC_DATASIZE_HALFWORD;
	hmdma.Init.DestDataSize = MDMA_DEST_DATASIZE_HALFWORD;
	hmdma.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
	hmdma.Init.BufferTransferLength = 128;
	hmdma.Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
	hmdma.Init.DestBurst = MDMA_DEST_BURST_SINGLE;
	hmdma.Init.SourceBlockAddressOffset = 0;
	hmdma.Init.DestBlockAddressOffset = 0;
	
	if (HAL_MDMA_Init(&hmdma) != HAL_OK)
  {
      Error_Handler();
  }
	
	__HAL_LINKDMA(&QSPI_Handler,hmdma,hmdma);

	HAL_MDMA_DeInit(&hmdma);  
	HAL_MDMA_Init(&hmdma);
  
	HAL_NVIC_SetPriority(MDMA_IRQn,5,0);
	HAL_NVIC_EnableIRQ(MDMA_IRQn);
}





