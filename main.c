
/*
 * GSM Module Communication for ThingSpeak with STM32F401RE
 *
 * Description:
 * This program demonstrates the integration of an STM32F401RE microcontroller
 * with a Quectel M66 GSM module for sending data to ThingSpeak. The code
 * configures the GSM module using AT commands and periodically transmits data
 * to a ThingSpeak channel. Error handling and debugging features are implemented
 * to ensure reliable communication.
 *
 * Hardware:
 * - STM32F401RE microcontroller
 * - Quectel M66 GSM module
 *
 * Author: IEEE FST RAS SBC
 * Date: 26/11/2023
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define gsm_bugger_size 500
char buf[100];
char gsm_buffer[gsm_bugger_size];
int gsm_index;
int debug_index;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART6_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int gsm_wait_status;

void gsm_on()
{

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_Delay(1000);
}

void gsm_cmd(char *cmd, int gsm_delay)
{
        gsm_wait_status=0;
        memset(gsm_buffer,'\0',sizeof(gsm_buffer));
        HAL_UART_Transmit(&huart1,(uint8_t*) cmd, strlen(cmd),1000);
        HAL_Delay(300);
        if(gsm_index>0)
        {
        HAL_Delay(10);
        gsm_index=0;
        gsm_wait_status=1;
        HAL_UART_Transmit(&huart6, (uint8_t *)gsm_buffer, strlen(gsm_buffer), 300);

        }
}

char http[80] = "https://api.thingspeak.com/update?api_key="; //The key is to introduced in the demonstration during TSYP
char url[80];
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
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  gsm_on();
  HAL_UART_Transmit(&huart6, (uint8_t *)"GSM_INIT\r\n", sizeof("GSM_INIT\r\n"), 500);
  int a = 1;
  char var[80];
  char data[] = "EMERGENCY MESSAGE....\r\n";
  /* USER CODE END 2 */

  /* Infinite loop */
  while (1)
  {
  /* USER CODE END WHILE */
    HAL_UART_Transmit(&huart1, (uint8_t *)"AT\r\n", sizeof("AT\r\n"), 300);
    HAL_Delay(300);
    HAL_UART_Transmit(&huart6, (uint8_t *)gsm_buffer, sizeof(gsm_buffer), 300);
    HAL_Delay(500);

    HAL_UART_Transmit(&huart1, (uint8_t *)"AT+QIFGCNT=0\r\n", sizeof("AT+QIFGCNT=0\r\n"), 300);
    HAL_Delay(500);
    HAL_UART_Transmit(&huart6, (uint8_t *)gsm_buffer, sizeof(gsm_buffer), 300);
    HAL_Delay(500);


    HAL_UART_Transmit(&huart1, (uint8_t *)"AT+QICSGP=1,\"Airteliot.com\"\r\n", sizeof("AT+QICSGP=1,\"Airteliot.com\"\r\n"), 300);
    HAL_Delay(500);
    HAL_UART_Transmit(&huart6, (uint8_t *)gsm_buffer, sizeof(gsm_buffer), 300);
    HAL_Delay(500);

    HAL_UART_Transmit(&huart1, (uint8_t *)"AT+QIREGAPP\r\n", sizeof("AT+QIREGAPP\r\n"), 300);
    HAL_Delay(1000);
    HAL_UART_Transmit(&huart6, (uint8_t *)gsm_buffer, sizeof(gsm_buffer), 300);
    HAL_Delay(1000);



    HAL_UART_Transmit(&huart1, (uint8_t *)"AT+QIACT\r\n", sizeof("AT+QIACT\r\n"), 300);
    HAL_Delay(3000);
    HAL_UART_Transmit(&huart6, (uint8_t *)gsm_buffer, sizeof(gsm_buffer), 300);
    HAL_Delay(1000);

    sprintf(var, "%s%d", http, a++);
    sprintf(url,"AT+QHTTPURL=%d,40\r\n",strlen(var));



    HAL_Delay(2000);
    HAL_UART_Transmit(&huart1, (uint8_t *)url, strlen(url), 300);
    HAL_Delay(1000);

    HAL_UART_Transmit(&huart1, (uint8_t *)var, strlen(var), 300);
        HAL_Delay(1000);

    HAL_UART_Transmit(&huart1, (uint8_t *)"AT+QHTTPGET=40\r\n", sizeof("AT+QHTTPGET=80\r\n"), 300);
    HAL_Delay(6000);
    HAL_UART_Transmit(&huart6, (uint8_t *)gsm_buffer, sizeof(gsm_buffer), 300);

    HAL_UART_Transmit(&huart1, (uint8_t *)"AT+QHTTPREAD=60\r\n", sizeof("AT+QHTTPREAD=60\r\n"), 300);
    HAL_Delay(2000);
    HAL_UART_Transmit(&huart6, (uint8_t *)gsm_buffer, sizeof(gsm_buffer), 300);

    HAL_UART_Transmit(&huart6, (uint8_t *)var, strlen(var), 300);
    HAL_Delay(10000);
    HAL_UART_Transmit(&huart1, (uint8_t *)"AT+QIDEACT\r\n", sizeof("AT+QIDEACT\r\n"), 300);
    HAL_Delay(1000);
    HAL_UART_Transmit(&huart6, (uint8_t *)gsm_buffer, sizeof(gsm_buffer), 300);
    HAL_UART_Transmit(&huart6, (uint8_t *)data, sizeof(data), 500);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
