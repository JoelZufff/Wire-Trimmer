// Preprocesadores de Microcontrolador //
#include    <18f4550.h>                                        // Libreria del Microcontrolador
#device     adc = 10                                           // Resolucion del ADC en bits
#fuses      INTRC, NOWDT, NOPROTECT, NOLVP, CPUDIV1, PLL1      // Fusibles (Configuraciones del microcontrolador)
#use        delay(clock = 8M)                                  // Configuracion de delay
#use        rs232(rcv = pin_c7, xmit = pin_c6, baud = 9600, bits = 8, parity = n)

#include <stdlib.h>

// Preprocesadores LCD //
#define LCD_ENABLE_PIN  PIN_B2
#define LCD_RS_PIN      PIN_B0
#define LCD_RW_PIN      PIN_B1
#define LCD_DATA4       PIN_B4
#define LCD_DATA5       PIN_B5
#define LCD_DATA6       PIN_B6
#define LCD_DATA7       PIN_B7

#include    <lcd.c>

//  Constantes para mejor entendimiento   //
#define     DownButton     PIN_D6
#define     UpButton       PIN_D5
#define     LeftButton     PIN_D4
#define     RightButton    PIN_D7

#define     Left     1
#define     Right    0

// Macros y variables constantes
#define     WireSensor(ADC) (ADC < 900 ? 1 : 0)       // Macro para ver si hay un cable en fotoresistencia
#define     MinWireLenght     10                      // Valor minimo que puede tomar el cable

// Estructuras //
struct wire_print_order
{
   int16          PeelingLength;             // Longitud de pelado de cable (mm)
   int16          Length;                    // Longitud del cable (mm)
   int16          Amount;                    // Cantidad
};

struct stepper_motor
{
   long           StepPIN;                   // Pin para realizar un paso en el motor
   long           DirectionPIN;              // Pin para cambiar direccion del motor
};

// Variables globales
long     WireReel = 0;

int1     ConectionStatus = 0;
int1     PendingOrderBool = 0;

struct   wire_print_order ComputerOrder = {0,0,0};

// Interrupciones //
#int_rda
void ComputerConection()      // Para recibir datos de interfaz grafica
{
   char data = getch();

   if(data == '+')    // Se recibe el caracter de conexion
   {
      printf("+%05ld", WireReel);     // Enviamos informacion a interfaz grafica
      ConectionStatus = 1;
   }
   else if(!PendingOrderBool && data == '*')    // Si no hay ordenes pendientes por imprimir y recibimos el caracter de impresion
   {
      // *(2 digitos - longitud de pelado)(5 digitos - longitud del cable)(4 digitos - cantidad de cables)
      char PellingBuffer[3];
      char LenghtBuffer[6];
      char AmountBuffer[5];

      // Recibimos caracteres de longitud de pelado de cable
      for(int i = 0; i < 2; i++)
         PellingBuffer[i] = getch();
      PellingBuffer[2] = '\0';

      // Recibimos caracteres de longitud de cable
      for(int i = 0; i < 5; i++)
         LenghtBuffer[i] = getch();
      LenghtBuffer[5] = '\0';

      // Recibimos caracteres de cantidad de cables
      for(int i = 0; i < 4; i++)
         AmountBuffer[i] = getch();
      AmountBuffer[5] = '\0';

      char *endptr;
      
      // Guardamos la orden para que se realize la impresion
      ComputerOrder.PeelingLength = strtoul(PellingBuffer, &endptr, 10);
      ComputerOrder.Length = strtoul(LenghtBuffer, &endptr, 10);
      ComputerOrder.Amount = strtoul(AmountBuffer, &endptr, 10);

      PendingOrderBool = 1;
   }
}

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
   struct stepper_motor WireMovementMotor       = {PIN_D0, PIN_D1};
   struct stepper_motor WireCuttingMotor        = {PIN_D2, PIN_D3};
   struct stepper_motor ReelMotor               = {PIN_C1, PIN_C2};

   // Mensaje Introductorio
   lcd_init();    lcd_gotoxy(3,1);
   printf(lcd_putc,"WIRE TRIMMER\n Prog. Avanzada");
   delay_ms(1000);
   
   while(true)
   {
      /*WireReel = read_eeprom(1);*/  WireReel = 10000;   // Valor maximo 65,000 mm de carrete (Para no desbordar las variables)
      
      // Evaluacion de disponibilidad de cable
      if(!WireSensor(read_adc()) || (WireReel < MinWireLenght))
      {
         printf(lcd_putc,"\f!! NO SE DETECTA\nCABLE DISPONIBLE");
         delay_ms(2000);

         // Funcion para proceso de recarga de cable
      }
      
      // Creacion de orden
      Main:

      if(ConectionStatus)        // Se utiliza interfaz grafica para realizar ordenes
      {
         // Animacion mientras recibe datos
         printf(lcd_putc, "\f   Recibiendo   ");
         for(int i = 1; !PendingOrderBool; (i > 16) ? (i = 1) : i++ )
         {
            lcd_gotoxy(1,2);     printf(lcd_putc, "                ");
            lcd_gotoxy(i,2);     lcd_putc('-');       delay_ms(100);
         }

         // Imprimimos orden recibida
         Wire_Print(ComputerOrder);
         // Ya no hay orden pendiente y avisamos a interfaz que se finalizo a impresion
         PendingOrderBool = 0;      
         putc('*');

         // Desconectamos computadora para confirmar conexion
         ConectionStatus = 0;
         delay_ms(100);
      }
      else                       // Se utiliza la interfaz fisica para realizar ordenes
      {
         printf(lcd_putc,"\fCable: %5ld mm\nNueva Orden  -->", WireReel);
         while(!input(RightButton))
            if(ConectionStatus)
               goto Main;
         while(input(RightButton));

         // Creamos orden de impresion
         struct wire_print_order PhysicalOrder = {5, 0, 0};

         Peeling:
         // Valor maximo en funcion de longitud de cada cable
         if( Number_Select( (char*) "Pelado (mm):", &PhysicalOrder.PeelingLength, 0, 20) )
            goto Main;
         
         Length:
         // Valor maximo en funcion de: Carrete disponible - 2 veces longitud del pelado
         if( Number_Select( (char*) "Longitud (mm):", &PhysicalOrder.Length, MinWireLenght , WireReel - (PhysicalOrder.PeelingLength * 2)) )
            goto Peeling;
         
         Amount:
         // Valor maximo en funcion del carrete disponible y la longitud solicitada
         if( Number_Select( (char*) "Cantidad:", &PhysicalOrder.Amount, 1, WireReel / (PhysicalOrder.Length + (PhysicalOrder.PeelingLength * 2))) )
            goto Length;

         // Mensaje de confirmacion de orden
         printf(lcd_putc, "\f %4ld Cable(s)  \n    %5ld mm    ",PhysicalOrder.Amount, PhysicalOrder.Length);
         while(!input(RightButton))
            if(input(LeftButton))   {  while(input(LeftButton));  goto Amount; }
         while(input(RightButton));
         
         // Impresion del orden de cable
         Wire_Print(PhysicalOrder);
      
         printf(lcd_putc,"\fORDEN FINALIZADA\n Vuelva Pronto !");
         delay_ms(2000);
      }
   }
}

