#include <stm32f30x.h>

int contatore = 0;

int main()
{
  
  RCC-> AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOEEN;      //Abilito il Clock del GPIOA e del GPIOE
  GPIOE-> MODER |= 0x55550000;                                  //Imposto dal PE8 al PE15 come General Output
  GPIOA-> MODER &= ~(GPIO_MODER_MODER0);                        //Imposto il PA0 in modalità Input
 
  while(1){
    if ((GPIOA-> IDR & GPIO_IDR_0) == GPIO_IDR_0)               //Verifico se il pulsante USER è premuto
    {
      
      while ((GPIOA-> IDR & GPIO_IDR_0) == GPIO_IDR_0);
      GPIOE-> ODR &= 0;                                         //Resetto i LED
      
      contatore++;                                              //Incremento il contatore
      GPIOE-> ODR |= contatore<<8;                              //Accendo il lED corrispondente al numero contato
      if (contatore == 255)
      {
        contatore = 0;                                          //Resetto il contatore al raggiungimento del valore massimo
      }
    }
  };

  return 0;
}