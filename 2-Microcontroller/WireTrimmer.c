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

#define     In       1
#define     Out      0

const long LEDS[5] = {PIN_E0, PIN_A5, PIN_A4, PIN_A2, PIN_A1};

// Macros y variables constantes
#define     WireSensor(ADC) (ADC < 900 ? 1 : 0)       // Macro para ver si hay un cable en fotoresistencia
#define     MinWireLenght     30                      // Valor minimo de carrete disponible
#define     MaxWireLenght     10000                   // Valor maximo de carrete disponible

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
long     WireReel = 0;                       // Carrete diponible (mm)
int1     ConectionStatus = 0;                // Booleano de conexion con interfaz grafica
int1     PendingOrderBool = 0;               // Booleano de orden pendiente

struct   wire_print_order ComputerOrder = {0,0,0};

// Configuramos Motores a Pasos
struct stepper_motor WireMovementMotor       = {PIN_D1, PIN_D0};
struct stepper_motor ReelMotor               = {PIN_C1, PIN_C2};
struct stepper_motor WireCuttingMotor        = {PIN_D2, PIN_D3};

// Interrupciones //
#int_rda
void ComputerConection()      // Para recibir datos de interfaz grafica
{
   char data = getch();

   if(data == '+')    // Se recibe el caracter de conexion y hay cable disponible
   {
      if(WireReel < MinWireLenght)        // Si no hay cable disponible se lo informamos a la interfaz
         putc('-');
      else
      {
         printf("+%05ld", WireReel);     // Enviamos informacion a interfaz grafica
         ConectionStatus = 1;
      }
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
      AmountBuffer[4] = '\0';

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
void WireReel_ValueReset(long Length, int1 ResetBool);                                                   // Funcion para actualizar valor de carrete
void Wire_Movement(int32 Steps);                                                                    // Funcion para mover el motor ciertos pasos
void Wire_Recharge();                                                                               // Funcion para recarga de cable

void main()
{  
   // Desactivamos LEDS para evitar errores
   for(int i = 0; i < 5 ; i++)
      output_low(LEDS[i]);
   
   // Activamos Interrupciones
   enable_interrupts(GLOBAL);       enable_interrupts(int_rda);
   
   // Configuramos puertos ADC para sensor laser
   setup_adc(adc_clock_div_2);
   set_tris_a(0b00000001);
   setup_adc_ports(AN0, VSS_VREF);     // Sensor con voltaje de referencia
   set_adc_channel(0);     delay_us(10);

   output_low(PIN_A1);

   // Mensaje Introductorio
   lcd_init();    lcd_gotoxy(3,1);
   printf(lcd_putc,"WIRE TRIMMER\n Prog. Avanzada");
   delay_ms(1000);

   while(true)
   {
      /*WireReel = read_eeprom(1);*/  WireReel = 5;   // Valor maximo 10,000 mm de carrete
      
      // Si el cable es insuficiente, PROCESO DE RECARGA
      while(WireReel < MinWireLenght)
      {
         printf(lcd_putc,"\fCarrete Escaso");
         
         // Animacion
         for (long timer = 0; !input(RightButton); (timer > 1000) ? (timer = 0) : (timer++))
         {
            lcd_gotoxy(1, 2);

            if (timer == 1)
               printf(lcd_putc, "Nuevo Carrete->");
            else if(timer == 500)
               printf(lcd_putc, "               ");

            delay_ms(1);
         }
         while(input(RightButton));

         // Proceso de recarga de cable
         Wire_Recharge();
      }

      Main:
      // Creacion de orden
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
         
         // Ya no hay orden pendiente y desconectamos conexion con computadora para confirmar conexion
         PendingOrderBool = ConectionStatus = 0;      
         putc('*');
         delay_ms(100);
      }
      else                       // Se utiliza la interfaz fisica para realizar ordenes
      {
         printf(lcd_putc,"\fCable: %5ld mm", WireReel);
         // Animacion
         for (long Timer = 0; !input(RightButton); (Timer > 1000) ? (Timer = 0) : (Timer++))
         {
            lcd_gotoxy(1, 2);

            if (Timer == 1)
               printf(lcd_putc, "Nueva Orden  ->");
            else if(Timer == 500)
               printf(lcd_putc, "               ");

            if(ConectionStatus)
               goto Main;

            delay_ms(1);
         }
         while(input(RightButton));

         // Creamos orden de impresion
         struct wire_print_order PhysicalOrder = {5, 0, 0};

         Peeling:
         // Valor maximo en funcion de longitud de cada cable
         if( Number_Select( (char*) "Pelado (mm):", &PhysicalOrder.PeelingLength, 0, 20) )
            goto Main;
         
         Length:
         // Valor maximo en funcion de: Carrete disponible - 2 veces longitud del pelado
         if( Number_Select( (char*) "Longitud (mm):", &PhysicalOrder.Length, 10 , WireReel - (PhysicalOrder.PeelingLength * 2)) )
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

      // Conversion de mm de pedido a pasos del motor (Hacer macro para conversion PENDIENTE)
      Wire_Movement(ActualOrder.PeelingLength);

      // Realizamos corte de pelado (Usar macro para conversion PENDIENTE)
      Wire_Movement(ActualOrder.Length);

      // Realizamos corte de pelado (Usar macro para conversion PENDIENTE
      Wire_Movement(ActualOrder.PeelingLength);

      // Realizamos corte final de cable PENDIENTE
      // Al finalizar impresion de cable modificar memoria eeprom con nuevo valor PENDIENTE
   }
}

void Wire_Recharge()
{  
   WireReel = 0;     // Reiniciamos la cantidad del carrete

   printf(lcd_putc, "\fColoque el CABLE\nen su posicion");
   while(!WireSensor(read_adc()));     // Mientras no se detecte el cable en su posicion
   delay_us(10);
   
   int32    Steps = 0;              // Contador de pasos del motor
   
   RechargingProcess:

   printf(lcd_putc, "\fEnrollar     ->\nDetener      <-");

   // Establecemos direccion de motores
   output_bit(ReelMotor, In);   
   output_bit(WireMovementMotor, In);

   int1     MovementBool = 0;       // Booleano para control de movimiento
   long     Timer = 0;              // Timer para movimiento de motores

   while (WireSensor(read_adc()))   // Mientras el sensor detecte cable
   {
      if(input(RightButton))
         MovementBool = 1;
      else if(input(LeftButton))
         MovementBool = 0;
      
      if(MovementBool)
      {
         if(Timer == 50)          // Si pasaron 50 ms 
         {
            output_high(WireMovementMotor.StepPIN);
            output_low(WireMovementMotor.StepPIN);
            Steps++;
         }
         else if(Timer == 100)    // Si pasaron 100 ms
         {
            output_high(ReelMotor.StepPIN);
            output_low(ReelMotor.StepPIN);
         }
         else if(Timer > 100)
            Timer = 0;
         
         delay_ms(1);
         Timer++;
      }
      else
         Timer = 0;
   }

   for(Timer = 0; WireSensor(read_adc()); (Timer > 100) ? (Timer = 0) : (Timer++))
   {
      if(Timer == 50)          // Si pasaron 50 ms 
      {
         output_high(WireMovementMotor.StepPIN);
         output_low(WireMovementMotor.StepPIN);
         Steps--;
      }
      else if(Timer == 100)    // Si pasaron 100 ms
      {
         output_high(ReelMotor.StepPIN);
         output_low(ReelMotor.StepPIN);
      }

      delay_ms(1);
   }
   
   printf(lcd_putc, "\fCable Enrollado?\nContinuar     ->");
   
   while(!input(RightButton))
   {
      if (WireSensor(read_adc()))
         goto RechargingProcess;
   }
      
   // Volvemos a mover el cable hasta detectarlo

   // Establecemos direccion de motores
   output_bit(ReelMotor, Out);
   output_bit(WireMovementMotor, Out);

   for(Timer = 0; !WireSensor(read_adc()); (Timer > 100) ? (Timer = 0) : (Timer++))
   {
      if(Timer == 50)          // Si pasaron 50 ms 
      {
         output_high(WireMovementMotor.StepPIN);
         output_low(WireMovementMotor.StepPIN);
         Steps--;
      }
      else if(Timer == 100)    // Si pasaron 100 ms
      {
         output_high(ReelMotor.StepPIN);
         output_low(ReelMotor.StepPIN);
      }

      delay_ms(1);
   }
   
   // Usar macro para convertir los pasos a mm PENDIENTE

   WireReel_ValueReset(Steps, TRUE);
}

void WireReel_ValueReset(long Length, int1 RestetBool)
{
   if(RestetBool)
      WireReel = Length;
   else
      WireReel += Length;
   
   // Prendemos los leds correspondientes al tamaño del carrete
   for(int i = 0; i < 5; i++)
   {
      if(i < (WireReel / MaxWireLenght * 5))
         output_high(LEDS[i]);
      else
         output_low(LEDS[i]);
   }

   // Acutalizar memoria eeprom
}

void Wire_Movement(int32 Steps)
{
   // Establecemos direccion a los motores
   output_bit(ReelMotor.DirectionPIN, Out);
   output_bit(WireMovementMotor.DirectionPIN, Out);
   
   int32 ActualStep;
   
   for(long Timer = 0, ActualStep = 0; ActualStep < Steps ; (Timer > 100) ? (Timer = 0) : (Timer++))
   {
      if(Timer == 50)          // Si pasaron 50 ms 
      {
         output_high(WireMovementMotor.StepPIN);
         output_low(WireMovementMotor.StepPIN);
         ActualStep++;
      }
      else if(Timer == 100)    // Si pasaron 100 ms
      {
         output_high(ReelMotor.StepPIN);
         output_low(ReelMotor.StepPIN);
      }

      delay_ms(1);
   }

   // Revisar sensor de cable constantemente PENDIENTE
}

// PENDIENTES:
// Obtener y modificar cantidad de cable disponible de memoria eeprom
// Terminar funcion Wire_Print
// Crear interrupcion de timer para revisar constantemente el sensor de cable

// Corregir posible error cuando haya poco carrete en pelado
// Crear version donde en lugar de restringir la cantidad maxima del pedido le de una advertencia diciendo que no hay sufiente cable

// Posible espacio en memoria eeprom para guardar booleano para saber si se concluyo la impresion que comenzo (Para posibles apagones)

