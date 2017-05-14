#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float BinarioDecimal (char * x);
float mediaAritmetica(float * numeros, int n);
void desviacion(float media,float * numeros,int n);
void Varianza (float d_estandar);
void ErrorProbable (float d_estandar);

////////////////////////////////////////////////////////////////////////////////////////
////							AnalisisEstadistico.c 								////
////																				////
////																				////
//// Programa que a partir de una serie de mediciones dadas por el usuario, calcula	////
//// la media aritmética, el error probable, la desviación estándar y la varianza, 	////
//// además, es capaz de convertir de binario a decimal en caso de requerirlo.		////
////																				////
////																				////
//// Autor: Romero Gamarra Joel Mauricio											////
////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char const *argv[])
{
	float media, *numeros;
	int n, i = 1, j = 0, sistema;
	char * numeroBinario = (char *) malloc (sizeof (char *));				//Creamos un arreglo dinámico ya que no sabemos el numero de bits
	printf("\n\n%cCuantos numeros va a ingresar?\t", 168);
	scanf("%d",&n);
	numeros = (float *) malloc (sizeof (float *) * n);						//Creamos el arreglo donde guardaremos los numeros
	system("cls");
	printf("%cEn que sistema los va a ingresar?\n", 168);
	printf("\n\n1.Decimal\t\t2.Binario\n\n");
	scanf ("%d", &sistema);
	if (sistema != 1 && sistema != 2)
	{
		printf("Opcion invalida.\n");
		return 0;															//Salimos del programa si la opcion no es correcta
	}
	while(i != (n+1))
	{
		system ("cls");
		printf("\nIngrese el numero %d:\t", i++);
		if (sistema == 1)
		{
			scanf("%f", &numeros[j++]);										//Guardamos los numeros ingresados en el arreglo
		}else 
		{
			scanf ("%s", numeroBinario);
			numeros [j++] = BinarioDecimal (numeroBinario);					//Convertimos a decimal y lo guardamos en el arreglo
		}
	}
	numeros[j] = '\0';														//Insertamos nulo para saber el final de la cadena
	system("cls");
	media = mediaAritmetica (numeros, n);									//Sumamos todos los numeros y los dividimos entre n
	desviacion (media, numeros, n);											//Calculamos desviacion estandar, promedio y desviacion de la media de mediciones
	return 0;
}

float BinarioDecimal (char * x)
{
	float numeroDecimal = 0;
	int flag = 0;
	char *pt = x, *ptr = x;
	for (; *pt != '\0'; pt++); pt--;								//Mandamos un apuntador al final de la cadena, o a la posicion 2^0
	for (; pt >= ptr; pt--)
	{
		if (*pt == '1')
		{
			numeroDecimal += (float ) pow (2, flag);				//Si encuentra un 1, sumamos a la variable 2^flag
		}
		flag++;														//Cada que se mueva a la izquierda, sumamos la potencia del 2
	}
	return numeroDecimal;
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
	media = suma / n;
	printf("\n\nMEDIA ARITMETICA:\t%f", media);
	return media;
}

void desviacion(float media, float * numeros, int n)
{
	float d[n], suma=0, d_prom, suma2 = 0, d_estandar;
	int i = 0, j = 1;
	float *pt = numeros;
	float *ptr;
	while(*pt != '\0')
		d[i++] = (*pt++) - media;												//Calculamos la desviación de cada lectura recibida (medida - mediaDeMediciones)
	d[i] = '\0';
	ptr = d;																	//Apuntador al inicio de las desviaciones para imprimirlas
	printf("\n\nDESVIACI%cN DE LA MEDIA", 224);
	while(*ptr != '\0')
	{
		if(*ptr < 0)
		{
			(*ptr) *= -1;														//Cambiamos el signo a positivo en caso de resultar negativo
		}
		printf("\nd%d:\t%f", j++, *ptr);										//Imprimimos cada una de las desviaciones de la media
		suma += (*ptr);															//Sumamos todas las desviaciones
		suma2 += ((*ptr)*(*ptr));												//Sumamos todas las desviaciones al cuadrado para la desviación estándar
		ptr++;
	}
	d_prom = suma/n;															//Calculamos la desviación promedio
	printf("\n\nDESVIACI%cN PROMEDIO:\t%f", 224, d_prom);
	d_estandar = sqrt(suma2/(n-1));												//Calculamos la desviación estándar
	printf("\n\nDESVIACI%cN ESTANDAR:\t%f", 224, d_estandar);
	Varianza (d_estandar);														//Calculamos la varianza
	ErrorProbable (d_estandar);													//Calculamos el error probable
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