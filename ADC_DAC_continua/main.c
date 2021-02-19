#include <stm32f30x.h>
#include <stdio.h>

//DAC
int DOR_in = 1000;
float tensione_out;

//ADC
float tensione_in;
int DOR_out;

void main()
{
  //Abilito i clock delle periferiche
  RCC->APB1ENR |= RCC_APB1ENR_DACEN;    //DAC
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;    //GPIOA
  RCC->AHBENR |= RCC_AHBENR_ADC12EN;    //ADC12
  
  //Imposto PA4 e PA2 in modalità analogica
  GPIOA->MODER |= GPIO_MODER_MODER4;
  GPIOA->MODER |= GPIO_MODER_MODER2;
  
  //Abilito il DAC
  DAC->CR |= DAC_CR_EN1;
  DAC->DHR12R1 = DOR_in;
  for (int i = 0; i<1000; i++);
  
  //Imposto l'ADC1
  //Regolatore di tensione
  ADC1->CR &= ~ADC_CR_ADVREGEN_1;
  ADC1->CR |= ADC_CR_ADVREGEN_0;
  for (int i = 0; i < 1000; i++);
  
  //Configuro il clock
  ADC1_2->CCR |= ADC12_CCR_CKMODE_0;
  
  //Calibrazione ADC
  ADC1->CR |= ADC_CR_ADCAL;
  while((ADC1->CR & ADC_CR_ADCAL) == ADC_CR_ADCAL);
  
  //Abilitazione ADC
  ADC1->CR |= ADC_CR_ADEN;
  while((ADC1->ISR & ADC_ISR_ADRD) != ADC_ISR_ADRD);
  
  //Configurazione ADC
  ADC1->CFGR &= ~ADC_CFGR_CONT;
  ADC1->SQR1 = (3<<6);
  ADC1->SQR1 &= ~ADC_SQR1_L;
  ADC1->SMPR1 |= ADC_SMPR1_SMP3;
  
  //Abilito l'interruzione sull'EOC dell'ADC1
  ADC1->IER |= ADC_IER_EOC;
  NVIC->ISER[0] |= 1<<18;
  
  //Conversione
  ADC1->CR |= ADC_CR_ADSTART;
  
  while(1);
}

void ADC1_2_IRQHandler() {
  
  //Risultato DAC
  tensione_out = DOR_in * (3.0/4095.0);
  
  printf("Tensione in uscita al DAC:\n");
  printf("Parola codice in ingresso: %d \n", DOR_in);
  printf("Tensione: %fV \n", tensione_out);
  
  //Risultato ADC1
  DOR_out = ADC1->DR;
  tensione_in = DOR_out * (3.0/4096.0);
  
  printf("Parola codice in uscita all'ADC:\n");
  printf("Tensione in ingresso: %fV \n", tensione_in);
  printf("Parola codice: %d \n", DOR_out);
  
} 
