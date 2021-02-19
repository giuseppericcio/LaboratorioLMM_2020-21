#include <stm32f30x.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define N 100
#define pi 3.14

int fck = 72000000;
short int vettDAC[N];
short int vettADC[N];
short int j = 0;
short int i = 0;
float tensione = 0;
float k = 0;

void main()
{
  //Abilito i clock delle periferiche
  RCC->APB1ENR |= RCC_APB1ENR_DACEN;    //DAC
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;    //GPIOA
  RCC->AHBENR |= RCC_AHBENR_ADC12EN;    //ADC12
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;   //TIM2
  
  //Imposto PA4 e PA2 in modalità analogica
  GPIOA->MODER |= GPIO_MODER_MODER4;
  GPIOA->MODER |= GPIO_MODER_MODER2;
  
  //Abilito il DAC con TSEL=100, trigger TIM2
  DAC->CR |= DAC_CR_TEN1;
  DAC->CR |= DAC_CR_TSEL1_2;
  DAC->CR |= DAC_CR_EN1;
  
  //Abilito le interruzioni del TIMER2
  TIM2->DIER |= TIM_DIER_UIE;
  NVIC->ISER[0] |= 1<<28;
  
  //Abilito le interruzioni sull'ADC1
  ADC1->IER |= ADC_IER_EOC;
  NVIC->ISER[0] |= 1<<18;
  
  //Genero i campioni
  for (i = 0; i<N; i++){
    k = (float) i/N;
    tensione = 1.5 + 1.5*sin(2*pi*k);
    vettDAC[i] = (short int) (tensione * 4095/3);
  }
  
  //Imposto l'ADC1
  //Regolatore di tensione
  ADC1->CR &= ~ADC_CR_ADVREGEN_1;
  ADC1->CR |= ADC_CR_ADVREGEN_0;
  for (int i = 0; i < 1000; i++);       //Attesa di 10us
  
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
  
  /* Calcolo Nck conteggi
  *  fck = 72MHz frequenza del clock
  *  Tck = 1/fck = 1/72MHz periodo del clock
  *  tupdate = T/N = 10^(-3)/10^2 = 10us
  *  Nck = Deltat/Tck = 10^(-5) * 72000000
  *  dunque TIM2_ARR = 10^(-5) * 72000000
  */
  //Imposto il TIM2 con un conteggio di dieci microsecondi
  TIM2->ARR = fck/100000;
  TIM2->CR2 = TIM_CR2_MMS_1;
  
  //Inserisco il primo valore del DAC nel Data Holding Register
  DAC->DHR12R1 = vettDAC[j];
  
  //Abilito il conteggio del TIM2
  TIM2->CNT = 0;
  TIM2->CR1 |= TIM_CR1_CEN;
  
  while(1);
}

void ADC1_2_IRQHandler() {
  
  //Valore di tensione convertito in parola codice
  vettADC[j] = ADC1->DR;
  
}

void TIM2_IRQHandler() {
  TIM2->SR = 0; //Resetto il bit UIF del Timer2
  
  //Conversione
  ADC1->CR |= ADC_CR_ADSTART;
  
  j++;
  if (j == N){
    j = 0;
  }
  
  DAC->DHR12R1 = vettDAC[j];    //Inserisco il valore successivo del DAC nel DHR
}