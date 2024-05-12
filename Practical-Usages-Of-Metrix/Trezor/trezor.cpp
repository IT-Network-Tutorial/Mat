#include <stdio.h>
#include <dos.h>
#define PORT1 0x300
#define PORT2 0x301
//tlacitka a displej
int pos1 = 0xFE;
int pos2 = 0xFD;
int pos3 = 0xFB;
int pos4 = 0xF7;

int pozice = 0;
//zvolene cisla
int num1 = 0;
int num2 = 0;
int num3 = 0;
int num4 = 0;
//heslo
int pas1 = 0;
int pas2 = 0;
int pas3 = 0;
int pas4 = 0;

int mask_door = 0x01;
int mask_klav = 0x02;
int input;
int stat = 0;
//pole {zavrena dvirka, tl. ^, tl. v, tl. set, tl. mode}
int hrana [5] = {2, 2, 2, 2, 2};
//funkce pro zobrazeni na dispeji
int light (int pos, int num)
{
outportb (PORT1,pos);
outportb (PORT2,num);
delay (5);
return 0;
}
int show (int numb1, int numb2, int numb3, int numb4)
{
light (pos1, numb1);
light (pos2, numb2);
light (pos3, numb3);
light (pos4, numb4);
return 0;
}

int main (void)
{		//hlavni smycka
	while (1)
	{
	switch (stat)
	{
		case 0:
		input = inportb(PORT1);
		input = input & mask_door;
		if(input == 0) //dvirka zavrena
		{
		if (input != hrana[0])
		{printf("Zadejte kod \n");}
        stat = 1;
		}
		else
        {
		if (input != hrana[0])
		{printf ("Pro zmenu hesla stiskni mode \n");}
		stat = 2;
		}
		hrana[0] = input;
		show(num1, num2, num3, num4);
		break;

		case 1:
		outportb (PORT1, pos1);		
		input = inportb (PORT1);
		input = input & mask_klav;
		if (input == 0)			//kontrola stisku tlacitka mode
		{
			if (input != hrana[4])
			{
			printf ("Heslo vymazano \n"); //stisknuto = nuluj displej
			 num1 = 0;
			 num2 = 0;
			 num3 = 0;
			 num4 = 0;
			 pozice = 0;
			}
		}
		hrana[4]=input;
		stat = 3;
		break;

		case 2:
		outportb (PORT1, pos1);
		input = inportb (PORT1);
		input = input & mask_klav;
		if (input == 0)		//kontrola stisku tlacitka mode
		{		//stisknuto = volba noveho hesla 
		 printf ("Urcete nove heslo \n");
		 stat = 6;
		}
		else
		{stat = 0;}
		break;

		case 3:					//zvysovani hodnoty
		outportb(PORT1, pos2);
		input = inportb(PORT1);
		input = input & mask_klav;
		if (input == 0)		//kontrola stisku tlacitka ^ 
		{
			if (input != hrana[1])
			{	
				if (pozice == 0)
				{
				num1 = num1 + 1;
					if (num1 == 10)
					{num1 = 0;}
				}
				else if (pozice == 1)
				{
				num2 = num2 + 1;
					if (num2 == 10)
					{num2 = 0;}
				}
				else if (pozice == 2)
				{
				num3 = num3 + 1;
					if (num3 == 10)
					{num3 = 0;}
				}
				else if (pozice == 3)
				{
				num4 = num4 + 1;
					if (num4 == 10)
					{num4 = 0;}
				}
			}
		}
		hrana[1]=input;
		stat = 4;
		break;

		case 4:					//snizovani hodnoty
		outportb(PORT1, pos3);
		input = inportb(PORT1);
		input = input & mask_klav;
		if (input == 0)		//kontrola stisku tlacitka v
		{
			if (input != hrana[2])
			{	
				if (pozice == 0)
				{
					if (num1 == 0)
					{num1 = 9;}
					num1 = num1 - 1;
				}
				else if (pozice == 1)
				{
					if (num2 == 0)
					{num2 = 9;}
					num2 = num2 - 1;
				}
				else if (pozice == 2)
				{
					if (num3 == 0)
					{num3 = 9;}
					num3 = num3 - 1;
				}
				else if (pozice == 3)
				{
					if (num4 == 0)
					{num4 = 9;}
					num4 = num4 - 1;
				}
			}
		}
		hrana[2]=input;
		stat = 5;
		break;

		case 5:			//potvrzeni hesla
		outportb(PORT1, pos4);	
		input = inportb(PORT1);
		input = input & mask_klav;
		if (input == 0)	//kontrola stisku set
		{
			if (input != hrana[3])
			{
				pozice = pozice + 1;
				if (pozice == 4)
				{
					if (num1 == pas1 && num2 == pas2 && num3 == pas3 && num4 == pas4)
					{
					outportb(PORT1, 0x7F); //otzevreni zamku
					printf("Trezor otevøen \n");
					}
					else
					{printf("Zadali jste spatne heslo \n");}
				pozice = 0;
				}
			}
		}
		hrana[3]=input;
		stat = 0;
		break;

		case 6:		//zvysovani hodnoty
		outportb(PORT1, pos2);
		input = inportb(PORT1);
		input = input & mask_klav;
		if (input == 0)	//kontrola stisku ^
		{
			if (input != hrana[1])
			{	
				if (pozice == 0)
				{
				pas1 = pas1 + 1;
					if (pas1 == 10)
					{pas1 = 0;}
				}
				else if (pozice == 1)
				{
				pas2 = pas2 + 1;
					if (pas2 == 10)
					{pas2 = 0;}
				}
				else if (pozice == 2)
				{
				pas3 = pas3 + 1;
					if (pas3 == 10)
					{pas3 = 0;}
				}
				else if (pozice == 3)
				{
				pas4 = pas4 + 1;
					if (pas4 == 10)
					{pas4 = 0;}
				}
			}
		}
		hrana[1]=input;
		show(pas1, pas2, pas3, pas4);
		stat = 7;
		break;

		case 7:		//snizovani hodnoty
		outportb(PORT1, pos3);
		input = inportb(PORT1);
		input = input & mask_klav;
		if (input == 0)	//kontrola stisku v
		{
			if (input != hrana[2])
			{
				if (pozice == 0)
				{
					if (pas1 == 0)
					{pas1 = 10;}
					pas1 = pas1 - 1;
				}
				else if (pozice == 1)
				{
					if (pas2 == 0)
					{pas2 = 10;}
					pas2 = pas2 - 1;
				}
				else if (pozice == 2)
				{
					if (pas3 == 0)
					{pas3 = 10;}
					pas3 = pas3 - 1;
				}
				else if (pozice == 3)
				{
					if (pas4 == 0)
					{pas4 = 10;}
					pas4 = pas4 - 1;
				}
			}
		}
		hrana[2]-input;
		stat = 8;
		break;

		case 8:		//zvoleni noveho hesla
		outportb(PORT1, pos4);
		input = inportb(PORT1);
		input = input & mask_klav;
		if (input == 0) //kontrola stisku set
		{
			if (input != hrana[3])
			{
				pozice = pozice + 1;
				if (pozice == 4)
				{
					pozice = 0;
					printf("Nove heslo je: %d %d %d %d \n", pas1, pas2, pas3, pas4);
					stat = 0;
				}
			}
		}
		else
		{stat = 6;}
		hrana[3]=input;
		break;
	}
	}
}
