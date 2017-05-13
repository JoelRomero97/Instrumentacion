#include <stdio.h>
#include <stdlib.h>
#define TAM 50

float mediaAritmetica(float arreglo[], int n);
void desviacion(float media,float arreglo [],int n);

int main()
{
	float x, media, arreglo[TAM];
	int n,i=1,j=0;
	printf("\n\n%cCuantos numeros va a ingresar?\t",168);
	scanf("%d",&n);
	system("cls");
	while(i!=(n+1))
	{
		printf("\nIngrese el numero %d:\t",i++);
		scanf("%f",&arreglo[j++]);
	}
	arreglo[j]='\0';
	system("cls");
	media = mediaAritmetica(arreglo,n);
	desviacion(media,arreglo,n);
}

float mediaAritmetica(float arreglo[], int n)
{
	float *pt = arreglo;
	float media, suma = 0;
	while(*pt!='\0')
	{
		suma+=(*pt);
		pt++;
	}
	printf("\n\nSUMA:\t%f",suma);
	media = suma/n;
	printf("\n\nMEDIA ARITMETICA:\t%f",media);
	return media;
}

void desviacion(float media,float arreglo [],int n)
{
	float d[TAM], suma=0, d_prom, suma2 = 0, d_estandar, varianza, error;
	int i = 0, j=1;
	float *pt = arreglo;
	float *ptr;
	while(*pt!='\0')
	{
		d[i++] = *pt - media;
		pt++;
	}
	d[i]='\0';
	ptr = d;
	printf("\n\nDESVIACI%cN DE LA MEDIA",224);		//DESVIACIÓN DE LA MEDIA
	while(*ptr!='\0')
	{
		if(*ptr<0)
		{
			(*ptr)*=-1;
		}
		printf("\nd%d:\t%f",j++,*ptr);
		suma+=(*ptr);
		suma2+=((*ptr)*(*ptr));
		ptr++;
	}
	d_prom = suma/n;
	printf("\n\nDESVIACI%cN PROMEDIO:\t%f",224,d_prom);		//DESVIACIÓN PROMEDIO
	d_estandar = suma2/(n-1);
	printf("\n\nDESVIACI%cN ESTANDAR:\t%f",224,d_estandar);		//DESVIACIÓN ESTANDAR
	varianza = d_estandar*d_estandar;
	printf("\n\nVARIANZA:\t%f",varianza);
	error = (0.6745)*(d_estandar);
	printf("\n\nERROR PROBABLE:\t%f",error);
}
