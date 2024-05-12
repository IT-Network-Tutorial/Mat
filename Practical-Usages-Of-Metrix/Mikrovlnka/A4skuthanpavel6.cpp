#include<stdio.h>              //vyber znaku 1-4 , motor roztocit 19, dvere 20 , svetlo 16
#include<dos.h>                //pozice 8-11 , zvukovy signal 15 , GND 21-24
#include<conio.h>              //pojistka dvere 7 , snimaci teploty 14 , tlacitko 12
#include<string.h>

#define inPortA 0x300
#define outPortA 0x300
#define outPortB 0x301
//Jednotlivé body v postupu
int postup = 0;
//Hodnoty pro rozsvícení segmentů
int segment1 = 0x7f;
int segment2 = 0xbf;
int segment3 = 0xdf;
int segment4 = 0xef;

//Hodnoty, které se budou zobrazovat na segmentech 1-4
int znak1 = 0;
int znak2 = 0;
int znak3 = 0;
int znak4 = 0;

//Masky:
int maska_tlc = 0x80;	//Maska pro tlačítko                                 10000000
int maska_dvere = 0xf7;	//Maska pro dveře                                    11110111
int maska_talir = 0xff;	//Maska pro talíř                                    11111111
int maska_zarovka = 0xff;//Maska pro talíř                                   11111111


//Proměnné:
int tlc;				//Pro vyhodnocení stisku tlačítka
int port;
int dvere;			//Pro vyhodnocení stavu dveří
int stisknute_tlacitko;

//Pole s hodnotami pro zobrazení na displej
int cisla[10] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 }; //Pole čísel
int zapnuto[4] = { 0x02, 0x0a, 0x16, 0x15 }; //Pro zobrazení "zapnuto" na displeji (zapn)
int vypnuto[4] = { 0x19, 0x1a, 0x16, 0x15 }; //Pro zobrazení "vypnuto" na displeji (vypn)
int otevreno[4] = { 0x00, 0x18, 0x0e, 0x19 };	//Pro zobrazení ,,otevřeno" na displeji (otev)
int cas[4] = { 0x0c, 0x0a, 0x05, 0x1d };	//Pro zobrazení ,,čas" na displeji (cas_)

//Žárovka
void svetlozap(void)
{
	port &= (0x80 ^ maska_zarovka);
	outportb(outPortB, port);
}

void svetlovyp(void)
{
	port = 0x80;
	outportb(outPortA, port);
}

