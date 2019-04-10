//
// Created by ubuntu on 10/10/18.
//


#ifndef QUIGEN0_0_GLOBAL_H
#define QUIGEN0_0_GLOBAL_H

#endif //QUIGEN0_0_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include "GlobalStructures.h"

extern float CharLength;
extern float OnePercentTol;
extern float TenPercentTol;
extern float AbsMidX;
extern float AbsMidY;

extern int flag;

extern char equals[];
extern char comma[];
extern char curlyb[];
extern char curlybend[];
extern char space[];
extern char openbracket[];
extern char closebracket[];

extern uint32_t NumCharacteristicLengths;
extern uint32_t NumCartesianPoints;
extern uint32_t NumLines;
extern uint32_t NumLineLoops;
extern uint32_t NumPlaneSurface;
extern uint32_t NumSurfaceLoops;
extern uint32_t NumSolidVolumes;
extern uint32_t NumPanels;
extern uint32_t NumDielectric;

extern char metadata1[];
extern char metadata2[];
extern char metadata3[];

//StepGen.c
int StepGen(char * file_geo, int instance);

//QuiGen.c
int QuiGen(char * file_postmesh);

//DielectricBound.c
int DielectricBoundary(char * file_geo, int instance);

//DielectricGen.c
int DielectricStepGen(char * file_name);

int ListGen(char * file_name);
void writeBMP(float *Matrix, int Matrix_dimension);