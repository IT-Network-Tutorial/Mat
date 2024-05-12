#define F_CPU 16000000UL
#define BAUD 9600
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<util/setbaud.h>

#include<stdio.h>

#define delickaDis 0x03
#define preteceniDis 0x01
#define delickaKlav 0x02
#define preteceniKlav 0b01000000

// displej na port E
#define portDisplej PORTE
#define portDisplejDDR DDRE

// klavesnice na port B
#define portKlavesnice PORTB
#define pinKlavesnice PINB
#define portKlavesniceDDR DDRB

#define casNestisknuti 1000

//jak dlouho se zobrazuje jeden znak
#define dobaObrazu 2000
//paměť pro stisknuté znaky
volatile int pametStisku[19] =
{128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128,128};
//má hodnotu indexu, do kterého se uloží další znak
volatile uint8_t indexStisknutych=0;
//0 - nahravani, 1 - prehravani
volatile uint8_t rezim=0;
//do této proměnné se uloží hodnota stisklé klávesy
volatile int klavesa=0;
//klavesa byla prave ted stisknuta
volatile uint8_t ted = 0;
//zda tlacitko bylo stisknuto
volatile uint8_t stisk[16];
//hodnota aktualniho tlacitka podle poctu stisku
volatile int realTimeStisk = 0;
//funkce tlacitek
volatile uint8_t hodnoty[16]={1, 2, 3, 64, 4, 5, 6, 20, 7, 8, 9, 30, 40, 0, 50, 100};

volatile uint8_t dekoder [37][7]=
{
{0b10001,0b01110,0b01100,0b01010,0b00110,0b01110,0b10001},//0
{0b11011,0b10011,0b11011,0b11011,0b11011,0b11011,0b00000},//1
{0b10001,0b01110,0b11110,0b11001,0b10111,0b01111,0b00000},//2
{0b10001,0b01110,0b11110,0b11001,0b11110,0b01110,0b10001},//3
{0b11100,0b11010,0b10110,0b01110,0b00000,0b11110,0b11110},//4
{0b00000,0b01111,0b00001,0b11110,0b11110,0b01110,0b10001},//5
{0b11001,0b10111,0b01111,0b00001,0b01110,0b01110,0b10001},//6
{0b00000,0b01110,0b11110,0b11101,0b11011,0b11011,0b11011},//7
{0b10001,0b01110,0b01110,0b10001,0b01110,0b01110,0b10001},//8
{0b10001,0b01110,0b01110,0b10000,0b11110,0b11101,0b10011},//9
{0b10001,0b01110,0b00000,0b01110,0b01110,0b01110,0b01110},//A
{0b00001,0b01110,0b00001,0b01110,0b01110,0b01110,0b00001},//B
{0b10001,0b01110,0b01111,0b01111,0b01111,0b01110,0b10001},//C
{0b00001,0b01110,0b01110,0b01110,0b01110,0b01110,0b00001},//D
{0b00000,0b01111,0b00011,0b01111,0b01111,0b01111,0b00000},//E
{0b00000,0b01111,0b00011,0b01111,0b01111,0b01111,0b01111},//F
{0b10000,0b01111,0b01100,0b01110,0b01110,0b01110,0b10001},//G
{0b01110,0b01110,0b00000,0b01110,0b01110,0b01110,0b01110},//H
{0b10001,0b11011,0b11011,0b11011,0b11011,0b11011,0b10001},//I
{0b11110,0b11110,0b11110,0b11110,0b11110,0b01110,0b10001},//J
{0b01110,0b01101,0b00011,0b01101,0b01110,0b01110,0b01110},//K
{0b01111,0b01111,0b01111,0b01111,0b01111,0b01111,0b00000},//L
{0b01110,0b00100,0b01010,0b01110,0b01110,0b01110,0b01110},//M
{0b01110,0b00110,0b01010,0b01100,0b01110,0b01110,0b01110},//N
{0b10001,0b01110,0b01110,0b01110,0b01110,0b01110,0b10001},//O
{0b00001,0b01110,0b00001,0b01111,0b01111,0b01111,0b01111},//P
{0b10001,0b01110,0b01110,0b01110,0b01110,0b01101,0b10010},//Q
{0b00001,0b01110,0b00001,0b01110,0b01110,0b01110,0b01110},//R
{0b10000,0b01111,0b10001,0b11110,0b11110,0b01110,0b10001},//S
{0b00000,0b11011,0b11011,0b11011,0b11011,0b11011,0b11011},//T
{0b01110,0b01110,0b01110,0b01110,0b01110,0b01110,0b10001},//U
{0b01110,0b01110,0b01110,0b01110,0b10101,0b10101,0b11011},//V
{0b01110,0b01110,0b01110,0b01110,0b01010,0b00100,0b01110},//W
{0b01110,0b10101,0b11011,0b10101,0b01110,0b01110,0b01110},//X
{0b01110,0b10101,0b11011,0b11011,0b11011,0b11011,0b11011},//Y
{0b00000,0b11110,0b11101,0b11011,0b10111,0b01111,0b00000},//Z
{0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111},//_
};
volatile uint8_t radek=0;
volatile uint8_t radekDisplej=0;
volatile uint8_t operace=0;
volatile uint8_t poradi=0;
volatile long cislo1=0;
volatile long cislo2=0;
volatile long vysledek=0;
volatile int poradiC=0;
volatile uint8_t nul=0;
volatile int segmentovka=0;
//uint8_t tlacitko = 0;
//------------------------------------------------------------
void cas(void)
{
  TCCR0 = delickaDis;
  TIMSK |= preteceniDis;

  TCCR2 = delickaKlav;
  TIMSK |= preteceniKlav;
  sei();
}
//------------------------------------------------------------
uint8_t dekoderRadku (int znak,uint8_t radekD)
{
  //jaký řádek svítí
  uint8_t radky[7]={0,4,2,6,1,5,3};
  uint8_t hodnotaNaPort=0;
  //obsah+číslo řádku
  hodnotaNaPort=(dekoder[znak][radekD]*8)+radky[radekD];
  return hodnotaNaPort;

}
//------------------------------------------------------------
//displej
volatile int test1=0;
volatile uint8_t test2=20;

