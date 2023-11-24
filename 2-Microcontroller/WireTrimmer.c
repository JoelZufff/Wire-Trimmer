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

//   Variables Constantes  //
#define     DelayMP  500

// Estructuras //
struct step_motor
{
   long           Steps[4];                  // Pines de cada paso del motor
   signed int     PosIndex;                  // Posicion actual del motor
};

struct wire_print_order
{
   int16          Amount;                    // Cantidad
   int16          Length;                    // Longitud del cable (mm)
   int16          PeelingLength;             // Longitud de pelado de cable (mm)
};

// Interrupcion //
#int_rda
void ComputerConection() {} // Activar y gestionar conexion con interfaz de computadora

//  Prototipos de funcion  //
int1 NumberSelect(char Title[16], int16* Number, int16 MinNumber, int16 MaxNumber);               // Funcion para seleccion de numero
void MotorSteps(int16 Amount, struct step_motor*, int1 turnDirection);  // Se mueve un motor "Amount" veces
void MotorSteps(int16 Amount, struct step_motor*, struct step_motor*, int1 turnDirection); // Sobrecarga para mover 2 motores a la vez

// Variables Globales
int16 LCD_Contrast = 50;

void main()
{
   // Activamos Interrupciones
   enable_interrupts(GLOBAL);       enable_interrupts(int_rda);
   
   // PWM para contraste de LCD
   setup_timer_2(T2_DIV_BY_16, 124, 1);
   setup_ccp1(CCP_PWM);
   set_pwm1_duty(LCD_Contrast);

   // Declaro e inicializo los motores a pasos
   struct step_motor WireMotor1 = {{PIN_D0, PIN_D1, PIN_D2, PIN_D3}, 0};
   struct step_motor WireMotor2 = {{PIN_D4, PIN_D5, PIN_D6, PIN_D7}, 0};

   // Obtenemos longitud del cartucho de cable de la memoria eeprom (cm)
   //float Reel = read_eeprom(1);
   float WireReel = 999.99;

   // Mensaje Introductorio
   lcd_init();       lcd_gotoxy(3,1);
   printf(lcd_putc,"WIRE TRIMMER\n Prog. Avanzada");
   delay_ms(1000);
   
   while(true)
   {
      // Revisar disponibilidad de cable con memoria eeprom, y con ADC de sensor laser de presencia de cable.
      // Revision de booleano para control con computadora

      Start:
      printf(lcd_putc, "\fPresione DERECHA\npara comenzar ->");
      while(!input(RightButton));   while(input(RightButton));

      // Creamos orden de impresion con valores minimos
      struct wire_print_order ActualOrder = { 1, 5, 0 };

      // Si number select devuelve true, hay que volver al menu anterior
      Amount:
      if( NumberSelect( (char*) "Cantidad:", &ActualOrder.Amount, 1, 100) )
         goto Start;
      
      Length:
      if( NumberSelect( (char*) "Longitud (cm):", &ActualOrder.Length, 5, 100) ) // Valor maximo en funcion de memoria eeprom y cantidad
         goto Amount;

      Peeling:
      if( NumberSelect( (char*) "Pelado (mm):", &ActualOrder.PeelingLength, 0, 10) ) // Valor maximo en funcion de longitud de cada cable
         goto Length;
   }
}

int1 NumberSelect(char Title[16], int16 *Number, int16 MinNumber, int16 MaxNumber)
{
   // Declaramos e inicializamos variables que ocuparemos
   int1 PrintBool = 1;  int1 PosChar = 1;    signed int MaxExponent = 0;   signed int Exponent = 0;

   // Sacamos el exponente del numero maximo posible que puede tomar el numero
   for(int16 aux = MaxNumber; aux >= 10; aux/=10, MaxExponent++, Exponent++);

   // Impresion de titulo y espacio para numeros
   printf(lcd_putc, "\f%s\n------    ------", Title);

   // Ciclo que se ejecuta el tiempo que se modifique el numero
   while((Exponent >= 0) && (Exponent <= MaxExponent))
   {
      long aux = 1;

      // Impresion en LCD
      if(PrintBool)
      {
         lcd_gotoxy(7,2);
         printf(lcd_putc,"%04lu", *Number);         
         PrintBool = 0;

         if(PosChar) { lcd_gotoxy(10-Exponent,2);    lcd_putc('*');    PosChar = 0; }
      }
      
      // Modificacion de variable
      if(input(RightButton) || input(LeftButton))
      {
         input(RightButton) ? Exponent-- : Exponent++;
         PrintBool = 1;    PosChar = 1;

         while (input(RightButton) || input(LeftButton));
      }
      else if(input(UpButton) || input(DownButton))
      {
         // Calculo de potencia de 10 a sumar
         for(int i = 0; i < Exponent; i++, aux *= 10);

         if(input(DownButton))
            (((signed long) *Number - aux) >= MinNumber) ? (*Number -= aux) : (*Number = MinNumber);
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

void MotorSteps(int16 Amount, struct step_motor* Motor1, int1 turnDirection) // Movimiento de un solo motor
{   
   for(int16 i = 0; i < Amount; i++)
   {
      // Establecer siguiente paso
      turnDirection ? Motor1->PosIndex++ : Motor1->PosIndex--;
      
      // Corregir desborde de paso
      (Motor1->PosIndex < 0) ? (Motor1->PosIndex = 3) : ((Motor1->PosIndex > 3) ? (Motor1->PosIndex = 0) : 0);
      
      output_high(Motor1->Steps[Motor1->PosIndex]);
      delay_ms(delayMP);
      output_low(Motor1->Steps[Motor1->PosIndex]);
   }
}

void MotorSteps(int16 Amount, struct step_motor* Motor1, struct step_motor* Motor2, int1 turnDirection) // Movimiento de 2 motores a la vez
{
   for(int16 i = 0; i < Amount; i++)
   {
      // Establecer siguiente paso
      turnDirection ? (Motor1->PosIndex++, Motor2->PosIndex++) : (Motor1->PosIndex--, Motor2->PosIndex--);
      
      // Corregir desborde de paso
      (Motor1->PosIndex < 0) ? (Motor1->PosIndex = 3) : ((Motor1->PosIndex > 3) ? (Motor1->PosIndex = 0) : 0);
      (Motor2->PosIndex < 0) ? (Motor2->PosIndex = 3) : ((Motor2->PosIndex > 3) ? (Motor2->PosIndex = 0) : 0);

      output_high(Motor1->Steps[Motor1->PosIndex]);   output_high(Motor2->Steps[Motor2->PosIndex]);
      delay_ms(delayMP);
      output_low(Motor1->Steps[Motor1->PosIndex]);    output_low(Motor2->Steps[Motor2->PosIndex]);
   }
}

// Hacer que lo que avanza la funcion motor STEP sean mm y no pasos (Realizar conversion adentro de funcion con datos de cuantos mm es un paso)
