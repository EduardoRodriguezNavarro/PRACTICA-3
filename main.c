#include "lib/include.h"

int main(void)
{
    //___________________________________________VARIABLES
    //uint16_t valor = 0;
    char c='5';
    int Largo=100;//VARIABLE DONDE ALMACENARÉ EL LARGO DE MI ARREGLO
    char Nombre[Largo]; //ARREGLO DONDE ALMACENARÉ EL NOMBRE



    Configurar_PLL(_25MHZ);  //Confiuracion de velocidad de reloj
    Configurar_UART1();//Yo FCLK 20MHZ Baudrate 9600


    //Experimento 1
    //  Configurar_UART1(); //Saul,Fabela -fclk 25MHZ Baud-rate 57600
    
    
    
     Configurar_GPIO();
   // Homogeneidad();
    //printString("Holis Bolis");
    //printChar(c);
    //printChar(b);
    //printString("HOLA\n");
   // char *arr = readString(',');
   // printString(&arr[0]);
    while(1)
    {
         c = readChar();
         switch(c)
         {
             case 'r':
                 //GPIODATA port F 662
                 printChar('a');
                 GPIOF->DATA = (1<<1);
                 break;
             case 'b':
                 //GPIODATA port F 662
                 printChar('b');
                 GPIOF->DATA = (1<<2);
                 break;
             case 'g':
                 //GPIODATA port F 662
                 printChar('c');
                 GPIOF->DATA = (1<<3);
                 break;

           //--------------LLAMADA A FUNCIÓN
           case 'a':
                 GPIOF->DATA = (1<<1) | (1<<2) | (1<<3);//PRENDEMOS TODOS LOS LED'S(OPCIONAL, LO PODEMOS ELIMINAR)
                 Largo=readString(',',&Nombre[0]);//FUNCION QUE CALCULA LARGO DEL ARREGLO
                 NomInvert(&Nombre[0],Largo);//FUNCION QUE INVIERTE Y AÑADE LOS NUMEROS
                 printString(&Nombre[0], Largo);//FUNCION QUE IMPRIME 
                 break;
             //---------------------------------------------
            
           // ---------------------------         

             //default:
              //   printChar((char)valor);
              //   GPIOF->DATA = (0<<1) | (0<<2) | (0<<3);
              //   break;
         }
    }

}