volatile long pocitaniPoradi=dobaObrazu*2;

ISR (TIMER0_OVF_vect)
{
  uint8_t hodnotaNaPort = 0;
  if(rezim==0)
    hodnotaNaPort = dekoderRadku(realTimeStisk, radekDisplej);
  else
  {
    if((pocitaniPoradi/dobaObrazu)>=indexStisknutych)
      //musime zacit od 3. hodnoty, na prvnich 2 je 0 a 1, které se nemají zobrazit
      pocitaniPoradi=2*dobaObrazu;
    hodnotaNaPort = dekoderRadku(pametStisku[pocitaniPoradi/dobaObrazu], radekDisplej);

    pocitaniPoradi++;
  }
  portDisplej = hodnotaNaPort;
  //portDisplej=0b10001000+6;
  radekDisplej++;
  if(radekDisplej>6)
    radekDisplej=0;
}

volatile int posledniTlacitko = 128;
volatile uint8_t pocetStisku = 0;
volatile int dobaNestisknuti = 0;
//zpracování stisků tlačítka
//-----------------------------------------------------------------
void pocitaniStiskuTlacitka(int tlacitko)
{
  if((klavesa==15)&&(indexStisknutych>6)&&(indexStisknutych<18))
    rezim=1;
  if(klavesa==14)
    rezim=0;
  //režim nahrávání
  if(rezim==0)
  {

    //Významy kláves po daném počtu stisků
    int prekladacStisku[16][6]={
    {1  ,128,128,128,128,128,128},//1
    {2  ,10 ,11 ,12 ,128,128,128},//2ABC
    {3  ,13 ,14 ,15 ,128,128,128},//3DEF
    {64 ,128,128,128,128,128,128},
    {4  ,16 ,17 ,18 ,128,128,128},//4GHI
    {5  ,19 ,20 ,21 ,128,128,128},//5JKL
    {6  ,22 ,23 ,24 ,128,128,128},//6MNO
    {64 ,128,128,128,128,128,128},
    {7  ,25 ,26 ,27 ,28 ,128,128},//7PQRS
    {8  ,29 ,30 ,31 ,128,128,128},//8TUV
    {9  ,32 ,33 ,34 ,35 ,128,128},//9WXYZ
    {64 ,128,128,128,128,128,128},
    {64 ,128,128,128,128,128,128},
    {0  ,36 ,128,128,128,128,128},//0_
    {64 ,128,128,128,128,128,128},
    {64 ,128,128,128,128,128,128},
    };
    //vypsani aktualni hodnoty
    if (posledniTlacitko==128)
    {
      if(indexStisknutych==0)
        realTimeStisk=0;
      else
        realTimeStisk=pametStisku[indexStisknutych-1];
    }
    else
      realTimeStisk=prekladacStisku[posledniTlacitko][pocetStisku];

    //pokud je to furt stejne tlacitko
    if(posledniTlacitko==tlacitko)
    {
      //mezi stisky byla casová mezera
      if(dobaNestisknuti>20)
      {
        pocetStisku++;
        dobaNestisknuti=0;
        //pro dany pocet stisku neni hodnota = nuluje se
        if(prekladacStisku[tlacitko][pocetStisku]==128)
          pocetStisku=0;
      }
    }
    //neni stiskle stejne tlacitko
    else
    {
      //nebylo stiskle zadne tlacitko
      if(tlacitko==128)
      {
        dobaNestisknuti++;
        //ulozeni tlacitka do pameti
        if(dobaNestisknuti>casNestisknuti)
        {
          //pokud se dostaneme mimo rozsah pole
          if(indexStisknutych==19)
            indexStisknutych=18;
          //pokud naposled bylo stiskle tlacitko
          if(posledniTlacitko!=128)
          {
            //ulozi si hodnotu
            pametStisku[indexStisknutych]=realTimeStisk;
            indexStisknutych++;
            //pokud se dostaneme mimo rozsah pole
            if(indexStisknutych==19)
              indexStisknutych=18;
            posledniTlacitko=128;
            pocetStisku=0;
          }
        }
      }
      //stiskle tlacitko je jine nez to predchozi
      else
      {
        if((tlacitko!=3)&&(tlacitko!=7)&&(tlacitko!=11)&&(tlacitko!=12)&&(tlacitko!=14)&&(tlacitko!=15))
        {
          if(posledniTlacitko!=128)
          {
            pametStisku[indexStisknutych]=realTimeStisk;
            indexStisknutych++;
          }
          dobaNestisknuti=0;
          posledniTlacitko=tlacitko;
          pocetStisku=0;
        }
      }
    }
  }
}

 //-----------------------------------------------------------------------

 //Ziskani cisla stisknute klavesy
