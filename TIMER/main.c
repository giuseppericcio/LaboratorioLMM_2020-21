#include <stm32f30x.h>

short int acceso = 0;

int main()
{
  //Abilito i clock
  RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
  RCC->APB1ENR |=  RCC_APB1ENR_TIM2EN;
  TIM2->CR1 |= TIM_CR1_CEN;
  
  //Imposto il PE8 del GPIOE come General Output
  GPIOE->MODER |= GPIO_MODER_MODER8_0;
  
  //Imposto il TIM2 con un conteggio di 1 secondo
  TIM2->ARR = 8000000;
  TIM2->CNT = 0;
  
  while(1){
    if ((TIM2->SR & TIM_SR_UIF ) == TIM_SR_UIF && !acceso){
      GPIOE->ODR |= 1<<8;               //accende il LED blu
      TIM2->SR &= ~TIM_SR_UIF;          //resetta l'UIF
      acceso = 1;                       //il LED è acceso
    }
    if ((TIM2->SR & TIM_SR_UIF) == TIM_SR_UIF && acceso){
      GPIOE->ODR &= 0;                  //spegne il LED blu
      TIM2->SR &= ~TIM_SR_UIF;          //resetta l'UIF
      acceso = 0;                       //il LED è spento
    }
  }
  
  return 0;
}
