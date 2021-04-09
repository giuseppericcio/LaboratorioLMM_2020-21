#include <stm32f30x.h>

#define V 1.52
#define slope 4.6

float tensione;
float temperatura; 

void main()
{
  //Abilitazione clock di ADC12 e GPIOA
  RCC->AHBENR |= RCC_AHBENR_ADC12EN;
  
  //Abilitazione del sensore di temperatura
  ADC1_2->CCR |= ADC12_CCR_TSEN;
  
  //Imposto l'ADC1
  //Regolatore di tensione
  ADC1->CR &= ~ADC_CR_ADVREGEN_1;
  ADC1->CR |= ADC_CR_ADVREGEN_0;
  for(int i=0; i<1000; i++);
  
  //Configuro il clock
  ADC1_2->CCR |= ADC12_CCR_CKMODE_0;
  
  //Calibrazione ADC
  ADC1->CR |= ADC_CR_ADCAL;
  while((ADC1->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL);
  
  //Abilitazione ADC
  ADC1->CR |= ADC_CR_ADEN;
  while((ADC1->ISR & ADC_ISR_ADRD) != ADC_ISR_ADRD);
  
  //Configurazione ADC
  ADC1->CFGR |= ADC_CFGR_CONT;
  ADC1->SQR1 &= ~ADC_SQR1_L;
  ADC1->SQR1 = ADC_SQR1_SQ1_4;
  ADC1->SMPR2 |= ADC_SMPR2_SMP16_1;
  
  //Conversione
  ADC1->CR |= ADC_CR_ADSTART;
  
  while(1){
    
    //attesa EOC = 1
    while((ADC1->ISR & ADC_ISR_EOC) != ADC_ISR_EOC);
    
    tensione = ADC1->DR*(3.0/4096.0); //tensione = conteggi*(VDD/2^N)
    
    temperatura = ((V - tensione)/slope) + 25;
    
  }
}