ISR (TIMER2_OVF_vect)
{
  //Vypočítá na jakém jsme řádku
  uint8_t radek = (poradiC/4)+4;
  //uloží si aktuální stav na pinu
  uint8_t pinKlav=pinKlavesnice;
  uint8_t portKlav=0;
  //odmaskuje a otestuje, jestli je dané tlačítko stisknuté
  if ((pinKlav & 0xF0)==((~(1 << (poradiC/4+4)))& 0xF0))
  {
  //uloží číslo stisklého tlačítka
  nul = poradiC;
  //pokud je tlačítko stisklé nově
    if(stisk[poradiC] != 1)
    {
      //uloží číslo tlačítka, s tím pak pracujem
      klavesa=poradiC;
      //zapomatuje si, že toto tlačítko bylo stisklé
      for(int n=0; n<16; n++)
      {
        stisk[n]=0;
      }
      stisk[poradiC]=1;
    }
  }
  //zapomatuje si, že tlačítko už není stisklé
  else
    if((stisk[nul] == 1)&&(poradiC == nul))
      stisk[nul] = 0;


  poradiC++;
  if(poradiC == 16)
  {
    pocitaniStiskuTlacitka(klavesa);
    poradiC = 0;
    klavesa=128;
  }

  portKlav|=0x0F;
  portKlav&=(~(1 << (poradiC%4)));
  portKlavesnice=portKlav;

}
//------------------------------------------------------------------------------
int main (void)
{
  cas();
  portKlavesniceDDR=0x0F;
  portDisplejDDR=0xff;

  while(1)
  {
  }
}

