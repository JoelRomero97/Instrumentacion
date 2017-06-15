﻿using MaterialSkin;
using MaterialSkin.Controls;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Intrumentacion
{
    public partial class Instru : MaterialForm
    {
        lectura_de_datos datos;
        operaciones op;
        Bitmap Osciloscopio = new Bitmap(401, 256);
        Bitmap luxes = new Bitmap(401, 256);
        Bitmap temp = new Bitmap(401, 256);
        //PicWinUSBAPI picwinusbapi = new PicWinUSBAPI();
        Int16 x = 0;
        public Instru()
        {
            InitializeComponent();
            var Material = MaterialSkinManager.Instance;
            Material.AddFormToManage(this);
            Material.Theme = MaterialSkinManager.Themes.DARK;
            Material.ColorScheme = new ColorScheme(Primary.BlueGrey800, Primary.BlueGrey900, Primary.BlueGrey500, Accent.LightBlue200, TextShade.WHITE);
            datos = new lectura_de_datos();
            op = new operaciones();
            //timer_luxes.Start();
            //timer_temperatura.Start();

        }

        private void btnSuma_Click(object sender, EventArgs e)
        {
            datos.numero_1 = Double.Parse(txtSum1.Text);
            datos.numero_2 = Double.Parse(txtSum2.Text);
            txtSumRes.Text = op.suma(datos.numero_1, datos.numero_2).ToString();
        }

        private void btnResta_Click(object sender, EventArgs e)
        {
            datos.numero_1 = Double.Parse(txtRes1.Text);
            datos.numero_2 = Double.Parse(txtRes2.Text);
            txtRes.Text = op.resta(datos.numero_1, datos.numero_2).ToString();
        }

        private void btnMulti_Click(object sender, EventArgs e)
        {
            datos.numero_1 = Double.Parse(txtMulti1.Text);
            datos.numero_2 = Double.Parse(txtMulti2.Text);
            txtMultiRes.Text = op.multi(datos.numero_1, datos.numero_2).ToString();
        }

        private void btnDiv_Click(object sender, EventArgs e)
        {
            datos.numero_1 = Double.Parse(txtDiv1.Text);
            datos.numero_2 = Double.Parse(txtDiv2.Text);

            if (datos.numero_2 == 0)
            {
                MessageBox.Show("Requieres cambiar el denominador, debe ser distinto de 0");
                txtDiv2.Clear();
                datos.numero_2 = Double.Parse(txtDiv2.Text);
            }

            txtDivRes.Text = op.div(datos.numero_1, datos.numero_2).ToString();
        }

        private void btnLEDToggle_Click(object sender, EventArgs e)
        {
            op.leds_toggle();
        }

        private void btnLEDVerdeOn_Click(object sender, EventArgs e)
        {
            op.led_verde_on();
        }

        private void btnLEDRojoOn_Click(object sender, EventArgs e)
        {
            op.led_rojo_on();
        }

        private void btnLEDon_Click(object sender, EventArgs e)
        {
            op.leds_on();
        }

        private void btnLEDof_Click(object sender, EventArgs e)
        {
            op.leds_off();
        }

        private void btnGrafica_Click(object sender, EventArgs e)
        {
            timer_ADC.Start();
            timer_ADC2.Start();
        }

        private void timer_ADC_Tick(object sender, EventArgs e)
        {
            bool bres;
            byte[] sdBuffer = new byte[3];           // Define send DataBuffer size
            byte[] rdBuffer = new byte[66];           // Define recieve DataBuffer size

            sdBuffer[0] = 0x0A;    //Modo ADC
            bres = op.picwinusbapi.Write_PicWinUSB(op.iHandle, sdBuffer);
            bres = op.picwinusbapi.Read_PicWinUSB(op.iHandle, rdBuffer);

            if (rdBuffer[0] > 0) //Si se recibe un valor mayor a 0
            {
                //AdcBar.Value = (int)(rdBuffer[0] * 100 / 254); //Dibujar en la barra
                if (x > 400)
                {
                    x = 0;
                    Osciloscopio.Dispose();
                    Osciloscopio = new Bitmap(401, 256);
                }
                Osciloscopio.SetPixel(x++, 255 - rdBuffer[0] / 2, Color.Red);
            }
            PicBoxOsiloscopio.Image = Osciloscopio;
        }

        private void timer_ADC2_Tick(object sender, EventArgs e)
        {
            bool bres;
            byte[] sdBuffer = new byte[3];           // Define send DataBuffer size
            byte[] rdBuffer = new byte[66];           // Define recieve DataBuffer size

            sdBuffer[0] = 0x0B;    //Modo ADC
            bres = op.picwinusbapi.Write_PicWinUSB(op.iHandle, sdBuffer);
            bres = op.picwinusbapi.Read_PicWinUSB(op.iHandle, rdBuffer);

            if (rdBuffer[0] > 0) //Si se recibe un valor mayor a 0
            {
                //AdcBar.Value = (int)(rdBuffer[0] * 100 / 254); //Dibujar en la barra
                if (x > 400)
                {
                    x = 0;
                    Osciloscopio.Dispose();
                    Osciloscopio = new Bitmap(401, 256);
                }
                Osciloscopio.SetPixel(x++, 255 - rdBuffer[0] / 2, Color.Red);
            }
            PicBoxOsiloscopio.Image = Osciloscopio;
        }

        private void timer_luxes_Tick(object sender, EventArgs e)
        {
            bool bres;
            byte[] sdBuffer = new byte[3];           // Define send DataBuffer size
            byte[] rdBuffer = new byte[1];           // Define recieve DataBuffer size

            sdBuffer[0] = 0x06;    //Modo ADC
            bres = op.picwinusbapi.Write_PicWinUSB(op.iHandle, sdBuffer);
            bres = op.picwinusbapi.Read_PicWinUSB(op.iHandle, rdBuffer);

            if (rdBuffer[0] > 0) //Si se recibe un valor mayor a 0
            {
                //AdcBar.Value = (int)(rdBuffer[0] * 100 / 254); //Dibujar en la barra
                if (x > 400)
                {
                    x = 0;
                    luxes.Dispose();
                    luxes = new Bitmap(401, 256);
                }
                luxes.SetPixel(x++, 255 - rdBuffer[0] / 2, Color.Red);
            }
            pictureBoxLuxes.Image = luxes;

        }

        private void timer_temperatura_Tick(object sender, EventArgs e)
        {
            bool bres;
            byte[] sdBuffer = new byte[3];           // Define send DataBuffer size
            byte[] rdBuffer = new byte[1];           // Define recieve DataBuffer size

            sdBuffer[0] = 0x05;    //Modo ADC
            bres = op.picwinusbapi.Write_PicWinUSB(op.iHandle, sdBuffer);
            bres = op.picwinusbapi.Read_PicWinUSB(op.iHandle, rdBuffer);

            if (rdBuffer[0] > 0) //Si se recibe un valor mayor a 0
            {
                //AdcBar.Value = (int)(rdBuffer[0] * 100 / 254); //Dibujar en la barra
                if (x > 400)
                {
                    x = 0;
                    temp.Dispose();
                    temp = new Bitmap(401, 256);
                }
                temp.SetPixel(x++, 255 - rdBuffer[0] / 2, Color.Red);
            }
            pictureBoxTemp.Image = temp;

        }

        private void timerSensor_Tick(object sender, EventArgs e)
        {
            bool bres;
            byte[] sdBuffer = new byte[3];
            byte[] rdBuffer = new byte[1];
            sdBuffer[0] = 0x04;
            bres = op.picwinusbapi.Write_PicWinUSB(op.iHandle, sdBuffer);
            bres = op.picwinusbapi.Read_PicWinUSB(op.iHandle, rdBuffer);
            if (rdBuffer[0] == 0)
            {
                pictureBoxSensor.Image = null;
            }
            else
            {
                pictureBoxSensor.Image.Save("Solution Items/persona.jpg");
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            timer_luxes.Start();
            timer_temperatura.Start();
            timerSensor.Start();
        }
    }
}
