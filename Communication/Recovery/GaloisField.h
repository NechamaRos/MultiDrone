#pragma once
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <algorithm>

#define MAX(x, y) (x > y ? x : y)
#define MIN(x, y) (x < y ? x : y)
#define NULL 0

#define PRIM(x) PRIM_ ## x
#define PRIM_0 0
#define PRIM_1 0   
#define PRIM_2 0   
#define PRIM_3 0xb
#define PRIM_4 0x13
#define PRIM_5 0x25
#define PRIM_6 0x43
#define PRIM_7 0x83
#define PRIM_8 0x11d
#define PRIM_9 0x211
#define PRIM_10 0x409
#define PRIM_11 0x805
#define PRIM_12 0x1053
#define PRIM_13 0x201b
#define PRIM_14 0x402b
#define PRIM_15 0x8003
#define PRIM_16 0x1002d
#define PRIM_17 0x20009
#define PRIM_18 0x40027
#define PRIM_19 0x80027
#define PRIM_20 0x100009
#define PRIM_21 0x200005
#define PRIM_22 0x400003
#define PRIM_23 0x800021
#define PRIM_24 0x100001b
#define PRIM_25 0x2000009
#define PRIM_26 0x4000047
#define PRIM_27 0x8000027
#define PRIM_28 0x10000009
#define PRIM_29 0x20000005
#define PRIM_30 0x40000053
#define PRIM_31 0x80000009


typedef uint8_t RS_WORD;

class GaloisField
{
public:
    RS_WORD* powTable, * logTable;
    int fieldPower;
    RS_WORD characteristic, primitivePoly;
    int width, height, depth; 
   // GaloisField();
    GaloisField(int fieldPower);
    ~GaloisField();
    RS_WORD multNoLUT(RS_WORD a, RS_WORD b);
    RS_WORD add(RS_WORD a, RS_WORD b);
    RS_WORD sub(RS_WORD a, RS_WORD b);
    RS_WORD mult(RS_WORD a, RS_WORD b);
    RS_WORD div(RS_WORD a, RS_WORD b);
    RS_WORD pow(RS_WORD x, RS_WORD power);
    RS_WORD inv(RS_WORD x);
    RS_WORD sqrt(RS_WORD x);

    ////////
    void printTables();
    void GenerateTables();
    void GenerateLogTable();
    //generateLogTable
};

