#include "png.h"

int cap(unsigned int n){
	if (n>64 && n<91) return 1;
	return 0;
}

int cap4(unsigned char *p,int i){
	if (cap(p[i]) && cap(p[i+1]) && cap(p[i+2]) && cap(p[i+3])) return 1;
	return 0;
}

chunk nextChunk(FILE *f, int *check){
	unsigned char len[4];
	unsigned int length;
	fread(&len,4,1,f);
	//for (int i=0;i<4;i++){
		//printf("% ",len[i]);
	//}
	length=len[3]+len[2]*256+len[1]*256*256+len[0]*256*256*256;
	//printf("%d\n",*check);
	printf("Length: %d\n",length);
	char *type=(char *)malloc(sizeof(char)*4);
	fread(type,1,4,f);
	*check = (type[0]!='I' || type[1] != 'E' || type[2] != 'N' || type[3]!='D');
	//printf("%d\n",*check);
	printf("%c%c%c%c\n",type[0],type[1],type[2],type[3]);
	unsigned char *data=(unsigned char *)malloc(sizeof(unsigned char)*length);
	fread(data,1,length,f);
	//printf("%d\n",*check);
	unsigned int crc;
	fread(&crc,4,1,f);
	//printf("%d\n",*check);
	return (chunk) {length, type, data, crc};
}

char **colorEscapes(RGBA p){
	char **arr=(char **)malloc(sizeof(char *)*3);
	for (int i=0;i<3;i++){
		unsigned char cur = i ? i>1 ? p.B : p.G :p.R;
		char *code=(char *)malloc(sizeof(char)*7);
		for (int j=0;j<7;j++){
			code[j]="\e[0;30m"[j];
		}
		code[5]=i ? i>1 ? '4' : '2' : '1';
		code[2]=cur>127 ? cur> 191 ? '1' : '0' : cur >63 ? '2' :'8';
		arr[i]=code;
	}
	return arr;
}

int coloredPixel(RGBA p,char **escapeArray){
	int r=p.R,g=p.G,b=p.B;
	int largest= ( (r>g ? r : g) >b ? (r>g ? r : g) : b);;
	int fraclargest=largest*191/256;
	fraclargest = fraclargest ? fraclargest : 1;
	//int colorNums[3]={1,2,4};
	int colorVal= r/fraclargest*1+g/fraclargest*2+b/fraclargest*4+(r/fraclargest&&g/fraclargest&&b/fraclargest ? 1 : 0);
	int sum=r+g+b;
	//int divisor = r/largest+g/largest+b/largest;
	//printf("R=%d G=%d B=%d colorVal=%d  sum: %d div: %d\n",r,g,b,colorVal,sum,divisor);
	for (int i=0;i<8;i++){
		escapeArray[i][5]=colorVal+'0';
		if (i) escapeArray[i][2]=i<5 ? '2' : '0';
		if (i&&i<5) escapeArray[i][9]=i<4?"░"[2]+i-1:"░"[2]-9;
		else if (i) escapeArray[i][9]=i<7?"░"[2]+i-4:"░"[2]-9;
		else escapeArray[i][9]="░"[2]-5;
	}
	//printf("%d\n",sum/32);
	return (int)(largest/32);
}


void printRGBA(png apng, int alpha, int color){
	char last='0';			
	char **escapeArray=(char **)malloc(sizeof(char *)*8);
	for (int i=0;i<8;i++){	
		escapeArray[i]=(char *)malloc(sizeof(char)*11);
		for (int j=0;j<11;j++){
			escapeArray[i][j]="\e[8;38m░"[j];
			//printf("%c,","\e[8;38m░"[j]);
		}
		//printf("%c\n",escapeArray[i][2]);
	}
	for (int h=0;h<apng.height;h++){
		for (int w=0;w<apng.width;w++){
			RGBA p=apng.pixels[h][w];
			if (color&&alpha) {			
	//			puts("hiya");
				//printf("%s",escapeArray[0]);
				int escIndex=coloredPixel(p,escapeArray);
	//			printf("%d ",escIndex);
				char *string=escapeArray[escIndex];
				//printf("%s",string);
				//puts("shit");
				if (last=='8'&&string[2]!='8') printf("\e[0;0m");
				//puts("beans");
				//printf("%d",string[2]!='8');
				//puts("fart");
				last=string[2];
				printf("%s",string);
				//puts("cheese");
			}
			else if(color){
				char **colors=colorEscapes(p);
				char norm[7]="\e[0;0m";
				printf("%sX%s%sX%s%sX%s",colors[0],norm,colors[1],norm,colors[2],norm);
			}
			else if (alpha) printf("%02X %02X %02X %02X\t",p.R,p.G,p.B,p.A);		
			else printf("%02X%02X%02X ",p.R,p.G,p.B);
		}
		if(color&&alpha) printf("\n");
		else printf("\n\n");
	}
}

