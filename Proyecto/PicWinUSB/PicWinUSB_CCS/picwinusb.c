/////////////////////////////////////////////////////////////////////////
////                         PicWinUSB.c                             ////
////                                                                 ////
//// Este ejemplo muestra como desarrollar un sencillo dispositivo   ////
//// USB con un microcontrolador PIC de la serie 18Fxx5x.            ////
//// El dispositivo hace control de leds cuando se le envia la orden ////
//// desde la PC, y realiza la conversi�n A/D del canal AN0 enviando ////
//// el valor obtenido a la PC v�a USB.                              ////
//// Esto lo realiza haciendo uso del Driver WinUSB de Microsoft     ////
//// compatible con Windows 7/Vista/XP de 32 y 64 bits.              ////
//// http://www.microsoft.com/whdc/device/connect/WinUsb_HowTo.mspx  ////
////                                                                 ////
//// Realizado con el compilador CCS PCWH 4.057                      ////
////                                                                 ////
//// Modificado Por: Romero Gamarra Joel Mauricio                    ////
//// Tomado de http://www.hobbypic.com                               ////
/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Al conectar el PicUSB al PC encendemos el Led Rojo hasta que el dispositivo
// halla sido configurado por el PC, en ese momento encederemos el Led Verde.
// Esperaremos hasta que se reciba un paquete proveniente del PC. Comprobaremos
// el primer byte del paquete recibido para comprobar si queremos entrar en el
// modo Suma, donde se realizar� una suma de dos operandos, que corresponderan
// con los dos bytes restantes del paquete recibido; una vez realizada la suma
// enviaremos el paquete con el resultado de vuelta al PC. Si entramos en el
// modo Led comprobaremos el segundo byte del paquete recibido para comprobar
// si deberemos apagar los leds, encender el verder o el rojo, togglear los leds,
// o realizar la conversi�n AD del canal AN0 y enviar la conversi�n. 
/////////////////////////////////////////////////////////////////////////////

#include <18F2550.h>
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL3,CPUDIV1,VREGEN  //PLL3 (XTAL 12 MHz) PLL5(XTAL 20MHZ)
#use delay(clock=48000000)

#define USB_HID_DEVICE     FALSE             //Disabled HID
#define USB_EP1_TX_ENABLE  USB_ENABLE_BULK   //Turn on EP1(EndPoint1) for IN bulk/interrupt transfers
#define USB_EP1_RX_ENABLE  USB_ENABLE_BULK   //Turn on EP1(EndPoint1) for OUT bulk/interrupt transfers
#define USB_EP1_TX_SIZE    2                 //Size to allocate for the tx endpoint 1 buffer
#define USB_EP1_RX_SIZE    64                 //Size to allocate for the rx endpoint 1 buffer

#include <pic18_usb.h>     //Microchip PIC18Fxx5x Hardware layer for CCS's PIC USB driver
#include <PicWinUSB.h>     //Descriptors and USB configuration
#include <usb.c>           //handles usb setup tokens and get descriptor reports

#define LEDV         PIN_B6
#define LEDR         PIN_B7
#define LED_ON       output_high
#define LED_OFF      output_low
#define LED_TOGGLE   output_toggle

#define modo      recibe[0]
#define param1    recibe[1]
#define param2    recibe[2]
#define resultado envia[0]
//#define conversion conv[0]

void main(void) {

   int8 recibe[3];                  //Buffer de recepci�n 
   int8 envia[64];                   //Buffer de salida
   int8 conversion;

   //Configurar el ADC
   setup_adc_ports(AN0_TO_AN1);
   setup_adc(ADC_CLOCK_INTERNAL);
   
   //Tomar una lectura del AN0 del ADC
   set_adc_channel(0);              //Establecer el canal 0 del ADC
   delay_us(10);                    //Esperar 10 microsegundos
   conversion = read_adc();         //Lanzar y leer la conversion
   
   //Tomar una lectura del AN1 del ADC
   set_adc_channel(1);              //Establecer el canal 1 del ADC
   delay_us(10);                    //Esperar 10 microsegundos
   conversion = read_adc();         //Lanzar y leer la conversion
   
   //Led Verde OFF, Led Rojo ON
   LED_OFF(LEDV);
   LED_ON(LEDR);

   //Inicializar USB en el PIC
   usb_init();

   //Arranque de la tarea encargada del modulo USB
   usb_task();

   //Esperar a la enumeraci�n del dispositivo USB por parte del HOST al que este conectado
   usb_wait_for_enumeration();
   
   //Led Verde ON, Led Rojo OFF
   LED_ON(LEDV);
   LED_OFF(LEDR);

   //Realizar 
   while (TRUE)
   {
      if(usb_enumerated())          //si el PicUSB est� configurado
      {
         if (usb_kbhit(1))          //si el endpoint contiene datos del host
         {
            usb_get_packet(1, recibe, 3); //Tomar el paquete de tama�o 3bytes del EP1 y almacenamos en recibe

            if (modo == 0) // Modo_Suma
            {
               resultado = param1 + param2;  //hacemos la suma
               usb_put_packet(1, envia, 64, USB_DTS_TOGGLE); //enviamos el paquete de tama�o 64 bytes del EP1 al PC
            }
            
            if (modo == 1) // Modo_Resta
            {
               resultado = param1 - param2;  //hacemos la resta
               usb_put_packet(1, envia, 64, USB_DTS_TOGGLE); //enviamos el paquete de tama�o 64 bytes del EP1 al PC
            }
            
            if (modo == 2) // Modo_Multiplicaci�n
            {
               resultado = param1 * param2;  //hacemos la multiplicaci�n
               usb_put_packet(1, envia, 64, USB_DTS_TOGGLE); //enviamos el paquete de tama�o 64 bytes del EP1 al PC
            }
             
            
            if (modo == 3) // Modo_Divisi�n
            {
               resultado = param1 / param2;  //hacemos la divisi�n
               usb_put_packet(1, envia, 64, USB_DTS_TOGGLE); //enviamos el paquete de tama�o 64 bytes del EP1 al PC
            }            

            if (modo == 9) // Modo_Led
            {
               if (param1 == 0) {LED_OFF(LEDV); LED_OFF(LEDR);} //apagamos los leds
               if (param1 == 1) {LED_ON(LEDV); LED_OFF(LEDR);} //encendemos led verde
               if (param1 == 2) {LED_OFF(LEDV); LED_ON(LEDR);} //encendemos led rojo
               if (param1 == 3) {LED_ON(LEDV); LED_ON(LEDR);} //encendemos ambos leds
               if (param1 == 4) {LED_TOGGLE(LEDV); LED_TOGGLE(LEDR);} //toggleamos los leds
            }                    
            if (modo == 10) // Modo_ADC
            {
               resultado = read_adc();  //Leer el valor de la ultima conversi�n del ADC
               usb_put_packet(1, envia, 64, USB_DTS_TOGGLE); //enviamos el paquete de tama�o 64 bytes del EP1 al PC
            }
         }
      }
      
      
      //CODIGO DE MULTIPLEXAR Y ESPERAR PARA RECOGER MUESTRAS
      
      
      
   }
}