#include <stdio.h>
#include <dos.H>
#include <conio.H>

int patro;
int stav;
int port300;
int port301;
int spusteni = 0;//pomocn� prom�nn�
int mp1 = 0x7f;
int mp2 = 0xbf;
int mp3 = 0xdf;
int mp4 = 0xef;//masky pro �idla, p�ivol�vac� tla��tka na patrech
int mp[] = {0, 0x7f, 0xbf, 0xdf, 0xef };//5 pozic kv�li 0
int mds = 0xfe;//Maska pro dve�n� sp�na�
int mps = 0xfd;//Maska pro podlahov� sp�na�
int dvere = 0;//pomocn� prom�nn�
int ps = 0;//pomocn� prom�nn�
int mps0[] = { 0x7f, 0xbf, 0xdf, 0xef };//Masky pro v�echny tla��tka
int mps1[] = { 0xf7, 0xfb, 0xfd, 0xfe };//Masky pro tla��tka u kabinky
int cislo[] = { 0, 0xfc, 0xfa, 0xfe, 0xf9 };//Hodnoty pro segment (nula je pomocn�)
int stisk;
int pozadovane_patro=1;//Prvn� po�adovan� patro je v�choz� prvn�
int zakaz = 0;//pomocn� prom�nn� pro z�kaz zm�ny volby b�hem chodu
int zvuk = 0;//pomocn� pro zvukov� sign�l

void nahoru()
{
	clrscr();//vycisteni obrazovky
	outportb(0x300, 0x5f);//Aktivace motoru, sm�r nahoru, signalizace
	zakaz = 1;//Kdy� v�tah jede, je zak�z�no m�nit volbu patra
}

void dolu()
{
	clrscr();//vycisteni obrazovky
	outportb(0x300, 0x2f);//Aktivace motoru, sm�r dolu, signalizace
	zakaz = 1;//Kdy� v�tah jede, je zak�z�no m�nit volbu patra
}
void stop()
{
	outportb(0x300, 0xff);//Deaktivace v�eho
	zakaz = 0;//Kdy� v�tah stoj�, je povoleno m�nit volbu
}

void zz()
{
	clrscr();//vycisteni obrazovky
	outportb(0x300, 0xfe);//Zvukov� sign�l
	delay(100);//Zpo�d�n�, b�hem kter�ho bude aktiivn� sir�na
	outportb(0x300, 0xff);//Deaktivace v�eho
    printf("Jste na patre");//Indikace v pc
}
int main()
{
	while(stav!=mp1)//Dokud nen� v prvn�m pat�e
	{
	  port300 = inportb(0x300);//p�e�te hodnotu z portu
	  stav = port300 | mp1;//Maskuje
	  dolu();//Jede dolu
	}
   stop();//Po dosa�en� se zastav�
	while (1)
	{      
		for (int i = 0; i <= 4; i++)//Cyklus pro zji�t�n�, v jak�m pat�e se program nach�z�
		{
			port300 = inportb(0x300);//p�e�te hodnotu z portu pro zji�t��� patra
			stav = port300 | mp[i];//Maskov�n�
			if (stav == mp[i])//Pokud se maska rovn� hodnot�
			{
				patro = i;//Patro je rovno indexu
				outportb(0x301, cislo[i]);//Po�le na port hodnotu pro zobrazen� akt. patra na segmentu
			}
		}
		port300 = inportb(0x300);//p�e�te hodnotu pro zji�t�n� stavu dve�n�ho sp�na�e
		stav = port300 | mds;//Maskov�n� (dve�n� sp�na�)
		if (stav == 0xfe) dvere = 1;//Pokud doch�z� ke shod�, dve�e jsou zav�eny
		else dvere = 0;
		if (dvere == 1)//Pokud jsou zav�eny, program pokra�uje
		{
			port300 = inportb(0x300);//p�e�te hodnotu z portu
			stav = port300 | mps;//Maskov�n�
			if (stav == 0xfd) ps = 1;//Podlahov� sp�na� aktivn�
			else ps = 0;//Podlahov� sp�na� neaktivn�
			if (zakaz == 0)//Stisk se m��e vyhodnotit pouze v p��pad�, �e v�tah stoj�
			{
				switch (ps) //Pro vyhodnocen� stisku
				{
				case 0: //Podlahov� sp�na� neaktivn�
					for (int z = 0; z <= 3; z++)//Smy�ka pro kontrolu stisku
					{
						port301 = inportb(0x301);//p�e�te hodnotu z portu
						stav = port301 | mps0[z];//Maskuje
						if (stav == mps0[z])//Pokud se maska rovn� hodnot�
						{
							stisk = z;//Patro je rovno indexu
							zvuk = 1;//Povolen� zvuku p�i dojezdu
							pozadovane_patro = z+1;//po�adovan� patro je rovno indexu +1 (velikost pole)
						}
					}
					break;
				case 1://Podlahov� sp�na� aktivn�
					for (int j = 0; j <= 3; j++)//Smy�ka pro kontrolu stisku
					{
						port301 = inportb(0x301);//p�e�te hodnotu z portu
						stav = port301 | mps1[j];//Maskuje
						if (stav == mps1[j])//Pokud se maska rovn� hodnot�
						{
							stisk = j;//Patro je rovno indexu
							zvuk = 1;//Povolen� zvuku p�i dojezdu
							pozadovane_patro = j+1;//po�adovan� patro je rovno indexu +1 (velikost pole)
						}
					}
					break;
				}
			}

			if (pozadovane_patro>patro)//Pokud je zvolen� patro v�t�� ne� aktu�ln�
			{
				nahoru();//sm�r nahoru
			}
			else if (pozadovane_patro < patro)//Pokud je po�adovan� men�� ne� aktu�ln�
			{
				dolu();//sm�r dol�
			}
			else 
			{
				stop();//Zastaven� po dojet� na patro
			}
			
			if ((zvuk == 1) && (pozadovane_patro==patro))//Pokud je povolen zvuk a z�rove� jste dojeli na patro
			{
				zz();//Aktivuje se zvukov� sign�l
				zvuk = 0;//Deaktivace zvuku
			}
		}
		else stop();//Pokud dve�e nejsou zav�eny, v�tah se zastav�
	}
}

