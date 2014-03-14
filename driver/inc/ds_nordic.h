//ds_nordic.h
#include <stm32f30x.h>
#include <nrf24l01.h>

#define nordic1_CS_HIGH() GPIO_SetBits(GPIOC, GPIO_Pin_1)
#define nordic1_CS_LOW() GPIO_ResetBits(GPIOC, GPIO_Pin_1)

#define nordic2_CS_HIGH() GPIO_SetBits(GPIOB, GPIO_Pin_2)
#define nordic2_CS_LOW() GPIO_ResetBits(GPIOB, GPIO_Pin_2)

void ds_nordic_init(void);
int spiReadWrite(SPI_TypeDef *SPIx,uint8_t *rbuf, const uint8_t *tbuf, int cnt, uint16_t speed);
uint8_t ds_nordic_send_spi_byte(uint8_t data);
