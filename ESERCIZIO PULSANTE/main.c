#define RCCENAHB (unsigned int*) 0x40021014;
#define GPIOAMODER (unsigned int*) 0x48000000;
#define GPIOAIDR (unsigned int*) 0x48000010;
#define GPIOEMODER (unsigned int*) 0x48001000;
#define GPIOEODR (unsigned int*) 0x48001014;

unsigned int *puntatore;
unsigned int IDR0;

int main()
{

  puntatore = RCCENAHB;         //RCC AHB enable register (pag. 42)
  *puntatore |= 1<<17;          //Per cambiare solo il bit d'interesse
  puntatore = GPIOAMODER;       //Registro base GPIOA (pag. 42)
  puntatore = GPIOAIDR;         //Registro GPIOA Input Data Register
  IDR0 = *puntatore;             //Valore dell'IDR
  IDR0 &= 1<<0;                  //Verifico se IDR0 è alto, cioè se il pulsante è premuto
  if (IDR0 == 1)
  {
    puntatore = RCCENAHB;       //RCC AHB enable register (pag. 42)
    *puntatore |= 1<<21;        //Per cambiare solo il bit d'interesse
    puntatore = GPIOEMODER;     //Registro base GPIOE (pag. 42)
    *puntatore |= 1<<16;        //Per cambiare solo il bit d'interesse
    puntatore = GPIOEODR;       //Registro GPIOE Output Data Register
    *puntatore |= 1<<8;         //Per cambiare solo il bit d'interesse
    puntatore = GPIOEMODER;     //Registro base GPIOE (pag. 42)
    *puntatore |= 1<<18;        //Per cambiare solo il bit d'interesse
    puntatore = GPIOEODR;       //Registro GPIOE Output Data Register
    *puntatore |= 1<<9;         //Per cambiare solo il bit d'interesse
  }
  while(1);
  return 0;
  
}