#include    <18f4550.h>                                        // Libreria del Microcontrolador
#device     adc = 10                                           // Resolucion del ADC en bits
#fuses      INTRC, NOWDT, NOPROTECT, NOLVP, CPUDIV1, PLL1      // Fusibles (Configuraciones del microcontrolador)
#use        delay(clock = 8M)                                  // Configuracion de delay
//#use        rs232(rcv = pin_c7, xmit = pin_c6, baud = 9600, bits = 8, parity = n)

// Configuraciones LCD // 
#include    <lcd.c>
#define     LCD_DATA_PORT getenv("SFR:PORTD")

//  Constantes para mejor entendimiento   //
#define     LeftButton     PIN_C0
#define     RightButton    PIN_C1
#define     OKButton       PIN_C2
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
   int16          Length;                    // Longitud del cable (mm)
   int            PeelingLength;             // Longitud de pelado de cable (mm)
   int            Gauge;                     // Calibre
   int            Amount;                    // Cantidad
};

#int_rda
void ComputerConection() // Activar y gestionar conexion con interfaz de computadora
{
   
}

//  Prototipos de funcion  //
int16 NumberSelect(int16 MaxNumber, signed int Exponent);               // Funcion para seleccion de numero
void MotorSteps(int16 Amount, struct step_motor*, int1 turnDirection);  // Se mueve un motor "Amount" veces
void MotorSteps(int16 Amount, struct step_motor*, struct step_motor*, int1 turnDirection); // Sobrecarga para mover 2 motores a la vez

void main()
{
   // Activamos Interrupciones
   enable_interrupts(GLOBAL);       enable_interrupts(int_rda);
      
   // Declaro e inicializo los motores a pasos
   struct step_motor WireMotor1 = {{PIN_B0, PIN_B1, PIN_B2, PIN_B3}, 0};
   struct step_motor WireMotor2 = {{PIN_B4, PIN_B5, PIN_B6, PIN_B7}, 0};

   // Obtenemos longitud del cartucho de cable de la memoria eeprom (cm)
   //float Reel = read_eeprom(1);
   float WireReel = 999.99;

   // Mensaje Introductorio
   lcd_init();       lcd_gotoxy(3,1);
   printf(lcd_putc,"WIRE TRIMMER\n Prog. Avanzada");
   delay_ms(1000);
   
   while(true)
   {
      // Revisar disponibilidad de cable con memoria eeprom, y con sensor de presencia de cable.
      // Revision de booleano para control con computadora

      // Falta validacion de datos para ver forma de restringir pedir mas cable del disponible
      // Ocupo calcular Exponente dentro de la funcion a partir del numero maximo y mandarle que el numero maximo sea el cartucho disponible
      
      struct wire_print_order ActualOrder = {0,0,0,0};
      
      printf(lcd_putc, "\fLongitud (mm):");
      ActualOrder.Length = NumberSelect(1000,3);

      printf(lcd_putc, "\fPelado (mm):");
      ActualOrder.Length = NumberSelect(10,1);
   }
}

int16 NumberSelect(int16 MaxNumber, signed int Exponent)
{
   int1 PrintBool = 0;   int16 Number = 0;    int auxExp = Exponent;

   printf(lcd_putc, "\n------0000------");
   lcd_gotoxy(10-Exponent,2);   lcd_putc('*');

   while(Exponent >= 0)
   {
      int16 aux = 1;

      if(PrintBool)
      {
         lcd_gotoxy(7,2);
         printf(lcd_putc,"%04lu", Number);         
         PrintBool = 0;
      }
      
      if(input(OKButton))
      {
         while (input(OKButton));   Exponent--;
         lcd_gotoxy(7,2);           printf(lcd_putc,"%04lu", Number);
         lcd_gotoxy(10-Exponent,2);   lcd_putc('*');
      }
      else if(input(LeftButton))
      {
         while (input(LeftButton));
         
         for(int i = 0; i < Exponent; i++, aux *= 10);
         (Number >= aux) ? (Number -= aux, PrintBool = 1) : 0;
      }
      else if(input(RightButton))
      {
         while (input(RightButton));
         
         for(int i = 0; i < Exponent; i++, aux *= 10);
         ((Number + aux) <= MaxNumber) ? (Number += aux, PrintBool = 1) : 0; 
      }
   } 

   return Number;
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
