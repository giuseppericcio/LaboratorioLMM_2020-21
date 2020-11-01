#include <stm32f30x.h>

int main()
{
  int contatore = 0;
  int premuto = 0;
  int ciclo = 0;
  
  RCC-> AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOEEN;      //Abilito il Clock del GPIOA e del GPIOE
  GPIOE-> MODER |= GPIO_MODER_MODER8_0;                         //Imposto il PE8 come General Output
  GPIOE-> MODER |= GPIO_MODER_MODER9_0;                         //Imposto il PE9 come General Output
  GPIOE-> MODER |= GPIO_MODER_MODER10_0;                        //Imposto il PE10 come General Output
  GPIOE-> MODER |= GPIO_MODER_MODER11_0;                        //Imposto il PE11 come General Output
  GPIOE-> MODER |= GPIO_MODER_MODER12_0;                        //Imposto il PE12 come General Output
  GPIOE-> MODER |= GPIO_MODER_MODER13_0;                        //Imposto il PE13 come General Output
  GPIOE-> MODER |= GPIO_MODER_MODER14_0;                        //Imposto il PE14 come General Output
  GPIOE-> MODER |= GPIO_MODER_MODER15_0;                        //Imposto il PE15 come General Output
  GPIOA-> MODER &= ~(GPIO_MODER_MODER0);                        //Imposto il PA0 in modalità Input
 
  while(1){
    if ((GPIOA-> IDR & GPIO_IDR_0) == GPIO_IDR_0)               //Verifico se il pulsante USER è premuto
    {
      premuto = 1;
      if ((contatore%8) == 0)  GPIOE-> ODR &= 0;                //Se il contatore è un multiplo di 8 resetto i LED
    }
    else if (premuto == 1 && contatore < 255) 
    {
      contatore++;                                              //Incremento il contatore
      if ((contatore%8) == 0) 
      {
        ciclo ++;                                               //Incremento il ciclo di conteggio
        GPIOE-> ODR |= 1<<15;                                   //Accendo il LED collegato al PE15
      }
      else {
      GPIOE-> ODR |= 1<<(7+contatore%8);                        //Accendo il lED corrispondente al numero contato
      }
      premuto = 0;
    }
    else if (contatore == 255)
    {
      contatore = 0;                                            //Resetto il contatore
    }
  };

  return 0;
  
}