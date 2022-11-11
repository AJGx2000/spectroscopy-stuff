#include "peakAnalysis.h"

dList evaluate(pList list, double certainty){
    dList alljvals=findJvals(list);
    //h meaning half
    //puts("1");
    //int hjlen=sumHeight(list)/2;
    //puts("2");
	/*
    dList hjvals={(double *) malloc(sizeof(double)*alljvals.length/2),alljvals.length/2};
    for (int i=1;i<alljvals.length/2;i++){
        hjvals.values[i]=alljvals.values[i];
    }*/
	
	dList hjvals=takeBetween(copy(alljvals),0,alljvals.length/2+1);
    
	//print(hjvals);
    //free(alljvals.values);
    //puts("3");
	/*
    int numCombs=0;
    for (int i=2;i<=hjvals.length;i++){
        numCombs+=calcCombination(hjvals.length,i);
    }
	*/
    //puts("A");
	//print(combine(hjvals,1,0,-1));
	//print(combine(hjvals,2,0,-1));
	double smallest=1000.0;
    dList ret=takeBetween(copy(hjvals),-1,1);
	dList combinations=combineAll(ret);
	findReplaceMatch(hjvals, combinations, certainty,&smallest);
	for(int i=1;i<hjvals.length;i++){
		if (hjvals.values[i]){
			ret=concat(ret,(dList){ (double [1]) { hjvals.values[i] },1});
			combinations=combineAll(ret);
    //print(combinations);
    //puts("B");
			findReplaceMatch(hjvals, combinations, certainty, &smallest);	
    //puts("C");
		}
	}
	printf("The closest non-elimination had a difference of %lf.\n",smallest);
	
	//remove0(&alljvals);
    return ret;
}

void assignMult(dList jvals){
	char mults[9]={'d','t','q','p','s','h','o','n','d'};
	int count=0;
	sortVals(jvals,0);
	double last=jvals.values[jvals.length-1];
	printf("\nThe multiplicity for this peak is: ");
	for (int i=jvals.length-2;i>-1;i--){
		//printf("%lf %lf %d\n",last, jvals.values[i], count);
		if (jvals.values[i]==last){
			count++;
		}
		else{
			printf("%c",mults[count]);
			count=0;
			last=jvals.values[i];
		}
	}
	printf("%c\n\n", mults[count]);
	sortVals(jvals,1);
}

int main(void){
    int running=1;
    while (running){
		puts("makeList:");
        pList peaks =makeList();
        dList jvals=findJvals(peaks);
		puts("\nJvals:");
        print(jvals);
        
        //sortVals(jvals,sumHeight(raw,size),1);
        //print(jvals,sumHeight(raw,size));
        //rprint(jvals,size);
        //puts("plot:")
		puts(" ");
        plot(peaks,jvals);
		puts("\nEnter a value for how close a combination of jvals has to be to another to eliminate it (suggested 0.1-.25):");
		double cert=.1;
		scanf("%lf",&cert);
        dList important=evaluate(peaks,cert);
		puts(" ");
		sortVals(important,1);
        print(important);
		assignMult(important);	
		/*
		dList test={(double []){10000.0,1000.0,100.0,10.0,1.0,},5};
		puts("Original:");
		print(test);
		puts("\n1:\n");
        print(combine(test,1,0,-1));
		puts("\n2:\n");
		print(combine(test,2,0,-1));
		puts("\n3:\n");
		print(combine(test,3,0,-1));
		puts("\n4\n");
		print(combine(test,4,0,-1));
		puts("\n5\n");
		print(combine(test,5,0,-1));
		*/

		//print(combine(testlist,size,1,0),size-1);
        //print(combine(testlist,size,2,-1), calcCombination(size,2));
        //print(combineAll(testlist,size),calcCombination(size,2),calcCombination(size,2));
        puts("Enter a non-zero number to run again, 0 to quit");
        scanf("%d",&running);
    }
    return 0;
}

