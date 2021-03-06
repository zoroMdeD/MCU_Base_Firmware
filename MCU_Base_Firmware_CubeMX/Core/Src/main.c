/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "lwip.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../eth/Inc/net.h"
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
extern struct netif gnetif;
extern char str_ethernet[];

extern uint8_t flag_iput_spi2;
extern uint8_t SPI_rx_buf[1];
extern uint8_t SPI_tx_buf[1];

char trans_str[64] = {0,};
volatile uint16_t adc[4] = {0,}; 	//Массив для хранения данных АЦП
double adcValue[4] = {0,};			//Массив для хранения обработанных данных АЦП

extern char a[32];
extern uint8_t b;
extern char c[32];
extern uint8_t d;

////----------------FATfs----------------------
//volatile uint16_t Timer1=0;
FATFS FATFS_Obj;	//Инициализация структуры описывающей инициализацию файловой системы
FRESULT result;		//Инициализация структуры описывающей статусы работы карты памяти
FIL MyFile;			//Инициализация структуры описывающей выбранный файл
uint32_t BytesToWrite = 0;	//Кол-во записанных байт
int firmwareBytesToWrite = 0;	//Кол-во байт которые нужно записать(размер файла)
int firmwareBytesCounter = 0;	//Счетчик полной почсылки (248 байт)

bool check_init = false;
////-------------------------------------------

//RTC_TimeTypeDef sTime = {0};
//RTC_DateTypeDef DateToUpdate = {0};
//
//char trans_str[64] = {0,};

uint8_t Time_Counter_Init = 0;	//Счетчик таймера до запуска функции инициализации датчиков температуры
uint8_t Time_Counter_Read = 0;	//Счетчик таймера до запуска функции чтения датчиков температуры
bool OneWire_Test_Flag_Init = false;	//Флаг запуска функции инициализации датчиков температуры
bool OneWire_Test_Flag_Read = false;	//Флаг запуска функции чтения датчиков температуры

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_LWIP_Init();
  MX_TIM4_Init();
  MX_RTC_Init();
  MX_TIM6_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */

	HAL_Delay(1000);	//Ждем загрузки данных на дисплее (примерная задержка)
	DWT_Init();		//Инициализация микросекундных задержек
	HAL_CRC_MspInit(&hcrc);		//Включаем тактирование аппаратного CRC

	EN_Interrupt();		//Для дебага по USART3


	//----------------GSM_test------------------
//	INIT_GSM();
	//------------------------------------------

												//Для организации обмена данными по блютуз нужно включить:
												//EN_Interrupt(); INIT_GSM(); GSM_RxCpltCallback();(Вкл в main)

	//----------------ETH_test------------------
//	net_ini();
	//------------------------------------------

												//Для организации обмена данными по ethernet нужно включить:
												//net_ini(); MX_LWIP_Process();(Вкл в main) также закоментить в файле stm32f4xx_it.c строчки для отладки через DEBUG_main();

	//----------------ADC_test------------------
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc, 4);		//Стартуем АЦП
	HAL_TIM_OC_Start(&htim4, TIM_CHANNEL_4);
	//------------------------------------------

	//-------------1-WIRE_test------------------
	UpdateTempSens();
	//------------------------------------------


//	HAL_UART_Receive_IT(&huart3,(uint8_t*)str_ethernet,1);		//Настройка прерывания COM для отладки ETH (!?)

//	HAL_SPI_TransmitReceive_IT(&hspi2, (uint8_t *)SPI_tx_buf, (uint8_t *)SPI_rx_buf, 1);	//Настройка прерывания по spi для МК

	//----------------PWM_test------------------
	//------------------------------------------
	//---------------FATfs----------------------
	my_init_card();
	SEND_str("Init sd card -> success\n");
	//------------------------------------------
