#include <stdio.h>
#include <stdlib.h>
#pragma once

typedef struct{
    double *values;
    int length;
    } dList;

int main(void);
void print(dList);
void rprint(dList);
dList newDList(double *, int);
void sortVals(dList list, int desc);
