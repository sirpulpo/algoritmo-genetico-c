#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
# define Tpob 3 // -> 4 cromosomas.
# define Tcro 6 // -> 2^(6), 7 genes.

void f_FirstPobl(int [Tpob][Tcro]);
void f_ValorN(int [Tpob][Tcro], int [Tpob]);
int f_Criterio(int [Tpob], double *);
void f_Fitness(int [Tpob], int [1]);
void f_Mutar(int [Tpob][Tcro], int [1]);
void f_again(int *);

int main()
{
	int again = 0, bndr = 0, epoch = 1, Mpob[Tpob][Tcro], Vvalor[Tpob], Vselct[1];
	double max = 0.0;
	do{
		f_FirstPobl(Mpob);
		epoch = 1;
		max = 0.0;
		do{
			printf("\n\n  ------ Generacion: %i ------", epoch);
			f_ValorN(Mpob, Vvalor);
			bndr = f_Criterio(Vvalor, &max);
			if(bndr == 0)// Si la bandera es activada el algoritmo termina.
			{
				f_Fitness(Vvalor, Vselct);
				f_Mutar(Mpob, Vselct);
				epoch++;
			}
		}while(bndr != 0);
		f_again(&again);
	}while(again == 1);
}

void f_FirstPobl(int Mpbln[Tpob][Tcro])
{
	int i=0, j=0;
	time_t t;
	srand((unsigned)time(&t)); //Aqui se inicia una semilla aleatoria para rand.
	for(i = 0; i <= Tpob; i++) //Esto genera la población.
	{
		for (j = 0; j <= Tcro; j++) //Esto genera un cromosoma.
		{
			Mpbln[i][j] = rand() % 2; //Nos garantiza que sea 0, 1.
		}
	}
	printf("\n --- Primera poblacion aleatoria ---");
	for(i = 0; i <= Tpob; i++)
	{
		printf("\nC%i - ",i+1);
		for (j = Tcro; j >= 0; j--)
		{
			printf(" %i ", Mpbln[i][j]);
		}
	}
	printf("\n");
}

void f_ValorN(int Mpob[Tpob][Tcro], int Vvalor[Tpob])
{
	int i=0, j=0, suma=0;
	for(i = 0; i <= Tpob; i++)
	{
		suma = 0;
		for (j = 0; j <= Tcro; j++)
		{
			if(Mpob[i][j] == 1)// Calculamos el valor numerico.
			{
				suma += pow(2, j);
			}
		}
		Vvalor[i] = suma;// Cargamos el valor al vector.
	}
	printf("\n\n --- Valores numericos de la poblacion ---");
	for(i = 0; i <= Tpob; i++)
	{
		printf("\nC%i - ",i+1);
		for (j = Tcro; j >= 0; j--)
		{
			printf(" %i ", Mpob[i][j]);
		}
		printf(" --> %i", Vvalor[i]);
	}	
}

int f_Criterio(int Vvalor[Tpob], double *max)
{
	int i = 0, bndr = 0;
	double aux = 0.0, Vcrit[Tpob];
	for(i = 0; i <= Tpob; i++)
	{
		Vcrit[i] = sin((double)Vvalor[i]); // Calcula la función para toda la población.
	}
	aux = *max;
	*max = Vcrit[0];
	for(i = 1; i <= Tpob; i++)
	{
		if(Vcrit[i] > *max) // Busca al maximo en la población.
			*max = Vcrit[i];
	}
	printf("\n\n --- Sin(x) ---\n |");
	for(i = 0; i <= Tpob; i++)
	{
		printf("  %.3f |",Vcrit[i]);
	}
	printf("\n El valor mas alto es: %.3f\n", *max);
	printf("\n El anterior es: %.3f\n", aux);
	// Si el valor anterior es mejor que el nuevo y se aproxima bastante a 1, entonces satisface. 
	if((*max > 0.97) && (aux > *max))
	{
		bndr = 1;
	}
	return bndr;
}