int main()
{
	while (1) //Nekonečná smyčka
	{

		//*******Svícení displeje
		outportb(outPortB, segment1);	//Aktivace 1. segmentu
		port = inportb(outPortB);	//Přečtení hodnoty z portu
		tlc = port & maska_tlc;		//Maskování (zjistím, zda nebyl bit 1 v hodnotě 0 tím, že všechny bity až na jeden, který potřebuji, vynásobím 0
		if (tlc == 0) stisknute_tlacitko = 1; //Pokud je číslo rovno 0, bylo detekováno stisknutí tlačítka (1=MODE)
		outportb(outPortA, znak1);	//Rozsvícený znak
		delay(1);			//1ms zpoždění

		outportb(outPortB, segment2);	//Aktivace 2. segmentu
		port = inportb(outPortB);
		tlc = port & maska_tlc;
		if (tlc == 0) stisknute_tlacitko = 2; //2=šipka nahoru
		outportb(outPortA, znak2);	//Rozsvícený znak
		delay(1);

		outportb(outPortB, segment3);	//Aktivace 3. segmentu
		port = inportb(outPortB);
		tlc = port & maska_tlc;
		if (tlc == 0) stisknute_tlacitko = 3; //3=šipka dolu
		outportb(outPortA, znak3);     //Rozsvícený znak
		delay(1);

		outportb(outPortB, segment4);	//Aktivace 4. segmentu
		port = inportb(outPortB);
		tlc = port & maska_tlc;
		if (tlc == 0) stisknute_tlacitko = 4; //4=SET
		outportb(outPortA, znak4);       //Rozsvícený znak
		delay(1);

		switch (postup)          //Obsluha mikrovlnky
		{
		case 0:

			printf("Stisknete MODE pro nastaveni casu nebo SET pro otevření dveří."); //Výpis v pc
			postup++; //Tento bod slouží jen pro výpis pokynu
			break;

		case 1:
			znak1 = zapnuto[0];
			znak2 = zapnuto[1];
			znak3 = zapnuto[2];
			znak4 = zapnuto[3];    //znaky, které na displeji vypíší ,,zapn"
			if (stisknute_tlacitko == 4) //pokud bylo stisknuto ,,set" otevřou se dveře
			{
				segment1 = segment1 & maska_dvere; //Bitový součin- jak segment tak dveře jsou aktivní v nule, tudíž vynásobím danou proměnnou jedničkami a nulou na bitu, který chci změnit na nulu
				stisknute_tlacitko = 0; //Nulování vyhodnocení stisku tlačítka
				printf("Dvere otevreny, je nutne je zavrít pro dalsi krok."); //Výpis stavu
				postup++;						//Posunutí se na další bod (case)
			}
			else if (stisknute_tlacitko == 1)
			{
				postup++; //Pokud bylo stisknuto ,,mode", uživatel nechtěl otevřít dveře a pokračuje dále
				stisknute_tlacitko = 0; //Nulování vyhodnocení stisku tlačítka
			}
			break;

		case 2:
			if (dvere == 0)
			{
				printf("Dvere jsou zavreny");
				postup++;			//Posun na další bod
			}
			else
			{
				znak1 = otevreno[0];
				znak2 = otevreno[1];
				znak3 = otevreno[2];
				znak4 = otevreno[3];	//Znaky pro zobrazení ,,otev" na displeji
			}
			break;
		case 3:
			printf("Stisknete SET pro nastaveni casu."); //Pouze výpis v pc
			postup++; //přesunutí na další bod
			break;

		case 4:
			znak1 = cas[0];
			znak2 = cas[1];
			znak3 = cas[2];
			znak4 = cas[3];	//Znaky pro zobrazení ,,cas_" na displeji

			if (stisknute_tlacitko == 4) //SET
			{
				znak1 = cisla[1];
				znak2 = cisla[1];
				znak3 = cisla[1];
				znak4 = cisla[1];//0000 na displeji
				postup++;
				stisknute_tlacitko = 0;		//Nulování pomocné proměnné
			}
			break;

		case 5:
			printf("Stisknete MODE pro zapnuti ohrevu a SET pro nezapnutí ohřevu."); //Pouze výpis v pc
			postup++; //přesunutí na další bod
			break;

		case 6:
			if (stisknute_tlacitko == 1)    //Pokud bylo stisknuto ,,mode", uživatel spustil ohřev
			{
				znak1 = zapnuto[0];
				znak2 = zapnuto[1];
				znak3 = zapnuto[2];
				znak4 = zapnuto[3];    //znaky, které na displeji vypíší ,,zapn"
				portA &= (0x40 ^ maska_talir);
				outportb(outPortA, portA);
				printf("Talir se toci");
				postup++; //Pokud bylo stisknuto ,,mode", uživatel spustil ohřev
				stisknute_tlacitko = 0; //Nulování vyhodnocení stisku tlačítka

			}

			else if (stisknute_tlacitko == 4)
			{
				postup++; //Pokud bylo stisknuto ,,set", uživatel nechtěl spustit ohřev
				stisknute_tlacitko = 0; //Nulování vyhodnocení stisku tlačítka
			}

		case 7:
			printf("Stisknete SET pro vypnuti ohrevu."); //Pouze výpis v pc
			postup++; //přesunutí na další bod
			break;

		case 8:
			if (stisknute_tlacitko == 4)    //Pokud bylo stisknuto ,,set", uživatel vypnul ohřev
			{
				znak1 = vypnuto[0];
				znak2 = vypnuto[1];
				znak3 = vypnuto[2];
				znak4 = vypnuto[3];    //znaky, které na displeji vypíší ,,zapn"
				portA = 0x40;
				outportb(outPortA, portA);
				printf("Talir se netoci");
				postup++; //Pokud bylo stisknuto ,,set", uživatel nechtěl spustit ohřev
				stisknute_tlacitko = 0; //Nulování vyhodnocení stisku tlačítka
				break;
			}
		}
	}
}