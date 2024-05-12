#include <stdio.h>
#include <conio.h>
#include <dos.h>

int klavesa;
int maska_smer1 = 0x02;
int maska_smer0 = 0xfd;//masky pro sm�r
int maska_otaceni = 0x7f;
int maska_hlrameno = 0xbf;
int maska_chapadlo = 0xdf;
int maska_ramenochapadlo = 0xef;//Masky pro motory
int maska_deaktivace = 0xff; //deaktivace v�ech motor�
int port1 = 0xff;//Hodnota k odesl�n� na port 301 
int port2 = 0xff;//Hodnota k odesl�n� na port 300
int takt1 = 0x80;//1
int takt2 = 0x0;//0
//Z�vory
int zavora_otaceni;
int zavora_ramenochap;
int zavora_chapadlo;
int zavora_hlrameno;//Konc�ky
int stav_zavor;
int ir_ot;
int ir_ramchap;
int ir_chap;
int ir_hlram;//Prom�nn� pro z�vory

//Takty motor�
int t_ot = 0;
int t_hlram = 0;
int t_chap = 0;
int t_ramchap = 0;


int main()
{
	//Najet� do v�choz� polohy
	while (zavora_otaceni != 1)//Dokud nen� z�vora aktivn�
	{
		stav_zavor = inportb(0x301);//Na�ten� do prom�nn�
		ir_ot = stav_zavor | maska_otaceni; //Masky mohu vyu��t stejn�
		if (ir_ot == 0x7f)//Pokud je na dan�m bitu 0
		{
			port1 = 0xff;
			port2 = 0xff;
			zavora_otaceni = 1;//Konc�k je aktivn�
		}
		else
		{
			port2 = port2 & maska_otaceni;//aktivace motoru
			port1 = port1 & maska_smer0;//Ot��en� vlevo
			zavora_otaceni = 0;//Pokud ne, nen� aktivn�
			//Takt
			outportb(0x301, takt1);
			delay(1);
			outportb(0x301, takt2);
			delay(1);
			//Odesl�n� na port
			outportb(0x301, port1);
			outportb(0x300, port2);//Odesl�n� hodnoty na port pro rozh�b�n� robota
		}
	}

	while (zavora_hlrameno != 1)//Dokud nen� z�vora aktivn�
	{
		stav_zavor = inportb(0x301);//Na�ten� do prom�nn�
		ir_hlram = stav_zavor | maska_hlrameno;
		if (ir_hlram == 0xbf)//Pokud je na dan�m bitu 0
		{
			port1 = 0xff;
			port2 = 0xff;
			zavora_hlrameno = 1;//Konc�k je aktivn�
		}
		else
		{
			port2 = port2 & maska_hlrameno;//aktivace hl. ramena
			port1 = port1 & maska_smer0;//sm�r nahoru
			zavora_hlrameno = 0;//Pokud ne, nen� aktivn�
			//Takt
			outportb(0x301, takt1);
			delay(1);
			outportb(0x301, takt2);
			delay(1);
			//Odesl�n� na port
			outportb(0x301, port1);
			outportb(0x300, port2);//Odesl�n� hodnoty na port pro rozh�b�n� robota
		}
	}

	while (zavora_chapadlo != 1)//Dokud nen� z�vora aktivn�
	{
		stav_zavor = inportb(0x301);//Na�ten� do prom�nn�
		ir_chap = stav_zavor | maska_chapadlo;
		if (ir_chap == 0xdf)//Pokud je na dan�m bitu 0
		{
			port1 = 0xff;
			port2 = 0xff;
			zavora_chapadlo = 1;//Konc�k je aktivn�
		}
		else
		{
			port2 = port2 & maska_chapadlo;//aktivace chapadla
			port1 = port1 & maska_smer0;//otev��t chapadlo
			zavora_chapadlo = 0;//Pokud ne, nen� aktivn�
			//Takt
			outportb(0x301, takt1);
			delay(1);
			outportb(0x301, takt2);
			delay(1);
			//Odesl�n� na port
			outportb(0x301, port1);
			outportb(0x300, port2);//Odesl�n� hodnoty na port pro rozh�b�n� robota
		}
	}

	while (zavora_ramenochap != 1)//Dokud nen� z�vora aktivn�
	{
		stav_zavor = inportb(0x301);//Na�ten� do prom�nn�
		ir_ramchap = stav_zavor | maska_ramenochapadlo;
		if (ir_ramchap == 0xef)//Pokud je na dan�m bitu 0
		{
			port1 = 0xff;
			port2 = 0xff;
			zavora_ramenochap = 1;//Konc�k je aktivn�
		}
		else
		{
			port2 = port2 & maska_ramenochapadlo;//aktivace pohybu chapadla
			port1 = port1 | maska_smer1;//sm�r nahoru
			zavora_ramenochap = 0;//Pokud ne, nen� aktivn�
			//Takt
			outportb(0x301, takt1);
			delay(1);
			outportb(0x301, takt2);
			delay(1);
			//Odesl�n� na port
			outportb(0x301, port1);
			outportb(0x300, port2);//Odesl�n� hodnoty na port pro rozh�b�n� robota
		}
	}

	while (1)//Nekone�� cyklus
	{
		printf("Otaceni zakladny pomoci klaves a,d.");
		printf("Pohyb hlavniho ramena pomoci klaves w,s.");
		printf("Pohyb ramena chapadla pomoci klaves m,n.");
		printf("Sevreni chapadla/rozevreni chapadla pomoci klaves f,g.");

		stav_zavor = inportb(0x301);//Na�ten� do prom�nn
		ir_ot = stav_zavor | maska_otaceni; //Masky mohu vyu��t stejn�
		if (ir_ot == 0x7f)//Pokud je na dan�m bitu 0
		{
			printf("Dosazeno mezni polohy, stisk a zakazan.");//Indikace v pc
			zavora_otaceni = 1;//Konc�k je aktivn�
			t_ot = 0;//nulov�n� po��t�n� takt�
		}
		else zavora_otaceni = 0;//Pokud ne, nen� aktivn�

		ir_hlram = stav_zavor | maska_hlrameno;
		if (ir_hlram == 0xbf)//Pokud je na dan�m bitu 0
		{
			printf("Dosazeno mezni polohy, stisk w zakazan.");//Indikace v pc
			zavora_hlrameno = 1;//Konc�k je aktivn�
			t_hlram = 0;//nulov�n� po��t�n� takt�
		}
		else zavora_hlrameno = 0;//Pokud ne, nen� aktivn�

		ir_chap = stav_zavor | maska_chapadlo;
		if (ir_chap == 0xdf)//Pokud je na dan�m bitu 0
		{
			printf("Dosazeno mezni polohy, stisk f zakazan.");//Indikace v pc
			zavora_chapadlo = 1;//Konc�k je aktivn�
			t_chap = 0;//nulov�n� po��t�n� takt�
		}
		else zavora_chapadlo = 0;//Pokud ne, nen� aktivn�

		ir_ramchap = stav_zavor | maska_ramenochapadlo;
		if (ir_ramchap == 0xef)//Pokud je na dan�m bitu 0
		{
			printf("Dosazeno mezni polohy, stisk m zakazan.");//Indikace v pc
			zavora_ramenochap = 1;//Konc�k je aktivn�
			t_ramchap = 0;//nulov�n� po��t�n� takt�
		}
		else zavora_ramenochap = 0;//Pokud ne, nen� aktivn�

		if (kbhit())//detekce stisknut�ho tla��tka
		{
			klavesa = getch(); //jeho zaps�n� do prom�nn�

			switch (klavesa)
			{
			case 97://a
				if (zavora_otaceni != 1)//Pokud z�vora nen� aktivn�
				{
					port2 = port2 & maska_otaceni;//aktivace motoru
					port1 = port1 & maska_smer0;//Ot��en� vlevo
					t_ot--;//Ode��t�n� takt�
				}
				else
				{
					port1 = 0xff;
					port2 = 0xff;
				}
				break;
			case 100://d
				if (t_ot < 1800) //Pokud je po�et takt� men�� ne� 1800
				{
					port2 = port2 & maska_otaceni;
					port1 = port1 | maska_smer1;//sm�r vpravo
					t_ot++;//P�i��t�n� takt�
				}
				else
				{
					printf("Dosazeno mezni polohy, stisk d zakazan.");//Indikace v pc
				}
				break;
			case 119://w
				if (zavora_hlrameno != 1)//Pokud z�vora nen� aktivn�
				{
					port2 = port2 & maska_hlrameno;//aktivace hl. ramena
					port1 = port1 & maska_smer0;//sm�r nahoru
					t_hlram--;//Ode��t�n� takt�
				}
				break;
			case 115://s
				if (t_hlram < 500)//Pokud je po�et takt� men�� ne� 500
				{
					port2 = port2 & maska_hlrameno;//aktivace hl. ramena
					port1 = port1 | maska_smer1;//sm�r dol�
					t_hlram++;//P�i��t�n� takt�
				}
				else
				{
					printf("Dosazeno mezni polohy, stisk s zakazan.");//Indikace v pc
				}
				break;
			case 102://f
				if (zavora_chapadlo != 1)//Pokud z�vora nen� aktivn�
				{
					port2 = port2 & maska_chapadlo;//aktivace chapadla
					port1 = port1 & maska_smer0;//otev��t chapadlo
					t_chap--;//Ode��t�n� takt�
				}
				break;
			case 103://g
				if (t_chap < 450)//Pokud je po�et takt� men�� ne� 450
				{
					port2 = port2 & maska_chapadlo;//aktivace chapadla
					port1 = port1 | maska_smer1;//zav��t chapadlo
					t_chap++;//P�i��t�n� takt�
				}
				else
				{
					printf("Dosazeno mezni polohy, stisk g zakazan.");//Indikace v pc
				}
				break;

			case 109://m
				if (zavora_ramenochap != 1)
				{
					port2 = port2 & maska_ramenochapadlo;//aktivace pohybu chapadla
					port1 = port1 | maska_smer1;//sm�r nahoru
					t_ramchap--;//Ode��t�n� takt�
				}
				break;

			case 110://n
				if (t_ramchap < 3000)//Pokud je po�et takt� men�� ne� 3000
				{
					port2 = port2 & maska_ramenochapadlo;//aktivace pohybu chapadla
					port1 = port1 & maska_smer0;//sm�r dolu
					t_ramchap++;//P�i��t�n� takt�
				}
				else
				{
					printf("Dosazeno mezni polohy, stisk n zakazan.");//Indikace v pc
				}
				break;
			}

			//Takt
			outportb(0x301, takt1);
			delay(1);
			outportb(0x301, takt2);
			delay(1);

			outportb(0x301, port1);
			outportb(0x300, port2);//Odesl�n� hodnoty na port pro rozh�b�n� robota
		}
		else //Pokud nen� stisknut� tla��tko 
		{
			port1 = 0xff;
			port2 = 0xff;//Pokud nen� nic stisknuto, nuluj� se porty
		}
		clrscr();//Vymaz�n� v�eho z monitoru
	}

}

