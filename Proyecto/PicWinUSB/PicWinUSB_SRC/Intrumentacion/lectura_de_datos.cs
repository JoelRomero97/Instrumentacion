namespace Intrumentacion
{
    public  class lectura_de_datos
    {
        public double numero_1 { get; set; }
        public double numero_2 { get; set; }
        public bool led_verde { get; set; }
        public bool led_rojo { get; set; }

        public lectura_de_datos(double numero1, double numero2)
        {
            this.numero_1 = numero1;
            this.numero_2 = numero2;
        }

        public lectura_de_datos()
        {

        }
    }
}