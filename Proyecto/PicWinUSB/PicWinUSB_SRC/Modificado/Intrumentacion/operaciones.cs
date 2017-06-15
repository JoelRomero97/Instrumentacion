using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Intrumentacion
{
    class operaciones
    {
       public IntPtr iHandle;                             // global device handle definition
       public PicWinUSBAPI picwinusbapi = new PicWinUSBAPI();
      // public Int16 x = 0;

        public operaciones()
        {

            Guid InterfaceGuid = new Guid("31415926-5358-9793-2384-626433832795"); // .Inf defined Guid                        
            iHandle = picwinusbapi.Init_PicWinUSB(InterfaceGuid);
        }

        public byte suma(double numero_1, double numero_2)
        {
            bool bres;
            byte[] sdBuffer = new byte[3];           // Define send DataBuffer size
            byte[] rdBuffer = new byte[1];           // Define recieve DataBuffer size

            sdBuffer[0] = 0x00;    //Modo SUMA
            sdBuffer[1] = Convert.ToByte(numero_1);
            sdBuffer[2] = Convert.ToByte(numero_2);
            bres = picwinusbapi.Write_PicWinUSB(iHandle, sdBuffer);
            bres = picwinusbapi.Read_PicWinUSB(iHandle, rdBuffer);
            return rdBuffer[0];

        }

        public byte resta(double numero_1, double numero_2)
        {
            bool bres;
            byte[] sdBuffer = new byte[3];           // Define send DataBuffer size
            byte[] rdBuffer = new byte[1];           // Define recieve DataBuffer size

            sdBuffer[0] = 0x01;    //Modo RESTA
            sdBuffer[1] = Convert.ToByte(numero_1);
            sdBuffer[2] = Convert.ToByte(numero_2);
            bres = picwinusbapi.Write_PicWinUSB(iHandle, sdBuffer);
            bres = picwinusbapi.Read_PicWinUSB(iHandle, rdBuffer);
            return rdBuffer[0];
        }

        public byte div(double numero_1, double numero_2)
        {
            bool bres;
            byte[] sdBuffer = new byte[3];           // Define send DataBuffer size
            byte[] rdBuffer = new byte[1];           // Define recieve DataBuffer size

            sdBuffer[0] = 0x03;    //Modo DIVISION
            sdBuffer[1] = Convert.ToByte(numero_1);
            sdBuffer[2] = Convert.ToByte(numero_2);
            bres = picwinusbapi.Write_PicWinUSB(iHandle, sdBuffer);
            bres = picwinusbapi.Read_PicWinUSB(iHandle, rdBuffer);
            return rdBuffer[0];
        }

        public byte multi(double numero_1, double numero_2)
        {
            bool bres;
            byte[] sdBuffer = new byte[3];           // Define send DataBuffer size
            byte[] rdBuffer = new byte[1];           // Define recieve DataBuffer size

            sdBuffer[0] = 0x02;    //Modo MULTIPLICACION
            sdBuffer[1] = Convert.ToByte(numero_1);
            sdBuffer[2] = Convert.ToByte(numero_2);
            bres = picwinusbapi.Write_PicWinUSB(iHandle, sdBuffer);
            bres = picwinusbapi.Read_PicWinUSB(iHandle, rdBuffer);
            return rdBuffer[0];

        }

        public void led_verde_on()
        {
            bool bres;
            byte[] sdBuffer = new byte[3];                          // Definer el tamaño del buffer
            sdBuffer[0] = 0X09;                                     //Modo Led
            sdBuffer[1] = 0X01;                                     //Led ROJO OFF Led Verde ON
            bres = picwinusbapi.Write_PicWinUSB(iHandle, sdBuffer); //Enviar

        }

        public void led_rojo_on()
        {
            bool bres;
            byte[] sdBuffer = new byte[3];                          // Definer el tamaño del buffer
            sdBuffer[0] = 0X09;                                     //Modo Led
            sdBuffer[1] = 0X02;                                     //Led ROJO ON Led Verde OFF
            bres = picwinusbapi.Write_PicWinUSB(iHandle, sdBuffer); //Enviar
        }

        public void leds_on()
        {

            bool bres;
            byte[] sdBuffer = new byte[3];                          // Definer el tamaño del buffer
            sdBuffer[0] = 0X09;                                     //Modo Led
            sdBuffer[1] = 0X03;                                     //Leds ON
            bres = picwinusbapi.Write_PicWinUSB(iHandle, sdBuffer); //Enviar

        }

        public void leds_off()
        {
            bool bres;
            byte[] sdBuffer = new byte[3];                          // Definer el tamaño del buffer
            sdBuffer[0] = 0X09;                                     //Modo Led
            sdBuffer[1] = 0X00;                                     //Leds OFF
            bres = picwinusbapi.Write_PicWinUSB(iHandle, sdBuffer); //Enviar
        }

        public void leds_toggle()
        {
            bool bres;
            byte[] sdBuffer = new byte[3];                          // Definer el tamaño del buffer
            sdBuffer[0] = 0X09;                                     //Modo Led
            sdBuffer[1] = 0X04;                                     //Led Toggle
            bres = picwinusbapi.Write_PicWinUSB(iHandle, sdBuffer); //Enviar
        }
    }
}
