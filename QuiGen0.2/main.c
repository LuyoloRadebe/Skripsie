//
// Created by ubuntu on 10/7/18.
//

/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Distribution: The file is distributed "as is", without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. Distribution of this file (unmodified or modified) is not
**  permitted. Atollic AB permit registered Atollic TrueSTUDIO(R) users the
**  rights to distribute the assembled, compiled & linked contents of this
**  file as part of an application binary file, provided that it is built
**  using the Atollic TrueSTUDIO(R) toolchain.
**
**
*****************************************************************************
*/

#include "Global.h"

float CharLength = 0.001;
float OnePercentTol = 0.01;
float TenPercentTol = 0.1;
float AbsMidX = 0, AbsMidY = 0;

int flag;

uint32_t NumCharacteristicLengths = 0;
uint32_t NumCartesianPoints = 0;
uint32_t NumLines = 0;
uint32_t NumLineLoops = 0;
uint32_t NumPlaneSurface = 0;
uint32_t NumSurfaceLoops = 0;
uint32_t NumSolidVolumes = 0;
uint32_t NumPanels = 0;
uint32_t NumDielectric = 0;

char equals[] = "=";
char comma[] = ",";
char curlyb[] = "{";
char curlybend[] = "}";
char space[] = " ";
char openbracket[] = "(";
char closebracket[] = ")";

char metadata1[] = "ISO-10303-21;\nHEADER;\nFILE_SCHEMA ((\'AUTOMOTIVE_DESIGN { 1 0 10303 214 3 1 1 }\'));\nENDSEC;\nDATA;\n#10=SHAPE_REPRESENTATION_RELATIONSHIP('None','relationship between cube-None and cube-None',#22,#11);\n#11=ADVANCED_BREP_SHAPE_REPRESENTATION('cube-None',(#32),#187);\n#12=SHAPE_DEFINITION_REPRESENTATION(#13,#22);\n#13=PRODUCT_DEFINITION_SHAPE('','',#14);\n#14=PRODUCT_DEFINITION(' ','',#16,#15);\n#15=PRODUCT_DEFINITION_CONTEXT('part definition',#21,'design');\n#16=PRODUCT_DEFINITION_FORMATION_WITH_SPECIFIED_SOURCE(' ',' ',#18, .NOT_KNOWN.);\n#17=PRODUCT_RELATED_PRODUCT_CATEGORY('part','',(#18));\n#18=PRODUCT('cube','cube',' ',(#19));\n#19=PRODUCT_CONTEXT(' ',#21,'mechanical');";
char metadata2[] = "\n#20=APPLICATION_PROTOCOL_DEFINITION('international standard','automotive_design',2010,#21);\n#21=APPLICATION_CONTEXT('core data for automotive mechanical design processes');\n#22=SHAPE_REPRESENTATION('cube-None',(#130),#187);\n#23=PRESENTATION_LAYER_ASSIGNMENT('1','Layer 1',(#32));\n#24=STYLED_ITEM('',(#25),#32);\n#25=PRESENTATION_STYLE_ASSIGNMENT((#26));\n#26=SURFACE_STYLE_USAGE(.BOTH.,#27);\n#27=SURFACE_SIDE_STYLE('',(#28));\n#28=SURFACE_STYLE_FILL_AREA(#29);\n#29=FILL_AREA_STYLE('',(#30));\n#30=FILL_AREA_STYLE_COLOUR('',#31);\n#31=COLOUR_RGB('Orange',1.,0.6,0.);\n#32=MANIFOLD_SOLID_BREP('',#33);\n#33=CLOSED_SHELL('',(#34));\n";
char metadata3[] = "ENDSEC;\nEND-ISO-10303-21;";

