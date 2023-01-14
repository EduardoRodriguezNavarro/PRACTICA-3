#include "lib/include.h"
//-------------------------------------------------------------------------------------------------------
extern void Configurar_UART0(void)
{
    SYSCTL->RCGCUART  = (1<<0);   //Paso 1 (RCGCUART) pag.344 UART/modulo0 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<0);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port A
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOA_AHB->AFSEL = (1<<1) | (1<<0);
    //GPIO Port Control (GPIOPCTL) PA0-> U0Rx PA1-> U0Tx pag.688
    GPIOA_AHB->PCTL = (GPIOA_AHB->PCTL&0xFFFFFF00) | 0x00000011;// (1<<0) | (1<<4);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOA_AHB->DEN = (1<<0) | (1<<1);//PA1 PA0
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
//--------------------------------------------------------------------------------

extern void Configurar_UART1(void)
{
    SYSCTL->RCGCUART  = (1<<1);   //Paso 1 (RCGCUART) pag.1163 UART/modulo0 0->Disable 1->Enable AQUI HABILITO E INICIALIZO EL UART
    SYSCTL->RCGCGPIO |= (1<<1);     //Paso 2 (RCGCGPIO) pag.384 Enable clock port A/HABILITO CON EL BIT 1 EL RELOJ DEL PUERTO B
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOB_AHB->AFSEL = (1<<1) | (1<<0);//EL AFSEL ES PARA DECIR QUE TRABAJARÉ CON FUNCIONES ALTERNATIVASp.1808 y tablita de p.771
                    //  (pin 1 habilitado)|(pin 0 habilitado)

    //GPIO Port Control (GPIOPCTL) PA0-> U0Rx PA1-> U0Tx pag.688
    GPIOB_AHB->PCTL = (GPIOB_AHB->PCTL&0xFFFFFF00) | 0x00000011;// (1<<0) | (1<<4);//0x00000011
    //                                    LAS F'S son mascaras que guardan la comfiguración pasada, donde los ceros son espacios limpios para posteriormente sumar los 1
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOB_AHB->DEN = (1<<0) | (1<<1);//PA1 PA0
    //AQUI HABILITO LOS PINES COOMO DIGITALES
    
    //_____________________________________________________________________
    //UART0 UART Control (UARTCTL) pag.918 DISABLE!! 
    //EL CTL CONTROLA EL UAQRT!!!
    UART1->CTL = (0<<9) | (0<<8) | (0<<0); //DESACTVO EL CTL

    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 25,000,000 / (16 * 57600) = 27.1
    UARTFBRD[DIVFRAC] = integer(0.1 * 64 + 0.5) = 7
    */
    UART1->IBRD = 27;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART1->FBRD = 7;
    //AQUI AGREGAMOS LA PARTE DECIMAL
    //  UART Line Control (UARTLCRH) pag.916
    UART1->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939
    UART1->CC =(0<<0);//ES PARA DECIR QUE VAMOS A TRABAJAR CON EL RELOJ INTERNO
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART1->CTL = (1<<0) | (1<<8) | (1<<9);//ACTIVAMOS EL CTL 
//___________________________________________________________________________________


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
extern void printString(char* string)
{
    while(*string)
    {
        printChar(*(string++));
    }
}

extern char * readString(char delimitador)
{

   int i=0;
   char *string = (char *)calloc(10,sizeof(char));
   char c = readChar();
   while(c != delimitador)
   {
       *(string+i) = c;
       i++;
       if(i%10==0)
       {
           string = realloc(string,(i+10)*sizeof(char));
       }
       c = readChar();
   }

   return string;

}
//Experimento 2

//El envio es su nombre  (rave) 

// invertirlo y regresarlo con numeros consecutivos
// entre letras (e1v2a3r) 

