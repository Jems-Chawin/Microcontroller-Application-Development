/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "i2c.h"
#include "rng.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "am2320.h"
#include "ILI9341_Touchscreen.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "shanpic.h"
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
/* Private variables ---------------------------------------------------------*/
uint8_t screen_state = 0;
uint8_t select_screen = 0;
char ch;
uint8_t r = 0;
uint8_t g = 0;
uint8_t b = 0;
float temperature, humidity;
uint16_t TP_Read(void);
void TP_Write(uint8_t value);
uint8_t TP_Read_Coordinates(uint16_t Coordinates[2]);
uint8_t TP_Touchpad_Pressed(void);
uint32_t count=0;
uint16_t rgb_to_uint16(uint8_t r, uint8_t g, uint8_t b) {
    uint16_t red = (r >> 3) & 0x1F;    // Scale and shift red to 5 bits
    uint16_t green = (g >> 2) & 0x3F;  // Scale and shift green to 6 bits
    uint16_t blue = (b >> 3) & 0x1F;   // Scale and shift blue to 5 bits
    return (red << 11) | (green << 5) | blue; // Combine the values into a uint16
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

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

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

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
  MX_USART3_UART_Init();
  MX_SPI5_Init();
  MX_TIM1_Init();
  MX_RNG_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  ILI9341_Init();//initial driver setup to drive ili9341
  HAL_UART_Receive_IT(&huart3, (uint8_t*) &ch, 1);
  Am2320_HandleTypeDef Am2320_;
  Am2320_ = am2320_Init(&hi2c1, 0x5C << 1);
  char group[50] = "Group No.20";
  char first_name[50] = "Pollawat";
  char last_name[50] = "Patllawol";
  char student_id[50] = "64011201";
  ILI9341_Fill_Screen(WHITE);
  ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  //----------------------------------------------------------PERFORMANCE TEST
//	  first screen
	  if (select_screen == 0){
		  char tempC[30];
		  char humid[30];
		  char rper[30];
		  char gper[30];
		  char bper[30];

		  uint16_t mix_color = rgb_to_uint16(r*255/10,g*255/10,b*255/10);

		  am2320_GetTemperatureAndHumidity(&Am2320_, &temperature, &humidity);

		  sprintf(tempC, "%.1f C", temperature);
		  sprintf(humid, "%.1f %cRH", humidity,'%');
		  sprintf(rper, "%d %c",r*10,'%');
		  sprintf(gper, "%d %c",g*10,'%');
		  sprintf(bper, "%d %c",b*10,'%');


	  //	  top bar
		  ILI9341_Draw_Filled_Circle(160,45,30,mix_color);
		  ILI9341_Draw_Text(tempC, 30, 40, BLACK, 2, WHITE);
		  ILI9341_Draw_Text(humid, 210, 40, BLACK, 2, WHITE);
	  //	  red bar
		  ILI9341_Draw_Filled_Circle(40,95,20,RED);
		  ILI9341_Draw_Filled_Rectangle_Coord(75,83,220,107,0xF410);
		  ILI9341_Draw_Filled_Rectangle_Coord(75,83,75 + (145*r/10),107,RED);
		  ILI9341_Draw_Text(rper, 250, 90, BLACK, 2, WHITE);
	  //	  green bar
		  ILI9341_Draw_Filled_Circle(40,145,20,GREEN);
		  ILI9341_Draw_Filled_Rectangle_Coord(75,133,220,157,0X9772);
		  ILI9341_Draw_Filled_Rectangle_Coord(75,133,75 + (145*g/10),157,GREEN);
		  ILI9341_Draw_Text(gper, 250, 140, BLACK, 2, WHITE);
	  //	  blue bar
		  ILI9341_Draw_Filled_Circle(40,195,20,BLUE);
		  ILI9341_Draw_Filled_Rectangle_Coord(75,183,220,207,0xAEDC);
		  ILI9341_Draw_Filled_Rectangle_Coord(75,183,75 + (145*b/10),207,BLUE);
		  ILI9341_Draw_Text(bper, 250, 190, BLACK, 2, WHITE);
		  HAL_Delay(20);
	  }

	  while(1){
		  if(select_screen == 0){
			  char tempC[30];
			  char humid[30];
			  am2320_GetTemperatureAndHumidity(&Am2320_, &temperature, &humidity);

			  sprintf(tempC, "%.1f C", temperature);
			  sprintf(humid, "%.1f %cRH", humidity,'%');
			  ILI9341_Draw_Text(tempC, 30, 40, BLACK, 2, WHITE);
			  ILI9341_Draw_Text(humid, 210, 40, BLACK, 2, WHITE);
			  HAL_Delay(20);
		  }
		  if(select_screen == 1 && count>=5){
			  select_screen = 0;
			  ILI9341_Fill_Screen(WHITE);
			  break;
		  }
		  if(TP_Touchpad_Pressed())
		  {
			  uint16_t x_pos = 0;
			  uint16_t y_pos = 0;

			  HAL_GPIO_WritePin(GPIOB, LD3_Pin|LD2_Pin, GPIO_PIN_SET);

			  uint16_t position_array[2];

			  if(TP_Read_Coordinates(position_array) == TOUCHPAD_DATA_OK)
			  {
				  x_pos = position_array[0];
				  y_pos = position_array[1];
				  if (select_screen == 0){
					  if((130 <= y_pos && y_pos <= 190) && (165 <= x_pos && x_pos <= 240)){
						  select_screen = 1;
						  count = 0;
						  HAL_TIM_Base_Start_IT(&htim1);
						  HAL_GPIO_WritePin(GPIOB, LD3_Pin|LD2_Pin, GPIO_PIN_RESET);
						  break;
					  }
					  if((20 <= y_pos && y_pos <= 60) && (125 <= x_pos && x_pos <= 165)){
						  r = (r+1)%11;
						  HAL_GPIO_WritePin(GPIOB, LD3_Pin|LD2_Pin, GPIO_PIN_RESET);
						  break;
					  }
					  if((20 <= y_pos && y_pos <= 60) && (75 <= x_pos && x_pos <= 115)){
						  g = (g+1)%11;
						  HAL_GPIO_WritePin(GPIOB, LD3_Pin|LD2_Pin, GPIO_PIN_RESET);
						  break;
					  }
					  if((20 <= y_pos && y_pos <= 60) && (25 <= x_pos && x_pos <= 65)){
						  b = (b+1)%11;
						  HAL_GPIO_WritePin(GPIOB, LD3_Pin|LD2_Pin, GPIO_PIN_RESET);
						  break;
					  }
				  }
				  else{
					  if ((25 <= y_pos && y_pos <= 125) && (110 <= x_pos && x_pos <= 210)){
						  select_screen = 0;
						  ILI9341_Fill_Screen(WHITE);
						  HAL_GPIO_WritePin(GPIOB, LD3_Pin|LD2_Pin, GPIO_PIN_RESET);
						  break;
					  }
				  }


			  }
		  }
		  else{
			  HAL_GPIO_WritePin(GPIOB, LD3_Pin|LD2_Pin, GPIO_PIN_RESET);
		  	  }
	  	  }
	//	  second screen
	  	  if (select_screen == 1){
	  		  ILI9341_Fill_Screen(WHITE);
			  ILI9341_Set_Rotation(SCREEN_HORIZONTAL_1);
			  uint16_t count = 0;
			  uint16_t mix_color = rgb_to_uint16(r*255/10,g*255/10,b*255/10);
			  for (int y=0;y<117;y++){
				  for (int x=0;x<100;x++){
					  ILI9341_Draw_Pixel(25+x,40+y,shan_pic[count]);
					  count++;
				  }
			  }
			  ILI9341_Draw_Text(group, 150, 40, mix_color, 2, WHITE);
			  ILI9341_Draw_Text(first_name, 150, 80, mix_color, 2, WHITE);
			  ILI9341_Draw_Text(last_name, 150, 120, mix_color, 2, WHITE);
			  ILI9341_Draw_Text(student_id, 150, 160, mix_color, 2, WHITE);
			  HAL_Delay(20);
	  	  }

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
  RCC_OscInitStruct.PLL.PLLN = 200;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart3){
		HAL_UART_Receive_IT(&huart3,(uint8_t*)&ch,1);
		if (ch == 'r'){
			r = (r+1)%11;
		}
		if (ch == 'g'){
			g = (g+1)%11;
		}
		if (ch == 'b'){
			b = (b+1)%11;
		}
		screen_state = 0;
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
  while(1)
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
