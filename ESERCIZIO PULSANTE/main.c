#include <stm32f30x.h>

unsigned int *puntatore;

int main()
{
 
  RCC-> AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOEEN;      //Abilito il Clock del GPIOA e del GPIOE
  GPIOE-> MODER |= 1<<16;                                       //Imposto il PE8 come General Output
  GPIOA-> MODER &= ~(GPIO_MODER_MODER0);                        //Imposto il PA0 in modalità Input
 
  while(1){
    if ((GPIOA-> IDR & GPIO_IDR_0) == GPIO_IDR_0)               //Verifico se il pulsante USER è premuto
    {
      GPIOE-> ODR |= 1<<8;         //Accendo il lED blu
    }
    else 
    {
     GPIOE-> ODR &= 0xfffffeff;    //Spengo il LED blu
    }
  };
  return 0;
  
}