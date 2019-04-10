//
// Created by ubuntu on 10/10/18.
//

#include "Global.h"

FILE *filein;
FILE *fileout;
char GlobalBuffer[300];
int i,j,k,l,m,n;

uint32_t DoPlane(uint32_t Pcount, char *write, int16_t LineLoop);
uint32_t DoFaceBound(uint32_t Fcount_DoFaceBound, char *write, int16_t LineLoop);
struct Axis3D Do3DAxis(int16_t LineLoop);
uint32_t DoOrientedEdge(int* RefCount,uint32_t Fcount_DoOrientedEdge, int16_t LineRef, char* write);
struct EdgeCurve DoEdgeCurve(int16_t LineRef);
void enclosure(void);


int StepGen(char * file_geo, int instance);
int StepGen(char * file_geo, int instance)
{

    int busy = 0;
    int ConductorCount, PanelCount = 0;
    int8_t panelnumber = 0;
    int process1 = 0; // READING THE FILE
    uint8_t ID = 0;
    char filenamein[30];
    char filenameout[30];
    char LocalBuffer[300];
    char *token;
    const char equals[] = "=";
    const char comma[] = ",";
    const char curlyb[] = "{";
    const char curlybend[] = "}";
    const char space[] = " ";
    char dielectric[10];
    char state[1] = "y";
    int NumDielectric = 1;

//START READING PROCESS
    filein = fopen(file_geo,"r");
    process1 = 1;
    while (process1)
    {
        if (filein)
        {
            memset(LocalBuffer,0,sizeof(LocalBuffer));
            fgets(LocalBuffer,300,filein);
        }
        ID = LocalBuffer[0];

// WHAT CATEGORY OF DATA ARE WE RECEIVING?
        switch (ID)
        {
            case 'l': // LENGTH
                NumCharacteristicLengths++;
                token = strtok(LocalBuffer,space);
                strcat(LC.namestring[NumCharacteristicLengths],token);
                LC.name[NumCharacteristicLengths] = LC.namestring[NumCharacteristicLengths];
                token = strtok(NULL,equals);
                LC.value[NumCharacteristicLengths] = atof(token);
                break;
            case 'P':
                if(LocalBuffer[1] == 'l') // PLANE SURFACE
                {
                    NumPlaneSurface++;
                    token = strtok(LocalBuffer,curlyb);
                    token = strtok(NULL,comma);
                    PS.LineLoop[NumPlaneSurface] = NumPlaneSurface;
                }
                else // POINT
                {
                    NumCartesianPoints++;
                    token = strtok(LocalBuffer,curlyb);
                    for(i=1;i<4;i++)
                    {
                        token = strtok(NULL,comma);
                        CP.Coordinate[i][NumCartesianPoints] = atof(token);
                    }
                    token = strtok(NULL,curlybend);
                    token++;
                    for(i=1;i<5;i++)
                    {
                        if (LC.name[i] != 0x0)
                        {
                            if((strcmp(token,LC.name[i])) == 0)
                            {
                                CP.Coordinate[4][NumCartesianPoints] = LC.value[i];
                            }
                        }
                    }
                }
                break;
            case 'L':
                if(LocalBuffer[5] == 'L') // LINE LOOP
                {
                    NumLineLoops++;
                    token = strtok(LocalBuffer,curlyb);
                    busy = 1;
                    i = 1;
                    token = strtok(NULL,comma);
                    while(busy)
                    {
                        LL.Line[i][NumLineLoops] = atoi(token);
                        i++;
                        token = strtok(NULL,comma);
                        if(token == NULL)
                        {
                            busy = 0;
                            LL.Line[0][NumLineLoops] = i-1;
                        }
                    }

                }
                else // LINE
                {
                    NumLines++;
                    token = strtok(LocalBuffer, curlyb);
                    token = strtok(NULL,comma);
                    L.CartesianPoint[1][NumLines] = atoi(token);
                    token = strtok(NULL,comma);
                    L.CartesianPoint[2][NumLines] = atoi(token);
                }
                break;
            case 'S': //SURFACE LOOP
                NumSurfaceLoops++;
                int surfacecount = 0;
                token = strtok(LocalBuffer, curlyb);
                busy = 1;
                i = 1;
                token = strtok(NULL,comma);
                while(busy)
                {
                    SL.PlaneSurface[i][NumSurfaceLoops] = atoi(token);
                    surfacecount++;
                    i++;
                    token = strtok(NULL,comma);
                    if(token == NULL)
                    {
                        busy = 0;
                    }
                }
                SL.count[NumSurfaceLoops] = surfacecount;
                break;
            case 'V': //SolidVolume
                NumSolidVolumes++;
                token = strtok(LocalBuffer, curlyb);
                token = strtok(NULL,comma);
                V.SurfaceLoop[NumSolidVolumes] = NumSolidVolumes;
                break;
            default:
                if((NumPlaneSurface>=1)&&(NumSolidVolumes != 0))
                {
                    process1 = 0;
                }
                break;
        }
    }
//NOW ALL DATA IS CATALOGUED, LETS DETERMINE THE "BOX" IN WHICH THE CIRCUIT SITS
    enclosure();
//NOW, LET'S GENERATE DIELECTRIC SURFACES WHERE SPECIFIED

//NOW, LETS READ AND INTERPRET THE CONDUCTORS
    for (ConductorCount = 1; ConductorCount<NumSolidVolumes+2; ConductorCount++) // Conductor Count always represents the current volume number we are busy with
    {
        int count = 0;
        uint32_t FOBcount = 0;
        uint32_t Pcount = 0;
        char write[300] = "";
        if(NumSolidVolumes == 0)
        {
            Conductor.NumAdvancedFaces = NumPlaneSurface;
        }
        else
        {
            Conductor.NumAdvancedFaces = SL.count[V.SurfaceLoop[ConductorCount]]; //amount of faces we are piecing together
        }
        for (PanelCount = 1; PanelCount<Conductor.NumAdvancedFaces+1;PanelCount++) // j always represents the surface we are busy with
        {
            sprintf(filenameout,"C%dP%d.stp",ConductorCount,PanelCount);
            fileout = fopen(filenameout,"w+");
            fputs(metadata1,fileout);
            fputs(metadata2,fileout);
            count = 34;
            Pcount = 35;
            FOBcount = 40;
            sprintf(write,"#%d=ADVANCED_FACE('',(#%d),#%d,.F.);\n",count,FOBcount,Pcount);
            fputs(write,fileout);
            memset(write,0,sizeof(write));
            count = DoPlane(Pcount,write, PS.LineLoop[SL.PlaneSurface[PanelCount][ConductorCount]]); //takes the current count, the string to write to and the line loop to reference
            count = DoFaceBound(count,write, PS.LineLoop[SL.PlaneSurface[PanelCount][ConductorCount]]); // takes the current count, the string to write and the line loop reference
            fputs(metadata3,fileout);
            fclose(fileout);
        }
        if(Conductor.NumAdvancedFaces == 0)
        {
            ConductorCount = NumSolidVolumes+3;
        }
    }
    fclose(filein);
    return(0);
}
uint32_t DoFaceBound(uint32_t Fcount_DoFaceBound, char *write, int16_t LineLoop)
{
    int i;
    int EdgeLoopRef[LL.Line[0][LineLoop]];
    Fcount_DoFaceBound++;
    sprintf(write,"#%d=FACE_OUTER_BOUND('',#%d,.T.);\n",Fcount_DoFaceBound-1,Fcount_DoFaceBound);
    fputs(write,fileout);
    memset(write,0,sizeof(write));
    for(i = 1;i<LL.Line[0][LineLoop];i++)
    {
        EdgeLoopRef[i] = Fcount_DoFaceBound+i;
    }
    sprintf(write,"#%d=EDGE_LOOP('',(",Fcount_DoFaceBound);
    for(i = 1; i<LL.Line[0][LineLoop];i++)
    {
        sprintf(GlobalBuffer,"#%d,",Fcount_DoFaceBound+i);
        strcat(write,GlobalBuffer);
    }
    sprintf(GlobalBuffer,"#%d));\n",Fcount_DoFaceBound+i);
    strcat(write,GlobalBuffer);
    fputs(write,fileout);
    memset(write,0,sizeof(write));
    memset(GlobalBuffer,0,sizeof(GlobalBuffer));
    for(i = 1; i<LL.Line[0][LineLoop]+1;i++)
    {
        sprintf(write,"#%d=ORIENTED_EDGE('',*,*,#%d,.T.);\n",Fcount_DoFaceBound+i,Fcount_DoFaceBound+LL.Line[0][LineLoop]+i);
        fputs(write,fileout);
        memset(write,0,sizeof(write));
    }
    for(i = 1; i<LL.Line[0][LineLoop];i++)
    {
        sprintf(write,"#%d=EDGE_CURVE('',#%d,#%d,#%d,.T.);\n",(Fcount_DoFaceBound+LL.Line[0][LineLoop]+i), (Fcount_DoFaceBound+(2*(LL.Line[0][LineLoop]))+i),(Fcount_DoFaceBound+2*(LL.Line[0][LineLoop])+i+1),(Fcount_DoFaceBound+3*(LL.Line[0][LineLoop])+i));
        fputs(write,fileout);
        memset(write,0,sizeof(write));
    }
    sprintf(write,"#%d=EDGE_CURVE('',#%d,#%d,#%d,.T.);\n",(Fcount_DoFaceBound+LL.Line[0][LineLoop]+i), (Fcount_DoFaceBound+2*(LL.Line[0][LineLoop])+i),(Fcount_DoFaceBound+2*(LL.Line[0][LineLoop])+1),(Fcount_DoFaceBound+3*(LL.Line[0][LineLoop])+i));
    fputs(write,fileout);
    memset(write,0,sizeof(write));
    for(i = 1; i<=LL.Line[0][LineLoop];i++)
    {
        sprintf(write,"#%d=VERTEX_POINT('',#%d);\n",Fcount_DoFaceBound+2*(LL.Line[0][LineLoop])+i,Fcount_DoFaceBound+4*(LL.Line[0][LineLoop])+(i-1)*4+1);
        fputs(write,fileout);
        memset(write,0,sizeof(write));
    }
    for(i = 1; i<LL.Line[0][LineLoop]+1;i++)
    {
        sprintf(write,"#%d=LINE('',#%d,#%d);\n",Fcount_DoFaceBound+3*(LL.Line[0][LineLoop])+i,Fcount_DoFaceBound+4*((LL.Line[0][LineLoop]))+((i-1)*4)+2,Fcount_DoFaceBound+4*((LL.Line[0][LineLoop]))+((i-1)*4)+3);
        fputs(write,fileout);
        memset(write,0,sizeof(write));
    }
    Fcount_DoFaceBound = Fcount_DoFaceBound+4*(LL.Line[0][LineLoop])+1;
    for(i = 1;i<LL.Line[0][LineLoop]+1;i++) //For each edge (line), do the following required steps
    {
        Fcount_DoFaceBound = DoOrientedEdge(&EdgeLoopRef[i],Fcount_DoFaceBound,LL.Line[i][LineLoop],write);
    }
    return Fcount_DoFaceBound;
}

