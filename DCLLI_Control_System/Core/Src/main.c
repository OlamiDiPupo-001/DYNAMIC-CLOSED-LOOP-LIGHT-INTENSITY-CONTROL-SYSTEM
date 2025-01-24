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
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include "bh1750_config.h"
#include "led_config.h"
#include "btn_config.h"
#include "disp_config.h"
#include "lcd_config.h"
#include <stdio.h>
#include "aio.h"
#include "arm_math.h"
//#include "IIR1_biquad_df1.h"
//#include "FIR1_fir.h"
#include "menu_config.h"
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
uint8_t tx_buffer[8];
const int tx_msg_len = 4;
unsigned int Illuminance_lux_Int = 0;
float Illuminance_lux = 0.0f, lux_out = 0.0f;
float pot1_vol = 0.0f, integral = 0.0f, init_done = 0.0f, mode_f;
float duty_cycle = 0.0f, duty_change = 0.0f;
float Ki = 0.05, error = 0.0f, aim = 0.0f;
float max_lux = 0.0f, min_lux = 0.0f, pot_out = 0.0f;


enum InputMode {
  ANALOG_MODE = 1,
  SERIAL_MODE = 0
};
enum InputMode inputMode = SERIAL_MODE;  // Default to serial mode
/* USER CODE END PV */
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
/*
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim == &htim7)
  {
    HAL_ADC_Start(&hadc1);
      if(HAL_ADC_PollForConversion(&hadc1, ADC1_TIMEOUT) == HAL_OK)
      // Read potentiometer value and map it to lux range
      pot1_vol = ADC_REG2DUTY(HAL_ADC_GetValue(&hadc1));
      pot_out = min_lux + (pot1_vol / 100.0f) * (max_lux - min_lux);
        // POTENTIOMETER CONTROL

        static unsigned int cnt = 0;
        cnt++;
      }
//      MENU_ROUTINE(&hmenu);
}
*/
volatile uint8_t timer_flag = 0;

