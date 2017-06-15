/////////////////////////////////////////////////////////////////////////
////                         PicWinUSB.c                             ////
////                                                                 ////
//// Este ejemplo muestra como desarrollar un sencillo dispositivo   ////
//// USB con un microcontrolador PIC de la serie 18Fxx5x.            ////
//// El dispositivo hace control de leds cuando se le envia la orden ////
//// desde la PC, y realiza la conversión A/D del canal AN0 enviando ////
//// el valor obtenido a la PC vía USB.                              ////
//// Esto lo realiza haciendo uso del Driver WinUSB de Microsoft     ////
//// compatible con Windows 7/Vista/XP de 32 y 64 bits.              ////
//// http://www.microsoft.com/whdc/device/connect/WinUsb_HowTo.mspx  ////                                                      ////
////                                                                 ////
//// Realizado con el compilador CCS PCWH 4.057                      ////
////                                                                 ////
//// Modificado Por: Edgardo Adrián Franco Martínez                  ////
//// Tomado de http://www.hobbypic.com                               ////
/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Al conectar el PicUSB al PC encendemos el Led Rojo hasta que el dispositivo
// halla sido configurado por el PC, en ese momento encederemos el Led Verde.
// Esperaremos hasta que se reciba un paquete proveniente del PC. Comprobaremos
// el primer byte del paquete recibido para comprobar si queremos entrar en el
// modo Suma, donde se realizará una suma de dos operandos, que corresponderan
// con los dos bytes restantes del paquete recibido; una vez realizada la suma
// enviaremos el paquete con el resultado de vuelta al PC. Si entramos en el
// modo Led comprobaremos el segundo byte del paquete recibido para comprobar
// si deberemos apagar los leds, encender el verder o el rojo, togglear los leds,
// o realizar la conversión AD del canal AN0 y enviar la conversión. 
/////////////////////////////////////////////////////////////////////////////

#include <18F2550.h>
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL3,CPUDIV1,VREGEN  //PLL3 (XTAL 12 MHz) PLL5(XTAL 20MHZ)
#use delay(clock=48000000)

#define USB_HID_DEVICE     FALSE             //Disabled HID
#define USB_EP1_TX_ENABLE  USB_ENABLE_BULK   //Turn on EP1(EndPoint1) for IN bulk/interrupt transfers
#define USB_EP1_RX_ENABLE  USB_ENABLE_BULK   //Turn on EP1(EndPoint1) for OUT bulk/interrupt transfers
#define USB_EP1_TX_SIZE    64                 //Size to allocate for the tx endpoint 1 buffer
#define USB_EP1_RX_SIZE    3                 //Size to allocate for the rx endpoint 1 buffer

#include <pic18_usb.h>     //Microchip PIC18Fxx5x Hardware layer for CCS's PIC USB driver
#include <PicWinUSB.h>     //Descriptors and USB configuration
#include <usb.c>           //handles usb setup tokens and get descriptor reports

#define LEDV         PIN_B6
#define LEDR         PIN_B7
#define LEDPRUEBA PIN_B1
#define Presencia PIN_B0
#define Temperatura PIN_A2
#define Luz PIN_A3
#define LED_ON       output_high
#define LED_OFF      output_low
#define LED_TOGGLE   output_toggle

#define TAM 67                   //Tamaño del arreglo en bytes
#define modo      recibe[0]
#define param1    recibe[1]
#define param2    recibe[2]
#define resultado envia[0]
#define inicio conversion0[0]    //Inicio del canal AN0
#define final conversion0[1]     //Final del canal AN0
#define inicio1 conversion1[0]   //Inicio del canal AN1
#define final1 conversion1[1]    //Final del canal AN1
#define num0 conversion0[2]      //Numero de bytes enviados
#define num1 conversion1[2]      //Numero de bytes enviados

