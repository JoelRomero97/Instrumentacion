#include <stdio.h>
#include <stdlib.h>
#define TAM 5

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
	char cola[TAM], colaFinal[TAM];
	int opc, final, j = 0, inicio = 0, i, k = 0;
	while (opc != 2)
	{
		system ("cls");
		if (j < TAM)										//La cola no está llena
		{
			printf ("Ingrese un caracter:\t");
			fflush(stdin);
			scanf ("%c", &cola[j++]);
		}else												//La cola está llena
		{
			j = 0;inicio = (TAM + 1);
			printf ("Ingrese un caracter:\t");
			fflush(stdin);
			scanf ("%c", &cola[j++]);
		}
		printf("%cDesea encolar otro?\n1.Si\t\t2.No\n\n", 168);
		fflush(stdin);
		scanf ("%d",&opc);
	}
	if (j < TAM && inicio == (TAM + 1))
	{
		final = j - 1;
		inicio = j;
	}else if (j == TAM && inicio == (TAM + 1))
	{
		final = 4;
		inicio = 0;
	}else if (inicio == 0)
	{
		final = j - 1;
	}
	cola[TAM] = '\0';
	printf("La cola hasta el momento es:\t%s",cola);
	printf("\n\n\n");
	printf("Los indices son:\n\n1.INICIO:\t%d\n2.FINAL:\t%d", inicio, final);
	printf("\n\n\n");
	i = inicio;
	for (j = 0; j < TAM; j++)
	{
		colaFinal[j] = cola[i++];
		if (i == TAM)
			i = 0;
	}
	colaFinal[TAM] = '\0';
	printf("La cola despues de reacomodar de acuerdo a los indices es:\t%s\n", colaFinal);
	return 0;
}