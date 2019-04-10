//
// Created by ubuntu on 10/14/18.
//
#include "Global.h"

FILE *filein;
FILE *fileout;
char GlobalBuffer[300];
int i,j,k,l,m,n;

int DielectricStepGen(char * file_name);
int DielectricStepGen(char * file_name)
{
    char filenameout[20];
    char write[200];
    for(m = 1; m<=NumDielectric;m++) {
        sprintf(filenameout, "D%d.stp", m);
        fileout = fopen(filenameout, "w+");
        fputs(metadata1, fileout);
        fputs(metadata2, fileout);
        int count = 34;
        int Pcount = 35;
        int FOBcount = 40;
        sprintf(write, "#%d=ADVANCED_FACE('',(#%d),#%d,.F.);\n", count, FOBcount, Pcount);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=PLANE('',#%d);\n", Pcount, Pcount + 1);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=AXIS2_PLACEMENT_3D('',#%d,#%d,#%d);\n", Pcount + 1, Pcount + 2, Pcount + 3, Pcount + 4);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=CARTESIAN_POINT('',(%.20f,%.20f,%.20f));\n", Pcount + 2, DI.Properties[m][1][0],DI.Properties[m][1][1],DI.Properties[m][1][2]);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=DIRECTION('',(%f,%f,%f));\n", Pcount + 3, 0.000, 0.000, 1.000);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=DIRECTION('',(%f,%f,%f));\n", Pcount + 4, 0.000, 1.000, 0.000);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        Pcount = Pcount + 5;
        sprintf(write, "#%d=FACE_OUTER_BOUND('',#%d,.T.);\n", Pcount, Pcount + 1);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=EDGE_LOOP('',(", Pcount + 1);
        Pcount = Pcount + 1;
        for (i = 1; i < 4; i++) {
            sprintf(GlobalBuffer, "#%d,", Pcount + i);
            strcat(write, GlobalBuffer);
        }
        sprintf(GlobalBuffer, "#%d));\n", Pcount + i);
        strcat(write, GlobalBuffer);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        memset(GlobalBuffer, 0, sizeof(GlobalBuffer));
        for (i = 1; i <= 4; i++) {
            sprintf(write, "#%d=ORIENTED_EDGE('',*,*,#%d,.T.);\n", Pcount + i, Pcount + 4 + i);
            fputs(write, fileout);
            memset(write, 0, sizeof(write));
        }
        Pcount = Pcount + 4;
        for (i = 1; i < 4; i++) {
            sprintf(write, "#%d=EDGE_CURVE('',#%d,#%d,#%d,.T.);\n", (Pcount + i), (Pcount + 4 + i), (Pcount + 5 + i),
                    (Pcount + 8 + i));
            fputs(write, fileout);
            memset(write, 0, sizeof(write));
        }
        sprintf(write, "#%d=EDGE_CURVE('',#%d,#%d,#%d,.T.);\n", (Pcount + i), (Pcount + 4 + i), (Pcount + 5),
                (Pcount + 8 + i));
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        Pcount = Pcount + 4;
        for (i = 1; i <= 4; i++) {
            sprintf(write, "#%d=VERTEX_POINT('',#%d);\n", Pcount + i, Pcount + (i - 1) * 4 + 9);
            fputs(write, fileout);
            memset(write, 0, sizeof(write));
        }
        Pcount = Pcount + 4;
        for (i = 1; i <= 4; i++) {
            sprintf(write, "#%d=LINE('',#%d,#%d);\n", Pcount + i, Pcount + ((i - 1) * 4) + 6,
                    Pcount + ((i - 1) * 4) + 7);
            fputs(write, fileout);
            memset(write, 0, sizeof(write));
        }
        Pcount = Pcount + 5;
        sprintf(write, "#%d=CARTESIAN_POINT('',(%.20f,%.20f,%.20f));\n", Pcount, DI.Properties[m][1][0],DI.Properties[m][1][1],DI.Properties[m][1][2]);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=CARTESIAN_POINT('',(%.20f,%.20f,%.20f));\n", Pcount + 1, DI.Properties[m][2][0],DI.Properties[m][2][1],DI.Properties[m][2][2]);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=VECTOR('',#%d,1.);\n", Pcount + 2, Pcount + 3);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=DIRECTION('',(%f,%f,%f));\n", Pcount + 3, EdgeCurve.VECTOR[1], EdgeCurve.VECTOR[2], EdgeCurve.VECTOR[3]);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        Pcount = Pcount + 4;
        sprintf(write, "#%d=CARTESIAN_POINT('',(%.20f,%.20f,%.20f));\n", Pcount, DI.Properties[m][2][0],DI.Properties[m][2][1],DI.Properties[m][2][2]);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=CARTESIAN_POINT('',(%.20f,%.20f,%.20f));\n", Pcount + 1, DI.Properties[m][3][0],DI.Properties[m][3][1],DI.Properties[m][3][2]);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=VECTOR('',#%d,1.);\n", Pcount + 2, Pcount + 3);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=DIRECTION('',(%f,%f,%f));\n", Pcount + 3, EdgeCurve.VECTOR[1], EdgeCurve.VECTOR[2], EdgeCurve.VECTOR[3]);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        Pcount = Pcount + 4;
        sprintf(write, "#%d=CARTESIAN_POINT('',(%.20f,%.20f,%.20f));\n", Pcount,DI.Properties[m][3][0],DI.Properties[m][3][1],DI.Properties[m][3][2]);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=CARTESIAN_POINT('',(%.20f,%.20f,%.20f));\n", Pcount + 1,DI.Properties[m][4][0],DI.Properties[m][4][1],DI.Properties[m][4][2]);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=VECTOR('',#%d,1.);\n", Pcount + 2, Pcount + 3);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=DIRECTION('',(%f,%f,%f));\n", Pcount + 3, EdgeCurve.VECTOR[1], EdgeCurve.VECTOR[2], EdgeCurve.VECTOR[3]);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        Pcount = Pcount + 4;
        sprintf(write, "#%d=CARTESIAN_POINT('',(%.20f,%.20f,%.20f));\n", Pcount, DI.Properties[m][4][0],DI.Properties[m][4][1],DI.Properties[m][4][2]);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=CARTESIAN_POINT('',(%.20f,%.20f,%.20f));\n", Pcount + 1, DI.Properties[m][1][0],DI.Properties[m][1][1],DI.Properties[m][1][2]);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=VECTOR('',#%d,1.);\n", Pcount + 2, Pcount + 3);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        sprintf(write, "#%d=DIRECTION('',(%f,%f,%f));\n", Pcount + 3, EdgeCurve.VECTOR[1], EdgeCurve.VECTOR[2],EdgeCurve.VECTOR[3]);
        fputs(write, fileout);
        memset(write, 0, sizeof(write));
        fputs(metadata3, fileout);
        fclose(fileout);
    }
}