// Modify the callback to set the flag
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim7)
  {
    // Set the flag to indicate that the timer has elapsed
    timer_flag = 1;
  }
}
/**
  * @brief  Rx Transfer completed callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart == &huart3)
  {
    aim = strtol((char*)tx_buffer, 0, 10);
    // SERIAL PORT CONTROL
    HAL_UART_Receive_IT(&huart3, tx_buffer, tx_msg_len);
  }
}

void displayDigitalOnSerialPort(void)
{
  uint8_t tx_buffer[128];
  int tx_msg_len = sprintf((char *)tx_buffer, "Reference_Lux = %u,        ILLuminance = %5u.%03u,        PWM = %u,        LED_Output = %u\r",
      (int)aim, Illuminance_lux_Int / 1000, Illuminance_lux_Int % 1000, (unsigned int)duty_cycle, (unsigned int)lux_out);
  HAL_UART_Transmit(&huart3, tx_buffer, tx_msg_len, 100);

}

void displayAnalogueOnSerialPort(void)
{
  uint8_t tx_buffer[128];
  int tx_msg_len = sprintf((char *)tx_buffer, "Pot_Value = %u,        ILLuminance = %5u.%03u,        PWM = %u,        LED_Output = %u\r",
      (unsigned int)pot_out, Illuminance_lux_Int / 1000, Illuminance_lux_Int % 1000, (unsigned int)duty_cycle, (unsigned int)lux_out);
  HAL_UART_Transmit(&huart3, tx_buffer, tx_msg_len, 100);

}
void displayModeSelection(void) {
    uint8_t tx_buffer[128];
    int tx_msg_len = sprintf((char *)tx_buffer, "Which mode do you want?A. Potentialmeter? Or B. Terminal?\r\n");
    HAL_UART_Transmit(&huart3, tx_buffer, tx_msg_len, 100);
}

void displayOnLCD(void)
{
  LCD_DIO_SetCursor(&hlcd1, 1, 0);
  LCD_DIO_printStr(&hlcd1, "Lux: ");
  LCD_DIO_printDecInt(&hlcd1, Illuminance_lux_Int / 1000);
  LCD_DIO_printStr(&hlcd1, ".");
  LCD_DIO_printDecInt(&hlcd1, Illuminance_lux_Int % 1000);
  DISP_TM1637_printDecUInt(&hdisp2, Illuminance_lux_Int);
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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_TIM9_Init();
  MX_TIM7_Init();
  MX_USART3_UART_Init();
  MX_TIM5_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  BH1750_Init(&hbh1750);
  LED_PWM_Init(&hld1);
  HAL_UART_Receive_IT(&huart3, tx_buffer, tx_msg_len);

  LCD_DIO_Init(&hlcd1);
  LCD_DIO_printStr(&hlcd1, "Light_Intensity");
  DISP_TM1637_SetBrightness(&hdisp2, 2);

  MENU_Init(&hmenu);
  HAL_TIM_Base_Start_IT(&htim7);
//  HAL_TIM_Base_Start_IT(hmenu.Timer);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    // Check if the user wants to switch input mode
    if (BTN_DIO_EdgeDetected(&husrbtn) == BTN_PRESSED_EDGE) {
        // Toggle input mode
        inputMode = (inputMode == SERIAL_MODE) ? ANALOG_MODE : SERIAL_MODE;

        // Wait for button release to avoid repeated mode changes
        while (BTN_DIO_Read(&husrbtn) == BTN_PRESSED);
    }
    if(BTN_DIO_EdgeDetected(&hbtn2) == BTN_PRESSED_EDGE && hmenu.Item->Next != NULL)
    {
      hmenu.Item = hmenu.Item->Next;
      hmenu.ItemChanged = 1;
    }
    if(BTN_DIO_EdgeDetected(&hbtn1) == BTN_PRESSED_EDGE && hmenu.Item->Prev != NULL)
    {
      hmenu.Item = hmenu.Item->Prev;
      hmenu.ItemChanged = 1;
    }

    // Perform initialization only once
    if (!init_done)
      {
          displayModeSelection();
          // Set duty cycle to 0 and record min_lux
          LED_PWM_WriteDuty(&hld1, 0);
          HAL_Delay(1000);  // Adjust the delay as needed
          min_lux = BH1750_ReadIlluminance_lux(&hbh1750);

          // Set duty cycle to 100 and record max_lux
          LED_PWM_WriteDuty(&hld1, 100);
          HAL_Delay(1000);  // Adjust the delay as needed
          max_lux = BH1750_ReadIlluminance_lux(&hbh1750);

          init_done = 1;
      }
    // Read illuminance value
    Illuminance_lux = BH1750_ReadIlluminance_lux(&hbh1750);
    Illuminance_lux_Int = Illuminance_lux * 1000.0f;

    LED_DIO_Write(&hldg1, Illuminance_lux_Int > 600000.0f);
    LED_DIO_Write(&hldb1, Illuminance_lux_Int > 1000000.0f);
    LED_DIO_Write(&hldr1, Illuminance_lux_Int > 1500000.0f);

    if (inputMode == SERIAL_MODE)
    {
        // Use aim value from user input
//      error = aim - Illuminance_lux;
 //     displayDigitalOnSerialPort();
    }
    else
    {
//        error = pot_out - Illuminance_lux;
 //     displayAnalogueOnSerialPort();
    }

//    error = 0.05 * (max_lux - min_lux);     //5% of control range
    error = aim - Illuminance_lux;  //Digital calculate
//    error = pot_out - Illuminance_lux;      //Analogue calculate
    integral += error;
    duty_cycle = Ki * integral;
    lux_out = min_lux + (duty_cycle * 0.01 * (max_lux - min_lux)); //led out lux
    LED_PWM_WriteDuty(&hld1, duty_cycle);

//    MENU_ROUTINE(&hmenu);
    displayDigitalOnSerialPort();
//    displayAnalogueOnSerialPort();
    displayOnLCD();
    HAL_Delay(1000);
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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
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
    // Blink an LED to indicate an error
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    HAL_Delay(500);
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
