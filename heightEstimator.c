#include <stdio.h>
#include <stdlib.h>
#include "dList.h"
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define NRM "\x1B[0m"
typedef struct{
	int* values;
	int len;
	} iList;

void iprint(iList list){
	printf("[ ");
	for (int i=0;i<list.len-1;i++){
		printf("%d, ",list.values[i]);
	}
	printf("%d ]\n",list.values[list.len-1]);
}

int isum(iList list){
	int sum=0;
	for (int i=0;i<list.len;i++){
		sum+=list.values[i];
	}
	return sum;
}

iList newList(void){
	int len;
    puts("how many peaks?");
    scanf("%d",&len);
	iList list= {(int *)malloc(sizeof(int)*len),len};
	puts("enter peak pixel heights");
	for (int i=0;i<len;i++){
		scanf("%d",&list.values[i]);
	}
	iprint(list);
	return list;
}

int findSmallest(iList list){
	int smallest=9999999;
	for (int i=0;i<list.len;i++){
		if (list.values[i]<smallest) smallest=list.values[i];
	}
	return smallest;
}

int isPow2(int n){
    double num=(double) n;
    while(num){
        if (num==1) return 1;
        num/=2;
    }
    return 0;
}

int abs(int num){
	if (num<0) num=0-num;
	return num;
}

float f_abs(float num){
	if (num<0) num=0-num;
	return num;
}

int findNearest(int num){
	int current=9999,last=10000,n=1;
	//printf("%d<%d?%d\n",current,last,n);
	while(current<last){
		last=current;
		current=abs(num-n);
		n*=2;
		//printf("%d<%d?%d\n",current,last,n);
	}
	return n/4;
}

int f_findNearest(float num){
    float current=9999,last=10000,n=1;
    //printf("%d<%d?%d\n",current,last,n);
    while(current<last){
        last=current;
        current=f_abs(num-n);
        n*=2;
        //printf("%d<%d?%d\n",current,last,n);
    }
    return (int) n/4;
}

int fractionScore(dList list){
	float val;
	float diff;
	int score=0;
	for (int i=0;i<list.length;i++){
		val=list.values[i];
		diff=(float) ((int) val+1)-val;
	//	printf("%d %f ",score,diff);
		for(float j=0.05;j<=.5;j+=.05){
			if (diff>j && diff<(1-j)) score++;
		}
	//	if(diff>.24 &&  diff<.76) score++;
	//	if(diff>.39 && diff<.61) score++;
	//	printf("%d\n",score);
	}
	return score;
}

int symmetric(iList list){
	int violations=0;
	for (int i=0;i<list.len/2;i++){
		if (list.values[i]!=list.values[list.len-1-i]) violations++;
	}
	return violations;
}

iList d2iList(dList list){
	iList l=(iList) {(int *)malloc(sizeof(int)*list.length),list.length};
	for (int i=0;i<l.len;i++){
		l.values[i]=(int) (list.values[i]+.5);
	}
	return l;
}

int ilog2(int num){
	int p=0;
	for(int n=1;((int) num/n)>0;n*=2) p++;
	return p-1;
}

void findBestFit(iList list){
	dList suggestion=(dList) {(double*)malloc(sizeof(double)*list.len),list.len};
	dList bestSuggestion=(dList) {(double *)malloc(sizeof(double)*list.len),list.len};
	int smallest=findSmallest(list),fscore=999, tempfscore;
	float best=0, difference=999, score, bestScore=99999999999999;
	//int violations=999;
	for (float i=5;i<smallest*2;i+=.5){
		float sum=0;
		for (int j=0;j<list.len;j++){
			sum+=((float) list.values[j])/i;
			//printf("\t%d %d\n",(int) list.values[j]/i,j);
		}
		int closest=f_findNearest(sum);
		float diff=f_abs(closest-sum);
		//printf("sum:%.2f i:%d close:%d diff:%.2f\n",sum,i,closest,diff);
		if (diff<=difference){
			if(closest<256){
				printf("sum:%.2f i:%.1f close:%d diff:%.2f  ",sum,i,closest,diff);
			}
			//printf("sum:%d i:%d close:%d diff:%d violations",sum,i,closest,diff);
			//difference=diff;
			//best=closest;
			for(int j=0;j<list.len;j++){
				suggestion.values[j]=(float) ((float) list.values[j])/ i;
			}
			//violations=symmetric(suggestion);
			//printf("%d\n",violations);
			tempfscore=fractionScore(suggestion);
			if (closest<256){
				printf("%d  ",tempfscore);
				print(suggestion);
			}
		}
		if(diff<=difference){
			score=(1+diff)*tempfscore*(1+symmetric(d2iList(suggestion)))*ilog2(closest)+(ilog2(closest)>list.len)*999999;
			if(closest<256) { printf("%.3f\n",score);}
			//printf("%f best: %f\n",score,bestScore);
			//if (tempscore<fscore && diff<difference || tempscore*2<=fscore || diff*2<difference || diff<.5&&tempscore+5<fscore){
			if(score<bestScore){
				bestScore=score;
				difference=diff;
				best=closest;
				fscore=tempfscore;
				for (int j=0;j<list.len;j++){
					bestSuggestion.values[j]=suggestion.values[j];
				}
			}
		}
	}
	printf("\n\nThe best fit has a total of %.2f, a difference of %.2f, and heights as follows: \n",best,difference);
	print(bestSuggestion);
	iList ibestSuggestion=d2iList(bestSuggestion);
	int total=isum(ibestSuggestion), sym = symmetric(ibestSuggestion);
	printf("Total: %s%d%s Symmetry violations: %s%d%s Heights: %s",isPow2(total)?GRN:RED,total,NRM,sym ? RED : GRN ,sym,NRM,GRN);
	iprint(ibestSuggestion);

}

int main(void){
	//printf("%d %d %d\n",findNearest(4), findNearest(7), findNearest(13));
	//printf("%d 1   %d 3   %d 4\n",ilog2(2),ilog2(8),ilog2(16));
	iList list=newList();
	findBestFit(list);
	return 0;
}

