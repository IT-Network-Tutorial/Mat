#include <stdio.h>
#include <dos.H>
#include <conio.H>

int klavesa;
int a[10][7] = { { 1, 1, 1, 1, 1, 1, 0 },//0
{ 0, 1, 1, 0, 0, 0, 0 },//1
{ 1, 1, 0, 1, 1, 0, 1 },//2
{ 1, 1, 1, 1, 0, 0, 1 },//3
{ 0, 0, 1, 0, 0, 1, 1 },//4
{ 1, 0, 1, 1, 0, 1, 1 },//5
{ 1, 0, 1, 1, 1, 1, 1 },//6
{ 1, 1, 1, 0, 0, 0, 0 },//7
{ 1, 1, 1, 1, 1, 1, 1 },//8
{ 1, 1, 1, 0, 0, 1, 1 } };//9
int r;//pomocná pro urèení øádku v 2d poli
int startovani = 0;//pomocná pro start displeje
int p = 0;//pomocná urèující poèet odeslaných èísel
void takt()//Takt pro posun v posuvném registru
{
	outportb(0x300, 0);
	outportb(0x300, 1);
}

void start()//Start displeje (data + takt)
{
	outportb(0x301, 1);//Data
	outportb(0x300, 0);//Takt
	outportb(0x300, 1);
}

void odeslani()//Odesílání pomocí 2d pole
{
	for (int i = 0; i <= 6; i++)//zaslání na 1 segment
	{
		outportb(0x301, a[r][i]);//r urèuje øádek, i sloupec
		takt();//takt pro posun v posuvném registru
	}
}
void indikace()//Pro výpis v pc + restart displeje
{
	clrscr();//vycisteni obrazovky
	printf("Pocet zapsanych cislic: %d",p);//Vypíše poèet zapsaných
}

int main()
{
	printf("Pro zobrazeni znaku na numerickem displeji stisknete 5 cisel (0-9).");
	while (1)
	{
		if (startovani == 0)
		{
			start();//Zaslání start bitu
			startovani = 1;
		}
		if (kbhit())//detekce stisknutého tlaèítka
		{
			klavesa = getch();
			switch (klavesa)
			{
			case 49://1
				p++;
				indikace();
				r = 1;//øádek 1 v 2d poli obsahuje hodnoty pro zobrazení 1
				odeslani();//odeslání hodnot na segment 
				break;
			case 50://2
				p++;
				indikace();
				r = 2;
				odeslani();
				break;
			case 51://3
				p++;
				indikace();
				r = 3;
				odeslani();
				break;
			case 52://4
				p++;
				indikace();
				r = 4;
				odeslani();
				break;
			case 53://5
				p++;
				indikace();
				r = 5;
				odeslani();
				break;
			case 54://6
				p++;
				indikace();
				r = 6;
				odeslani();
				break;
			case 55://7
				p++;
				indikace();
				r = 7;
				odeslani();
				break;
			case 56://8
				p++;
				indikace();
				r = 8;
				odeslani();
				break;
			case 57://9
				p++;
				indikace();
				r = 9;
				odeslani();
				break;
			case 48://0
				p++;
				indikace();
				r = 0;
				odeslani();
				break;
			}
		}
		if (p == 5)//Byly odeslány hodnoty na všech 5 segmentù
		{
			clrscr();//vycisteni obrazovky
			printf("Hodnoty byly odeslany, pro dalsi volbu stisknete 5 klaves.");
			p = 0;//nulování poètu zapsaných
			startovani = 0;//Pro další zápis je tøeba start bit
		}
	}
}