

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>


int main(int argc, char *argv[])
{
	short int nCherries, cherry, *cherries, value=SHRT_MAX;
	
	if(argc==1) { printf("Not enough arguments...\n"); exit(1); }
	else nCherries=atoi(argv[1]);
	
	if(argc>2) printf("Too many arguments...\n");
	if(argc==2)
	{
		if(nCherries==0 || nCherries==INT_MAX || nCherries<0) printf("Not a valid number...\n");
		else
		{
			printf("Cherries = %d\n", nCherries);
		}
	}
	
	cherries = (short*)malloc(sizeof(short)*nCherries);
	
	for (cherry = 0; cherry < nCherries; cherry ++) cherries[cherry] = cherry;
	while(nCherries>0)
	{
		for (cherry = 0; cherry < nCherries; cherry ++) printf("%d ", cherries[cherry]);
		value=SHRT_MAX;
		while(value>nCherries-1)
		{
			printf("\nEnter a value to remove from the array:\n");
			scanf("%hd", &value);
			if(value>nCherries-1) printf("\nValue must be between 0 and %d\n", nCherries-1);
		}
		nCherries--;
		for (cherry = value; cherry < nCherries; cherry += 1) cherries[cherry]=cherries[cherry+1];
		cherries=realloc(cherries, sizeof(short)*nCherries);
	}
}

/*void* removeFromArray(void* array, int *length, int size, short sizeOf, int index)*/
/*{*/
/*	short i;*/
/*	if(index>*length || index < 0) {fprintf(stderr, "Index out of bounds\n"); return array;}*/
/*	else if(size % *length != 0) {fprintf(stderr, "sizeof() non-integer\n"); return array;}*/
/*	else*/
/*	{*/
/*		for(i = index; i < *length; i++) ;*/
/*	}*/
/*}*/