uint32_t DoOrientedEdge(int* RefCount,uint32_t Fcount_DoOrientedEdge, int16_t LineRef, char* write)
{
    EdgeCurve = DoEdgeCurve(LineRef);
    sprintf(write,"#%d=CARTESIAN_POINT('',(%.20f,%.20f,%.20f));\n",Fcount_DoOrientedEdge,EdgeCurve.VERTEXSTART[1],EdgeCurve.VERTEXSTART[2],EdgeCurve.VERTEXSTART[3]);
    fputs(write,fileout);
    memset(write,0,sizeof(write));
    //sprintf(write,"#%d=CARTESIAN_POINT('',(%f,%f,%f));\n",Fcount+6,EdgeCurve.VERTEXEND[1],EdgeCurve.VERTEXEND[2],EdgeCurve.VERTEXEND[3]);
    //fputs(write,fileout);
    //memset(write,0,sizeof(write));
    sprintf(write,"#%d=CARTESIAN_POINT('',(%.20f,%.20f,%.20f));\n",Fcount_DoOrientedEdge+1,EdgeCurve.VERTEXEND[1],EdgeCurve.VERTEXEND[2],EdgeCurve.VERTEXEND[3]);
    fputs(write,fileout);
    memset(write,0,sizeof(write));
    sprintf(write,"#%d=VECTOR('',#%d,1.);\n",Fcount_DoOrientedEdge+2,Fcount_DoOrientedEdge+3);
    fputs(write,fileout);
    memset(write,0,sizeof(write));
    sprintf(write,"#%d=DIRECTION('',(%f,%f,%f));\n",Fcount_DoOrientedEdge+3,EdgeCurve.VECTOR[1],EdgeCurve.VECTOR[2],EdgeCurve.VECTOR[3]);
    fputs(write,fileout);
    memset(write,0,sizeof(write));
    Fcount_DoOrientedEdge = Fcount_DoOrientedEdge+4;
    return Fcount_DoOrientedEdge;
}



