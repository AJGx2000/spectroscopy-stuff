#include "dList.h"

typedef struct{
    double shift;
    int height;
    } peak;

typedef struct{
	peak *peaks;
	int length;
	} pList;

int main(void);
dList peak2d(pList);
int sumHeight(pList);
int maxHeight(pList);
double smallestDiff(pList);
void sortPeaks(pList, int);
dList findJvals(pList);
dList rfindJvals(pList);
void plot(pList, dList);
pList makeList();