void main(void) {

   int8 recibe[3];                  //Buffer de recepción
   int8 envia[1];                   //Buffer de salida
   int8 conversion0[TAM];           //Canal AN0
   int8 conversion1[TAM];           //Canal AN1
   int8 luxes[10];                  //LDR
   int8 temp[10];                   //LM35
   int8 j = 3, flag = 0;            //Para guardar datos en el arreglo
   int8 llena = 0;                  //Bandera para saber si la cola ya se llenó
   inicio = 3;                      //Inicializamos a inicio en 2
   final = 3;                       //Inicializamos a final en 2
   inicio1 = 3;                     //Inicializamos a inicio en 2
   final1 = 3;                      //Inicializamos a final en 2
      
   //Configurar el ADC
   setup_adc_ports(AN0_TO_AN3);
   setup_adc(ADC_CLOCK_INTERNAL);
   
   //Led Verde OFF, Led Rojo ON
   LED_OFF(LEDV);
   LED_ON(LEDR);

   //Inicializar USB en el PIC
   usb_init();

   //Arranque de la tarea encargada del modulo USB
   usb_task();

   //Esperar a la enumeración del dispositivo USB por parte del HOST al que este conectado
   usb_wait_for_enumeration();
   
   //Led Verde ON, Led Rojo OFF
   LED_ON(LEDV);
   LED_OFF(LEDR);

   //Realizar 
   while (TRUE)
   {
      if(usb_enumerated())          //si el PicUSB está configurado
      {
         //TOMA DE MUESTRAS PARA OSCILOSCOPIO
      
         if (j < TAM)                              //La cola no está llena
         {
            //TOMAMOS LECTURAS DEL CANAL 0
            set_adc_channel(0);
            delay_us(10);     //Esperar 10 microsegundos
            conversion0[j] = read_adc(); //Lanzar y leer la conversión
            //TOMAMOS LECTURAS DEL CANAL 1
            set_adc_channel(1);
            delay_us(10);     //Esperar 10 microsegundos
            conversion1[j] = read_adc();  //Lanzar y leer la conversion
            j++;
         }else                                    //La cola está llena
         {
            llena = 1;
            j = 3;
            inicio = TAM+1;
            //TOMAMOS LECTURAS DEL CANAL 0
            set_adc_channel(0);
            delay_us(10);     //Esperar 10 microsegundos
            conversion0[j] = read_adc(); //Lanzar y leer la conversión
            //TOMAMOS LECTURAS DEL CANAL 1
            set_adc_channel(1);
            delay_us(10);     //Esperar 10 microsegundos
            conversion1[j] = read_adc();  //Lanzar y leer la conversion
            j++;
         }
         if (flag < 10)
         {
            //TOMAMOS LAS MUESTRAS DE LOS LUXES
            set_adc_channel(Luz);
            delay_us(50);
            luxes[flag] = read_adc();
            //TOMAMOS LAS MUESTRAS DE LA TEMPERATURA
            set_adc_channel(Temperatura);
            delay_us(50);
            temp[flag] = read_adc();
            flag ++;
         }else
         {
            flag = 0;
            //TOMAMOS LAS MUESTRAS DE LOS LUXES
            set_adc_channel(Luz);
            delay_us(50);
            luxes[flag] = read_adc();
            //TOMAMOS LAS MUESTRAS DE LA TEMPERATURA
            set_adc_channel(Temperatura);
            delay_us(50);
            temp[flag] = read_adc();
            flag ++;
         }
         
         //FIN DE LA TOMA DE MUESTRAS PARA EL OSCILOSCOPIO
         
         if (usb_kbhit(1))          //si el endpoint contiene datos del host
         {
         
            //OBTENEMOS EL INCIO Y EL FINAL DE LA COLA CIRCULAR
            if (inicio == TAM +1)
            {
               if (j < TAM)
               {
                  final = j - 1;
                  inicio = j;
               }else
               {
                  final = TAM;
                  inicio = 2;
               }
            }else
            {
               final = j - 1;
            }
            inicio1 = inicio;
            final1 = final;
            //Tomar el paquete de tamaño 3bytes del EP1 y almacenamos en recibe
            usb_get_packet(1, recibe, 3);

            if (modo == 0) // Modo_Suma
            {
               resultado = param1 + param2;  //hacemos la suma
               usb_put_packet(1, envia, 1, USB_DTS_TOGGLE); //enviamos el paquete de tamaño 1byte del EP1 al PC
            }
            
            if (modo == 1) // Modo_Resta
            {
               resultado = param1 - param2;  //hacemos la resta
               usb_put_packet(1, envia, 1, USB_DTS_TOGGLE); //enviamos el paquete de tamaño 1byte del EP1 al PC
            }
            
            if (modo == 2) // Modo_Multiplicación
            {
               resultado = param1 * param2;  //hacemos la multiplicación
               usb_put_packet(1, envia, 1, USB_DTS_TOGGLE); //enviamos el paquete de tamaño 1byte del EP1 al PC
            }
             
            
            if (modo == 3) // Modo_División
            {
               resultado = param1 / param2;  //hacemos la división
               usb_put_packet(1, envia, 1, USB_DTS_TOGGLE); //enviamos el paquete de tamaño 1byte del EP1 al PC
            }
            
            if (modo == 4) //Sensor de presencia
            {
               if(!input(Presencia))
               {
                  resultado = 1;
                  LED_ON(LEDPRUEBA);
               }else
               {
                  resultado = 0;
                  LED_OFF(LEDPRUEBA);
               }
               usb_put_packet(1, envia, 1, USB_DTS_TOGGLE);    //Enviamos el paquete de tamaño 1 byte del EP1 al PC
            }
            
            if (modo == 5) //Sensor LM35
            {
               //set_adc_channel(Temperatura);
               //resultado = read_adc();
               usb_put_packet(1,temp,10,USB_DTS_TOGGLE);
            }
            
            if (modo == 6) //Luxes
            {
               //set_adc_channel(Luz);
               //resultado = read_adc();
               usb_put_packet(1,luxes,10,USB_DTS_TOGGLE);      //Enviamos el paquete de tamaño 10 bytes del EP1 al PC
            }

            if (modo == 9) // Modo_Led
            {
               if (param1 == 0) {LED_OFF(LEDV); LED_OFF(LEDR);} //apagamos los leds
               if (param1 == 1) {LED_ON(LEDV); LED_OFF(LEDR);} //encendemos led verde
               if (param1 == 2) {LED_OFF(LEDV); LED_ON(LEDR);} //encendemos led rojo
               if (param1 == 3) {LED_ON(LEDV); LED_ON(LEDR);} //encendemos los leds
               if (param1 == 4) {LED_TOGGLE(LEDV); LED_TOGGLE(LEDR);} //toggleamos los leds
            }                    
            if (modo == 10)                //Modo_ADC_0
            {
               if (llena)
               {
                  num0 = TAM;
                  usb_put_packet(1, conversion0, TAM, USB_DTS_TOGGLE); //enviamos el paquete de tamaño 66 bytes del EP1 al PC
               }else
               {
                  num1 = j;
                  usb_put_packet(1, conversion0, j, USB_DTS_TOGGLE); //enviamos el paquete de tamaño j bytes del EP1 al PC
               }
            }
            if (modo == 11)                  //Modo_ADC_1
            {
               if (llena)
               {
                  num1 = TAM;
                  usb_put_packet(1, conversion1, TAM, USB_DTS_TOGGLE); //enviamos el paquete de tamaño 66 bytes del EP1 al PC
               }else
               {
                  num1 = j;
                  usb_put_packet(1, conversion1, j, USB_DTS_TOGGLE); //enviamos el paquete de tamaño j bytes del EP1 al PC
               }
            }
         }
      }
   }
}