int main() {

    FILE *filein;
    FILE *fileout;
    char file_gmsh_quigen[20];
    char file_user_gmsh[20];
    char file_name[20];
    char command_do[80];
    int errorflag = 0;
    int i,k;
    char user_prompt[20];
    char user_busy[5] = "y";
    float volume;

    flag = 1;

    DI.Properties[0][0][3] = 0;
    DI.Properties[0][1][3] = 0;
    DI.Properties[0][2][3] = 0;
    DI.Properties[0][3][3] = 0;
    // Read filename
    printf("Enter the name of the Geo file you wish to convert: ");
    fgets(file_user_gmsh, sizeof(file_user_gmsh), stdin);
    file_user_gmsh[strcspn(file_user_gmsh, "\n")] = 0;
    strcpy(file_gmsh_quigen, file_user_gmsh);
    file_gmsh_quigen[strcspn(file_gmsh_quigen, ".")] = 0;
    strcpy(file_name, file_gmsh_quigen);
	while(user_busy[0] == ('y'|'Y'))
	{
	    NumDielectric++;
		printf("Enter Dielectric %d constant: \n",NumDielectric);
		fgets(user_prompt, sizeof(user_prompt),stdin);
		DI.Properties[NumDielectric][0][0] = atof(user_prompt);
		printf("Enter Dielectric %d thickness: \n",NumDielectric);
		fgets(user_prompt, sizeof(user_prompt),stdin);
		DI.Properties[NumDielectric][0][3] = atof(user_prompt)+ DI.Properties[NumDielectric-1][0][3];
		DI.Properties[NumDielectric][1][3] = atof(user_prompt)+ DI.Properties[NumDielectric-1][1][3];
		DI.Properties[NumDielectric][2][3] = atof(user_prompt)+ DI.Properties[NumDielectric-1][2][3];
		DI.Properties[NumDielectric][3][3] = atof(user_prompt)+ DI.Properties[NumDielectric-1][3][3];
		printf("Continue? Y/N \n");
		fgets(user_busy, sizeof(user_busy),stdin);
		if(NumDielectric>10)
		{
            user_busy[0] = 'n';
			printf("Error: Maximum number of dielectric layers exceeded!");
		}
	}
    DI.Properties[NumDielectric+1][0][0] = 1; //set default to air
    // Open Gmesh and Mesh the network of conductors to create step files
    errorflag = StepGen(file_user_gmsh, 1);
    volume = (ENCL.Limits[1][0] - ENCL.Limits[0][0])*(ENCL.Limits[1][1] - ENCL.Limits[0][1])*(ENCL.Limits[1][2] - ENCL.Limits[0][2]);
    CharLength = 10000*cbrt(volume);

    for (i = 1; i <= NumSolidVolumes; i++)
    {
        for (k = 1; k <= SL.count[i]; k++)
        {
            sprintf(command_do, "./gmsh -2 C%dP%d.stp -clmax %.3e -o C%dP%d -format pos", i,k,CharLength,i,k);
            system(command_do);
            memset(command_do,0, sizeof(command_do));
        }
    }
    errorflag = QuiGen(file_gmsh_quigen);

    sprintf(command_do, "./fastcap -q %s.qui",file_name);
    system(command_do);
    printf("%s\n",strerror(errno));
    memset(command_do,0, sizeof(command_do));
    errorflag = DielectricBoundary(file_gmsh_quigen,1);
    errorflag = DielectricStepGen(file_gmsh_quigen);

    for(i = 1;i<=NumDielectric;i++)
    {
        sprintf(command_do, "./gmsh -2 D%d.stp -clmax %.3e -o D%d -format pos", i,CharLength,i);
        system(command_do);
        memset(command_do,0, sizeof(command_do));
    }
    for(i = 1;i<=NumDielectric;i++)
    {
        sprintf(command_do, "D%d", i);
        errorflag = QuiGen(command_do);
        memset(command_do,0, sizeof(command_do));
    }
    errorflag = ListGen(file_name);

    sprintf(command_do,"%s_ChargeData",file_name); //Rename Charge Data File
    rename("Charge Details",command_do);

    sprintf(command_do, "./fastcap -l%s.lst",file_name);
    system(command_do);
    printf("%s\n",strerror(errno));

    memset(user_prompt,0, sizeof(user_prompt));

    return 0;
}