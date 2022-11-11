test: 
	echo 'test'

all: combinations.o peakAnalysis peak.o dList.o

combinations: dList.o 
	gcc -o combinations combinations.c dList.o

dList:
	gcc -o dList dList.c

peak: dList.o
	gcc -o peak peak.c dList.o

imageAnalysis: png
	gcc -o imageAnalysis imageAnalysis.c png.o -lz
	./imageAnalysis

png:
	gcc -g -c png.c

peakAnalysis: combinations.o peak.o dList.o
	gcc -o peakAnalysis peakAnalysis.c dList.o combinations.o peak.o

heightEstimator: dList.o
	gcc -o heightEstimator heightEstimator.c dList.o
	./heightEstimator

%.o: %.c
	gcc -g -c $<

executeable: combinations.o peakAnalysis.o peak.o dList.o
	gcc -o executeable combinations.o peakAnalysis.o peak.o dList.o


clean:
	bash Clean.sh