//	test_create_json();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		//----------------RCT_test----------------
//        HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN); // RTC_FORMAT_BIN , RTC_FORMAT_BCD
//        snprintf(trans_str, 63, "Time %d:%d:%d\n", sTime.Hours, sTime.Minutes, sTime.Seconds);
//        //HAL_UART_Transmit(&huart1, (uint8_t*)trans_str, strlen(trans_str), 1000);
//        SEND_str(trans_str);
//
//        HAL_RTC_GetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN);
//        snprintf(trans_str, 63, "Date %d-%d-20%d\n", DateToUpdate.Date, DateToUpdate.Month, DateToUpdate.Year);
//        //HAL_UART_Transmit(&huart1, (uint8_t*)trans_str, strlen(trans_str), 1000);
//        SEND_str(trans_str);
//    	HAL_Delay(1000);
    	//----------------------------------------

		//---------------RS485_test---------------
//		RS485_Tx;
//			HAL_Delay(50);
//			SEND_RS485_str("Test_addr\n");
//			HAL_Delay(50);
//		RS485_Rx;
//
//		RS485_main();				//Обработчик принятия данных по rs485
		//----------------------------------------

		//----------------ETH_test----------------
//		MX_LWIP_Process();			//Обработчик передачи данных по ETH
		//----------------------------------------

		//----------------GSM_test----------------
//		GSM_RxCpltCallback();		//Обработчик принятых данных от модуля GSM
		//----------------------------------------

		//--------------SPI_test_MK---------------
//		SPI_available();			//Необходимо переделать так чтобы на дисплее был только статус вывода.
		//----------------------------------------

		//----------------ADC_test----------------
//		sprintf(Buff, "%.5f", adcValue[0]);
////		sprintf(Buff, "%d", (uint16_t)adc[0]);
//		SEND_str(Buff);
//		SEND_str("\n");
//		HAL_Delay(2000);
		//----------------------------------------

		//----------------PWM_test----------------
		//----------------------------------------

		//------------------DEBUG-----------------
		DEBUG_main();
		//----------------------------------------

		//--------------ReINIT_GPIO---------------
		CheckReWriteAiDo();
		//----------------------------------------
		//----------TEST_EXAMPLE_1-WIRE-----------
//		TempSensMain();
		CheckReWriteTSiDo();
		//----------------------------------------
    /* USER CODE END WHILE */

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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//	if(huart == &huart1)	//GSM interrupt
//	{
//		GSM_RxCpltCallback();
//	}
//	if(huart == &huart2)	//RS485 interrupt
//	{
//
////		RS485_RxCpltCallback();
//	}

////Нужно для отладки Ethernet по USART
//	if(huart == &huart3)	//COM interrupt
//	{
//		UART3_RxCpltCallback();
//	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	CheckReWriteDiDo();
}
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{

	  //Написать обработчик сообщенй по SPI от MCU

	if(hspi == &hspi2)
	{
	    if(!hspi2.TxXferCount && !flag_iput_spi2)
	    {
	    	flag_iput_spi2 = 1;
	    }
	}
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1)
    {
    	adcValue[0] = Conversion_ADC1((uint16_t)adc[0], 0);
        adcValue[1] = Conversion_ADC1((uint16_t)adc[1], 1);
        adcValue[2] = Conversion_ADC1((uint16_t)adc[2], 2);
        adcValue[3] = Conversion_ADC1((uint16_t)adc[3], 3);
    }
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
    if(htim->Instance == TIM6) //check if the interrupt comes from TIM6
    {
		Time_Counter_Init++;
		Time_Counter_Read++;

		if(Time_Counter_Init == 16)		//Запрос данных каждую ~0.8 сек (old 16: 83 to 49999)
        {
	    	OneWire_Test_Flag_Init = true;
        }
		if(Time_Counter_Read == 32)	//Запрос данных каждую ~1.6 сек (old 32: 83 to 49999))
        {
	    	OneWire_Test_Flag_Read = true;

	    	Time_Counter_Init = 0;
	    	Time_Counter_Read = 0;
        }
    }

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
