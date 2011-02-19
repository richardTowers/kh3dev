

#include "pacman.h"



int main(int argc, char *argv[])
{
	short int nCherries, cherry, value=SHRT_MAX;
	CvPoint *cherries;
	
	if(argc==1) { printf("Not enough arguments...\n"); exit(1); }
	else nCherries=atoi(argv[1]);
	
	if(argc>2) printf("Too many arguments...\n");
	if(argc==2)
	{
		if(nCherries==0 || nCherries==INT_MAX || nCherries<0) printf("Not a valid number...\n");
		else printf("Cherries = %d\n", nCherries);
	}
	
	cherries = (CvPoint*)malloc(sizeof(CvPoint)*nCherries);
	
	for (cherry = 0; cherry < nCherries; cherry ++) cherries[cherry] = cvPoint(rand()%1000,rand()%1000);
	while(nCherries>0)
	{
		for (cherry = 0; cherry < nCherries; cherry ++) printf("%d,%d\n", cherries[cherry].x,cherries[cherry].y);
		value=SHRT_MAX;
		while(value>nCherries-1)
		{
			printf("\nEnter a value to remove from the array:\n");
			scanf("%hd", &value);
			if(value>nCherries-1) printf("\nValue must be between 0 and %d\n", nCherries-1);
		}
		removeFromArray(cherries, &nCherries, value);
	}
	return 0;
}

CvPoint* removeFromArray(CvPoint* array, short *length, const short index)
{
	short i;
	if(index>*length || index < 0) {fprintf(stderr, "removeFromArray(): Index out of bounds\n"); return array;}
	else
	{
		*length=*length-1;
		for(i = index; i < *length; i++) *(array+i) = *(array+(i+1));
	}
	return (CvPoint*)realloc(array, sizeof(CvPoint)*(*length));
}


