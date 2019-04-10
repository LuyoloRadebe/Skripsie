//
// Created by ubuntu on 10/13/18.
//

#include "Global.h"

FILE *filein;
FILE *fileout;
char GlobalBuffer[500000];
int i = 0,j,k,l,x,y,p;
uint64_t ConstantK = 90000000000;

int DielectricBoundary(char * file_geo,int instance);
int DielectricBoundary(char * file_geo,int instance)
{
    int busy = 1;
    char LocalBuffer[300];
    char* token;
    float distance = 0;
    float charge = 0;
    float DielectricZ = 0;
    float DiffX = 0, DiffY = 0, DiffZ = 0;
    float MinX = 0, MinY = 0,MaxX = 0, MaxY = 0,MidX = 0, MidY = 0;

    //Read Data In
    sprintf(LocalBuffer,"%s_ChargeData",file_geo);
    filein = fopen(LocalBuffer,"r");
    i =0;
    while(busy)
    {
        i++;
        fgets(LocalBuffer,300,filein);
        if(LocalBuffer[0] == 'X')
        {
            token = strtok(LocalBuffer, space);
            token = strtok(NULL, space);
            Charge.Details[1][i] = atof(token);
            token = strtok(NULL, space);
            Charge.Details[2][i] = atof(token);
            token = strtok(NULL, space);
            Charge.Details[3][i] = atof(token);
            token = strtok(NULL, space);
            Charge.Details[0][i] = atof(token);
            memset(LocalBuffer,0, sizeof(LocalBuffer));
        }
        else
        {
            busy = 0;
            NumPanels = i;
        }
    }
    fclose(filein);

    for(i = 1; i <= NumDielectric; i++)
    {
        DI.Properties[i][1][2] = DI.Properties[i][0][3];
        DI.Properties[i][2][2] = DI.Properties[i][0][3];
        DI.Properties[i][3][2] = DI.Properties[i][0][3];
        DI.Properties[i][4][2] = DI.Properties[i][0][3];

        for (x = 1; x <= 4; x++)
        {
            switch (x)
            {
                case 1:
                    MaxX = ENCL.Limits[0][0];
                    MinX = ENCL.Limits[0][0] - fabs(ENCL.Limits[1][0] - ENCL.Limits[0][0]);
                    MidX = 0.5*(MaxX+MinX);
                    while (((fabs(MaxX-MinX))/fabs(MidX))>(OnePercentTol))
                    {
                        MidX = 0.5*(MaxX+MinX);
                        charge = 0;
                        for (p = 1; p<=NumPanels;p++)
                        {
                            DielectricZ = DI.Properties[i][0][3];
                            DiffX = (MidX - Charge.Details[1][p])*(MidX - Charge.Details[1][p]);
                            DiffY = (AbsMidY - Charge.Details[2][p])*(AbsMidY - Charge.Details[2][p]);
                            DiffZ = (DielectricZ - Charge.Details[3][p])*(DielectricZ - Charge.Details[3][p]);
                            distance = (DiffX+DiffY+DiffZ);
                            charge = charge + ((Charge.Details[0][p])/(distance));
                        }
                        if (charge>0) //find point where charge is not significantly changing
                        {
                            MinX = MidX;
                        }
                        if (charge<0)
                        {
                            MaxX = MidX;
                        }
                    }
                    DI.Properties[i][1][0] = MidX*0.5;
                    DI.Properties[i][2][0] = MidX*0.5;
                    break;
                case 2:
                    MinY = ENCL.Limits[1][1];
                    MaxY = ENCL.Limits[1][1] + 2*fabs(ENCL.Limits[1][1] - ENCL.Limits[0][1]);
                    MidY = 0.5*(MaxY+MinY);
                    while (((fabs(MaxY-MinY))/fabs(MidY))>(OnePercentTol))
                    {
                        MidY = 0.5*(MaxY+MinY);
                        charge = 0;
                        for (p = 1; p<=NumPanels;p++)
                        {
                            DielectricZ = DI.Properties[i][0][3];
                            DiffX = (AbsMidX - Charge.Details[1][p])*(AbsMidX - Charge.Details[1][p]);
                            DiffY = (MidY - Charge.Details[2][p])*(MidY - Charge.Details[2][p]);
                            DiffZ = (DielectricZ - Charge.Details[3][p])*(DielectricZ - Charge.Details[3][p]);
                            distance = (DiffX+DiffY+DiffZ);
                            charge = charge + ((Charge.Details[0][p])/(distance));
                        }
                        if (charge>0) //find point where charge is not significantly changing
                        {
                            MinY = MidY;
                        }
                        if (charge<0)
                        {
                            MaxY = MidY;
                        }
                    }
                    DI.Properties[i][2][1] = MidY*0.5;
                    DI.Properties[i][3][1] = MidY*0.5;
                    break;
                case 3:
                    MinX = ENCL.Limits[1][0];
                    MaxX = ENCL.Limits[1][0] + 2*fabs(ENCL.Limits[1][0] - ENCL.Limits[0][0]);
                    while (((fabs(MaxX-MinX))/fabs(MidX))>(OnePercentTol))
                    {
                        MidX = 0.5*(MaxX+MinX);
                        charge = 0;
                        for (p = 1; p<=NumPanels;p++)
                        {
                            DielectricZ = DI.Properties[i][0][3];
                            DiffX = (MidX - Charge.Details[1][p])*(MidX - Charge.Details[1][p]);
                            DiffY = (AbsMidY - Charge.Details[2][p])*(AbsMidY - Charge.Details[2][p]);
                            DiffZ = (DielectricZ - Charge.Details[3][p])*(DielectricZ - Charge.Details[3][p]);
                            distance = (DiffX+DiffY+DiffZ);
                            charge = charge + ((Charge.Details[0][p])/(distance));
                        }
                        if (charge>0) //find point where charge is not significantly changing
                        {
                            MinX = MidX;
                        }
                        if (charge<0)
                        {
                            MaxX = MidX;
                        }
                    }
                    DI.Properties[i][3][0] = MidX*0.5;
                    DI.Properties[i][4][0] = MidX*0.5;
                    break;
                case 4:
                    MaxY = ENCL.Limits[0][1];
                    MinY = ENCL.Limits[0][1] - 2*fabs(ENCL.Limits[1][1] - ENCL.Limits[0][1]);
                    while (((fabs(MaxY-MinY))/fabs(MidY))>(OnePercentTol))
                    {
                        MidY = 0.5*(MaxY+MinY);
                        charge = 0;
                        for (p = 1; p<=NumPanels;p++)
                        {
                            DielectricZ = DI.Properties[i][0][3];
                            DiffX = (AbsMidX - Charge.Details[1][p])*(AbsMidX - Charge.Details[1][p]);
                            DiffY = (MidY - Charge.Details[2][p])*(MidY - Charge.Details[2][p]);
                            DiffZ = (DielectricZ - Charge.Details[3][p])*(DielectricZ - Charge.Details[3][p]);
                            distance = (DiffX+DiffY+DiffZ);
                            charge = charge + ((Charge.Details[0][p])/(distance));
                        }
                        if (charge>0) //find point where charge is not significantly changing
                        {
                            MinY = MidY;
                        }
                        if (charge<0)
                        {
                            MaxY = MidY;
                        }
                    }
                    DI.Properties[i][4][1] = MidY*0.5;
                    DI.Properties[i][1][1] = MidY*0.5;
                    break;
            }

        }
    }
}