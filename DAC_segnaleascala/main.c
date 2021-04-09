#include <stm32f30x.h>
#include <math.h>
#include <stdbool.h>

#define N 100
#define pi 3.14

int fck = 72000000;
float tensione = 0;
short int vettDAC[N];
short int i = 0;
short int j = 0;

int main()
{
  //Abilito i clock delle periferiche
  RCC->APB1ENR |= RCC_APB1ENR_DACEN;    //DAC
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;    //GPIOA
  RCC->APB1ENR |=  RCC_APB1ENR_TIM2EN;  //TIMER2
  
  //Abilito il DAC con TSEL=111, software trigger
  DAC->CR |= DAC_CR_TSEL1_0;
  DAC->CR |= DAC_CR_TSEL1_1;
  DAC->CR |= DAC_CR_TSEL1_2;
  DAC->CR |= DAC_CR_TEN1;
  DAC->CR |= DAC_CR_EN1;
  
  //Genero i campioni
  for (i = 0; i<N; i++){
    tensione = (float) 3*(i/(N-1));
    vettDAC[i] = (short int) (tensione * 4095/3);
  }
  
  //Abilito le interruzioni del TIMER2
  TIM2->DIER |= TIM_DIER_UIE;
  NVIC->ISER[0] |= 1<<28;
  
  //Imposto il PA4 come analogico
  GPIOA->MODER |= GPIO_MODER_MODER4;
  
  /* Calcolo Nck conteggi
  *  fck = 72MHz frequenza del clock
  *  Tck = 1/fck = 1/72MHz periodo del clock
  *  tupdate = Tsegnale/N = 3*10^(-3)/10^2 = 30us
  *  Nck = Deltat/Tck = 3*10(-5) * 72000000
  *  dunque TIM2_ARR = 3*10^(-5) * 72000000
  */
  //Imposto il TIM2 con un conteggio di trenta microsecondi
  TIM2->ARR = fck/(3*100000);
  TIM2->CNT = 0;
  
  //Inserisco il primo valore del DAC nel Data Holding Register
  DAC->DHR12R1 = vettDAC[j];
  
  //Abilito il conteggio del TIM2
  TIM2->CR1 |= TIM_CR1_CEN;

  while(1);
  return 0;
}

void TIM2_IRQHandler() {
  TIM2->SR = 0; //Resetto il bit UIF del Timer2
  
  DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG1;  //Abilito il software trigger
  
  j++;
  if (j == N){
    j = 0;
  }
  
  DAC->DHR12R1 = vettDAC[j];    //Inserisco il valore successivo del DAC nel DHR
}
