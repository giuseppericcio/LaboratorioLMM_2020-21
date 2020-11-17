#include <stm32f30x.h>
#include <stdio.h>

#define Tck 1/8000000

int conteggio = 0;

int main()
{
  float tempo = 0;
  
  //Abilito i Clock
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  
  while(1){
    if ((GPIOA->IDR & GPIO_IDR_0) == GPIO_IDR_0){
      
      TIM2->CR1 |= TIM_CR1_CEN;         //Abilito il Clock del Timer2
      TIM2->CNT = 0;                    //Resetto il Conteggio
      
      while ((GPIOA->IDR & GPIO_IDR_0) == GPIO_IDR_0);
      
      TIM2->CR1 &= ~TIM_CR1_CEN;        //Disabilito il Clock del Timer2 per smettere di contare
      conteggio = TIM2->CNT;            //Copio il conteggio nella variabile
      tempo =(float)conteggio*Tck;      //Per ottenere Deltas applico la formula Ds = Ncont * Tck
      
      printf("Intervallo di tempo: %f \n", tempo);
      
    }
    
  }
  
  return 0;
}
