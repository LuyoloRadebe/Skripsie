//
// Created by ubuntu on 10/10/18.
//

#ifndef QUIGEN0_0_GLOBAL_STRUCTURES_H
#define QUIGEN0_0_GLOBAL_STRUCTURES_H

#endif //QUIGEN0_0_GLOBAL_STRUCTURES_H


struct Enclosure{
    float Limits[2][3]; //[0 = LOWEST Z; 1 = HIGHEST Z][0 = LOWEST X LOWEST Y; 1 LOWEST X HIGHEST Y; 2 HIGHEST X HIGHEST Y; 3 HIGHEST X LOWEST Y]
} ENCL;

struct Dielectric{
    float Properties[11][5][4];
    float ElectricPotential[501][501];
    uint16_t Panel[5][100];
} DI;

//GEO CATALOGUED VARIABLES BEGIN
struct LengthCharacteristic {
    float value[5];
    char namestring[5][15];
    char *name[5];
} LC;

struct CartesianPoint {
    float Coordinate[5][400];
} CP;

struct Line {
    int CartesianPoint[3][400];
} L;

//format is Reference to Line number = [x] of Line Loop number [y]
struct LineLoop {
    int count[300];
    int Line[300][300];
} LL;

//format is Reference to Line Loop number [x] of Plane Surface number [y]
struct PlaneSurface {
    int LineLoop[300];
} PS;

//format is [Amount of Surfaces] = Plane Surface Number Reference of Surface loop number [y]
struct SurfaceLoop {
    int count[400];
    int PlaneSurface[100][400];
} SL;

struct SolidVolume {
    int SurfaceLoop[5];
} V;

//GEO CATALOGUED VARIABLES END

//STEP CATALOGUED VARIABLES BEGIN

struct Axis3D {
    int AXIS;
    int REF;
    int CP;
    float REFUNITVECTOR[4];
    float AXISUNITVECTOR[4];
} Axis3D;

struct EdgeCurve {
    float VERTEXSTART[4];
    float VERTEXEND[4];
    float CPOINT[4];
    float VECTOR[4];
} EdgeCurve;

struct Direction {
    float COORD[4][400];
};

struct OrientedEdges {
    int RefLine1[10];
    int RefLine2[10];
    int RefLine3[10];
    int RefLine4[10];
    struct EdgeCurve EC;
    int Orientation[10];
};

struct EdgeLoop{
    int RefLineLoop[10];
    struct OrientedEdges OE;
} EL;

struct Plane {
    struct Direction A;
    struct Direction B;
    struct CartesianPoint Point;
} Plane;

struct FaceOuterBound {
    int RefSurfaceLoop[10];
    struct EdgeLoop EL;
    int Orientation[10];
};

struct DefAdvancedFaces {
    struct FaceOuterBound Face;
    struct Plane Plane;
    int SameSense[10];
};

struct Element {
    int OpenClosed;
    int NumAdvancedFaces;
    struct DefAdvancedFaces Def;
} Conductor;
//STEP CATALOGUED VARIABLES END

struct ChargeDetails{
    float Details[4][10000];
} Charge;

char dielectric[10];