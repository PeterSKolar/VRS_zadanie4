/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static char messageBuffer[MESSAGE_MAX_LENGTH + 2U];
static uint16_t messageLength = 0U;
static uint8_t receivingMessage = 0U;

static uint16_t uppercaseCount = 0U;
static uint16_t lowercaseCount = 0U;

static uint8_t txBuffer[128];
volatile uint32_t systemTickMs = 0U;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void receive_dma_data(const uint8_t* data, uint16_t len);

static void sendText(const char *text);
static void processCompletedMessage(void);
static void reportDmaUsage(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  USART2_RegisterCallback(receive_dma_data);

  uint32_t lastReportTime = 0U;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
#if POLLING
	//Polling for new data, no interrupts
	USART2_CheckDmaReception();
#endif

	// Report DMA buffer usage
	uint32_t currentTime = systemTickMs;
	if ((currentTime - lastReportTime) >= REPORT_INTERVAL_MS)
	{
		lastReportTime = currentTime;
		reportDmaUsage();
	}

#if POLLING

	LL_mDelay(1);
#else
	/* Interrupt mode: allow interrupts to process reception */
	__WFI();
#endif

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();  
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
  SysTick->CTRL = 0U;							// CLOCK DOESENT INCREMENT WIDOUGHT THIS FOR SOME REASON
    SysTick->LOAD = 8000000U / 1000U - 1U;
    SysTick->VAL = 0U;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;

}

/* USER CODE BEGIN 4 */
void receive_dma_data(const uint8_t* data, uint16_t len)
{
	for (uint16_t i = 0U; i < len; i++)
	{
		char c = (char)data[i];
	// wait for message
		if (receivingMessage == 0U)
		{
			if (c == '#')
			{
				receivingMessage = 1U;
				messageLength = 0U;
				uppercaseCount = 0U;
				lowercaseCount = 0U;
				messageBuffer[messageLength++] = c;
			}
			continue;
		}

		if (c == '#')
		{
			messageLength = 0U;
			uppercaseCount = 0U;
			lowercaseCount = 0U;
			messageBuffer[messageLength++] = c;
			continue;
		}

		if (c >= 'A' && c <= 'Z')
		{
			uppercaseCount++;
		}
		else if (c >= 'a' && c <= 'z')
		{
			lowercaseCount++;
		}

		if (c == '$')
		{
			messageBuffer[messageLength++] = c;
			messageBuffer[messageLength] = '\0';
			receivingMessage = 0U;
			processCompletedMessage();
			messageLength = 0U;
			continue;
		}

		if (messageLength >= MESSAGE_MAX_LENGTH)
		{
			receivingMessage = 0U;
			messageLength = 0U;
			uppercaseCount = 0U;
			lowercaseCount = 0U;
			continue;
		}
		messageBuffer[messageLength++] = c;
	}
}

static void processCompletedMessage(void)
{
	int length = snprintf( (char *)txBuffer, sizeof(txBuffer),
			"Message: %s | Uppercase: %u | Lowercase: %u\r\n",
			messageBuffer, (unsigned int)uppercaseCount, (unsigned int)lowercaseCount );


	if (length > 0 && length < (int)sizeof(txBuffer))
	{
		USART2_PutBuffer(txBuffer, (uint8_t)length);
	}
}

static void reportDmaUsage(void)
{
	uint16_t used = USART2_GetDmaRxUsage();


	int length = snprintf( (char *)txBuffer, sizeof(txBuffer),
			"DMA RX buffer: %u/%u bytes used\r\n",
			(unsigned int)used, (unsigned int)DMA_USART2_BUFFER_SIZE );

	if (length > 0 && length < (int)sizeof(txBuffer))
	{
		USART2_PutBuffer(txBuffer, (uint8_t)length);
	}

}


static void sendText(const char *text)
{
	size_t length = strlen(text);


	if (length > sizeof(txBuffer))
	{
		length = sizeof(txBuffer);
	}


	if (length > 0U)
	{
		USART2_PutBuffer((uint8_t *)text, (uint8_t)length);
	}

}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
