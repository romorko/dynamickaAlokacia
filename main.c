#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define NDEBUG
#include <assert.h>
#define PISMENA_RIADOK 30
#define RIADKY_SUBOR 10

void testuj(void * hodnota, const char * sprava);
void alokujPamatNoveRiadky(int zaciatok, int koniec,char **pPole);
void vypisVety(size_t pocet,char **pPole);
void uvolniVety(size_t pocet,char **pPole);
int cmp(const void * prva, const void *druha);
int cmpSamohlasky(const void * prva, const void *druha);
int vratPocetSamohlasok(const char * slovo);


int main()
{
    FILE *fr;
    testuj(fr=fopen("text.txt","r"),"Chyba pri otvarani suboru!");
    int znak;
    size_t pocetZnakov=0;
    size_t pocetRealokaciiZnak=1;
    size_t pocetRiadkov=0;
    size_t pocetRealokaciiRiadky=1;
    char **pole; //pole pointerov na jednotlive riadky t.j. pointer na pointer
    testuj(pole=(char **)malloc(RIADKY_SUBOR*sizeof(char *)),"Chyba pri alokacii pamate!");
    alokujPamatNoveRiadky(0,RIADKY_SUBOR,pole);
    while((znak=getc(fr))!=EOF) //citanie znakov az do konca suboru
    {
        if(znak!='\n') //ak nie som na konci riadku
        {
            ++pocetZnakov;
            if(pocetZnakov%PISMENA_RIADOK==0) //uz som na nasobku POCET pri pocte znakov, musim realokovat
            {
                ++pocetRealokaciiZnak;
                testuj(*(pole+pocetRiadkov)=(char *)realloc(*(pole+pocetRiadkov),PISMENA_RIADOK*pocetRealokaciiZnak*sizeof(znak)),"Chyba pri alokacii pamate!");
            }
            //mam este k dispoziii znaky v riadku
            *(*(pole+pocetRiadkov)+pocetZnakov-1)=znak;
        }
        else        //som uz na konci riadku
        {
            *(*(pole+pocetRiadkov)+pocetZnakov)='\0'; //zakoncim riadok textu
            ++pocetRiadkov;
            if(pocetRiadkov%RIADKY_SUBOR==0) //ak uz som minul vsetky riadky musim pridat nove
            {
                ++pocetRealokaciiRiadky;
                testuj(pole=(char **)realloc(pole,pocetRealokaciiRiadky*RIADKY_SUBOR*sizeof(char *)),"Chyba pri alokacii pamate!");
                alokujPamatNoveRiadky(RIADKY_SUBOR*(pocetRealokaciiRiadky-1),RIADKY_SUBOR*pocetRealokaciiRiadky,pole);
            }
            pocetZnakov=0;
            pocetRealokaciiZnak=1;
        }
    }
    vypisVety(pocetRiadkov,pole);
    //qsort(pole,pocetRiadkov,sizeof(char *),cmp);
    qsort(pole,pocetRiadkov,sizeof(char *),cmpSamohlasky);
    vypisVety(pocetRiadkov,pole);
    uvolniVety(pocetRealokaciiRiadky*RIADKY_SUBOR,pole);
    free(pole);
    pole=NULL;
    fclose(fr);
    return 0;
}

void alokujPamatNoveRiadky(int zaciatok, int koniec, char **pPole)
{
    for(int i=zaciatok; i<koniec; ++i)
    {
        testuj(*(pPole+i)=(char *)malloc(PISMENA_RIADOK*sizeof(char)),"Chyba pri alokacii pamate!");  //alokacia pointerov na jednotlive riadky pola
    }
}

void vypisVety(size_t pocet,char **pPole)
{
    for(size_t i=0; i<pocet; ++i)
    {
        puts(*(pPole+i));
        //printf("%d",vratPocetSamohlasok(*(pPole+i)));
    };
}

void uvolniVety(size_t pocet,char **pPole)
{
    for(size_t i=0; i<pocet; ++i)
    {
        free(*(pPole+i));
        *(pPole+i)=NULL;
    }
}

int cmp(const void * prva, const void *druha)
{
    char ** veta1 = (char **)prva;
    char ** veta2 = (char **)druha;
    //printf("\t\t%s",veta1);
    return strlen(*veta1)-strlen(*veta2);

}

int vratPocetSamohlasok(const char * slovo)
{
    size_t pocet=0;
    const char * samohlasky="aAeEiIoOuU";
    char *kde = strpbrk(slovo,samohlasky);
    while(kde!=NULL)
    {
        ++pocet;
        kde=strpbrk(kde+1,samohlasky);
    }
    return pocet;
}

int cmpSamohlasky(const void * prva, const void *druha)
{
    char ** veta1 = (char **)prva;
    char ** veta2 = (char **)druha;
    return vratPocetSamohlasok(*veta1)-vratPocetSamohlasok(*veta2);
}

void testuj(void * hodnota, const char * sprava)
{
    if(hodnota==NULL)
    {
        puts(sprava);
        exit(1);
    }
}
