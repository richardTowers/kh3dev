/*****************************************************************/
//  neuralNetwork.c
//	Created by richard at 12:28 on 06/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/

#include "neuralNetwork.h"

void ctrnn(float *y, const short n, const float *I, const float *b, const float *t, const float *w, const float dT)
{
	float *yOld;
	//i represents the postsynaptic neuron, j the presynaptic neuron
	short i, j;
	//k1-4 are the runge-kutta coefficients
	float k1, k2, k3, k4;
	//sum holds the value of the sum[w(ij)*sigmoid(y(i)-Theta(j)] term
	float sum=0;
	
	
	//Copy neuron states into new array:
	yOld=malloc(n*sizeof(float));
	for (i = 0; i < n; i += 1)
	{
		yOld[i] = y[i];
	}
	
	//Calculate neuron states at t+dT:
	for (i = 0; i < n; i += 1)
	{
		sum=0;
		for (j = 0; j < n; j += 1) sum+=w[j*n+i]*sigmoid(yOld[j]-b[j]);

		#ifdef RK4
		{
			k1=(-1*(yOld[i]          )+sum+I[i])/t[i];
			k2=(-1*(yOld[i]+0.5*k1*dT)+sum+I[i])/t[i];
			k3=(-1*(yOld[i]+0.5*k2*dT)+sum+I[i])/t[i];
			k4=(-1*(yOld[i]  +  k3*dT)+sum+I[i])/t[i];

			y[i]+=dT*(k1+2*k2+2*k3+k4)/6;
		}
		#elif EULER
		{
			y[i]=yOld[i]+(-yOld[i]+sum+I[i])/t[i];
		}
		#endif
		printf("%.3f, ", y[i]);
	}
	printf("\n");
	free(yOld);
	return;
}

void ffnn(float *y, const short n, const float *I, const float *w)
{
	float *yOld;
	//i represents the postsynaptic neuron, j the presynaptic neuron
	short i, j;
	
	//Copy neuron states into new array:
	yOld=malloc(n*sizeof(float));
	for (i = 0; i < n; i += 1) yOld[i] = y[i];

	//Calculate neuron states at t+1:
	for (i = 0; i < n; i += 1)
	{
		y[i]=0;
		for (j = 0; j < n; j += 1) y[i]+=(w[j*n+i])*(2*sigmoid(yOld[j])-1);
		y[i]+=I[i];
	}
	free(yOld);
	return;
}

void initSigmoid(void)
{
	int i;
	for(i=0;i<1024;i++)
		sigmoidTable[i]=1.0/(1+exp(-((i-512)*0.008)));
}

float sigmoid(float x)
{
	short index;
	x/=0.008; x+=512.5;
	if(x>1024)x=1024; if(x<0)x=0;
	index=(short)x;
	//printf("%hd, %1.1f\n", index, sigmoidTable[index]);
	return sigmoidTable[index];
}
