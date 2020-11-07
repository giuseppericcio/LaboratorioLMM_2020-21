#include <stm32f30x.h>

int contatore = 0;
int ciclo = 0;

int main()
{
  
  RCC-> AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOEEN;      //Abilito il Clock del GPIOA e del GPIOE
  GPIOE-> MODER |= 0x55550000;                                  //Imposto dal PE8 al PE15 come General Output
  GPIOA-> MODER &= ~(GPIO_MODER_MODER0);                        //Imposto il PA0 in modalità Input
 
  while(1){
    if ((GPIOA-> IDR & GPIO_IDR_0) == GPIO_IDR_0)               //Verifico se il pulsante USER è premuto
    {
      while ((GPIOA-> IDR & GPIO_IDR_0) == GPIO_IDR_0);
      
      if ((contatore%8) == 0) GPIOE-> ODR &= 0;
      if (contatore < 255){
          contatore++;                                          //Incremento il contatore
          if ((contatore%8) == 0) 
          {
            ciclo ++;                                           //Incremento il ciclo di conteggio
            GPIOE-> ODR |= 1<<15;                               //Accendo il LED collegato al PE15
          }
          else {
            GPIOE-> ODR |= 1<<(7+contatore%8);                  //Accendo il lED corrispondente al numero contato
          }
      }
      else {
          contatore = 0;                                        //Resetto il contatore
      }
    }                                                                              
  };

  return 0;
}