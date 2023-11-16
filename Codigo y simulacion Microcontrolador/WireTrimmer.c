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

//   Variables Constantes  //
#define     Left     1
#define     Right    0
#define     DelayMP  500

// Estructuras //
struct menu
{
   char           *MenuOptions[16];          // Arreglo de cadenas de caracteres
   int            OptionMaxIndex;            // Maximo indice para MenuOptions
   signed int     ActualOption;              // Indice actual para MenuOptions
};

struct stepmotor
{
   long          Steps[4];                   // Pines de cada paso del motor
   signed int     PosIndex;                  // Posicion actual del motor
};

#int_rda
void ComputerConection() // Activar y gestionar conexion con interfaz de computadora
{
   // Deteccion de coneccion con computadora
}

//  Prototipos de funcion  //
void MenuSelect(struct menu*);
void MotorSteps(int16 Amount, struct stepmotor*, int1 turnDirection); // Se mueve un motor "Amount" veces
void MotorSteps(int16 Amount, struct stepmotor*, struct stepmotor*, int1 turnDirection); // Sobrecarga para mover 2 motores a la vez

void main()
{
   // Activamos Interrupciones
   enable_interrupts(GLOBAL);       enable_interrupts(int_rda);
   
   // Defino los menus
   struct menu Principal = {{"Realizar Corte", "Configuraciones"}, 1, 0};
   
   // Defino los motores a pasos
   struct stepmotor WireMotor1 = {{PIN_B0, PIN_B1, PIN_B2, PIN_B3}, 0};
   struct stepmotor WireMotor2 = {{PIN_B4, PIN_B5, PIN_B6, PIN_B7}, 0};
   
   // Mensaje Introductorio
   lcd_init();       lcd_gotoxy(3,1);
   printf(lcd_putc,"WIRE TRIMMER\n Prog. Avanzada");
   delay_ms(1000);
   
   while(true)
   {
      // Revisar disponibilidad de cable con memoria eeprom, y con los foto-transistores.
      // Si los 2 estan bien comenzar impresion de menu principal

      // Revision de booleano para control con computadora
      
      MenuSelect(&Principal);

      switch (Principal.ActualOption)
      {
         case 0: // Realizar Corte
         {
            
         }
         break;
         case 1: // Configuraciones
         {
            
         }
         break;
         default: break;
      }
   }
}

void MenuSelect(struct menu* OpenMenu) // Funcion para impresion y seleccion de menus
{
   int1 BoolLCD = 1;    OpenMenu->ActualOption = 0; // Reiniciamos opciones
   
   while(!input(OKButton)) // Mientras el usuario no escoja una opcion
   {
      if (input(LeftButton))
      {
        while(input(LeftButton));
        OpenMenu->ActualOption--;    BoolLCD = 1;
      }
      else if(input(RightButton))
      {
         while(input(RightButton));
         OpenMenu->ActualOption++;   BoolLCD = 1;
      }
      
      // Corregir desborde de Opcion de menu
      OpenMenu->ActualOption < 0 ? (OpenMenu->ActualOption = OpenMenu->OptionMaxIndex) : (OpenMenu->ActualOption > OpenMenu->OptionMaxIndex ? (OpenMenu->ActualOption = 0) : 0);
      
      if(BoolLCD) // Evita que se actualize el LCD constantemente
      {
         printf(lcd_putc,"\f%s", OpenMenu->MenuOptions[OpenMenu->ActualOption]);
         BoolLCD = 0;
      }
   }
}

void MotorSteps(int16 Amount, struct stepmotor* Motor1, int1 turnDirection) // Movimiento de un solo motor
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

void MotorSteps(int16 Amount, struct stepmotor* Motor1, struct stepmotor* Motor2, int1 turnDirection) // Movimiento de 2 motores a la vez
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
