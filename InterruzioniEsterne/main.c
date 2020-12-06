#include <stm32f30x.h>

int flag = 0;
int fck = 72000000;

int main()
{
  //Abilito i clock delle periferiche
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; //Linee esterne
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;    //GPIOA
  RCC->AHBENR |= RCC_AHBENR_GPIOEEN;    //GPIOE
  RCC->APB1ENR |=  RCC_APB1ENR_TIM2EN;  //TIMER2
  
  //Abilito le interruzioni del TIMER2
  TIM2->DIER |= TIM_DIER_UIE;
  NVIC->ISER[0] |= 1<<28;
  
  //Abilito le interruzioni esterne sulla linea0
  SYSCFG->EXTICR[0] &= 0xFFFFFFF0;
  EXTI->IMR |= EXTI_IMR_MR0;    //L'interrupt esterno sulla linea0 non è mascherato
  EXTI->RTSR |= EXTI_RTSR_TR0;  //Abilito l'interruzione sul fronte di salita
  NVIC->ISER[0] |= 1<<6;        //Abilito l'interrupt sulla linea0
  
  //Imposto i LED del GPIOE come General Output
  GPIOE->MODER |= 0x55550000;
  
  //Imposto il PA0 come Input digitale
  GPIOA->MODER &= ~GPIO_MODER_MODER0;
  
  /* Calcolo N conteggi
  *  fck = 72MHz frequenza del clock
  *  Tck = 1/fck = 1/72MHz periodo del clock
  *  Deltat = 10s
  *  N = Deltat/Tck = 10 * 72000000
  *  dunque TIM2_ARR = 10 * 72000000
  */
  //Imposto il TIM2 con un conteggio di dieci secondi
  TIM2->ARR = 10*fck;
  TIM2->CNT = 0;
  
  //Abilito il conteggio del TIM2
  TIM2->CR1 |= TIM_CR1_CEN;
  
  GPIOE->ODR |= 0x0000FF00;     //Accendo tutti i LED
  flag = 1;
    
  while(1){
    
  }
  
  return 0;
}

void TIM2_IRQHandler(){
  
  TIM2->SR &= ~TIM_SR_UIF;        //Resetto il valore del bit0, ovvero dell'UIF
  
  if (flag == 0){
    GPIOE->ODR |= 0x0000FF00;     //Accendo tutti i LED
    flag = 1;
  }
  else {
    GPIOE->ODR &= ~(0x0000FF00);  //Spengo tutti i LED
    flag = 0;
  }
}

void EXTI0_IRQHandler(){
  
  EXTI->PR |= EXTI_PR_PR0;      //Resetto le richieste pendenti sulla linea0
  
  if ((GPIOE->ODR & GPIO_ODR_8) == GPIO_ODR_8)  //Verifico se il LED8 è acceso, oppure no
    GPIOE->ODR &= ~(1<<8);      //Spengo il LED8
  else
    GPIOE->ODR |= (1<<8);       //Accendo il LED8
}