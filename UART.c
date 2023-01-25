#include "lib/include.h"

extern void Configurar_UART1(void)
{
SYSCTL->RCGCUART  = (1<<1);   //Paso 1 (RCGCUART) pag.344 UART/modulo1 0->Disable 1->Enable
    SYSCTL->RCGCGPIO |= (1<<1);     //Paso 2 (RCGCGPIO) pag.340 Enable clock port B
    //(GPIOAFSEL) pag.671 Enable alternate function
    GPIOB->AFSEL = (1<<1) | (1<<0);
    //GPIO Port Control (GPIOPCTL) PB0-> U0Rx PB1-> U0Tx pag.688
    // (GPIOB->PCTL&0xFFFFFF00) | 0x00000011;// 
    GPIOB->PCTL = (1<<0) | (1<<4);//0x00000011
    // GPIO Digital Enable (GPIODEN) pag.682
    GPIOB->DEN = (1<<0) | (1<<1);//PB1 PB0
    //UART0 UART Control (UARTCTL) pag.918 DISABLE!!
    UART1->CTL = (0<<9) | (0<<8) | (0<<0);
 
    // UART Integer Baud-Rate Divisor (UARTIBRD) pag.914
    /*
    BRD = 25,000,000 / (16*57600) = 27.1267
    UARTFBRD[DIVFRAC] = integer(.1267 * 64 + 0.5)
    */
    UART1->IBRD = 27;
    // UART Fractional Baud-Rate Divisor (UARTFBRD) pag.915
    UART1->FBRD = 7;
    //  UART Line Control (UARTLCRH) pag.916
    UART1->LCRH = (0x3<<5)|(1<<4);
    //  UART Clock Configuration(UARTCC) pag.939
    UART1->CC =(0<<0);
    //Disable UART0 UART Control (UARTCTL) pag.918
    UART1->CTL = (1<<0) | (1<<8) | (1<<9);
}


/*
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
    
    BRD = 20,000,000 / (16 * 9600) = 130.2
    UARTFBRD[DIVFRAC] = integer(0.2 * 64 + 0.5) = 14
    */
   /*
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
*/
extern char readChar(void)
{
    //UART FR flag pag 911
    //UART DR data 906
    int v;
    char c;
    while((UART1->FR & (1<<4)) != 0 );
    v = UART1->DR & 0xFF;
    c = v;
    return c;
}
extern void printChar(char c)
{
    while((UART1->FR & (1<<5)) != 0 );
    UART1->DR = c;
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

extern int readString(char delimitador, char *string) //AQUI LEEMOS EL AREGLO NOMBRE Y CALCULAMOS CUANTOS ELEMTOS TIENE
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
//_______________________________VARIABLES
int i = 1;
int a=1;
int b=0;
int bandera = 0;
int Size;
Size=Largo;
   

//_________EN ESTA SECCION CALCULAMOS EL TAMAÑO DE MEMERIA QUE OCUPAMOS PARA GUARDAR EL NOMBRE INVERTIDO CON NUMERO
if (Largo < 10)
    
        Largo = Largo + (Largo-1);//AQUI DELIMITAMOS EL TAMAÑO DE MEMORIA PARA NOMBRES PEQUES. EXPLICACIÓN ABAJO
        //EJ. LALO TIENE 4 LETRAS Y 3 ESPACIOS ENTRE ELLAS DONDE IRAN LOS NUMEROS (1,2 Y 3) PARA QUE NOS QUEDE AL FINAL O1L2A3L
    
    else
    {
        Largo = 18 + ((Largo - 9)*3);// ESTE ES PARA NOMBRES MÁS LARGOS, LA EXPLICACIÓN ES UN POCO MÁS LARGA
    }    
        char invert[Largo];//UNA VEZ CALCULADO EL TAMAÑO QUE SE OCUPARA DECLARAMOS EL ARREGLO QUE LLEVARÁ ESA EXTENSIÓN
    
//______________

//char invert = (char)calloc(10,sizeof(char));
while(i<Largo) //LA LOGICA SE BASA EN UN IF QUE PONE LAS LETRAS Y UN ELSE QUE PONE LOS NUMEROS ENTRE LETRAS, ASÍ PRIMERO PONDRA UNA LETRA Y LUEGO UN #, Y ASÍ SE REPITE HASTA QUE INVIERTE TODO EL NOMBRE
{
if (bandera == 0 )//EN ESTE IF SE ENCARGARA DE LEER LAS LETRAS DEL ARREGLO Y COLOCARLAS ARLEVES
{
	invert[b]=Nombre[Size-i];
	i=i+1;
	b=b+1;
    bandera=1;

}
else// AQUÍ SE PONDRAN LOS NUMEROS QUE DEBEN IR ENTRE CADA LETRA EN ORDEN ASCENDENTE
{
	bandera=0;
	invert[b]=a;
	a=a+1;
	b=b+1;
}

//if(i%10==0)// AQUI SE TRATÓ DE USAR UNA FUNCIÓN MAS ESPECIALIZADA PARA APARTAR ESPACIO DE MEMORIA, PERO NO JALÓ. BYE
  //      {
    //        invert = realloc(invert, (1+10)*sizeof(char));
      //  }
//  DE INVERT A NOMBRE[]
}


//______________________________REASIGNACIÓN 
/* AQUÍ REASIGNAMOS EL VALOR DE NUESTRO ARREGLO INVERT(DONDE SE ALMACENÓ DURANTE EL PROCESO DE INVERSION NUESTRO NOMBRE CON LOS #'S)
AL ARREGLO NOMBRE, CON LA FINALIDAD DE POSTERIORMENTE LLAMARLO DESDE NUESTRO MAIN USANDO LA FUNCIÓN QUE IMPRIME LAS CADENAS 
(printString linea 85) */
  for(int j=0; j<=(Largo-1); j++)
{
    Nombre[j]=invert[j];
} 
  for(int n=Largo ;n <= 100;n++)/*COMO EN UN INCIO DIJIMOS QUE NUESTRO ARREGLO NOMBRE TENDRA UN TAMAÑO DE 100, 
  PARA RELLENAR TODAS LAS CASILLAS QUE FALTAN Y NO TENER PROBLEMAS LES PONEMOS UN 0 A LAS QUE QUEDARON VACIAS*/
    Nombre[n] = 0;
return;


//___________________________________________________
} 




//El envio es su nombre  (rave) 

// invertirlo y regresarlo con numeros consecutivos
// entre letras (e1v2a3r) 

