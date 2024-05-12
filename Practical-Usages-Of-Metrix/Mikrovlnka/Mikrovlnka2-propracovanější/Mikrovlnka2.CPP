#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <time.h> //knihovna časových funkcí

//inicializace proměnných
#define port1 0x300
#define port2 0x301

int teplota = 0; //zadaná teplota
int casminuty = 0; // zadaný čas v minutách
int cassekundy = 0; // zadaný čas v sekundách
int casporovnej = 0; // pro porovnání času se zadaným časem
int citanipwm = 0; //čítání PWM 
int k; // proměnná pro cyklus for

//klavesnice, každé tlačítko je jeden prvek pole (Mode, up, down, set) zárovrň sledování stavu kláves (aktivní v nule)
int klavesnice [4] = {1,1,1,1}; //0 = MODE, 1 = NAHORU, 2 = DOLU, 3 = SET
//hlášky na displeji, hexa adresy zadané v dokumentaci

//Spíše informativní, text zadaný v dokumentaci, použil jsem jiný způsob
int OPEN[4] = {0x00, 0x16, 0x0E, 0x15};
int CLOSE[4] = {0x0C,0x13, 0x00, 0x05};
int OFF[4] = {0x00, 0x0F, 0x0F, 0x1C};
int ON[4] = {0x00, 0x15, 0x1C, 0x1C};
int TIME[4] = {0x18, 0x01, 0x14, 0x0E};
int TEMP[4] = {0x1D, 0x1D, 0x1B, 0x0C};
int POW[4] = {0x16, 0x00, 0x19, 0x19};
int GOOD[4] = {0x10, 0x00, 0x00, 0x0D};

int nul = 0xC0; //Deaktivace aktivních prvků 192
int port2_cti; //čtení z portu 2
int kroky = 0; //velmi důležitá proměnná pro práci se switch/case

//Použití dvourozměrného pole pro displej pro lepší ovládání, zároveň obsluha displeje
//a zároveň obsluha prvků mikrovlnné trouby

int displej [4][2] =
{
  { 46,nul },
  { 45,nul },
  { 43,nul },
  { 39,nul }

};

//-----------------------------------------------------------------------------------------------------

