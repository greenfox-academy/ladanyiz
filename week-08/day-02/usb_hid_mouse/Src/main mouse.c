/**
 ******************************************************************************
 * @file    USB_Device/HID_Standalone/Src/main.c
 * @author  MCD Application Team
 * @version V1.2.0
 * @date    30-December-2016
 * @brief   USB device HID demo main file
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics International N.V.
 * All rights reserved.</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <math.h>

/* Private typedef -----------------------------------------------------------*/
typedef struct {
	int32_t x;
	int32_t y;
} coordinate_t;

/* Private define ------------------------------------------------------------*/
#define LIMIT	3

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
USBD_HandleTypeDef USBD_Device;
uint8_t HID_Buffer[4];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void) {
	/* Enable the CPU Cache */
	CPU_CACHE_Enable();

	/* STM32F7xx HAL library initialization:
	 - Configure the Flash ART accelerator on ITCM interface
	 - Configure the Systick to generate an interrupt each 1 msec
	 - Set NVIC Group Priority to 4
	 - Low Level Initialization
	 */
	HAL_Init();

	/* Configure the System clock to have a frequency of 216 MHz */
	SystemClock_Config();

	/* Configure LED1 */
	BSP_LED_Init(LED1);

	/* Init Device Library */
	USBD_Init(&USBD_Device, &HID_Desc, 0);

	/* Add Supported Class */
	USBD_RegisterClass(&USBD_Device, USBD_HID_CLASS);

	/* Start Device Process */
	USBD_Start(&USBD_Device);
	
	BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());

	TS_StateTypeDef ts_state;
	int16_t orig_x = 0;
	int16_t orig_y = 0;
	int16_t last_x = 0;
	int16_t last_y = 0;
	uint8_t first = 1;
	uint8_t click = 0;

	// This byte contains the button states
	// 0b00000001 - the left mouse button is pressed
	// 0b00000010 - the right mouse button is pressed
	// 0b00000100 - the middle mouse button is pressed
//	HID_Buffer[0] = 0;
	// This byte contains the x realative movement
//	HID_Buffer[1] = 5;
	// This byte contains the y realative movement
//	HID_Buffer[2] = 5;

	while (1) {
		// reset HID buffer
		for (uint8_t i = 0; i < 3; i++)
			HID_Buffer[i] = 0;
		BSP_TS_GetState(&ts_state);
		if (ts_state.touchDetected == 2) {
			click = 2;
		} else if (ts_state.touchDetected == 1) {
			if (click == 2)
				continue;
			// if first touch
			if (first) {
				// next won't be first
				first = 0;
				// possible clicking
				click = 1;
				// set original touch point; to check for clicking, if coordinates change beyond the set limit while touched, it is not a click
				orig_x = ts_state.touchX[0];
				orig_y = ts_state.touchY[0];
				// set the last point; to calculate movement
				last_x = ts_state.touchX[0];
				last_y = ts_state.touchY[0];
			// if still touched
			} else {
				// set the difference in coordinates in the buffer
				HID_Buffer[1] = (ts_state.touchX[0] - last_x) * 3; // * 3, otherwise too slow
				HID_Buffer[2] = (ts_state.touchY[0] - last_y) * 3;
				// set the latest point of touch
				last_x = ts_state.touchX[0];
				last_y = ts_state.touchY[0];
				// if it is still possibly a click
				if (click)
					// if coordinates changed beyond the limit, it cannot be a click anymore
					if ((abs(ts_state.touchX[0] - orig_x) > LIMIT) || (abs(ts_state.touchY[0] - orig_y) > LIMIT))
						click = 0;
				USBD_HID_SendReport(&USBD_Device, HID_Buffer, 4);
				HAL_Delay(10);
			}
		// screen is not touched now
		} else {
			// the next touch will be a new first
			first = 1;
			// if the touch was within the limit, therefore a click
			if (click == 2) {
				HAL_Delay(20);
				// set right mouse click in buffer
				HID_Buffer[0] = 2;
				USBD_HID_SendReport(&USBD_Device, HID_Buffer, 4);
				HAL_Delay(20);
				// click finished, set no click in buffer
				HID_Buffer[0] = 0;
				USBD_HID_SendReport(&USBD_Device, HID_Buffer, 4);
				HAL_Delay(20);
			} else if (click == 1) {
				HAL_Delay(20);
				// set left mouse click in buffer
				HID_Buffer[0] = 1;
				USBD_HID_SendReport(&USBD_Device, HID_Buffer, 4);
				HAL_Delay(20);
				// click finished, set no click in buffer
				HID_Buffer[0] = 0;
				USBD_HID_SendReport(&USBD_Device, HID_Buffer, 4);
				HAL_Delay(20);
			}
			// set no click
			click = 0;
		}
	}
}

/**
 * @brief This function provides accurate delay (in milliseconds) based
 *        on SysTick counter flag.
 * @note This function is declared as __weak to be overwritten in case of other
 *       implementations in user file.
 * @param Delay: specifies the delay time length, in milliseconds.
 * @retval None
 */

void HAL_Delay(__IO uint32_t Delay) {
	while (Delay) {
		if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
			Delay--;
		}
	}
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 216000000
 *            HCLK(Hz)                       = 216000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 25000000
 *            PLL_M                          = 25
 *            PLL_N                          = 432
 *            PLL_P                          = 2
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 7
 * @param  None
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 432;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/* Activate the OverDrive to reach the 216 Mhz Frequency */
	if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
	RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void) {
	/* User may add here some code to deal with this error */
	while (1) {
	}
}

/**
 * @brief  CPU L1-Cache enable.
 * @param  None
 * @retval None
 */
static void CPU_CACHE_Enable(void) {
	/* Enable I-Cache */
	SCB_EnableICache();

	/* Enable D-Cache */
	SCB_EnableDCache();
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
