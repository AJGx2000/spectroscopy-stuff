#include "dList.h"

int main(void);
void findReplaceMatch(dList vals, dList combos, double certainty, double* s);
void remove0 (dList *vals);
int factorial(int num);
int calcCombination(int n, int r);
dList copy(dList list);
dList takeAfter(dList list, int start);
dList takeBetween(dList, int, int);
dList concat(dList list1, dList list2);
dList combine(dList, int, int, int);
dList combineAll(dList list);
