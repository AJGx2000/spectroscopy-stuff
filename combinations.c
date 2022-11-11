#include "combinations.h"
/*
int main(void){
    printf("%d", factorial(3));
    return 0;
}*/

void findReplaceMatch(dList vals, dList combos, double certainty, double *s){
	//print(vals);
	double closeCall=10.0;
	//printf("%lf\n",certainty);
    for (int i=0;i<combos.length;i++){
		int match=1;
		//puts("----------------");
        for (int j=0; j<vals.length && match; j++){
            double diff=combos.values[i]-vals.values[j];
            diff = diff>0 ? diff : 0-diff;
			//printf("%lf %lf %lf\n",combos.values[i],vals.values[j],diff);
            if (diff<certainty){
                vals.values[j]=0.0;
				match=0;
			}
			else {
				if (diff<closeCall) closeCall=diff;
            }
        }
    }
	if (closeCall<*s) *s=closeCall;
	//printf("The closest non-elimination had a difference of %lf.\n",closeCall);
}

void remove0 (dList *vals){
    int count=0;
    for (int i=0; i<(*vals).length; i++){
        if ((*vals).values[i]==0.0){
            count++;
        }
    }
    int newLen=(*vals).length-count;
    double *newArray= (double *) malloc(sizeof(double)*(newLen));
    int index = 0;
    for (int i=0; i<(*vals).length; i++){
        if ((*vals).values[i]){
            newArray[index++]=(*vals).values[i];
        }
    }
    double *temp=(*vals).values;
    (*vals).values=newArray;
	(*vals).length=newLen;
    free(temp);
}

int factorial(int num){
    int result=1;    
    for (int i=num;i>1;i--){
        result*=i;
    }
    return result;
}

int calcCombination(int n, int r){
    return (factorial(n))/(factorial(r)*factorial(n-r));
}

dList copy(dList list){
    double *newArray=(double *) malloc(sizeof(double)*list.length);
    for (int i=0;i<list.length;i++){
        newArray[i]=list.values[i];
    }
    return (dList) {newArray, list.length};
}

dList takeAfter(dList list, int start){
    int newlen=list.length-1-start;
    double *newArray = (double *) malloc(sizeof(double)*newlen);
    for (int i=0;i<newlen;i++){
        newArray[i]=list.values[i+start+1];
    }
	free(list.values);
	list.values=newArray;
	list.length=newlen;
    return list;
}

dList takeBetween(dList list, int start, int end){
//start and end exclusive
	int newlen=end-start-1;
    double *newArray = (double *) malloc(sizeof(double)*newlen);
    for (int i=0;i<newlen;i++){
        newArray[i]=list.values[i+start+1];
    }
	free(list.values);
    list.values=newArray;
    list.length=newlen;
    return list;
}

dList concat(dList list1, dList list2){
    double *newArray=(double *) malloc(sizeof(double)*(list1.length+list2.length));
    int index=0;
    for (int i=0;i<2;i++){
		if (i ? list2.length : list1.length){
			for (int j=0;j<(i ? list2.length : list1.length);j++){
				newArray[index++]= i ? list2.values[j] : list1.values[j];
			}
		}
    }
    return (dList) {newArray,list1.length+list2.length};
}
/*
dList combine(dList list, int r, int start){
    if (r==1){
        return takeAfter(copy(list),start);
    }
    else{
        dList temp=combine(list, r-1, r-2);
        //int tlen = calcCombination(list.length,r-1);
		int index=0;
		puts("First temp:");
		print(temp);
        for (int i=0; i<temp.length; i++){
            temp.values[index++]+=list.values[0];
        }
		puts("First temp with addition");
		print(temp);
        for (int i=1; i<list.length-1-(r-2); i++){
			printf("%d %d\n",list.length,i);
			dList segment=(combine(list,r-1,i+(r-2)));
			puts("New segment:");
            print(segment);
			temp=concat(temp, segment);
			puts("Temp+Segment:");
			print(temp);
            for (int j=0; j<segment.length ;j++){
                temp.values[(index++)]+=list.values[i];
            }
        }
        return temp;
    }
}
*/
/*
void append(dList *list, int num){
	*list=concat(*list,(dList) {(double *) {(double) num} ,1});
}

int pow(int n, int p){
	int tot=1;
	for (int i=0;i<p;p++){
		n*=n;
	}
	return n;
}

dList combine(dList list, int r){
	int len=pow(list.length,r);
	dList* nList=(dList *) malloc(sizeof(dList) * len);
	for (int i=0;i<len;i++){
		nList[i]=(dList) {double *{},0};
	}
	 
}
*/
/*
0	1	2	3	4	 5

n	0	01	012	0123 01234
	1	02	013	0124
	2	03	014 0134
	3	04	023 0234
	4	12	024 1234
		13	034
		14	123
		23	124
		24	134
		34	234
*/
dList combine(dList original,int r,int level,int previous){
	//first call: combine(values, picking r elements per combination, 0, -1) 

	if (r==1) return copy(original);
	if (r==original.length){
		double *arr=(double *)malloc(sizeof(double));
		for (int i=0;i<original.length;i++){
			arr[0]+=original.values[i];
			}
		return (dList) {arr,1};
	}
	int end=original.length-(r-level)+1;
	if (level==r-1){
		return takeBetween(copy(original),previous,end);
	}
	dList result=(dList) {(double [0]){},0};
	for (int i=previous+1;i<end;i++){
		dList segment=combine(original,r,level+1,i);
		//printf("prev: %d end: %d, segment: \n", i,end);
		//print(segment);
		for (int j=0;j<segment.length;j++){
			//printf("%lf ",original.values[i]);
			segment.values[j]+=original.values[i];
		}
		//puts("");
		result=concat(result,segment);
	}
	return result;
	
}

dList combineAll(dList list){
    //puts("A1");
    dList combins=combine(list,2,0,-1);
    //puts("A2");
    //int curlen=calcCombination(len,2);
    //puts("A3");
    for (int i=3;i<=list.length;i++){
        //int newLen=calcCombination(len,i);
		dList segment=combine(list,i,0,-1);
        combins=concat(combins,segment);
        //curlen+=newLen;
    }
    //puts("A4");
    return combins;
}
