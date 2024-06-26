#include <stdio.h>
#include <dos.H>
#include <conio.H>

int znak[26][15] = { { 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0 },//A
{ 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0 }, //B
{ 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//C
{ 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },//D
{ 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0 },//E
{ 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0 },//F
{ 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0 },//G
{ 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0 },//H
{ 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },//I
{ 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//J
{ 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0 },//K
{ 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//L
{ 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0 },//M
{ 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0 },//N
{ 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//O
{ 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0 },//P
{ 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0 },//Q
{ 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0 },//R
{ 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0 },//S
{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0 },//T
{ 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },//U
{ 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0 },//V
{ 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0 },//W
{ 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0 },//X
{ 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 },//Y
{ 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 } };//Z

int klavesa;
int zbytek[] = { 0, 0, 0 };//pole pro zbyl� t�i hodnoty posuvn�ho reg
int r = 0;//��dkov�n� v 2D poli
int a1 = 0;//p�ep�n�n�
int a2 = 0;
int p = 0;//po�et zapsan�ch
int seg1;
int seg2;
int seg3;
int seg4;//pomocn� prom�nn�
int povoleni = 0;
int s1;
int s2;
int s3;
int s4;

void takt()//Takt pro posun v posuvn�m registru
{
	outportb(0x300, 0);
	outportb(0x300, 1);//N�b�n� hrana
}

void start()//Start displeje (data + takt)
{
	outportb(0x301, 1);//Data
	outportb(0x300, 0);//Takt
	outportb(0x300, 1);
}

void odeslani1()//Odes�l�n� na prvn� a t�et� segment
{
	for (int i = 0; i <= 14; i++)//Zasl�n� na segment 1
	{
		outportb(0x301, znak[s1][i]);
		takt();//takt pro posun v posuvn�m registru
	}
	for (int k = 0; k <= 14; k++)//Zasl�n� na segment 3
	{
		outportb(0x301, znak[s3][k]);
		takt();//takt pro posun v posuvn�m registru
	}
	a1 = 1;//P�epnut� na 1. a 3.segement
	a2 = 0;
	outportb(0x301, a1);//Hodnoty se ode�lou v po�ad� 31 a 32
	takt();
	outportb(0x301, a2);
	takt();
}
void odeslani2()//Odes�l�n� na druh� a �vtrt� segment
{
	for (int l = 0; l <= 14; l++)//Zasl�n� hodnot na 2. segment
	{
		outportb(0x301, znak[s2][l]);
		takt();//takt pro posun v posuvn�m registru
	}
	for (int m = 0; m <= 14; m++)//Zasl�n� na 4. segment
	{
		outportb(0x301, znak[s4][m]);
		takt();//takt pro posun v posuvn�m registru
	}
	a1 = 0;//Sv�t� 2 a 4 segment
	a2 = 1;
	outportb(0x301, a1);
	takt();
	outportb(0x301, a2);
	takt();
}

void navic()
{
	for (int j = 0; j <= 2; j++)//Zbyl� 3 bity
	{
		outportb(0x301, zbytek[j]);
		takt();//takt pro posun v posuvn�m registru
	}
}

void indikace()//V�pis v pc
{
	clrscr();//vycisteni obrazovky
	printf("Pocet zapsanych cislic: %d", p);//Vyp�e po�et zapsan�ch
}

int main()
{
	printf("Pro zobrazeni znaku na alfanumerickem displeji stisknete 4 pismena");
	printf("Vyber pismen je:a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z.");
	while (1)
	{
		if (kbhit())//detekce stisknut�ho tla��tka
		{
			klavesa = getch();
			switch (klavesa)
			{
			case 97://a
				r = 0;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 98://b
				r = 1;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 99://c
				r = 2;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 100://d
				r = 3;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 101://e
				r = 4;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 102://f
				r = 5;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 103://g
				r = 6;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 104://h
				r = 7;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 105://i
				r = 8;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 106://j
				r = 9;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 107://k
				r = 10;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 108://l
				r = 11;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 109://m
				r = 12;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 110://n
				r = 13;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 111://o
				r = 14;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 112://p
				r = 15;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 113://q
				r = 16;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 114://r
				r = 17;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 115://s
				r = 18;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 116://t
				r = 19;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 117://u
				r = 20;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 118://v
				r = 21;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 119://w
				r = 22;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 120://x
				r = 23;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 121://y
				r = 24;//lokace v 2D poli
				p++;
				indikace();
				break;
			case 122://z
				r = 25;//lokace v 2D poli
				p++;
				indikace();
				break;
			}
		}
		switch (p)
		{
		case 1:
			seg1 = r;//Z�pis do pomocn� prom�nn�
			break;
		case 2:
			seg2 = r;
			break;
		case 3:
			seg3 = r;
			break;
		case 4:
			seg4 = r;
			break;
		}

		if (p == 4)//Maxim�ln� po�et znak�
		{
			s1 = seg1;
			s2 = seg2;
			s3 = seg3;
			s4 = seg4;//vysv�tleno v refer�tu
			p = 0;//Po�et je op�t roven 0
			povoleni = 1;//povoleno zobrazovat na displeji
			printf("Vase kombinace se zobrazuje na displeji. ");
			printf("Vyber pismen je:a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z.");
		}

		
		if (povoleni == 1)//Ochrana kv�li nulov�m hodnot�m p�i spu�t�n�
		{
			start();//Zasl�n� start bitu
			odeslani1();//Zasl�n� hodnot na 1. a 3. segment
			navic();//Zasl�n� zbyl�ch 3. bit�
			start();//Start bit
			odeslani2();//Hodnoty pro 2. a 4. segment
			navic();//Zbyl� 3. bity
		}
	}
}