int main()
{
 //Při zapnutí programu, vysvícení na displeji znaků 6666
	for (k = 0; k < 4; k++) //4 opakování = 4 místa displeje
	
		//Maska která nuluje displej, nahrání 4x číslo 6
		displej[k][1] = displej[k][1] & 224; //vymažu předchozí hodnoty displeje
	displej[0][1] = displej[0][1] | 0x06;
	displej[1][1] = displej[1][1] | 0x06;
	displej[2][1] = displej[2][1] | 0x06;
	displej[3][1] = displej[3][1] | 0x06;
        

while (1) //nekonečná smyčka
{

	
	//Stálá kontrola klávesnice a dat na displeji, provede 4x pro každý tranzistor a tlačítko klávesnice
	
	for (k = 0; k < 4; k++)
	{
	   // Otevření tranzistoru
	outportb(port2, displej[k][0]);
		// Odesílání dat
	outportb(port2, displej[k][1]);
		// Čtení portu 2 kvůli klávesnici
	port2_cti = inportb(port2);
		// Zápis stavů z klávesnice
	klavesnice[k] = (port2_cti >> 4) % 2; //bitový posun o 4 doprava, za sebou doplňuje 0
		// Zpoždění pro rozsvícení displeje (4ms celkem)
	delay(1); // 1 v závorce znamená 1ms
	
	}
	
switch (kroky)
{
case 0:
{
//podmínka if, pokud stisknu tlačítko MODE
 if (klavesnice[0] == 0) // v nule je aktivní
	{
//Vysvícení NULL, což je nultý stav
printf("Vitejte v programu.\n");
    for (k = 0; k < 4; k++) //cyklus for, 4x pro každý segment displeje
	{
	displej[k][1] = displej[k][1] & 224; //nulování, logický AND, nuluji předešlé stavy

	displej[0][1] = displej[0][1] | 0x15; //M
	displej[1][1] = displej[1][1] | 0x19; //O
	displej[2][1] = displej[2][1] | 0x13; //D
	displej[3][1] = displej[3][1] | 0x13; //E
	
	
	}
	kroky = kroky +1;
	}

} break;
case 1:
{
	if (klavesnice[0] == 1) //případ puštění MODE
	kroky = kroky +1;
	break;
}
case 2:
{
	if (klavesnice[3] == 0) //stisknutí SET = otevření dveří
	{
		printf("Dvere otevreny.\n");
		for (k = 0; k < 4; k++)
		{
			displej [k][0] = displej [k][0] | 223; //223, 6-tý bit na 0 ==> otevření dveří
			
		}	
		kroky = kroky +1;
	}
	if (klavesnice[0] == 0) //Stisknutí mode, tudíž dveře se neotevřou
		kroky = kroky +1;
	printf("Dvere neotevreny.\n");
		
}break;
case 3:
{
	//vypsání hlášky TEMP (nastavení teploty)
	printf("Nastavovani teploty.\n");
		for (k = 0; k < 4; k++) //cyklus for, 4x pro každý segment displeje
	{
	displej[k][1] = displej[k][1] & 224; //nulování, logický AND, nuluji předešlé stavy

	displej[0][1] = displej[0][1] | 0x15; //T
	displej[1][1] = displej[1][1] | 0x19; //E
	displej[2][1] = displej[2][1] | 0x13; //M
	displej[3][1] = displej[3][1] | 0x13; //P
	
	
	}
	kroky = kroky + 1;	
		
	
	
	
}break;

case 4:
{
	if (klavesnice [3] == 0) //stisk SET a nastavení teploty
	{
		//vypsání 00°C, počáteční hodnota
	for (k = 0; k < 4; k++) //cyklus for, 4x pro každý segment displeje
		{
			
		displej[k][1] = displej[k][1] & 224; //nulování, logický AND, nuluji předešlé stavy

		displej[0][1] = displej[0][1] | (teplota/10); // desítky stupňů, Dělení celých čísel
		displej[1][1] = displej[1][1] | (teplota%10); // jednotky stupňů, využití znaku %, což vypíše zbytek po celočíselném dělení
		displej[2][1] = displej[2][1] | 0x1B; // °
		displej[3][1] = displej[3][1] | 0x0C; // C
		
		}
		kroky = kroky + 1;
	}
	
}break;

case 5:
{
	if (klavesnice[1] == 0) //Stisknutí tlačítka nahoru
	{
			if (teplota<99) //ošetření, zadávání teploty do 99
			teplota = teplota + 1;
			kroky = 7;
	}
	else
		kroky = kroky + 1;
	if (klavesnice[3] == 0) //stisknutí tlačítka set, potvrzení
	printf("Teplota byla nastavena.\n");
	kroky = 8;
}break;

case 6:
if(klavesnice [2] == 0) //stisknutí tlačítka dolu
	{
			if (teplota>0) //ošetření abychom nemohli zadat teplotu do záporných hodnot
			teplota = teplota - 1;
			kroky = 7;
	}

	 if (klavesnice[3] == 0) //stisknutí tlačítka set, potvrzení
	printf("Teplota byla nastavena.\n");
	kroky = 8;
	else
		kroky = 7;
	
	
}break;
case 7:
{
	//Aktualizace údajů na displeji mikrovlnky
	for (k = 0; k < 4; k++) //cyklus for, 4x pro každý segment displeje
		{
		displej[k][1] = displej[k][1] & 224; //nulování, logický AND, nuluji předešlé stavy

		displej[0][1] = displej[0][1] | (teplota/10); // desítky stupňů, Dělení celých čísel
		displej[1][1] = displej[1][1] | (teplota%10); // jednotky stupňů, využití znaku %, což vypíše zbytek po celočíselném dělení
		displej[2][1] = displej[2][1] | 0x1B; // °
		displej[3][1] = displej[3][1] | 0x0C; // C
		
		}
		
		
		kroky = 5;
	
}break;
case 8:
{
	//po konci nastavování teploty následuje nastavení času, vypsání TIME
			for (k = 0; k < 4; k++) //cyklus for, 4x pro každý segment displeje
		{
		displej[k][1] = displej[k][1] & 224; //nulování, logický AND, nuluji předešlé stavy
		
		displej[0][1] = displej[0][1] | 0x18; // T
		displej[1][1] = displej[1][1] | 0x01; // I
		displej[2][1] = displej[2][1] | 0x14; // M
		displej[3][1] = displej[3][1] | 0x0E; // E
		
		}
		printf("Nastaveni casu.\n");
		kroky = kroky + 1;
	
}break;
case 9:
{
	//Stisk SET a vypsání času
	if (klavesnice [3] == 0)
	{
		for (k = 0; k < 4; k++) //cyklus for, 4x pro každý segment displeje
		{
		displej[k][1] = displej[k][1] & 224; //nulování, logický AND, nuluji předešlé stavy

		displej[0][1] = displej[0][1] | (casminuty/10); // desítky minut, Dělení celých čísel
		displej[1][1] = displej[1][1] | (casminuty%10); // jednotky minut, využití znaku %, což vypíše zbytek po celočíselném dělení
		displej[2][1] = displej[2][1] | (cassekundy/10); // Stejné operace jako předešlé, akorát sekundy
		displej[3][1] = displej[3][1] | (cassekundy%10); 
		
		}
		kroky = kroky + 1;
	}
}break;	
case 10:
{
	if (klavesnice [1] == 0) //stisk nahoru
	{
		if(casminuty<99)
		casminuty = casminuty + 1;
	kroky = 12;
	}
	else
		kroky = kroky + 1;
		if (klavesnice [3] == 0) //stisk SET jako potvrzení
		kroky = 13;
		printf("Cas minuty byl nastaven.\n");
}
case 11:
{
	if (klavesnice [2] == 0) //stisk dolu
	{
		if(casminuty>0)
			casminuty = casminuty - 1;
		kroky = 12;
	}
	else
		kroky = kroky + 1;
		if (klavesnice [3] == 0) //SET potvrzení
		kroky = 13;
		printf("Cas minuty byl nastaven.\n");
}break;

case 12: //Aktualizace informací na displeji
{
	for (k = 0; k < 4; k++) //cyklus for, 4x pro každý segment displeje
		{
		displej[k][1] = displej[k][1] & 224; //nulování, logický AND, nuluji předešlé stavy

		displej[0][1] = displej[0][1] | (casminuty/10); // desítky minut, Dělení celých čísel
		displej[1][1] = displej[1][1] | (casminuty%10); // jednotky minut, využití znaku %, což vypíše zbytek po celočíselném dělení
		displej[2][1] = displej[2][1] | (cassekundy/10); // Stejné operace jako předešlé, akorát sekundy
		displej[3][1] = displej[3][1] | (cassekundy%10); 
		
		}
		
		kroky = 10;
	
}break;

case 13:
{
	if (klavesnice [1] == 0)
	{
		if (cassekundy < 59) //nastavování sekund do 59
		cassekundy = cassekundy + 1;
		kroky = 15;
	}
	else
		kroky = kroky + 1;
		if (klavesnice [3] == 0) //potvrzení
		kroky = 16;
		printf("Cas sekundy byl nastaven.\n");
}break;

case 14:
{
		if (klavesnice [2] == 0)
	{
		if (cassekundy > 0) //nastavování sekund do 59
		cassekundy = cassekundy - 1;
		kroky = 15;
	}
	else
		kroky = kroky + 1;
		if (klavesnice [3] == 0) //potvrzení
		kroky = 16;
		printf("Cas sekundy byl nastaven.\n");
}break;

case 15:
{
	
	for (k = 0; k < 4; k++) //cyklus for, 4x pro každý segment displeje
		{
		displej[k][1] = displej[k][1] & 224; //nulování, logický AND, nuluji předešlé stavy

		displej[0][1] = displej[0][1] | (casminuty/10); // desítky minut, Dělení celých čísel
		displej[1][1] = displej[1][1] | (casminuty%10); // jednotky minut, využití znaku %, což vypíše zbytek po celočíselném dělení
		displej[2][1] = displej[2][1] | (cassekundy/10); // Stejné operace jako předešlé, akorát sekundy
		displej[3][1] = displej[3][1] | (cassekundy%10); 
		
		}
		kroky = 13;
}break;

case 16:
{
	if (klavesnice [3] == 0) //SET stisknutý, nahrání hlášky ON..
	{
		printf("Zapnuti procesu.\n");
			for (k = 0; k < 4; k++) //cyklus for, 4x pro každý segment displeje
		{
		displej[k][1] = displej[k][1] & 224; //nulování, logický AND, nuluji předešlé stavy

		displej[0][1] = displej[0][1] | 0x00; //O 
		displej[1][1] = displej[1][1] | 0x15; //N 
		displej[2][1] = displej[2][1] | 0x1C; //. 
		displej[3][1] = displej[3][1] | 0x1C; //.
		
		}
		kroky = kroky + 1;
	}
}break;

case 17:
{
port2_cti = inportb(port1);
if (port2_cti == 0) //Kontrola jestli jsou dveře zavřeny
{
	printf("Cekam na zavrene dvere.\n");
for (k = 0; k < 4; k++)
						displej[k][0] = displej[k][0] | 32; //32 neboli 0010_0000 znamená dveře zavřeny
	printf("Dvere zavreny, proces zahajen.\n");
	kroky = kroky + 1;			
}

}break;

case 18:
{
	//Výkon motoru je nastaven na 70%, to znamená 700ms běží 300ms neběží
if(citanipwm <= 700) //vše je v milisekundách, rozdělil jsem si čas 1s na 10 částí po 100ms.
	for (k = 0; k < 4; k++) //cyklus for, 4x pro každý segment, jedná se o zapnutí motoru
	displej[k][0] = displej[k][0]& 63; //sepnutí motoru
	else
		for (k = 0; k < 4; k++) //cyklus for, 4x pro každý segment, vypnutí motoru
	displej[k][0] = displej[k][0]| 192; //vypnutí motoru

if(citanipwm >= 1000) //uběhla celá 1s, nový start
citanipwm = 0;
delay(1); //počká 1ms
citanipwm++; //přičte 1ms do čítání pwm

casporovnej = casporovnej + 1;
if (casporovnej == 250) //Při přepočtu 4x250ms = 1s, kvůli zpoždění na displeji
{
	if (cassekundy > 0)
		cassekundy = cassekundy - 1;
	else
	{
		casminuty = casminuty -1;
		cassekundy = 59;
	}
	casporovnej = 0; // nulování pro další odpočet
	
	//aktualizace času na displeji
	for (k = 0; k < 4; k++) //cyklus for, 4x pro každý segment displeje
		{
		displej[k][1] = displej[k][1] & 224; //nulování, logický AND, nuluji předešlé stavy

		displej[0][1] = displej[0][1] | (casminuty/10); // desítky minut, Dělení celých čísel
		displej[1][1] = displej[1][1] | (casminuty%10); // jednotky minut, využití znaku %, což vypíše zbytek po celočíselném dělení
		displej[2][1] = displej[2][1] | (cassekundy/10); // Stejné operace jako předešlé, akorát sekundy
		displej[3][1] = displej[3][1] | (cassekundy%10); 
		}
		if (casminuty == 0 && cassekundy == 0)
			kroky = 19;
		
}

}break;

case 19:
{
	printf("Vas pokrm je pripraven.\n");
	
		for (k = 0; k < 4; k++) //cyklus for, 4x pro každý segment displeje
		{
		displej[k][1] = displej[k][1] & 224; //nulování, logický AND, nuluji předešlé stavy

		displej[0][1] = displej[0][1] | 0x10; //G
		displej[1][1] = displej[1][1] | 0x00; //O 
		displej[2][1] = displej[2][1] | 0x00; //O 
		displej[3][1] = displej[3][1] | 0x0D; //D
		}
		for (k = 0; k < 4; k++)
		{
			displej[k][0] = displej[k][0] & 223; //Otevření dveří po ukončení procesu, neboli nulový 
			displej[k][0] = displej[k][0] | 192; //Vypnutí motoru
		}
		
 kroky = 0;
 teplota = 0; //zadaná teplota
 casminuty = 0; // zadaný čas v minutách
 cassekundy = 0; // zadaný čas v sekundách
 casporovnej = 0; // pro porovnání času se zadaným časem
 citanipwm = 0; //čítání PWM	
	
	
	
}break;
	
	
}

return 0;

}



	
	
	





