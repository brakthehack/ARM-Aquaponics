//ds_nordic.c
#include <ds_nordic.h>

void ds_nordic_init(void){


  /* Pin Assignments
GND 	GND
IRQ 	PC3
MISO 	PB4
MOSI 	PB5
SCK 	PB3
CSN 	PC1
CE 	    PC2
VCC 	3V 

2nd
CSN     PB2
*/

  // Setup SPI2 to talk to the SD card
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef SPI_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 , ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // Prepare to use DMA to accelerate the transactions */
  
  // Initialization for SCK, MISO, MOSI
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_6);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_6);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_6);

  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //IRQ
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  //1st CSN CE
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  //2nd CSN
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 

  nordic1_CS_HIGH();
  nordic2_CS_HIGH();

  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SPI3);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  // SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPI3, &SPI_InitStructure);

  /* Configure the RX FIFO Threshold */
  SPI_RxFIFOThresholdConfig(SPI2, SPI_RxFIFOThreshold_QF);
  /* Enable SPI1 */
  SPI_Cmd(SPI3, ENABLE);
  
  nrf24l01_initialize_debug(false, 32, true); 

} 

uint8_t ds_nordic_send_spi_byte(uint8_t data) {
  SPI_SendData8(SPI3,data);
  while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_ReceiveData8(SPI3);      
}
