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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
void mapTemperatureToColor(float temperature,int *color[3]) {
    //int color[3];
	int minval=0;
    if (temperature <= 25) {
        color[0] = minval;    // Red
        color[1] = minval;    // Green
        color[2] = 255;  // Blue
    }
    else {
        if (temperature >= 25 && temperature <= 27.5)
        {
            color[0] = minval;  // Red
            color[1] = minval + (int)((temperature - 25) * 102);    // Green
            color[2] = 255 - (int)((temperature - 25) * 102);    // Blue
        }
        else
        {
            if (temperature >= 27.5 && temperature <= 30)
            {
                color[0] = minval + (int)((temperature - 27.5) * 102);    // Red
                color[1] = 255 - (int)((temperature - 27.5) * 102);    // Green
                color[2] = minval;  // Blue
            }
            else
            {
                color[0] = 255;    // Red
                color[1] = minval;    // Green
                color[2] = minval;  // Blue
            }
        }
    }
//    for (int i = 0; i < 3; i++)
//    {
//        color[i] = 20*i;
//    }
//    return color;
}
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
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  AHT20INIT();
  OLED_Init();
  OLED_ColorTurn(0);
  OLED_DisplayTurn(0);
  float tem,hum;
  float temlist[20];
  float humlist[20];
  char msg[50];
  char demsg[50];
  char msg1[11];
  char msg2[11];

  int i=0,j=0;
  int idx=0;
  float init1,init2;
  float sum1=0;
  float sum2=0;
  AHT20READ(&init1,&init2);
  int dataline[128][32];
  for(i=0;i<128;i++)
  {
	  for(j=0;j<32;j++)
	  {
		  dataline[i][j]=0;
	  }
  }
//  for(i=0;i<128;i++)
//  {
//		  dataline[i][16]=1;
//  }
  for(i=0;i<32;i++)
  {
		  dataline[126][i]=1;
  }
  for(i=0;i<20;i++)
  {
	  temlist[i]=init1;
	  humlist[i]=init2;
  }
  int ro=0;
  int rl=0;
  int color[3]={0,0,0};
  int curdata=0;
  int xloop=126;
  float st=27.15;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //HAL_UART_Transmit(&huart2, "begin", 5, 200);
	  AHT20INIT();
//	  OLED_Init();
//	  OLED_ColorTurn(0);
//	  OLED_DisplayTurn(0);
	  AHT20READ(&tem,&hum);
	  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)==0)
	  {
		  ro=(ro+1)%2;
		  OLED_DisplayTurn(ro);
		  HAL_Delay(50);
	  }
	  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13)==0)
	  {
		  rl=(rl+1)%2;
		  OLED_ColorTurn(rl);
		  HAL_Delay(50);
	  }
	  temlist[idx]=tem;
	  humlist[idx]=hum;
	  sum1=0;
	  sum2=0;
	  idx=(idx+1)%20;
	  for(i=0;i<20;i++)
	  {
		  sum1+=temlist[i];
		  sum2+=humlist[i];
	  }

	  sprintf(msg,"TEM:%.2fC,HUM:%.2f%%\r\n",(sum1/20)*0.98,(sum2/20)*1.00);
	  curdata=(int)(
			  8*((sum1/20)-st)
			  );
	  if(curdata<=-16)
	  {
		  curdata=-8;
		  st=(sum1/20);
		  for(i=0;i<32;i++)
		  {
			  dataline[126][i]=1;
		  }
	  }
	  else
	  {
		  if(curdata>=15)
		  {
			  st=(sum1/20);
			  for(i=0;i<32;i++)
			  {
				  dataline[126][i]=1;
			  }
		  }
		  else
		  {
			  for(i=0;i<32;i++)
			  {
				  dataline[127][i]=0;
			  }
			  dataline[127][curdata+16]=1;
		  }
	  }
	  //xloop=(xloop+1)%128;

	  for(i=1;i<128;i++)
	  {
		  for(j=0;j<32;j++)
		  {
			  dataline[i-1][j]=dataline[i][j];
		  }
	  }

//	  if(  ((sum1/20)*0.98)<=25  )
//	  {
//		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
//		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
//		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
//	  }
//	  else
//	  {
//		  if(  ((sum1/20)*0.98)>=30  )
//		  {
//			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
//			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 0);
//			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
//		  }
//		  else
//		  {
//			  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
//			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, 1);
//			  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
//		  }
//	  }

	  //*color = mapTemperatureToColor((sum1/20)*0.98);
	  mapTemperatureToColor(
			  (sum1/20)*0.98,
			  &color
	  );
	  __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, color[0]*0.6);
	  __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, color[1]*1.0);
	  __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, color[2]*0.8);

	  sprintf(demsg,"%d,%d,%d%\r\n--",
			  color[0],
			  color[1],
			  color[2]);
	  HAL_UART_Transmit(&huart2, demsg, strlen(demsg), 200);

//	  __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, 200);
//	  __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 0);
//	  __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);


	  for(i=0;i<10;i++)
	  {
		  msg1[i]=msg[i];
		  msg2[i]=msg[i+11];
	  }
	  msg1[10]='\0';
	  msg2[10]='\0';

	  //HAL_UART_Transmit(&huart2, msg1, strlen(msg1), 200);
	  //HAL_UART_Transmit(&huart2, msg2, strlen(msg2), 200);
	  OLED_ShowString(8, 44, msg1, 16, 1);
	  //OLED_ShowString(8, 44, msg2, 16, 1);
	  for(i=0;i<128;i++)
	  {
		  for(j=0;j<32;j++)
		  {
			  OLED_DrawPoint(i,32-j,dataline[i][j]);
		  }
	  }
	  OLED_Refresh();
	  //HAL_Delay(10);
	  //HAL_UART_Transmit(&huart2, "end", 3, 200);
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
