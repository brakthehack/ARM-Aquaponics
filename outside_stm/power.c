#include <power.h>


/**
 * After this funtion is initialized, the ADC will be ready
 * to read values.  The values will be converted into a 12-bit number.
 */
void f3d_a2d_init_power(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    // initialize the clocks
    RCC_ADCCLKConfig(RCC_ADC12PLLCLK_Div2);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ADC12, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);


    // init pin as analog
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    // initialize voltage regulator and wait for it to stabilize
    ADC_VoltageRegulatorCmd(ADC2, ENABLE);
    f3d_delay_uS(10);


    ADC_CommonStructInit(&ADC_CommonInitStructure);
    ADC_CommonInitStructure.ADC_Mode=ADC_Mode_Independent;                                                      
    ADC_CommonInitStructure.ADC_Clock = ADC_Clock_AsynClkMode;                    
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;             
    ADC_CommonInitStructure.ADC_DMAMode = ADC_DMAMode_OneShot;                  
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = 0;          
    ADC_CommonInit(ADC2, &ADC_CommonInitStructure);


    ADC_StructInit(&ADC_InitStructure);   
    ADC_InitStructure.ADC_ContinuousConvMode = ADC_ContinuousConvMode_Enable;
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; 
    ADC_InitStructure.ADC_ExternalTrigConvEvent = ADC_ExternalTrigConvEvent_0;         
    ADC_InitStructure.ADC_ExternalTrigEventEdge = ADC_ExternalTrigEventEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_OverrunMode = ADC_OverrunMode_Disable;   
    ADC_InitStructure.ADC_AutoInjMode = ADC_AutoInjec_Disable;  
    ADC_InitStructure.ADC_NbrOfRegChannel = 1;
    ADC_Init(ADC2, &ADC_InitStructure);

    // configure specific adc chanel and timing
    ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_7Cycles5);

    // enable the adc and wait for it to become ready
    ADC_Cmd(ADC2, ENABLE);
    while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_RDY));

    // start the first conversation
    ADC_StartConversion(ADC2);

}


int f3d_read_adc_power(void) { 
    ADC_StartConversion(ADC2);
    while(ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC) == RESET);
    return(ADC_GetConversionValue(ADC2));
}
