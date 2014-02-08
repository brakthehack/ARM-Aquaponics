/* f3d_pressure.c --- 
 * 
 * Filename: f3d_pressure.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: Ching Yuen Ng/ Brian Rak
 * Created: Thu Sep 26 07:05:26 2013
 * Last-Updated: Ching Yuen Ng/ Brian Rak
 *           By: 02-7-2014
 *     Update #: 1
 */

#include <stm32f30x.h>
#include <f3d_pressure.h>

void f3d_pressure_interface_init() {
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef SPI_InitStructure;
  // enable the clocks
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 , ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  
  // Initialization for SCK, MISO, MOSI
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_5);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_5);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_5);
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure GPIO PIN for the Chip select */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  PRESSURE_CS_HIGH();

  /* SPI configuration */
  SPI_I2S_DeInit(SPI1);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPI1, &SPI_InitStructure);
  SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
  SPI_Cmd(SPI1, ENABLE);

}


static uint8_t f3d_pressure_sendbyte(uint8_t byte) {
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  SPI_SendData8(SPI1, byte);
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  return (uint8_t)SPI_ReceiveData8(SPI1);
}

void f3d_pressure_write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t
NumByteToWrite)
{
  if(NumByteToWrite > 0x01) {
    WriteAddr |= (uint8_t) ((uint8_t)0x40); // Set bit 6 if multibyte command
  }
  PRESSURE_CS_LOW();
  f3d_pressure_sendbyte(WriteAddr);
  while(NumByteToWrite >= 0x01) {
    f3d_pressure_sendbyte(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }
  PRESSURE_CS_HIGH();
}

void f3d_pressure_read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead) {
  if (NumByteToRead > 0x01) {
    ReadAddr |= (uint8_t)(0x80 | 0x40); // If sending more that one byte set multibyte commands
  }
  else {
    ReadAddr |= (uint8_t) (0x80); // Else just set the read mode
    }
  PRESSURE_CS_LOW();
  f3d_pressure_sendbyte(ReadAddr);
  while(NumByteToRead > 0x00) {
    *pBuffer = f3d_pressure_sendbyte(((uint8_t)0x00));
    NumByteToRead--;
    pBuffer++;
  }
  PRESSURE_CS_HIGH();
}

void f3d_pressure_init(void) {
  uint8_t ctrl1 = 0x00;
  f3d_pressure_interface_init();
  /* set bit 7 to 1 power on
			set bit 6:4 to 001 */
	ctrl1 |= (uint8_t) (((uint8_t)0x80) | ((uint8_t)0x00));
	f3d_pressure_write(&ctrl1, 0x20, 1);
}


void f3d_pressure_getdata(float *pData, float *tData) {
  uint8_t tmpbuffer[5] ={0}; // first 3 are pressure data, next 2 are temp data
  int16_t RawData[2] = {0};
  float sensitivity = 0;
  int i = 0, BYTES_TO_READ = 5;
  f3d_pressure_read(tmpbuffer,0x28,5);
  // Read the data for pressure and temp
  // pressure
	RawData[0] = (int16_t) ((int16_t) tmpbuffer[0] &
													 (int16_t) tmpbuffer[1] &
														(int16_t) tmpbuffer[2]);
	*pData = RawData[0]/4096;

	// temp
	RawData[1] = (int16_t) ((int16_t) tmpbuffer[3] &
													(int16_t) tmpbuffer[4]);
	*tData = 42.5 + RawData[1]/480;
}

