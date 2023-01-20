#include "lib/include.h"

extern void Configurar_UART0(void)
{
    SYSCTL->RCGCUART  = (1<<0);   //Paso 1 (RCGCUART) pag.344 UART/modulo0 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<0);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port A
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOA->AFSEL = (1<<1) | (1<<0);
    //GPIO Port Control (GPIOPCTL) PA0-> U0Rx PA1-> U0Tx pag.688
    GPIOA->PCTL = (GPIOA->PCTL&0xFFFFFF00) | 0x00000011;// (1<<0) | (1<<4);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOA->DEN = (1<<0) | (1<<1);//PA1 PA0
    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART0->CTL = (0<<9) | (0<<8) | (0<<0);

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 20,000,000 / (16 * 9600) = 130.2
    UARTFBRD[DIVFRAC] = integer(0.2 * 64 + 0.5) = 14
    */
    UART0->IBRD = 130;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART0->FBRD = 14;
    //  UART Line Control (UARTLCRH) pag.916
    UART0->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939
    UART0->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART0->CTL = (1<<0) | (1<<8) | (1<<9);



}

extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART0->FR & (1<<4)) != 0 );
    v = UART0->DR & 0xFF;
    c = v;
    return c;
}
extern void printChar(char c)
{
    while((UART0->FR & (1<<5)) != 0 );
    UART0->DR = c;
}
//FUNCIÓN QUE IMPRIME UNA CADENA DE CARACTERES
extern void printString(char* string, int Largo)
{
    int i=0;
    while(string[i]>47)
    {
        printChar(string[i]);
        i++;
    }
}

extern char readString(char delimitador, char *string)
{

   int i=0;
   //char *string = (char *)calloc(10,sizeof(char));ESTO NO LE ENTENDÍ 
   char c = readChar();
   while(c != delimitador)
   {
       string[i] = c;
       i++;
       c = readChar();
   }

   return i;

}
//Experimento 2


extern void NomInvert(char *Nombre, int Largo)
{
//______________________________________________________

int i = 1;
int a=1;
int b;
int bandera = 0;

char invert = (char)calloc(10,sizeof(char));
b=0;
while(i<Largo)
{
if (bandera =0)
{
	bandera==1;
	invert[b]=Nombre[Largo-i];
	i=i+1;
	b=b+1;
}
else
{
	bandera==0;
	invert[b]=a;
	a=a+1;
	b=b+1;
}

if(i%10==0)
        {
            invert = realloc(invert, (1+10)*sizeof(char));
        }

return invert;

}

//___________________________________________________
} 




//El envio es su nombre  (rave) 

// invertirlo y regresarlo con numeros consecutivos
// entre letras (e1v2a3r) 

