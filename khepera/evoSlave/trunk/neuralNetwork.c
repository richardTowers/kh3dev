/*****************************************************************/
//  neuralNetwork.c
//	Created by richard at 12:28 on 06/11/2010
//	Copyright Durham University 2010, all rights reserved
//
/*****************************************************************/

#include "neuralNetwork.h"

void ctrnn(short *y, const short n, const short *I, const short *b, const short *t, const short *w, const short dT)
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
	for (i = 0; i < n; i += 1) yOld[n] = *(y+n);
	
	//Calculate neuron states at t+dT:
	for (i = 0; i < n; i += 1)
	{
		for (j = 0, sum = 0; j < n; j += 1) sum+=(*(w+i*n+j))*sigmoid(yOld[j]-b[j]);
		k1=(-(yOld[i]          )+sum+I[i])/t[i];
		k2=(-(yOld[i]+0.5*k1*dT)+sum+I[i])/t[i];
		k3=(-(yOld[i]+0.5*k2*dT)+sum+I[i])/t[i];
		k4=(-(yOld[i]  +  k3*dT)+sum+I[i])/t[i];
		y[i]+=(k1+2*k2+2*k3+k4)/6;
	}
	free(yOld);
	return;
}