int1 Number_Select(char Title[16], int16 *Number, int16 MinNumber, int16 MaxNumber)
{  
   // Declaramos e inicializamos variables que ocuparemos
   int1 PrintBool = 1;     int1 PositionChar = 0;    signed int MaxExponent = 0;   signed int Exponent = 0;
   
   // Correccion de posible error en numero
   (*Number < MinNumber) ? (*Number = MinNumber) : (*Number > MaxNumber ? (*Number = MaxNumber) : 0);

   // Sacamos el exponente del numero maximo posible que puede tomar el numero
   for(int16 aux = MaxNumber; aux >= 10; aux /= 10, MaxExponent++, Exponent++);

   // Impresion de titulo y espacio para numeros
   printf(lcd_putc, "\f%s\n-----      -----", Title);

   long timer = 0;

   // Ciclo que se ejecuta el tiempo que se modifique el numero
   while((Exponent >= 0) && (Exponent <= MaxExponent))
   {
      long aux = 1;

      // Timer para animacion de seleccion
      delay_ms(1);
      (++timer == 1) ? (PrintBool = 1) : (timer == 500 ? (PositionChar = 1) : (timer == 1000) ? (timer = 0) : 0);
      
      // Impresion en LCD
      if(PrintBool)
      {
         lcd_gotoxy(6,2);
         printf(lcd_putc,"%06lu", *Number);     
         PrintBool = 0;    
      }
      else if(PositionChar)
      { 
         lcd_gotoxy(11-Exponent,2);    
         lcd_putc('_');    
         PositionChar = 0; 
      }
      
      // Modificacion de variable
      if(input(RightButton) || input(LeftButton))
      {
         input(RightButton) ? Exponent-- : Exponent++;
         timer = 0;

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

         timer = 0;

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
      printf(lcd_putc,"\fCable %4lu /%4lu", wire, ActualOrder.Amount);
      delay_ms(3000);

      
      
      // Revisar sensor de cable constantemente


      // Al finalizar impresion de cable modificar memoria eeprom con nuevo valor
   }
}

// PENDIENTES:
// Crear version donde en lugar de restringir la cantidad maxima del pedido le de una advertencia diciendo que no hay sufiente cable

// Crear y terminar funcion de recarga de cable
// Obtener y modificar cantidad de cable disponible de memoria eeprom
// Terminar funcion Wire_Print
// Configurar voltaje de referencia de sensor infrarojo
// Crear led RGB para indicar el estatus de la cantidad de cable
// Corregir posible error cuando haya poco carrete en pelado
// Crear interrupcion de timer para revisar constantemente el sensor de cable y conexion con computadora

// Posible espacio en memoria eeprom para guardar booleano para saber si se concluyo la impresion que comenzo (Para posibles apagones)