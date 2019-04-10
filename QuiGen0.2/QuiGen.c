//
// Created by ubuntu on 10/10/18.
// Reads in unspecified file and generates a qui file
//

#include "Global.h"
int i,j,k,l,m,n;
int flag;

int QuiGen(char * file_postmesh);

int QuiGen(char * file_postmesh)
{
    FILE *filein;
    FILE *fileout;
    FILE *fileout1;
    char LocalBuffer1[150];
    char LocalBuffer2[150];
    char LocalBuffer3[150];
    char LocalBuffer4[200];
    int8_t Conductor = 0;
    int8_t SearchBracket = 0;
    int8_t SearchSurface = 0;
    int busy;
    float coord;
    float coordz = 0, Tol = 0.01;
    char filename_out[20];
    char filename_in[20];
    char dielectric[20];
    char *token;

    strcpy(dielectric,file_postmesh);
    memset(filename_out,0, sizeof(filename_out));
    sprintf(filename_out,"%s.qui",file_postmesh);
    fileout = fopen(filename_out,"w+");
    sprintf(LocalBuffer1,"0 %s",file_postmesh);
    fputs(LocalBuffer1,fileout);
    memset(LocalBuffer1, 0, sizeof(LocalBuffer1));
    if (flag == 1) {
        for (i = 1; i <= NumSolidVolumes; i++) {
            for (j = 1; j <= SL.count[i]; j++) {
                memset(filename_in, 0, sizeof(filename_out));
                sprintf(filename_in, "C%dP%d", i, j);
                filein = fopen(filename_in, "r");

                sprintf(filename_out, "C%dP%d.qui", i, j);
                fileout1 = fopen(filename_out, "w+");
                sprintf(LocalBuffer1, "0 %s C%dP%d", file_postmesh, i, j);
                fputs(LocalBuffer1, fileout1);
                busy = 1;
                coordz = 200;
                while (busy) {
                    memset(LocalBuffer1, 0, sizeof(LocalBuffer1));
                    fgets(LocalBuffer1, sizeof(LocalBuffer1), filein);
                    if (LocalBuffer1[1] == 'T') {
                        token = strtok(LocalBuffer1, openbracket);
                        memset(LocalBuffer2, 0, sizeof(LocalBuffer2));
                        for (k = 1; k < 9; k++) {
                            token = strtok(NULL, comma);
                            coord = atof(token);
                            sprintf(LocalBuffer3, "%.5e ", coord);
                            strcat(LocalBuffer2, LocalBuffer3);
                            memset(LocalBuffer3, 0, sizeof(LocalBuffer3));
                        }
                        token = strtok(NULL, closebracket);
                        coord = atof(token);

                        if (coord < coordz) {
                            coordz = coord;
                        }
                        sprintf(LocalBuffer3, "%.5e ", coord);
                        strcat(LocalBuffer2, LocalBuffer3);

                        sprintf(LocalBuffer1, "\nT %d %s", i, LocalBuffer2);
                        sprintf(LocalBuffer4, "\nT 1 %s", LocalBuffer2);
                        fputs(LocalBuffer1, fileout);
                        fputs(LocalBuffer4, fileout1);
                    } else if (LocalBuffer1[0] == '}') {
                        for (l = 1; l <= NumDielectric; l++) {
                            if ((fabs(coordz - DI.Properties[l][0][3]) / (DI.Properties[l][0][3])) <= Tol) {
                                DI.Panel[i][j] = DI.Properties[l + 1][0][0];
                                DI.Properties[l][5][0] = 1; //Do not generate a panel
                            } else if ((coordz) < DI.Properties[l][0][3]) {
                                DI.Panel[i][j] = DI.Properties[l][0][0];
                            } else if ((coordz) > DI.Properties[l][0][3]) {
                                DI.Panel[i][j] = DI.Properties[l + 1][0][0];
                            }
                        }
                        busy = 0;
                    }
                }
                flag = 0;
                fclose(filein);
                fclose(fileout1);
            }
        }
    }
    else if (flag == 0)
    {
        memset(filename_in, 0, sizeof(filename_out));
        sprintf(filename_in, file_postmesh);
        filein = fopen(dielectric, "r");
        printf("%s\n",strerror(errno));
        sprintf(filename_out, "%s.qui", dielectric);
        fileout1 = fopen(filename_out, "w+");
        sprintf(LocalBuffer1, "0 %s", dielectric);
        fputs(LocalBuffer1, fileout1);
        busy = 1;
        coordz = 200;
        while (busy) {
            memset(LocalBuffer1, 0, sizeof(LocalBuffer1));
            fgets(LocalBuffer1, sizeof(LocalBuffer1), filein);
            if (LocalBuffer1[1] == 'T') {
                token = strtok(LocalBuffer1, openbracket);
                memset(LocalBuffer2, 0, sizeof(LocalBuffer2));
                for (k = 1; k < 9; k++) {
                    token = strtok(NULL, comma);
                    coord = atof(token);
                    sprintf(LocalBuffer3, "%.5e ", coord);
                    strcat(LocalBuffer2, LocalBuffer3);
                    memset(LocalBuffer3, 0, sizeof(LocalBuffer3));
                }
                token = strtok(NULL, closebracket);
                coord = atof(token);

                if (coord < coordz) {
                    coordz = coord;
                }
                sprintf(LocalBuffer3, "%.5e ", coord);
                strcat(LocalBuffer2, LocalBuffer3);

                sprintf(LocalBuffer1, "\nT %d %s", i, LocalBuffer2);
                sprintf(LocalBuffer4, "\nT 1 %s", LocalBuffer2);
                fputs(LocalBuffer1, fileout);
                fputs(LocalBuffer4, fileout1);
            } else if (LocalBuffer1[0] == '}') {
                for (l = 1; l <= NumDielectric; l++) {
                    if ((fabs(coordz - DI.Properties[l][0][3]) / (DI.Properties[l][0][3])) <= Tol) {
                        DI.Panel[i][j] = DI.Properties[l + 1][0][0];
                        DI.Properties[l][5][0] = 1; //Do not generate a panel
                    } else if ((coordz) < DI.Properties[l][0][3]) {
                        DI.Panel[i][j] = DI.Properties[l][0][0];
                    } else if ((coordz) > DI.Properties[l][0][3]) {
                        DI.Panel[i][j] = DI.Properties[l + 1][0][0];
                    }
                }
                busy = 0;
            }
        }
        fclose(filein);
        fclose(fileout1);
    }
    fclose(fileout);
    fflush(stdin);
    fflush(stdout);
}