unsigned char PaethPredictor (unsigned char a, unsigned char b, unsigned char c){
		//a = left, b = above, c = upper left
        int p,pa,pb,pc;
		unsigned char ret;
		p = a + b - c;      // initial estimate
        pa = (p - a)<0?0-(p-a):(p-a);    //  distances to a, b, c
        pb = (p - b)<0?0-(p-b):(p-b);
        pc = (p - c)<0?0-(p-c):(p-c);
        // return nearest of a,b,c,
		// breaking ties in order a,b,c.
        if (pa <= pb && pa <= pc) ret=a;
        else if (pb <= pc) ret=b;
        else ret=c;
		return ret;
}

png makePNG(char *fName){
	int fNL;
	for (fNL=0;fName[fNL];fNL++);
	fNL--;
	int fNLc=--fNL;
	while(fNL>=0 && fName[fNL]==' '){
		fName[fNL] = 0;
		fNL--;
	}
	//printf("%s--%d",fName,fName[fNLc]);
	FILE *fp=fopen(fName,"r");
	png p;
	//p.magic=(unsigned char *)malloc(sizeof(unsigned char)*8);
	fread(p.magic,1,8,fp);
	if (p.magic[1]!='P' || p.magic[2]!='N' || p.magic[3]!='G'){
		printf("\e[31m%s file type is %c%c%c not PNG!\e[0m\n",fName,p.magic[1],p.magic[2],p.magic[3]);
	}
	else{
		printf("\e[32m%s file type is %c%c%c!\e[0m\n",fName,p.magic[1],p.magic[2],p.magic[3]);
		p.chunks=(chunk *)malloc(sizeof(chunk)*10);
		int check=1, ci=0, size=10;
		while(check){
			//printf("%d %d\n",check,ci);
			p.chunks[ci]=nextChunk(fp,&check);
			ci++;
			if (ci==size){
				size*=2;
				chunk *temp=p.chunks;
				p.chunks=(chunk *)malloc(sizeof(chunk)*size);
				for (int i=0;i<ci;i++){
					p.chunks[i]=temp[i];
				}
				free(temp);
			}
		}
		fclose(fp);
		p.numChunks=ci;
		printf("%d Chunks total\n",p.numChunks);
		p.compressed.length=0;
		for (int i=0;i<p.numChunks;i++) {
			if (p.chunks[i].type[0]=='I' && p.chunks[i].type[1]=='D' && p.chunks[i].type[2]=='A' && p.chunks[i].type[3]=='T'){
				p.compressed.length+=p.chunks[i].length;
			}
		}
		printf("Compressed bytes: %d\n",p.compressed.length);
		p.compressed.data=(unsigned char *)malloc(sizeof(unsigned char)*p.compressed.length);
		int index = 0;
		for (int i=0; i<p.numChunks; i++){ 
			if (p.chunks[i].type[0]=='I' && p.chunks[i].type[1]=='D' && p.chunks[i].type[2]=='A' && p.chunks[i].type[3]=='T'){
				for (int j=0;j<p.chunks[i].length;j++){
					p.compressed.data[index++]=p.chunks[i].data[j];
				}
			}
		}
		chunk IHDRchunk=p.chunks[0];
		unsigned char *IHDR=IHDRchunk.data;
		p.width  = IHDR[0]*256*256*256+IHDR[1]*256*256+IHDR[2]*256+IHDR[3];
		p.height = IHDR[4]*256*256*256+IHDR[5]*256*256+IHDR[6]*256+IHDR[7];
		int colorTypeSamples[7]={1,0,3,1,2,0,4};
		int colorType=IHDR[9];
		int samplesPerPixel = colorTypeSamples[colorType];
		int bitsPerPixel = samplesPerPixel*IHDR[8];
		p.decompressed.length= p.height * p.width * bitsPerPixel/8+p.height;
		p.decompressed.data = (unsigned char*) malloc(sizeof(unsigned char) * p.decompressed.length);
		unsigned long totalBytes = p.decompressed.length;
		uncompress((Bytef *) p.decompressed.data, &totalBytes, (Bytef *) p.compressed.data,(unsigned long) p.compressed.length);
		p.pixels=(RGBA **)malloc(sizeof(RGBA *)*(p.height));
		for (int i=0;i<p.height;i++){
			p.pixels[i] = (RGBA *) malloc(sizeof(RGBA) *p.width);
		}
		index =0;
		unsigned char * raw=p.decompressed.data;
		for (int h=0;h<p.height;h++){
			unsigned char filter=raw[index++];
			//index+=4;
			for (int w=0;w<p.width;w++){
				switch (filter) {
					case(0): //none
                        p.pixels[h][w] = (RGBA) {raw[index],
                                                raw[index+1],
                                                raw[index+2],
                                                raw[index+3]};						
						break;
					case(1): //subtract left
						if(w){
						p.pixels[h][w] = (RGBA) {raw[index]+p.pixels[h][w-1].R,
												raw[index+1]+p.pixels[h][w-1].G,
												raw[index+2]+p.pixels[h][w-1].B,
												raw[index+3]+p.pixels[h][w-1].A};
						}
						else p.pixels[h][w]=(RGBA) {raw[index],raw[index+1],raw[index+2],raw[index+3]};
						break;
					case(2): //subtract up
						if(h){
                        p.pixels[h][w] = (RGBA) {raw[index]+p.pixels[h-1][w].R,
                                                raw[index+1]+p.pixels[h-1][w].G,
                                                raw[index+2]+p.pixels[h-1][w].B,
                                                raw[index+3]+p.pixels[h-1][w].A};
						}
						else p.pixels[h][h]=(RGBA) {raw[index],raw[index+1],raw[index+2],raw[index+3]};
						break;
					case(3):
						if(w&&h){
                        p.pixels[h][w] = (RGBA) {raw[index]+(p.pixels[h][w-1].R+p.pixels[h-1][w].R)/2,
                                                raw[index+1]+(p.pixels[h][w-1].G+p.pixels[h-1][w].G)/2,
                                                raw[index+2]+(p.pixels[h][w-1].B+p.pixels[h-1][w].B)/2,
                                                raw[index+3]+(p.pixels[h][w-1].A+p.pixels[h-1][w].A)/2};
						}
						else if (h) p.pixels[h][w]=(RGBA) {raw[index]+p.pixels[h-1][w].R/2,
													raw[index+1]+p.pixels[h-1][w].G/2,
													raw[index+2]+p.pixels[h-1][w].B/2,
													raw[index+3]+p.pixels[h-1][w].A/2};
						else if (w) p.pixels[h][w]=(RGBA) {raw[index]+p.pixels[h][w-1].R/2,
                                                    raw[index+1]+p.pixels[h][w-1].G/2,
                                                    raw[index+2]+p.pixels[h][w-1].B/2,
                                                    raw[index+3]+p.pixels[h][w-1].A/2};
						else p.pixels[h][w]=(RGBA) {raw[index],raw[index+1],raw[index+2],raw[index+3]};
						break;
					case(4):
						if(w&&h){
                        p.pixels[h][w] = (RGBA) {raw[index]+PaethPredictor(p.pixels[h][w-1].R,p.pixels[h-1][w].R,p.pixels[h-1][w-1].R),
                                                raw[index+1]+PaethPredictor(p.pixels[h][w-1].G,p.pixels[h-1][w].G,p.pixels[h-1][w-1].G),
                                                raw[index+2]+PaethPredictor(p.pixels[h][w-1].B,p.pixels[h-1][w].B,p.pixels[h-1][w-1].B),
                                                raw[index+3]+PaethPredictor(p.pixels[h][w-1].A,p.pixels[h-1][w].A,p.pixels[h-1][w-1].A)};
						}
						else if(h) p.pixels[h][w] = (RGBA) {raw[index]+PaethPredictor(0,p.pixels[h-1][w].R,0),
                                                raw[index+1]+PaethPredictor(0,p.pixels[h-1][w].G,0),
                                                raw[index+2]+PaethPredictor(0,p.pixels[h-1][w].B,0),
                                                raw[index+3]+PaethPredictor(0,p.pixels[h-1][w].A,0)};
						else if(w) p.pixels[h][w] = (RGBA) {raw[index]+PaethPredictor(p.pixels[h][w-1].R,p.pixels[h-1][w].R,0),
                                                raw[index+1]+PaethPredictor(p.pixels[h][w-1].G,0,0),
                                                raw[index+2]+PaethPredictor(p.pixels[h][w-1].B,0,0),
                                                raw[index+3]+PaethPredictor(p.pixels[h][w-1].A,0,0)};
						else p.pixels[h][w]=(RGBA) {raw[index],raw[index+1],raw[index+2],raw[index+3]};	
						break;
				}
				index+=4;
			}
		}
	}
	return p;

}

