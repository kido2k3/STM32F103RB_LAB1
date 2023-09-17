/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../MyLib/segment_led.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RED_GREEN 0
#define RED_YELLOW 1
#define YELLOW_RED 2
#define GREEN_RED 3
#define GREEN 3 //3s
#define YELLOW 2 //2s
#define SECONDS 1000 // 1s
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */
	unsigned short led_st = 0;
	unsigned short counterA = (GREEN + YELLOW);
	unsigned short counterB = GREEN;
	unsigned long timer_7seg = HAL_GetTick();
	struct seven_led ledA;
	struct seven_led ledB;

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	/* USER CODE BEGIN 2 */

	init7SEG(&ledA, GPIOA, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3,
	GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_9);

	init7SEG(&ledB, GPIOB, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_10,
	GPIO_PIN_11, GPIO_PIN_8, GPIO_PIN_9);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 0);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		switch (led_st) {
		case RED_GREEN:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 0);
			display7SEG(&ledA, counterA);
			display7SEG(&ledB, counterB);
			if (HAL_GetTick() - timer_7seg > SECONDS) {
				counterA--;
				counterB--;

				if (counterA <= 0 || counterB <= 0) {
					led_st = RED_YELLOW;
					counterB = YELLOW;
				}
				timer_7seg = HAL_GetTick();
			}
			break;
		case RED_YELLOW:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 0);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 1);
			display7SEG(&ledA, counterA);
			display7SEG(&ledB, counterB);
			if (HAL_GetTick() - timer_7seg > SECONDS) {
				counterA--;
				counterB--;
				if (counterA <= 0 || counterB <= 0) {
					led_st = GREEN_RED;
					counterA = GREEN;
					counterB = GREEN + YELLOW;
				}
				timer_7seg = HAL_GetTick();
			}
			break;
		case GREEN_RED:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 0);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);
			display7SEG(&ledA, counterA);
			display7SEG(&ledB, counterB);
			if (HAL_GetTick() - timer_7seg > SECONDS) {
				counterA--;
				counterB--;
				if (counterA <= 0 || counterB <= 0) {
					led_st = YELLOW_RED;
					counterA = YELLOW;
				}
				timer_7seg = HAL_GetTick();
			}
			break;
		default:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
			display7SEG(&ledA, counterA);
			display7SEG(&ledB, counterB);
			if (HAL_GetTick() - timer_7seg > SECONDS) {
				counterA--;
				counterB--;
				if (counterA <= 0 || counterB <= 0) {
					if (counterA < 1 || counterB < 1) {
						led_st = RED_GREEN;
						counterA = YELLOW + GREEN;
						counterB = GREEN;
					}
					timer_7seg = HAL_GetTick();
				}
			}
			/* USER CODE END WHILE */

			/* USER CODE BEGIN 3 */
		}
		/* USER CODE END 3 */
	}
}
/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA,
			GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4
					| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8
					| GPIO_PIN_9, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB,
			GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_10 | GPIO_PIN_11
					| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8
					| GPIO_PIN_9, GPIO_PIN_RESET);

	/*Configure GPIO pins : PA0 PA1 PA2 PA3
	 PA4 PA5 PA6 PA7
	 PA8 PA9 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
			| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8
			| GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PB0 PB1 PB2 PB10
	 PB11 PB5 PB6 PB7
	 PB8 PB9 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_10
			| GPIO_PIN_11 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8
			| GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
