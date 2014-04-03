#include <stm32f30x.h>

#define DAC_DHR12R2_ADDRESS  0x40007414
#define DAC_DHR8R1_ADDRESS   0x40007410


void f3d_dac_channel_setup(void);
void f3d_dac_init(void);
