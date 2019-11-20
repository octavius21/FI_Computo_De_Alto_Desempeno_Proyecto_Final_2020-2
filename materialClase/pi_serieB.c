#include <stdio.h>
#include <math.h>

double f(double a){
	return (4.0/(1.0+a*a));
}
int main(int argc, char argv[]){
	int n,i;
	double PI25DT = 3.141592653589793238462643;
	double x,pi,h,sum;
	n=100;
	h=1.0/(double) n;
	sum=0.0;
	for(i=1;i<=n;i++){
		x = h*((double)i-0.5);
		sum += f(x);
	}
	pi = h*sum;
	printf("\n pi es aproximadamente %.16f, el error cometido es  %.16f\n",pi,fabs(pi-PI25DT));
}
