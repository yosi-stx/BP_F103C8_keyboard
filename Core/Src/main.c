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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_hid.h"
#include "keyboard.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define KEYS_BUFFER_SIZE 1024
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */
extern USBD_HandleTypeDef hUsbDeviceFS;
uint8_t mouse_buffer[4];
uint8_t keyboard_buffer[8];
uint8_t wellcome[] = "Hello, I am a Yosi Keyboard";
uint8_t Uart_DMA_in_buffer[4];
uint8_t send_keys_buffer[KEYS_BUFFER_SIZE];
static uint16_t in_ptr=0;
static uint16_t out_ptr=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void mouse_move()
{
	  for(int i=0;i<6;i++)
	  {
		  USBD_HID_SendReport(&hUsbDeviceFS, mouse_buffer, 4);
		  HAL_Delay(25);
		  mouse_buffer[2] *= -1;
	  }
	  mouse_buffer[1] *= -1;
}
void keyboard_init()
{
	keyboard_buffer[0]=1;//reportID
	keyboard_buffer[1]=0;//modifier
	keyboard_buffer[2]=0;//OEM
	keyboard_buffer[3]=0;//keycode data
	keyboard_buffer[4]=0;//keycode data
	keyboard_buffer[5]=0;//keycode data
	keyboard_buffer[6]=0;//keycode data
	keyboard_buffer[7]=0;//keycode data
}
void keyboard_send_char(uint8_t tav)
{
	uint16_t modifier_and_key;
	uint8_t modifier = 0;
	uint8_t key = 0;

	modifier_and_key = ToHidKeyboardScancode(tav);
	modifier = modifier_and_key >> 8;
	key = modifier_and_key & 0xFF;

	keyboard_buffer[1]=modifier;//modifier (shift)
	keyboard_buffer[3]=key;//keycode data
	USBD_HID_SendReport(&hUsbDeviceFS,keyboard_buffer,8);
	HAL_Delay(30);
	keyboard_buffer[1]=0x0;//modifier
	keyboard_buffer[3]=0x0;//keycode data
	USBD_HID_SendReport(&hUsbDeviceFS,keyboard_buffer,8);
	HAL_Delay(30);

}
void keyboard_action()
{
	// Begin TODO 5 HID_KEYBOARD_HANDS_ON: Report PgDown press and release
	#warning "TODO 5 HID_KEYBOARD_HANDS_ON: Report PgDown press and release"
	//report keycode data - PgDwn press (0x4E)
	keyboard_buffer[3]=0x4E;//keycode data
	USBD_HID_SendReport(&hUsbDeviceFS,keyboard_buffer,8);
	HAL_Delay(100);
	keyboard_buffer[3]=0x0;//keycode data
	USBD_HID_SendReport(&hUsbDeviceFS,keyboard_buffer,8);
	HAL_Delay(100);
}
void keyboard_action2()
{
	for(int i=0;i<sizeof(wellcome);i++)
	{
		if(wellcome[i]!=0)
			keyboard_send_char(wellcome[i]);
		else
			break;
	}

}
void keyboard_action3()
{
	keyboard_buffer[1]=0x02;//modifier (shift)
	keyboard_buffer[3]=KEY_A;//keycode data
	USBD_HID_SendReport(&hUsbDeviceFS,keyboard_buffer,8);
	HAL_Delay(100);
	keyboard_buffer[1]=0x0;//modifier
	keyboard_buffer[3]=KEY_NONE;//keycode data
	USBD_HID_SendReport(&hUsbDeviceFS,keyboard_buffer,8);
	HAL_Delay(100);
}
void send_keys_handler()
{
	if(out_ptr != in_ptr)
	{
		// there are un-handled bytes from UART
		keyboard_send_char(send_keys_buffer[out_ptr]);
		out_ptr++;
		if(out_ptr == KEYS_BUFFER_SIZE)
		{
			out_ptr = 0;
		}
	}
}
void rt_buffer_save(uint8_t in_chr)
{

    send_keys_buffer[in_ptr] = in_chr;
    in_ptr++;
	if(in_ptr == KEYS_BUFFER_SIZE)
	{
		in_ptr = 0;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if( Uart_DMA_in_buffer[0] >= 0x20 )
    {
    	rt_buffer_save(Uart_DMA_in_buffer[0]);
    }

	HAL_GPIO_TogglePin(Led_GPIO_Port, Led_Pin);
  //HAL_UART_Receive_DMA(&huart2, Rx_data, 4);
}

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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  mouse_buffer[1]=20;
  mouse_buffer[2]=10;
  keyboard_init();
  HAL_UART_Receive_DMA(&huart2, Uart_DMA_in_buffer, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  GPIO_PinState last_state=1;
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if( HAL_GPIO_ReadPin(User_button_GPIO_Port,User_button_Pin) != last_state && last_state==0)
	  {
		  HAL_GPIO_TogglePin(Led_GPIO_Port, Led_Pin);
		  // mouse_move();
		  keyboard_action2();
	  }
//	  else
//	  {
//		  HAL_Delay(100);
//	  }
	  last_state = HAL_GPIO_ReadPin(User_button_GPIO_Port,User_button_Pin);
	  HAL_Delay(50);
	  send_keys_handler();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Led_GPIO_Port, Led_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Led_Pin */
  GPIO_InitStruct.Pin = Led_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Led_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : User_button_Pin */
  GPIO_InitStruct.Pin = User_button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(User_button_GPIO_Port, &GPIO_InitStruct);

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

