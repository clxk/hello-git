#ifndef __CLI_H__
#define __CLI_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef struct _polyPoint{
    float       *x;
    float       *y;
    //_polyPoint  *next;
}polyPoint;
typedef struct _polyLine{
    int         id;
    int         dir;
    int         num;
    polyPoint   *points;
    _polyLine   *next;
}polyLine;
typedef struct _hatchPoint{
    float       xs;
    float       ys;
    float       xe;
    float       ye;
    _hatchPoint *next;
}hatchPoint;
typedef struct _hatch{
    int         id;
    int         num;
    _hatchPoint *points;
    _hatch      *next;
}hatch;
typedef struct _layer{
    float       z;
    polyLine    *polyLines;
    hatch       *hatches;
    _layer      *next;
}layer;
typedef struct _object{
    char        header[256];
    layer       *layers;
}object;
#endif

