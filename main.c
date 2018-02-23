#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int TABGLOBE1[27];
int TABGLOBE2[14];
int TABGLOBE3[10];
char MSG_SF[96];


int Test_signe(float A)//fonction qui test le signe de A
{
    int bit;
    if (A<0)
    {
        bit=1;
    }
    else
        bit=0;
    return bit;
}


void Createur_binaire(int Val,int nbr)//Fonction qui se carge de creer un nombre binaire de "nbr" digites d'une variable "Val"
{
    int i=0,j,cmt=0;

    int TAB[27];

    for(i=0; i<nbr; i++)
    {

        TABGLOBE1[i]=0;
        TABGLOBE2[i]=0;
        TABGLOBE3[i]=0;

    }


    while(Val!=0)
    {
        TAB[cmt]=Val%2;
        // % ==> modulo
        Val=Val/2;
        cmt++;
    }


    if(cmt<nbr)//si moin de digits alors on remplie de 0 a gauche
    {
        j=nbr-1;
        for(i=0; i<cmt; i++)
        {

            TABGLOBE1[j]=TAB[i];
            TABGLOBE2[j]=TAB[i];
            TABGLOBE3[j]=TAB[i];
            j--;
        }

    }
    else//si trop de digits on supprimes des bits e poids faible
    {
        j=0;
        for(i=cmt-1; i>-1; i--)
        {

            TABGLOBE1[j]=TAB[i];
            TABGLOBE2[j]=TAB[i];
            TABGLOBE3[j]=TAB[i];
            j++;

        }

    }



}

void Creation_Msg_Sigfox(float Longitude1,float latitude1,float Longitude2,float latitude2,int val_horraire)//fonction qui cree le message
{
    //Initialisation de variable
    char TASCI[16]= {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    int j=0,i=0;
    int somme;
    char Hexa[3];
    //Creation de l'ecart Longitude et Latitude
    float Ecart_Long=(Longitude2-Longitude1);
    float Ecart_Lat=(latitude2-latitude1);
    if(Ecart_Lat<0)
    {
        Ecart_Lat=-Ecart_Lat;//on prend la valeur +
    }
    if(Ecart_Long<0)
    {
        Ecart_Long=-Ecart_Long;
    }

    //creation du tableau de bits a retourner
    int Msg_Binaire[96];
    //on test les signes et implemente les bits de signe
    Msg_Binaire[0]=Test_signe(Longitude1);
    Msg_Binaire[28]=Test_signe(latitude1);
    Msg_Binaire[67]=Test_signe(Ecart_Lat);
    Msg_Binaire[82]=Test_signe(Ecart_Long);

    //on implemente les differentes partie du messages

    Createur_binaire(latitude1*1000000,27);//creation message binaire latitude en n
    for(i=0; i<27; i++)
    {
        Msg_Binaire[i+1]=TABGLOBE1[i];

    }
    printf("message binaire de la valeur lattitude\n");//on affiche le message precedement crée
    for(i=0; i<27; i++)
    {
        printf("%d;",TABGLOBE1[i]);
    }
    printf("\n");



    Createur_binaire(Longitude1*1000000,27);//de meme que pour la latitude en n

    for(i=29; i<57; i++)
    {
        Msg_Binaire[i]=TABGLOBE1[i-29];
    }
    printf("message binaire de la valeur longitude\n");

    for(i=0; i<27; i++)
    {
        printf("%d;",TABGLOBE1[i]);
    }
    printf("\n");


    Createur_binaire(Ecart_Lat*1000000,14);
    for(i=0; i<14; i++)
    {
        Msg_Binaire[i+68]=TABGLOBE2[i];

    }
    printf("message binaire de la valeur ecart lattitude entre n et n-1\n");
    for(i=0; i<14; i++)
    {
        printf("%d:",TABGLOBE2[i]);
    }
    printf("\n");




    Createur_binaire(Ecart_Long*1000000,14);
    for(i=0; i<14; i++)
    {

        Msg_Binaire[i+83]=TABGLOBE2[i];
    }
    printf("message binaire de la valeur ecart longitude entre n et n-1\n");
    for(i=0; i<14; i++)
    {
        printf("%d;",TABGLOBE2[i]);
    }
    printf("\n");




    Createur_binaire(val_horraire,10);
    for(i=0; i<10; i++)
    {
        Msg_Binaire[i+57]=TABGLOBE3[i];
    }
    printf("message binaire de la valeur Val horraire entre n et n-1\n");
    for(i=0; i<10; i++)
    {
        printf("%d;",TABGLOBE3[i]);
    }
    printf("\n");



    //on affiche le message de 96 bits
    printf(" Voici le message binaire regroupant tous les precedant message avec leurs bits de signe comme demandé\n");
    for(i=0; i<97; i++)
    {
        printf("%d",Msg_Binaire[i]);
    }
    printf("\n");
    //convertion binaire vers hexa
    int k;
    for(i=0; i<96; i+=4)
    {
        k=0;
        somme=0;
        Hexa[0]=Msg_Binaire[i];
        Hexa[1]=Msg_Binaire[i+1];
        Hexa[2]=Msg_Binaire[i+2];
        Hexa[3]=Msg_Binaire[i+3];
        for(j=3; j>-1; j--)
        {
            if(Hexa[j]==1)
            {
                somme+=pow(2,k);

            }


            k++;
        }

        MSG_SF[i]=TASCI[somme];

    }

}

int main()
{   //on declare les deux position et leur val horraire
    int i;
    int Val_Horaire=106;
    float Longitude2=2.374503;//N
    float Latitude2=48.855623;//N
    float Longitude1=2.288764;//N+1
    float Latitude1=48.865164;//N=1
    Creation_Msg_Sigfox(Longitude1,Latitude1,Longitude2,Latitude2,Val_Horaire);//on créer le message
    printf("Voici le message SigFox(un espace vide correspond a un 0):\n");
    for(i=0; i<96; i+=4)//on affiche le message
    {
        printf("%c;",MSG_SF[i]);
    }
    return 0;
}
