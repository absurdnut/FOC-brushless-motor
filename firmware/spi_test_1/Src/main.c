/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define GPIO_PORT_SPI GPIOB
#define SPI_MOSI GPIO_PIN_5
#define SPI_MISO GPIO_PIN_4
#define SPI_SCK GPIO_PIN_3
#define SPI_CS GPIO_PIN_12
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// void SPI_GPIO_Transmit(uint16_t data, uint16_t* recv) { //第一边沿写入,第二边沿读取
//   HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_SCK, GPIO_PIN_RESET);  
//   HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_MOSI, GPIO_PIN_RESET); //在发送之前拉低时钟和mosi
//   // HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_CS, RESET); //选中芯片
//   *recv = 0x00;
//   for (int i = 15; i >= 0; i--) {
//       // 写入 MSB 到 MOSI 引脚
//       if (data & (1 << i)) {
//           HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_MOSI, GPIO_PIN_SET);
//           HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_SCK, GPIO_PIN_SET);  // SCK 置高
//           // HAL_Delay(1);
//           HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_SCK, GPIO_PIN_RESET);  // SCK 置低
//           // HAL_Delay(1);
//           if (HAL_GPIO_ReadPin(GPIO_PORT_SPI, SPI_MISO) == GPIO_PIN_SET){
//             *recv = *recv | (1 << i);
//           }
//           else{
//             *recv = *recv | (0 << i);
//           }

//       } else {
//           HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_MOSI, GPIO_PIN_RESET);
//           HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_SCK, GPIO_PIN_SET);  // SCK 置高
//           // HAL_Delay(1);
//           HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_SCK, GPIO_PIN_RESET);  // SCK 置低
//           // HAL_Delay(1);
//           if (HAL_GPIO_ReadPin(GPIO_PORT_SPI, SPI_MISO) == GPIO_PIN_SET){
//             *recv = *recv | (1 << i);
//           }
//           else{
//             *recv = *recv | (0 << i);
//           }
//       }
//   }
// }
void SPI_GPIO_Transmit(uint16_t data, uint16_t* recv) { //第一边沿读取,第二边沿写入


  HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_SCK, GPIO_PIN_RESET);  
  HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_MOSI, GPIO_PIN_RESET); //在发送之前拉低时钟和mosi
  // HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_CS, RESET); //选中芯片
  *recv = 0x00;
  for (int i = 15; i >= 0; i--) {
      // 写入 MSB 到 MOSI 引脚

      if (data & (1 << i)) {
          
          HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_SCK, GPIO_PIN_SET);  // SCK 置高
          // HAL_Delay(1);
          if (HAL_GPIO_ReadPin(GPIO_PORT_SPI, SPI_MISO) == GPIO_PIN_SET){
            *recv = *recv | (1 << i);
          }
          else{
            *recv = *recv | (0 << i);
          }
          HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_MOSI, GPIO_PIN_SET);
          HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_SCK, GPIO_PIN_RESET);  // SCK 置低
          // HAL_Delay(1);


      } else {
          
          HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_SCK, GPIO_PIN_SET);  // SCK 置高
          // HAL_Delay(1);
          if (HAL_GPIO_ReadPin(GPIO_PORT_SPI, SPI_MISO) == GPIO_PIN_SET){
            *recv = *recv | (1 << i);
          }
          else{
            *recv = *recv | (0 << i);
          }
          HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_MOSI, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(GPIO_PORT_SPI, SPI_SCK, GPIO_PIN_RESET);  // SCK 置低
          // HAL_Delay(1);

      }
  }
}

void DRV8301_init(void){

  uint16_t spi_read_0 = 0x8000; //读取00
  uint16_t spi_read_1 = 0x8800; //读取01
  uint16_t spi_read_2 = 0x9000; //读取02
  uint16_t spi_read_3 = 0x9800; //读取03
  uint16_t spi_write_2 = 0x1600; //写入2寄存器
  uint16_t spi_write_3 = 0x1600; //写入3寄存器
  uint16_t spi_recv[1] = {6};
  HAL_GPIO_WritePin(gate1_GPIO_Port, gate1_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(gate2_GPIO_Port, gate2_Pin,GPIO_PIN_SET);
  HAL_GPIO_WritePin(cs1_GPIO_Port, cs1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(cs1_GPIO_Port, cs1_Pin, GPIO_PIN_SET);
  while (spi_recv[0] != 0x1600){
    HAL_GPIO_WritePin(cs1_GPIO_Port, cs1_Pin, GPIO_PIN_RESET);
    SPI_GPIO_Transmit(spi_write_2, spi_recv);
    HAL_GPIO_WritePin(cs1_GPIO_Port, cs1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(cs1_GPIO_Port, cs1_Pin, GPIO_PIN_RESET);
    SPI_GPIO_Transmit(spi_read_2, spi_recv);
    HAL_GPIO_WritePin(cs1_GPIO_Port, cs1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(cs1_GPIO_Port, cs1_Pin, GPIO_PIN_RESET);
    SPI_GPIO_Transmit(spi_read_2, spi_recv);
    HAL_GPIO_WritePin(cs1_GPIO_Port, cs1_Pin, GPIO_PIN_SET);
  }
  // spi_recv[0] = 6;
  
  // while (spi_recv[0] != 0x1600){
  //   HAL_GPIO_WritePin(cs2_GPIO_Port, cs2_Pin, RESET);
  //   SPI_GPIO_Transmit(spi_write_2, spi_recv);
  //   HAL_GPIO_WritePin(cs2_GPIO_Port, cs2_Pin, SET);
  //   HAL_GPIO_WritePin(cs2_GPIO_Port, cs2_Pin, RESET);
  //   SPI_GPIO_Transmit(spi_read_2, spi_recv);
  //   HAL_GPIO_WritePin(cs2_GPIO_Port, cs2_Pin, SET);
  //   HAL_GPIO_WritePin(cs2_GPIO_Port, cs2_Pin, RESET);
  //   SPI_GPIO_Transmit(spi_read_2, spi_recv);
  //   HAL_GPIO_WritePin(cs2_GPIO_Port, cs1_Pin, SET);
  // }
}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

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
  /* USER CODE BEGIN 2 */
  // uint16_t spi_trans_1 = 0x9000;
  // uint16_t spi_trans_2 = 0x1600;  //只读寄存器0

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    DRV8301_init();

    HAL_Delay(100);

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
