#include <stdio.h>
#include <dos.H>
#include <conio.H>

int patro;
int stav;
int port300;
int port301;
int spusteni = 0;//pomocná promìnná
int mp1 = 0x7f;
int mp2 = 0xbf;
int mp3 = 0xdf;
int mp4 = 0xef;//masky pro èidla, pøivolávací tlaèítka na patrech
int mp[] = {0, 0x7f, 0xbf, 0xdf, 0xef };//5 pozic kvùli 0
int mds = 0xfe;//Maska pro dveøní spínaè
int mps = 0xfd;//Maska pro podlahový spínaè
int dvere = 0;//pomocná promìnná
int ps = 0;//pomocná promìnná
int mps0[] = { 0x7f, 0xbf, 0xdf, 0xef };//Masky pro všechny tlaèítka
int mps1[] = { 0xf7, 0xfb, 0xfd, 0xfe };//Masky pro tlaèítka u kabinky
int cislo[] = { 0, 0xfc, 0xfa, 0xfe, 0xf9 };//Hodnoty pro segment (nula je pomocná)
int stisk;
int pozadovane_patro=1;//První požadované patro je výchozí první
int zakaz = 0;//pomocná promìnná pro zákaz zmìny volby bìhem chodu
int zvuk = 0;//pomocná pro zvukový signál

void nahoru()
{
	clrscr();//vycisteni obrazovky
	outportb(0x300, 0x5f);//Aktivace motoru, smìr nahoru, signalizace
	zakaz = 1;//Když výtah jede, je zakázáno mìnit volbu patra
}

void dolu()
{
	clrscr();//vycisteni obrazovky
	outportb(0x300, 0x2f);//Aktivace motoru, smìr dolu, signalizace
	zakaz = 1;//Když výtah jede, je zakázáno mìnit volbu patra
}
void stop()
{
	outportb(0x300, 0xff);//Deaktivace všeho
	zakaz = 0;//Když výtah stojí, je povoleno mìnit volbu
}

void zz()
{
	clrscr();//vycisteni obrazovky
	outportb(0x300, 0xfe);//Zvukový signál
	delay(100);//Zpoždìní, bìhem kterého bude aktiivní siréna
	outportb(0x300, 0xff);//Deaktivace všeho
    printf("Jste na patre");//Indikace v pc
}
int main()
{
	while(stav!=mp1)//Dokud není v prvním patøe
	{
	  port300 = inportb(0x300);//pøeète hodnotu z portu
	  stav = port300 | mp1;//Maskuje
	  dolu();//Jede dolu
	}
   stop();//Po dosažení se zastaví
	while (1)
	{      
		for (int i = 0; i <= 4; i++)//Cyklus pro zjištìní, v jakém patøe se program nachází
		{
			port300 = inportb(0x300);//pøeète hodnotu z portu pro zjištìñí patra
			stav = port300 | mp[i];//Maskování
			if (stav == mp[i])//Pokud se maska rovná hodnotì
			{
				patro = i;//Patro je rovno indexu
				outportb(0x301, cislo[i]);//Pošle na port hodnotu pro zobrazení akt. patra na segmentu
			}
		}
		port300 = inportb(0x300);//pøeète hodnotu pro zjištìní stavu dveøního spínaèe
		stav = port300 | mds;//Maskování (dveøní spínaè)
		if (stav == 0xfe) dvere = 1;//Pokud dochází ke shodì, dveøe jsou zavøeny
		else dvere = 0;
		if (dvere == 1)//Pokud jsou zavøeny, program pokraèuje
		{
			port300 = inportb(0x300);//pøeète hodnotu z portu
			stav = port300 | mps;//Maskování
			if (stav == 0xfd) ps = 1;//Podlahový spínaè aktivní
			else ps = 0;//Podlahový spínaè neaktivní
			if (zakaz == 0)//Stisk se mùže vyhodnotit pouze v pøípadì, že výtah stojí
			{
				switch (ps) //Pro vyhodnocení stisku
				{
				case 0: //Podlahový spínaè neaktivní
					for (int z = 0; z <= 3; z++)//Smyèka pro kontrolu stisku
					{
						port301 = inportb(0x301);//pøeète hodnotu z portu
						stav = port301 | mps0[z];//Maskuje
						if (stav == mps0[z])//Pokud se maska rovná hodnotì
						{
							stisk = z;//Patro je rovno indexu
							zvuk = 1;//Povolení zvuku pøi dojezdu
							pozadovane_patro = z+1;//požadované patro je rovno indexu +1 (velikost pole)
						}
					}
					break;
				case 1://Podlahový spínaè aktivní
					for (int j = 0; j <= 3; j++)//Smyèka pro kontrolu stisku
					{
						port301 = inportb(0x301);//pøeète hodnotu z portu
						stav = port301 | mps1[j];//Maskuje
						if (stav == mps1[j])//Pokud se maska rovná hodnotì
						{
							stisk = j;//Patro je rovno indexu
							zvuk = 1;//Povolení zvuku pøi dojezdu
							pozadovane_patro = j+1;//požadované patro je rovno indexu +1 (velikost pole)
						}
					}
					break;
				}
			}

			if (pozadovane_patro>patro)//Pokud je zvolené patro vìtší než aktuální
			{
				nahoru();//smìr nahoru
			}
			else if (pozadovane_patro < patro)//Pokud je požadované menší než aktuální
			{
				dolu();//smìr dolù
			}
			else 
			{
				stop();//Zastavení po dojetí na patro
			}
			
			if ((zvuk == 1) && (pozadovane_patro==patro))//Pokud je povolen zvuk a zároveò jste dojeli na patro
			{
				zz();//Aktivuje se zvukový signál
				zvuk = 0;//Deaktivace zvuku
			}
		}
		else stop();//Pokud dveøe nejsou zavøeny, výtah se zastaví
	}
}