struct EdgeCurve DoEdgeCurve(int16_t LineRef)
{
    struct EdgeCurve element;
    for(l=1;l<4;l++)
    {
        if(LineRef<0)
        {
            element.VERTEXSTART[l] = CP.Coordinate[l][L.CartesianPoint[2][abs(LineRef)]];
            element.VERTEXEND[l] = CP.Coordinate[l][L.CartesianPoint[1][abs(LineRef)]];
            element.CPOINT[l] = element.VERTEXSTART[l];
        }
        else
        {
            element.VERTEXSTART[l] = CP.Coordinate[l][L.CartesianPoint[1][(LineRef)]];
            element.VERTEXEND[l] = CP.Coordinate[l][L.CartesianPoint[2][(LineRef)]];
            element.CPOINT[l] = element.VERTEXSTART[l];
        }

        if((element.VERTEXSTART[l])==(element.VERTEXEND[l]))
        {
            element.VECTOR[l] = 0;
        }
        else
        {
            element.VECTOR[l] = (((element.VERTEXSTART[l])-(element.VERTEXEND[l])))<0 * (-1);
            if(element.VECTOR[l] == 0)
            {
                element.VECTOR[l] = 1;
            }
        }
    }
    return element;
}



uint32_t DoPlane(uint32_t Pcount, char *write, int16_t LineLoop)
{
    struct Axis3D axis;
    axis = Do3DAxis(LineLoop);
    sprintf(write,"#%d=PLANE('',#%d);\n",Pcount,Pcount+1);
    fputs(write,fileout);
    memset(write,0,sizeof(write));
    sprintf(write,"#%d=AXIS2_PLACEMENT_3D('',#%d,#%d,#%d);\n",Pcount+1,Pcount+2,Pcount+3,Pcount+4);
    fputs(write,fileout);
    memset(write,0,sizeof(write));
    sprintf(write,"#%d=CARTESIAN_POINT('',(%.20f,%.20f,%.20f));\n",Pcount+2,CP.Coordinate[1][axis.CP],CP.Coordinate[2][axis.CP],CP.Coordinate[3][axis.CP]);
    fputs(write,fileout);
    memset(write,0,sizeof(write));
    sprintf(write,"#%d=DIRECTION('',(%f,%f,%f));\n",Pcount+3,axis.REFUNITVECTOR[1],axis.REFUNITVECTOR[2],axis.REFUNITVECTOR[3]);
    fputs(write,fileout);
    memset(write,0,sizeof(write));
    sprintf(write,"#%d=DIRECTION('',(%f,%f,%f));\n",Pcount+4,axis.AXISUNITVECTOR[1],axis.AXISUNITVECTOR[2],axis.AXISUNITVECTOR[3]);
    fputs(write,fileout);
    memset(write,0,sizeof(write));
    return Pcount+5;
}

