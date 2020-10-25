#define RCCENAHB (unsigned int*) 0x40021014;
#define GPIOEMODER (unsigned int*) 0x48001000;
#define GPIOEODR (unsigned int*) 0x48001014;

unsigned int *puntatore;

int main()
{

  puntatore = RCCENAHB;       //RCC AHB enable register (pag. 42)
  *puntatore |= 1<<21;        //per cambiare solo il bit d'interesse
  puntatore = GPIOEMODER;     //Registro base GPIOE (pag. 42)
  *puntatore |= 1<<16;        //per cambiare solo il bit d'interesse
  puntatore = GPIOEODR;       //Registro GPIO Output Data Register
  *puntatore |= 1<<8;         //per cambiare solo il bit d'interesse
  puntatore = GPIOEMODER;     //Registro base GPIOE (pag. 42)
  *puntatore |= 1<<18;        //per cambiare solo il bit d'interesse
  puntatore = GPIOEODR;       //Registro GPIO Output Data Register
  *puntatore |= 1<<9;         //per cambiare solo il bit d'interesse
  puntatore = GPIOEODR;       //Registro GPIO Output Data Register
  *puntatore &= ~(1<<8);      //per cambiare solo il bit d'interesse

while(1);
return 0;

}