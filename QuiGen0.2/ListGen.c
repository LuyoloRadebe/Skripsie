//
// Created by ubuntu on 10/14/18.
//
#include "Global.h"

FILE *filein;
FILE *fileout;
char GlobalBuffer[300];
int i,j,k,l,m,n;

int ListGen(char * file_name);
int ListGen(char * file_name)
{
    char ListFileOut[20];
    char LocalBuffer[30];

    sprintf(ListFileOut,"%s.lst",file_name);
    fileout = fopen(ListFileOut,"w+");

    for (i = 1; i<=NumSolidVolumes;i++)
    {
        for (j = 1; j<SL.count[i]; j++)
        {
            sprintf(LocalBuffer,"C C%dP%d.qui %d 0 0 0 + \n",i,j,DI.Panel[i][j]);
            fputs(LocalBuffer,fileout);
            memset(LocalBuffer,0, sizeof(LocalBuffer));
        }
        sprintf(LocalBuffer,"C C%dP%d.qui %d 0 0 0 \n",i,j,DI.Panel[i][j]);
        fputs(LocalBuffer,fileout);
        memset(LocalBuffer,0, sizeof(LocalBuffer));
    }
    fclose(fileout);
}