void f_Fitness(int Vvalor[Tpob], int Vselct[1])
{
	int i=0, j=0, aux1=0, aux2=0;
	double sum=0.0, Vporct[Tpob], Vfit[Tpob];
	for(i = 0; i <= Tpob; i++)
	{
		Vfit[i] = 50 * (sin((double)Vvalor[i]) + 1);//Se aplica funcion fitness.
	}
	printf("\n --- Fitness ---\n |");
	for(i = 0; i <= Tpob; i++)
	{
		printf("  %.3f |",Vfit[i]);
	}
	for(i = 0; i <= Tpob; i++)
	{
		sum += Vfit[i];
	}
	for(i = 0; i <= Tpob; i++)
	{
		Vporct[i] = Vfit[i] / sum * 100;
	}
	// Se calculan los porcentajes para identificar a los mejores cromosomas.
	printf("\n\n --- Sus porcentajes --- \n |");
	for(i = 0; i <= Tpob; i++)
	{
		printf("  %.3f |",Vporct[i]);
	}
	// Se determinan los mejores porcentajes.
	aux1 = 0;
	for(i = 2; i <= Tpob; i++)
	{
		if(Vporct[aux1] < Vporct[i])
			aux1 = i;
	}
	Vselct[0] = aux1;
	aux2 = 1;
	for(i = 0; i < Tpob; i++)
	{
		if((i != aux1) && (Vporct[aux2] < Vporct[i]))
			aux2 = i;
	}
	Vselct[1] = aux2;
	printf("\n\n Los 2 mas altos son:\t");
	for(i = 0; i <= 1; i++)
	{
		printf(" %i ",Vselct[i]+1);
		if(i != 1)
			printf("y");
	}
	printf("\n");
}

void f_Mutar(int Mpob[Tpob][Tcro], int Vselct[1])
{
	int Mselct[1][Tcro], i=0, j=0, k = 0, aux=0, aux2=0, cross=0;
	int Mnewpob[Tpob][Tcro]={0};
	time_t t;
	srand((unsigned)time(&t));
	for(i = 0; i <= 1; i++)
	{
		aux = Vselct[i];
		printf("\n C%i - ", aux+1);
		for(j = Tcro; j >= 0; j--)
		{
			printf(" %d ",Mpob[aux][j]);
		}
	}
	for(i = 0; i <= 1; i++)// Aquí se combinan los dos mejores cromosomas de la población actual.
	{
		cross = rand() % 6 + 1;// Esto nos da puntos de intercambio aleatorios.
		for(j = 0; j <= 1; j++)
		{
			aux = Vselct[0]; // a
			if(j == 1)
				aux = Vselct[1]; // b
			for(k = 0; k < cross; k++)
			{
				Mnewpob[(2*i) + j][k] = Mpob[aux][k];
			}
			aux = Vselct[1]; // b
			if(j == 1)
				aux = Vselct[0]; // a
			for(k = cross; k <= Tcro; k++)
			{
				Mnewpob[(2*i) + j][k] = Mpob[aux][k];
			}
		}
	}
	printf("\n\n --- La nueva generacion es: ---");
	for(i = 0; i <= Tpob; i++)
	{
		for(j = 0; j <= Tcro; j++)
		{
			Mpob[i][j] = Mnewpob[i][j];
		}
	}
	for(i = 0; i <= Tpob; i++)
	{
		printf("\n C%i - ",i+1);
		for(j = Tcro; j >= 0; j--)
		{
			printf(" %i ", Mpob[i][j]);
		}
	}
	printf("\n");
}

void f_again(int *again)
{
	do{
		printf("\n\n\nDesea repetir el algoritmo: ");
		printf("\n\n1) Si.");
		printf("\n\n2) No.");
		scanf("%d",again);
	}while(*again < 1 || *again > 2);
}