png interface(void){
	puts("Please enter a png file to analyze");
	char fileName[100];
	scanf("%[^\n]",fileName);
	//printf("%s",fileName);
	//puts("5");
	png apng=makePNG(fileName/*"line3_3.png""spectra.png""dot.png""test.png"*/);
	chunk IHDR=apng.chunks[0];
	int IHDRlen = IHDR.length;
	printf("chunk:\n\tlength: %d\n\tname: %s\n\twidth: %02X %02X %02X %02X  %d\n\theight: %02X %02X %02X %02X  %d\n\tbit depth: %d\n\tcolor type: %d\n\tcompression method: %d\n\tfilter method: %d\n\tinterlace method: %d\n",
		IHDRlen,
		IHDR.type,
		IHDR.data[0],
		IHDR.data[1],
		IHDR.data[2],
		IHDR.data[3],
		IHDR.data[3]+IHDR.data[2]*256+IHDR.data[1]*256*256+IHDR.data[0]*256*256*256,
		IHDR.data[4],
		IHDR.data[5],
		IHDR.data[6],
		IHDR.data[7],
		IHDR.data[7]+IHDR.data[6]*256+IHDR.data[5]*256*256+IHDR.data[4]*256*256*256,
		IHDR.data[8],
		IHDR.data[9],
		IHDR.data[10],
		IHDR.data[11],
		IHDR.data[12]);
	/*
	chunk view=apng.compressed;
	for (int i=0;i<view.length;i++){
		unsigned char byte =view.data[i];
		if (byte>=65 && byte <=90 || byte>=97 && byte <=122){
			printf("%02X ",byte);
		}
		else{
			printf("%02X ",byte);
		}
		if ((i+1)%4==0) printf("\t");
		if ((i+1)%16==0) printf("\n");
	}
	puts("");
	int r=0,c=0;
	for (int i=0;i<apng.decompressed.length;i++){
		unsigned char byte=apng.decompressed.data[i];

		if (byte>=65 && byte <=90 || byte>=97 && byte <=122){
            printf("%02X ",byte);
        }
        else{
            printf("%02X ",byte);
        }
        if (c%4==0 ) printf("\t");
		c++;
		if (c==3*4+1) {r++;printf("\n");c=0;}
	}*/
	//just filter methods:
	/*
	for (int i=0;i<apng.decompressed.length;i++){
		if (i%(apng.width*4+1)==0) printf("%02X ",apng.decompressed.data[i]);
	}
	puts("");
	printRGBA(apng,0);
	*/
	puts("Do you want to print the pixels of this PNG file? <1/0>");
	int choice=0;
	scanf("%d",&choice);
	if (choice) printRGBA(apng,1,1);
	return apng;
}
