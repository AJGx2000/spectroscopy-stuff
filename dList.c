#include "dList.h"
/*
int main(void){	
	dList test=newDList( (double[]) {1.0, 2.0, 3.0} ,3);
	print(test);
	return 0;
}
*/
void print(dList list){
    printf("[ ");
    for (int i=0;i<list.length-1;i++){
        printf("%7.2lf,  ", list.values[i]);
    }
    printf("%7.2lf ]\n",list.values[list.length-1]);
}

void rprint(dList list){
    printf("[ ");
    for (int i=0;i<list.length-1;i++){
        printf("%7.2lf,", list.values[list.length-1-i]);
    }
    printf("%7.2lf ]\n",list.values[0]);
}

dList newDList(double *vals, int len){
	dList ret={vals,len};
	return ret;
}

void sortVals(dList list, int desc){
    double temp;
    int unsorted=1;
    while (unsorted){
        int switches=0;
        if (desc){
            for (int i=0;i<list.length-1;i++){
                if (list.values[i]<list.values[i+1]){
                    temp=list.values[i];
                    list.values[i]=list.values[i+1];
                    list.values[i+1]=temp;
                    switches++;
                }
            }

        }
        else{
            for (int i=0;i<list.length-1;i++){
                if (list.values[i]>list.values[i+1]){
                    temp=list.values[i];
                    list.values[i]=list.values[i+1];
                    list.values[i+1]=temp;
                    switches++;
                }
            }
        }
        unsorted = switches>0 ? 1 :0;
    }
}