struct Axis3D Do3DAxis(int16_t LineLoop)
{
    struct Axis3D Answer;
    int find1 = (LL.Line[1][LineLoop]);
    int find2 = (LL.Line[2][LineLoop]);
    int i,j,k,flag = 0;
    int CPstart1 =0, CPend2 =0;
    int dotprod = 0;
    float float1,float2;

    if(find1<0)
    {
        CPstart1 = L.CartesianPoint[2][abs(find1)];
        Answer.CP = L.CartesianPoint[1][abs(find1)];
    }
    else
    {
        CPstart1 = L.CartesianPoint[1][abs(find1)];
        Answer.CP = L.CartesianPoint[2][abs(find1)];
    }
    if(find2<0)
    {
        CPend2 = L.CartesianPoint[1][abs(find2)];
    }
    else
    {
        CPend2 = L.CartesianPoint[2][abs(find2)];
    }


    for(i = 1; i<4; i++)
    {
        if((CP.Coordinate[i][CPstart1]) ==(CP.Coordinate[i][CPend2]))//)/fabs(CP.Coordinate[i][CPend2]))<Tol)
        {
            dotprod = i;
        }
    }
    Answer.REFUNITVECTOR[1] = 0;
    Answer.REFUNITVECTOR[2] = 0;
    Answer.REFUNITVECTOR[3] = 0;
    Answer.REFUNITVECTOR[dotprod] = 1;
    for(i = 1;i<4;i++) //for each x y z
    {
        float1 = CP.Coordinate[i][L.CartesianPoint[1][find1]];
        float2 = CP.Coordinate[i][L.CartesianPoint[2][find1]];
        if(float1==float2)
        {
            Answer.AXISUNITVECTOR[i] = 0;
        }
        else
        {
            Answer.AXISUNITVECTOR[i] = (float1-float2)<0 * (-1);
            if(Answer.AXISUNITVECTOR[i] == 0)
            {
                Answer.AXISUNITVECTOR[i] = 1;
            }
        }
    }
    return Answer;
}

