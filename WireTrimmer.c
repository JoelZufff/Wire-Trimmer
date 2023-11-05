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
   int            OptionMaxIndex;              // Maximo indice para MenuOptions
   signed int     ActualOption;               // Indice actual para MenuOptions
};

//  Prototipos de funcion  //
void MenuSelect(struct menu*);
void MotorStep(int16 Amount ,int1 BoolM1, int1 BoolM2, char turnDirection);


void main()
{
   // Defino los menus
   struct menu Principal = {{"Realizar Corte", "Configuraciones"}, 1, 0};
   
   // Mensaje Introductorio
   lcd_init();       lcd_gotoxy(3,1);
   printf(lcd_putc,"WIRE TRIMMER\n Prog. Avanzada");
   delay_ms(1000);
   
   while(true)
   {
      // Revisar disponibilidad de cable con memoria eeprom, y con los foto-transistores.
      // Si los 2 estan bien comenzar impresion de menu principal
      
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

void MotorStep(int16 Amount, int1 BoolM1, int1 BoolM2, int1 turnDirection) // 1 = Izquierda | 0 = Derecha | M1 = Motor 1 | M2 = Bandera Motor 2
{
   static int16   posM1 = PIN_B0 - 1, posM2 = PIN_B4 - 1;
   
   for(int i = 0; i < Amount; i++)
   {
      // Establecer siguiente paso
      (turnDirection && BoolM1) ? posM1++ : (BoolM1 ? posM1-- : 0);
      (turnDirection && BoolM2) ? posM2++ : (BoolM2 ? posM2-- : 0);
      
      // Corregir desborde de paso
      (posM1 > PIN_B3) ? (posM1 = PIN_B0) : (posM1 < PIN_B0 ? (posM1 = PIN_B3) : 0);
      (posM2 > PIN_B7) ? (posM2 = PIN_B4) : (posM2 < PIN_B4 ? (posM2 = PIN_B7) : 0);
   
      
      if (BoolM1 && BoolM2)                           // Los 2 motores se mueven
      {
         output_high(posM1);     output_high(posM2);
         delay_ms(delayMP);
         output_low(posM1);      output_low(posM2);
      }
      else if(BoolM1)                                 // Solo Motor 1 se mueve
      {
         output_high(posM1);
         delay_ms(delayMP);
         output_low(posM1);
      }
      else if(BoolM2)                                 // Solo Motor 2 se mueve
      {
         output_high(posM2);
         delay_ms(delayMP);
         output_low(posM2);
      }
   }
}
   

// Hacer que lo que avanza la funcion motor STEP sean mm y no pasos (Realizar conversion adentro de funcion con datos de cuantos mm es un paso)