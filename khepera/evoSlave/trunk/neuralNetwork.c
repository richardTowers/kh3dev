/*****************************************************************/
//  neuralNetwork.c
//	Created by richard at 12:28 on 06/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/

#include "neuralNetwork.h"

void ctrnn(short *y, const short n, const short *I, const short *b, const short *t, const short *w, const short dT)
{
	short *yOld;
	//i represents the postsynaptic neuron, j the presynaptic neuron
	short i, j;
	//k1-4 are the runge-kutta coefficients
	float k1, k2, k3, k4;
	//sum holds the value of the sum[w(ij)*sigmoid(y(i)-Theta(j)] term
	float sum=0;
	
	
	//Copy neuron states into new array:
	yOld=malloc(n*sizeof(short));
	for (i = 0; i < n; i += 1) yOld[n] = y[n];
	
	//Calculate neuron states at t+dT:
	for (i = 0; i < n; i += 1)
	{
		for (j = 0, sum = 0; j < n; j += 1) sum+=(int)((float)((WEIGHT_RANGE*(*(w+i*n+j)))/WEIGHT_IN)*sigmoid(yOld[j]-b[j]));
		k1=(-(yOld[i]          )+sum+I[i])/t[i];
		k2=(-(yOld[i]+0.5*k1*dT)+sum+I[i])/t[i];
		k3=(-(yOld[i]+0.5*k2*dT)+sum+I[i])/t[i];
		k4=(-(yOld[i]  +  k3*dT)+sum+I[i])/t[i];
		if((int)((int)y[i]+(int)((k1+2*k2+2*k3+k4)/6))>SHRT_MAX)
			//Incrementing would cause overflow so set to max:
			y[i]=SHRT_MAX;
		else if((int)((int)y[i]+(int)((k1+2*k2+2*k3+k4)/6))<SHRT_MIN)
			//Incrementing would cause underflow so set to min:
			y[i]=SHRT_MIN;
		else
			y[i]+=(k1+2*k2+2*k3+k4)/6;
		printf("%d ", y[i]);
	}
	printf("\n");
	free(yOld);
	return;
}

void ffnn(short *y, const short n, const short *I, const short *w)
{
	short *yOld;
	//i represents the postsynaptic neuron, j the presynaptic neuron
	short i, j;
	
	//Copy neuron states into new array:
	yOld=malloc(n*sizeof(short));
	for (i = 0; i < n; i += 1) yOld[n] = y[n];

	//Calculate neuron states at t+1:
	for (i = 0; i < n; i += 1)
	{
		y[i]=0;
		for (j = 0; j < n; j += 1) y[i]+=(int)(0.5+((float)(*(w+i*nNeurons+j)))*sigmoid(yOld[j]));
		y[i]+=I[i];
	}
	free(yOld);
	return;
}

void initSigmoid(void)
{
	#define R SHRT_MAX
	#define RR 2*SHRT_MAX
	int i;
	for(i=0;i<1024;i++)
		sigmoidTable[i]=(double)(RR/(1+exp(-(double)(((long)(i)<<8))/R))-R);
	sigmoidTable[1024]=R;
}

short sigmoid(short x)
{
	int s, y, j;
	if((s=x)<0)x=-x;
	y=*(sigmoidTable+(j=x >> 5));
	y+=((*(sigmoidTable+j+1)-y)*(x&0x1F)) >> 5;
	if(s < 0) y=-y;
	return y;
}
