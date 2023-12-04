// Preprocesadores de Microcontrolador //
#include    <18f4550.h>                                        // Libreria del Microcontrolador
#device     adc = 10                                           // Resolucion del ADC en bits
#fuses      INTRC, NOWDT, NOPROTECT, NOLVP, CPUDIV1, PLL1      // Fusibles (Configuraciones del microcontrolador)
#use        delay(clock = 8M)                                  // Configuracion de delay
//#use        rs232(rcv = pin_c7, xmit = pin_c6, baud = 9600, bits = 8, parity = n)

// Preprocesadores LCD // 
#define     LCD_DATA_PORT getenv("SFR:PORTB")
#include    <lcd.c>

//  Constantes para mejor entendimiento   //
#define     DownButton     PIN_D6
#define     UpButton       PIN_D5
#define     LeftButton     PIN_D4
#define     RightButton    PIN_D7

#define     Left     1
#define     Right    0

// Macros y variables constantes
#define     WireSensor(ADC) (ADC < 900 ? 1 : 0)       // Macro para ver si hay un cable en sensor
#define     MinWireLenght     30                      // Valor minimo que puede tomar el cable

// Estructuras //
struct wire_print_order
{
   int16          Amount;                    // Cantidad
   int16          Length;                    // Longitud del cable (mm)
   int16          PeelingLength;             // Longitud de pelado de cable (mm)
};

struct step_motor
{
   long           StepPIN;                   // Pin para realizar un paso en el motor
   long           DirectionPIN;              // Pin para cambiar direccion del motor
   long           Delay;                     // Delay entre cada paso del motor
};

// Interrupcion //
#int_rda
void ComputerConection() {} // Activar y gestionar conexion con interfaz de computadora

//  Prototipos de funcion  //
int1 Number_Select(char Title[16], int16* Number, int16 MinNumber, int16 MaxNumber);                // Funcion para seleccion de numero
void Wire_Print(struct wire_print_order ActualOrder);                                               // Funcion de impresion de cable

void main()
{
   // Activamos Interrupciones
   enable_interrupts(GLOBAL);       enable_interrupts(int_rda);
   
   // Configuramos puertos ADC para fotoresistencia
   setup_adc(adc_clock_div_2);
	set_tris_a(0b00000001);
	setup_adc_ports(AN0);
   set_adc_channel(0);     delay_us(10);

   // Configuramos Motores a Pasos
   struct step_motor Motor1 = {PIN_D0, PIN_D1, 500};
   struct step_motor Motor2 = {PIN_D2, PIN_D3, 500};

   // Mensaje Introductorio
   lcd_init();    lcd_gotoxy(3,1);
   printf(lcd_putc,"WIRE TRIMMER\n Prog. Avanzada");
   delay_ms(1000);
   
   while(true)
   {
      /*float Reel = read_eeprom(1);*/  long WireReel = 10000;   // Valor maximo 65,000 mm de carrete (Para no desbordar las variables)
      
      // Evaluacion de disponibilidad de cable
      if(!WireSensor(read_adc()) || (WireReel < MinWireLenght))
      {
         printf(lcd_putc,"\f!! NO SE DETECTA\nCABLE DISPONIBLE");
         delay_ms(2000);

         // Funcion para proceso de recarga de cable
      }
      
      // Creacion de orden
      Start:
      printf(lcd_putc,"\fCable: %5ld mm\nNueva Orden  -->", WireReel);
      while(!input(RightButton));   while(input(RightButton));

      struct wire_print_order ActualOrder = { 0, 0, 0 };

      Amount:
      // Valor maximo en funcion del carrete disponible y el valor minimo de longitud
      if( Number_Select( (char*) "Cantidad:", &ActualOrder.Amount, 1, WireReel / MinWireLenght) )
         goto Start;
      
      Length:
      // Valor maximo en funcion de memoria eeprom y cantidad
      if( Number_Select( (char*) "Longitud (mm):", &ActualOrder.Length, MinWireLenght , WireReel / ActualOrder.Amount) )
         goto Amount;
      
      Peeling:
      // Valor maximo en funcion de longitud de cada cable
      if( Number_Select( (char*) "Pelado (mm):", &ActualOrder.PeelingLength, 0, ActualOrder.Length / 10) )
         goto Length;

      // Confirmacion de orden
      printf(lcd_putc, "\f %4ld Cable(s)  \n    %5ld mm    ",ActualOrder.Amount, ActualOrder.Length);
      while(!input(RightButton))
      {
         if(input(LeftButton))   {  while(input(LeftButton));  goto Peeling; }
      }
      while(input(RightButton));

      // Impresion del orden de cable
      Wire_Print(ActualOrder);

      printf(lcd_putc,"\fORDEN FINALIZADA\n Vuelva Pronto !");
      delay_ms(2000);
   }
}

