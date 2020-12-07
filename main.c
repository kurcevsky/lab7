#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "mpi.h"
#define REZERWA 20
#define LADOWNOSC 25
#define CUMUJ 1
#define NIE_CUMUJ 0
#define PRZYSTAN 1
#define START 2
#define REJS 3
#define KONIEC_REJSU 4
#define KATASTROFA 5
#define PRZELADOWANIE 6
#define TANKUJ 50
int paliwo = 50;
int CUMUJ=1, NIE_CUMUJ=0;
int liczba_procesow;
int nr_procesu;
int ilosc_statkow;
int ilosc_pasazerow;
int ilosc_miejsc=4;
int ilosc_zajetych_miejsc=0;
int tag=1;
int wyslij[2];
int odbierz[2];
MPI_Status mpi_status;
void Wyslij(int nr_statku, int stan)
{
	wyslij[0]=nr_statku;
	wyslij[1]=stan;
	MPI_Send(&wyslij, 2, MPI_INT, 0, tag, MPI_COMM_WORLD);
	sleep(1);
}
void Przystan(int liczba_procesow)
{
	int nr_statku,status;
	ilosc_statkow = liczba_procesow - 1;
	if(rand()%2==1)
	{
		printf("Piekna pogoda, zapraszam na rejs!\n");
	}
	else
	{
		printf("SZTORM, PROSZÊ NIE WYCHODZIC NA ZEWNATRZ\n");
	}
	printf("Zyczymy Panstwu, przyjemnej podrozy \n \n \n");
	sleep(2);
	while(ilosc_miejsc<=ilosc_statkow)
	{
		MPI_Recv(&odbierz,2,MPI_INT,MPI_ANY_SOURCE,tag,MPI_COMM_WORLD, &mpi_status);
		nr_statku=odbierz[0];
		status=odbierz[1];
		if(status==1)
		{
			printf("Statek %d stoi na przystani, nie wyplynal w zaden rejs\n", nr_statku);
		}
		if(status==2)
		{
			printf("Statek %d prosi o pozwolenie na rejs. Miejsce w porcie:%d\n",nr_statku, ilosc_zajetych_miejsc);
			ilosc_zajetych_miejsc--;
		}
		if(status==3)
		{
			//printf("Samolot %d LATA\n", nr_samolotu);
			printf("Statek %d wyplynal w rejs\n", nr_statku);
		}
		if(status==4)
		{
			if(ilosc_zajetych_miejsc<ilosc_miejsc)
			{
				ilosc_zajetych_miejsc++;
				MPI_Send(&CUMUJ, 1, MPI_INT, nr_statku, tag, MPI_COMM_WORLD);
			}
			else
			{
				MPI_Send(&NIE_CUMUJ, 1, MPI_INT, nr_statku, tag, MPI_COMM_WORLD);
			}
		}
		if(status==5)
		{
			ilosc_statkow--;
			printf("Ilosc statkow %d\n", ilosc_statkow);
		}
	}
	printf("==================\n");
	printf("======KONIEC======\n");
	printf("==================\n");
}
void Statek()
{
	int stan,suma,i;
	stan=REJS;
	while(1)
	{
		if(stan==1)
		{
			if(rand()%2==1)
			{
				stan=START;
				paliwo=TANKUJ;
				ilosc_pasazerow=rand()%35;
				if(ilosc_pasazerow>=25)
				{
					stan=6;
				}
				printf("Statek %d prosi o pozwolenie na wyplyniecie\n", nr_procesu);
				Wyslij(nr_procesu,stan);
			}
		else
		{
			Wyslij(nr_procesu,stan);
		}
	}
	else if(stan==2)
	{
		printf("Statek %d wyruszl w rejs\n",nr_procesu);
		stan=REJS;
		Wyslij(nr_procesu,stan);
	}
	else if(stan==3)
	{
		paliwo-=rand()%5;
		if(paliwo<=REZERWA)
		{
		stan=KONIEC_REJSU;
		printf("prosze o pozwolenie na cumowanie\n");
		Wyslij(nr_procesu,stan);
		}
		else
		{
			for(i=0; rand()%100;i++);
		}
	}
	else if(stan==4)
	{
		int temp;
		MPI_Recv(&temp, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &mpi_status);
		if(temp==CUMUJ)
		{
			stan=PRZYSTAN;
			printf("Statek %d zacumowal\n", nr_procesu);
		}
		else
		{
			paliwo-=rand()%500;
			if(paliwo>0)
			{
				Wyslij(nr_procesu,stan);
			}
			else
			{
				stan=KATASTROFA;
				printf("rozbilem sie\n");
				Wyslij(nr_procesu,stan);
				return;
			}
		}
	}
	else if(stan==6)
	{
		//Nastapilo przeladowanie statku, nastapi wieksze spalanie!
		printf("Za duzo ludzi na statku! Spalanie wzroslo!");
		paliwo-=rand()%10;
		stan=2;
	}
}	
}
int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&nr_procesu);
	MPI_Comm_size(MPI_COMM_WORLD,&liczba_procesow);
	srand(time(NULL));
	if(nr_procesu == 0)
		Przystan(liczba_procesow);
	else
		Statek();
	MPI_Finalize();
return 0;
}
