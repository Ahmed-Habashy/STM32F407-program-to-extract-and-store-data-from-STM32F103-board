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
#include "dma.h"
#include "fatfs.h"
#include "usart.h"
#include "usb_host.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdbool.h>
#include  <stdio.h>
#include <stdlib.h>
#include <stdint.h>


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#ifdef __GNUC__
  /* With GCC, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern ApplicationTypeDef Appli_state ;
//FatFS variable
FATFS myUsbFatFS;
//USB Logical path
extern char USBHPath[4];  /* USBH logical drive path */
//File IO Variables
FIL myFile;
FRESULT res;
UINT byteswritten, bytesread;
char rwtext[100];
char readtext[200];
char rdtx[100];
char test[100];
int r ;
int  c=0;
int read_count=0;
char file_name[15]="RDSC.TXT";
char rxIMU[100];
char rxPod2[100] ;
char usb_buffer[100];
char wifi_code[5];
char tx[100] ;
UINT  count=0;
int msg_len=65;
bool write_flag = false ;
bool byte_check = false ;
bool read_flag = false ;
bool pass_correct = false;
bool destroy_file = false;
bool send_data = true ;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

void MX_USB_HOST_Process(void);

//1. USB test Write function
bool UsbTest_Write(void);
//2. USB test Read function
bool UsbTest_Read(void);

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
  HAL_Delay (65000);
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FATFS_Init();
  MX_UART4_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_USB_HOST_Init();
  /* USER CODE BEGIN 2 */
printf("Starting.\r\n");
HAL_UART_Receive_DMA(&huart2 , (uint8_t *)rxIMU,msg_len );
HAL_UART_Receive_DMA(&huart4 , (uint8_t *)rxPod2,msg_len  );
HAL_UART_Receive_DMA(&huart6 , (uint8_t *)wifi_code,5 );
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();
//		test[0]= '1';
//		test[1]= 0xff;
//    test[2]= 'c';
//		test[3]= 0x00;
//		test[4]= 0x33;
//		 r = strlen (test);
    /* USER CODE BEGIN 3 */
		switch(Appli_state)
		{
			case APPLICATION_READY:
				//  if(HAL_GPIO_ReadPin( PUSH_BUTTON_GPIO_Port , PUSH_BUTTON_Pin ) == GPIO_PIN_SET)
				      if( pass_correct == true)
        				{
									HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
									read_count=0;
									if(UsbTest_Read()) 
									{
										
										HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
										HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
										//HAL_Delay (5000);
									}
										else HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
						  	}
							 else if(destroy_file == true)
									f_unlink (file_name);
							 else if(UsbTest_Write()) 
								{
								HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);	
								HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
								HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
								}
							
							else HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
								 //		HAL_Delay(500);
				break;
			
			case APPLICATION_IDLE:
				break;
			
			case APPLICATION_DISCONNECT:
				HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
		  	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
			break;
			
			case APPLICATION_START:		
				/* Register the file system object to the FatFs module */
				if(f_mount(&myUsbFatFS, (TCHAR const*)USBHPath, 0) != FR_OK)
				{
				/* FatFs Initialization Error */
					Error_Handler();
				}
				else
				{
					HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
				}
				break;
			
			default:
				break;
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

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
}

/* USER CODE BEGIN 4 */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
 // HAL_UART_Transmit(&huart4, (uint8_t *)&ch, 1, 0xFFFF);
  HAL_UART_Transmit(&huart6, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	UNUSED(huart);
	if (wifi_code[0] == '*' && wifi_code[1] == '*' && wifi_code[2] == '*' && wifi_code[3] == '*' && wifi_code[4] == '*') 
	{
		pass_correct = true;
		send_data = false ;
	}
	else if (wifi_code[0] == 'x' && wifi_code[1] == 'x' && wifi_code[2] == 'x' && wifi_code[3] == 'x' && wifi_code[4] == 'x') 
	{
		destroy_file= true;
		send_data = false ;
	}
	else if ( send_data == true  ) //if (strlen(rxIMU)) send_data= true;
                       {
												 memset (wifi_code , '\0', 5);
//												 sprintf(rwtext, "%d#", count );
//												 HAL_UART_Transmit (&huart6 , (uint8_t *)rwtext , 6 , 10);
												// printf("#%d#", count );				 
												 HAL_UART_Transmit (&huart6 , (uint8_t *)rxIMU , msg_len , 200);   //strlen((char*)rxIMU)
												 HAL_UART_Transmit (&huart6 , (uint8_t *)rxPod2, msg_len , 200);
												 write_flag  = true;
						
//			                   if (strlen(rxPod2))	
//												 { 

//												   memset (rxPod2 , '\0', msg_len);												 
//											   } 
											 }
	}

bool UsbTest_Write()
{
	//Open/Create file for Writing
	//if(f_open(&myFile, "trial2.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) 
	if(f_open(&myFile, file_name , FA_OPEN_APPEND | FA_WRITE) != FR_OK)
	{
		return 0; //error
	}
	else
	{
		//Set text string to write to file
	  //	sprintf(rwtext, "Hello world from habashy test to the ability of storing speed\r\n");
		
		//write to file
		if (write_flag  == true) 
		{
			//printf("Writting to USB...\n");
			//res = f_write(&myFile, (uint8_t *)usb_buffer, strlen(usb_buffer), &byteswritten);
			//sprintf(rwtext, "%d#w", count );
			res = f_write(&myFile, (uint8_t *)rxIMU, msg_len , &byteswritten); //strlen((char*)rxIMU)
			//res = f_write(&myFile, (uint8_t *)rwtext, 7 , &byteswritten);
			//printf("W#");
			write_flag  = false;
			//count++;
			memset (rxIMU , '\0', msg_len);
		}
		if((byteswritten == 0) || (res != FR_OK))
		{
			return 0; //error
		}
	}
		//Close file
	f_close(&myFile);
	return 1; //success
}
bool UsbTest_Read(void)
{
	//Open file for Reading
	if(f_open(&myFile, file_name, FA_READ) != FR_OK) 
	{
		return 0; //error
	}
	else
	{ printf("Reading from USB storage...\r\n");
		do
		//Read text from files until NULL
			{ for(uint32_t i=0; i< 100; i++)
				{
					res = f_read(&myFile, (uint8_t*)&readtext [i], 1, &bytesread);
//					if (strlen(readtext) == 0 && i > 198) 
//						{
//							read_flag  = true;
//							break;
//						}	
					if(readtext[i] == '\n')
						{
							HAL_UART_Transmit (&huart6 , (uint8_t *)readtext , (i+1) , 100);
							memset (readtext , '\0', (i+1));
							bytesread = i;
							c += i ;
							read_count ++;
							break;
						}
				}
	
   				//Reading error handling
	       	if(c == 0) return 0;
						if (read_flag == true) break ;
	     } while (destroy_file == false);
	 } 
	//Close file
	f_close(&myFile);
	 read_flag = false ;
	 pass_correct  = false ;
	 send_data = false ;
	 memset (wifi_code ,'\0',5);
	 printf("Reading done successfully...\r\n");
	 printf("**************************************\r\n");
	 printf("**************************************\r\n");
	return 1; //success
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
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
