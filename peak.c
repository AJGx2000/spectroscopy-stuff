#include "peak.h"
/*
int main(void){
	return 0;
}*/

int sumHeight(pList list){
    int total=0;
    for (int i=0; i<list.length; i++){
        total+=list.peaks[i].height;
    }
    return total;
    }

int maxHeight(pList list){
    int max=0;
    for (int i=0;i<list.length;i++){
        max = list.peaks[i].height>max ? list.peaks[i].height : max;
    }
    return max;
}

dList peak2d(pList list){
    dList d = {(double *) malloc(sizeof(double)*list.length),list.length};
    for (int i=0;i<list.length;i++){
        d.values[i]=list.peaks[i].shift;
    }
    return d;
}

double smallestDiff(pList list){
    double smallest=list.peaks[0].shift-list.peaks[1].shift;
    for (int i=0;i<list.length-1;i++){
        double diff = list.peaks[i].shift-list.peaks[i+1].shift;
        smallest = diff < smallest ? diff : smallest;
    }
    return smallest;
}

void sortPeaks(pList list, int desc){
    peak temp;
    int unsorted=1;
    while (unsorted){
        int switches=0;
        if (desc){
            for (int i=0;i<list.length-1;i++){
                if (list.peaks[i].shift<list.peaks[i+1].shift){
                    temp=list.peaks[i];
                    list.peaks[i]=list.peaks[i+1];
                    list.peaks[i+1]=temp;
                    switches++;
                }
            }

        }
        else{
            for (int i=0;i<list.length-1;i++){
                if (list.peaks[i].shift>list.peaks[i+1].shift){
                    temp=list.peaks[i];
                    list.peaks[i]=list.peaks[i+1];
                    list.peaks[i+1]=temp;
                    switches++;
                }
            }
        }
        unsorted = switches>0 ? 1 :0;
    }
}

dList findJvals(pList list){
    int len = sumHeight(list);
    double *store=(double *) malloc(sizeof(double)*len);
    int sIndex=0;
    for (int i=0;i<list.length;i++){
        for (int j=0;j<list.peaks[i].height;j++){
            store[sIndex++]=list.peaks[0].shift-list.peaks[i].shift;
        }
    }
    return (dList) {store,len};
}

dList rfindJvals(pList list){
    int len = sumHeight(list);
    double *store=(double *) malloc(sizeof(double)*len);
    int sIndex=len-1;
    for (int i=0;i<list.length;i++){
        for (int j=0;j<list.peaks[list.length-1-i].height;j++){
            store[sIndex--]=list.peaks[list.length-1-i].shift-list.peaks[list.length-1].shift;
        }
    }
    return (dList) {store,len};
}


void plot(pList peakList, dList jList ){
	double diff=smallestDiff(peakList);
    /*double scale = diff>= 1 ?
		diff > 6 ? .33 :
			1
        : diff>=.5 ? 2
			: 4;
    */
	double scale=1/diff*(2+1/diff);
	//printf("%lf %lf\n",diff,scale);
	int length = (jList.values[jList.length-1])*scale+2+1;//2 is for _ at beginning and end, 1 is for \n at end
    int height = maxHeight(peakList);
    int size = (length+1) * height;
    char line[size];
    for (int i=0;i<size;i++){
        line[i]=(i+1)%(length+1) == 0 ? '\n' :
            i>=(length*(height-1)+height-1) ? '_': ' ';
    }
    for (int i=0;i<peakList.length;i++){
        int position=(peakList.peaks[0].shift-peakList.peaks[i].shift)*scale+1;
        //printf("%d\n",position);
        for (int j=0;j<peakList.peaks[i].height;j++){
           line[(length+1)*(height-1-j)+position ]='A';
        }
    }
    line[size]='\0';
    printf("%s",line);
}

pList makeList(){
	int size=0;
    puts("How many values are you making into a list?");
    scanf("%d",&size);
    pList list = {(peak *) malloc(sizeof(peak)*size),size};
    puts("Enter your values(float) followed by their relative height(int) (in this format: \"value height\"):");
    for (int i=0;i<size;i++){
        scanf("%lf %d",&list.peaks[i].shift,&list.peaks[i].height);
    }
    puts("List completed!");
    printf("[ ");
    for (int i=0;i<size-1;i++){
        printf("%.2lf %d, ", list.peaks[i].shift, list.peaks[i].height);
    }
    printf("%.2lf %d ]\n",list.peaks[size-1].shift, list.peaks[size-1].height);
    return list;
}
