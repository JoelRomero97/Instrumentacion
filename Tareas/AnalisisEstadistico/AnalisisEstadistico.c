#include <stdio.h>
#include <stdlib.h>
#define TAM 50

float mediaAritmetica(float * numeros, int n);
void desviacion(float media,float * numeros,int n);
void Varianza (float d_estandar);
void ErrorProbable (float d_estandar);

////////////////////////////////////////////////////////////////////////////////////////
////							AnalisisEstadistico.c 								////
////																				////
////																				////
//// Programa que a partir de una serie de mediciones dadas por el usuario, calcula	////
//// la media aritmética, el error probable, la desviación estándar y la varianza.	////
////																				////
////																				////
//// Autor: Romero Gamarra Joel Mauricio											////
////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[])
{
	float media, numeros[TAM];
	int n, i = 1, j = 0;
	printf("\n\n%cCuantos numeros va a ingresar?\t",168);
	scanf("%d",&n);
	system("cls");
	while(i != (n+1))
	{
		system ("cls");
		printf("\nIngrese el numero %d:\t", i++);
		scanf("%f", &numeros[j++]);
	}
	numeros[j] = '\0';
	system("cls");
	media = mediaAritmetica (numeros, n);
	desviacion (media, numeros, n);
	return 0;
}

float mediaAritmetica(float * numeros, int n)
{
	float *pt = numeros;
	float media, suma = 0;
	while (*pt != '\0')
	{
		suma += (*pt);
		pt++;
	}
	printf("\n\nSUMA:\t%f", suma);
	media = suma / n;
	printf("\n\nMEDIA ARITMETICA:\t%f", media);
	return media;
}

void desviacion(float media, float * numeros, int n)
{
	float d[TAM], suma=0, d_prom, suma2 = 0, d_estandar;
	int i = 0, j = 1;
	float *pt = numeros;
	float *ptr;
	while(*pt != '\0')
	{
		d[i++] = *pt - media;				//Calculamos la desviación de cada lectura recibida
		pt++;
	}
	d[i] = '\0';
	ptr = d;
	printf("\n\nDESVIACI%cN DE LA MEDIA", 224);
	while(*ptr != '\0')
	{
		if(*ptr < 0)
		{
			(*ptr) *= -1;							//Cambiamos el signo a positivo en caso de resultar negativo
		}
		printf("\nd%d:\t%f", j++, *ptr);
		suma += (*ptr);
		suma2 += ((*ptr)*(*ptr));
		ptr++;
	}
	d_prom = suma/n;
	printf("\n\nDESVIACI%cN PROMEDIO:\t%f", 224, d_prom);
	d_estandar = suma2/(n-1);
	printf("\n\nDESVIACI%cN ESTANDAR:\t%f", 224, d_estandar);
	Varianza (d_estandar);										//Calculamos la varianza
	ErrorProbable (d_estandar);									//Calculamos el error probable
	return;
}

void Varianza (float d_estandar)
{
	float varianza;
	varianza = d_estandar * d_estandar;
	printf("\n\nVARIANZA:\t%f\n\n", varianza);
	return;
}

void ErrorProbable (float d_estandar)
{
	float error;
	error = (0.6745) * d_estandar;
	printf("ERROR PROBABLE:\t%f\n", error);
	return;
}