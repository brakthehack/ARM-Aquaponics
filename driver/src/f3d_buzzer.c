/* f3d_buzzer.c --- 
 * 
 * Filename: f3d_buzzer.c
 * Description: 
 * Author: Ching Yuen Ng/ Brian Rak
 * Maintainer: ching, brak
 * Created: February 21 2014
 * Last-Updated: 02/21/2014
 *           By: ching, brak
 *     Update #: 1
 * Keywords: 
 * Compatibility: 
 * 
 */
/* Code: */
#include <stm32f30x_gpio.h>
#include <stm32f30x_tim.h>
#include <f3d_buzzer.h>

/*
    Code to call when using the systick
    to buzz
*/

void f3d_buzzer_init(void) {
  // initialize the port and pins for the 8 leds
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void f3d_buzzer_on(void) {
  GPIOD->BSRR = GPIO_Pin_12;
}
void f3d_buzzer_off(void) {
  GPIOD->BRR = GPIO_Pin_12;
}

/*
    Code to use when calling frequency(int)
*/
static void f3d_timer4_pin(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    // Enable the clock to Port D
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
    // Configure PD12 as an output
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_2);
}

void f3d_timer4_init(void) {
    

    // Enable the clock to Timer4
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    // Initialize at 2khz frequency
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = (SystemCoreClock / 4000) - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);
    TIM_CCxCmd(TIM4,TIM_Channel_1,TIM_CCx_Enable);
    TIM_Cmd(TIM4, ENABLE);
}

void f3d_frequency(uint32_t freq) {
    uint32_t period, prescale;
    if (freq) {
        freq<<=1; // multiply by 2
        period = (SystemCoreClock / freq) - 1;
        prescale = 1;
        while (period>0xFFFF) {
            period = ((SystemCoreClock/++prescale)/freq)-1;
            if (prescale>=0xFFFF) {
                break;
            }
        }
        TIM_TimeBaseStructure.TIM_Prescaler = prescale-1;
        TIM_TimeBaseStructure.TIM_Period = period;
        TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
        TIM_CCxCmd(TIM4,TIM_Channel_1,TIM_CCx_Enable);
    }
    else {
        TIM_CCxCmd(TIM4,TIM_Channel_1,TIM_CCx_Disable);
    }
}

/* f3d_buzzer.c ends here */