void enclosure(void) {
    int busy, i, j;
    int countnow = 0, countnext,holdvalue = 0, point = 0;
    float maxvalue = 0, minvalue = 0,  midvalue = 0;
    float DifferenceX = 0, DifferenceY = 0;
    for (point = 1; point <= 3; point++)
    {
        busy = 1;
        maxvalue = 0;
        minvalue = 1000;
        countnext = 1;
        while (busy)
        {
            countnow++;
            countnext++;
            if (CP.Coordinate[point][countnow] >= maxvalue)
            {
                maxvalue = CP.Coordinate[point][countnow];
            }
            if (CP.Coordinate[point][countnow] <= minvalue)
            {
                minvalue = CP.Coordinate[point][countnow];

            }
            if (countnow == NumCartesianPoints) {
                countnow = 0;
                busy = 0;
            }
        }

        midvalue = 0.5 * (maxvalue - minvalue);
        holdvalue = point-1;
        ENCL.Limits[0][holdvalue] = minvalue;
        ENCL.Limits[1][holdvalue] = maxvalue;
        if (point == 1)
        {
            AbsMidX = midvalue;
        }
        if (point == 2)
        {
            AbsMidY = midvalue;
        }
    }

//    DifferenceX = (MaxX - MinX)/500;
//    DifferenceY = (MaxY - MinY)/500;
//    for (j = 1; j <= 500; j++)
//    {
//        DI.ElectricPotential[0][j] = MinY + j*DifferenceY;
//    }
//    for (j = 1; j <= 500; j++)
//    {
//        DI.ElectricPotential[j][0] = MinX + j*DifferenceX;
//    }
    for (j = 1; j <= NumDielectric; j++)
    {
        DI.Properties[NumDielectric][0][3] = ENCL.Limits[0][2] + DI.Properties[NumDielectric][0][3];
        DI.Properties[NumDielectric][1][3] = ENCL.Limits[0][2] + DI.Properties[NumDielectric][1][3];// + ENCL.Limits[0][2];
        DI.Properties[NumDielectric][2][3] = ENCL.Limits[0][2] + DI.Properties[NumDielectric][2][3];// + ENCL.Limits[0][2];
        DI.Properties[NumDielectric][3][3] = ENCL.Limits[0][2] + DI.Properties[NumDielectric][3][3];// + ENCL.Limits[0][2];
    }
}