int1 Number_Select(char Title[16], int16 *Number, int16 MinNumber, int16 MaxNumber)
{
   // Declaramos e inicializamos variables que ocuparemos
   int1 PrintBool = 1;  int1 PositionChar = 1;    signed int MaxExponent = 0;   signed int Exponent = 0;
   
   // Correccion de posible error en numero
   if(*Number < MinNumber)
      *Number = MinNumber;
   else if(*Number > MaxNumber)
      *Number = MaxNumber;

   // Sacamos el exponente del numero maximo posible que puede tomar el numero
   for(int16 aux = MaxNumber; aux >= 10; aux/=10, MaxExponent++, Exponent++);

   // Impresion de titulo y espacio para numeros
   printf(lcd_putc, "\f%s\n-----      -----", Title);

   // Ciclo que se ejecuta el tiempo que se modifique el numero
   while((Exponent >= 0) && (Exponent <= MaxExponent))
   {
      long aux = 1;

      // Impresion en LCD
      if(PrintBool)
      {
         lcd_gotoxy(6,2);
         printf(lcd_putc,"%06lu", *Number);         
         PrintBool = 0;

         if(PositionChar) 
         { 
            lcd_gotoxy(11-Exponent,2);    
            lcd_putc('*');    
            PositionChar = 0; 
         }
      }
      
      // Modificacion de variable
      if(input(RightButton) || input(LeftButton))
      {
         input(RightButton) ? Exponent-- : Exponent++;
         PrintBool = 1;    PositionChar = 1;

         while (input(RightButton) || input(LeftButton));
      }
      else if(input(UpButton) || input(DownButton))
      {
         // Calculo de potencia de 10 a sumar
         for(int i = 0; i < Exponent; i++, aux *= 10);

         if(input(DownButton))
            (((signed int32) *Number - aux) >= MinNumber) ? (*Number -= aux) : (*Number = MinNumber);
            // Si no se hace negativo, le restamos el numero
         else if(input(UpButton))
            ((*Number + aux) <= MaxNumber) ? (*Number += aux) : (*Number = MaxNumber);
            // Si no se hace mayor que el numero maximo, se lo sumamos

         PrintBool = 1;

         while (input(DownButton) || input(UpButton));
      }
   } 

   // Si el exponente es mayor a MaxExponent significa que se presiono back desde el primer numero
   if(Exponent > MaxExponent)
      return true;
   else
      return false;
}

void Wire_Print(struct wire_print_order ActualOrder)
{
   for(int16 wire = 1; wire <= ActualOrder.Amount; wire++)     // Impresion de cada cable
   {
      printf(lcd_putc,"\fIMPRIMIENDO #%3lu\n-", wire);
      delay_ms(3000);

      // Al finalizar impresion de cable modificar memoria eeprom con nuevo valor
   }
}

// PENDIENTES:
// Crear y terminar funcion de recarga de cable
// Terminar funcion Wire_Print
// Obtener y modificar cantidad de cable disponible de memoria eeprom
// Si hay cable, checar booleano de control para saber si se mandan instrucciones con interfaz grafica o fisica
// Terminar funcion de la interrupcion del protocolo RS232
// Configurar voltaje de referencia
// Corregir detalles en pelado

// Posible interrupcion de timer para revisar constantemente presencia de cable
// Posible boton de interrupcion para cancelar una orden que se esta imprimiendo
// Posible espacio en memoria eeprom para guardar booleano para saber si se concluyo la impresion que comenzo (Para posibles apagones)
// Si no concluyo comenzar proceso de recarga