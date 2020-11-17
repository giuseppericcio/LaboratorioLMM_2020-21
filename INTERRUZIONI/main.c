#include <stm32f30x.h>

int flag = 0;

int main()
{
  //Abilito i clock
  RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
  RCC->APB1ENR |=  RCC_APB1ENR_TIM2EN;
  
  //Abilito le interruzioni
  TIM2->DIER |= TIM_DIER_UIE;
  NVIC->ISER[0] |= 1<<28;
  
  //Imposto i LED del GPIOE come General Output
  GPIOE->MODER |= 0x55550000;
  
  //Imposto il TIM2 con un conteggio di mezzo secondo
  TIM2->ARR = 36000000;
  TIM2->CNT = 0;
  
  //Abilito il conteggio del TIM2
  TIM2->CR1 |= TIM_CR1_CEN;
  
  while(1){
    GPIOE->ODR |= 1<<9;                     //Accendo il LED9
    for (int i = 0; i < 1000000; i++);      //Simulo un ritardo
    GPIOE->ODR &= ~(1<<9);                  //Spengo il LED9
    for (int i = 0; i < 1000000; i++);      //Simulo un ritardo
  }
  
  return 0;
}

void TIM2_IRQHandler(){
  
  TIM2->SR &= ~TIM_SR_UIF;        //Resetto il valore del bit0, ovvero dell'UIF
  
  if (flag == 0){
    GPIOE->ODR |= 1<<8;           //Accendo il LED8
    flag = 1;
  }
  else {
    GPIOE->ODR &= ~(1<<8);        //Spengo il LED8
    flag = 0;
  }
}