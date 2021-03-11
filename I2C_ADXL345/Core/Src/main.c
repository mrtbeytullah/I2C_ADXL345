#include "main.h"

#define ADXL345_Adress 0xA6

uint8_t Data[6];
int16_t x,y,z;
float Gx,Gy,Gz;

I2C_HandleTypeDef hi2c1;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

void ADXL345_WriteRegister(uint8_t rAdress,uint8_t Value);
void ADXL345_ReadRegister(uint8_t rAdress,uint8_t size);
void ADXL345_Init();

int main(void)
{

  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();
  ADXL345_Init();

  while (1)
  {

	  ADXL345_ReadRegister(0x32, 6);
	  x=(Data[1]<<8)|Data[0];
	  y=(Data[3]<<8)|Data[2];
	  z=(Data[5]<<8)|Data[4];

	  Gx=x*0.0086;
	  Gy=y*0.0086;
	  Gz=z*0.0086;
	  HAL_Delay(100);
  }

}



void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

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


static void MX_I2C1_Init(void)
{




  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }


}


static void MX_GPIO_Init(void)
{


  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}


void ADXL345_WriteRegister(uint8_t rAdress,uint8_t Value)
{
	uint8_t Data[2];
	Data[0]=rAdress;
	Data[1]=Value;
  HAL_I2C_Master_Transmit(&hi2c1, ADXL345_Adress, Data,2,100);
}
void ADXL345_ReadRegister(uint8_t rAdress,uint8_t Size)
{
	HAL_I2C_Mem_Read(&hi2c1, ADXL345_Adress, rAdress, 1, Data,Size,100);
}
void ADXL345_Init()
{
	ADXL345_ReadRegister(0x00, 1);

	ADXL345_WriteRegister(0x2D,0);

	ADXL345_WriteRegister(0x2D, 0x08);

	ADXL345_WriteRegister(0x31, 0x01);
}

void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }

}
