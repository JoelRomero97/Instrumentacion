#include <stdio.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////////////
////								ColaEstatica.c 									////
////																				////
////																				////
//// Implementación de cola estática sin utilizar apuntadores, únicamente usando 	////
//// índices para recorrer los elementos "viejos" e insertar los nuevos, que servi-	////
//// rá como muestreo para las lecturas del PIC18F2550 y reconstruir correctamente	////
//// las señales de los canales AN0 y AN1.											////
////																				////
////																				////
//// Autor: Romero Gamarra Joel Mauricio											////
////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[])
{
	char cola[5];
	int opc, i = 0, j, k;
	while (opc != 2)
	{
		system ("cls");
		if (i < 5)							//La cola no está llena
		{
			printf ("Ingrese un caracter:\t");
			fflush(stdin);
			scanf ("%c", &cola[i++]);
		}else								//La cola está llena
		{
			i = 4;k=0;
			for (j = 1; j < 5; j++)
			{
				cola[k++] = cola[j];
			}
			printf ("Ingrese un caracter:\t");
			fflush(stdin);
			scanf ("%c", &cola[i++]);
		}
		printf("%cDesea encolar otro?\n1.Si\t\t2.No\n\n", 168);
		fflush(stdin);
		scanf ("%d",&opc);
	}
	cola[i] = '\0';
	printf("La cola hasta el momento es:\t%s",cola);
	printf("\n\n\n");
	return 0;
}