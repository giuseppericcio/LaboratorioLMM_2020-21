#define RCCENAHB (unsigned int*) 0x40021014;
#define GPIOEMODER (unsigned int*) 0x48001000;
#define GPIOEODR (unsigned int*) 0x48001014;

unsigned int *puntatore;

int main()
{

  puntatore = RCCENAHB;       //RCC AHB enable register (pag. 42)
  *puntatore |= 1<<21;        //Abilito il Clock del GPIOE
  puntatore = GPIOEMODER;     //Registro base GPIOE (pag. 42)
  *puntatore |= 1<<16;        //Abilito il PE8 come general output
  puntatore = GPIOEODR;       //Registro GPIO Output Data Register
  *puntatore |= 1<<8;         //Accendo il LED BLU

  while(1);
  return 0;
 
}