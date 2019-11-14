#include <stdio.h>
#include <math.h>
#include <mpi.h>

double f(double a){
	return (4.0/(1.0+a*a));
}

int main(int argc, char *argv[]){
	int n,i;
	double PI25DT = 3.141592653589793238462643;
	double x,pi,h,sum;
///////////////nuevas////////////////
	int source,dest,tag=100,rank,size,offset;
	double sum_local;
	MPI_Status status;
/////////////////////////////////////
///////////Inicia Ambiente MPI //////
	MPI_Init(&argc,&argv);
////////////////////////////////////
//////////Datos del ambiente////////
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	printf("\nProceso %d de %d", rank, size);
///////////////////////////////////
///////////////Todos///////////////
	n=1000000000000;
	h=1.0/(double) n;
	sum=0.0;
	offset=n/size;
/////////////rank=0///////////////
if (rank==0) {
	for(i=0;i<offset;i++)
	{
	/*for(i=1;i<=n;i++){*/
		x = h*((double)i-0.5);
		sum_local += f(x);
	}/*for*/
printf("\n Proceso %d de %d, sum_local= %.16f\n",rank,size,sum_local);

}/*rank0*/
///////////////////////////////////
/////////////1<rank<size-2/////////
if(rank>0 && rank<=size-2){
	for(i=(rank*offset);i<(offset*(rank+1));i++){
		x=h*((double)i-0.5);
		sum_local += f(x);
	}/*for*/
printf("\n Proceso %d de %d, sum_local= %.16f\n",rank,size,sum_local);

}/*if*/
//////////////////////////////////
///////////rank = size -1/////////
if(rank==size-1){
	for(i=(rank*offset);i<n;i++){
		x=h*((double)i-0.5);
		sum_local += f(x);
	}/*for*/
printf("\n Proceso %d de %d, sum_local= %.16f\n",rank,size,sum_local);

}/*if*/
//////////////////////////////////
/////////////rank = 0/////////////
if(rank==0){
	sum=sum_local;
	for(i=1;i<size;i++){
		source=i;
		MPI_Recv(&sum_local,1,MPI_DOUBLE,source,tag,MPI_COMM_WORLD, &status);
		sum += sum_local;
	}/*for*/

	pi = h*sum;
	printf("\n pi es aproximadamente %.16f, el error cometido es  %.16f\n",pi,fabs(pi-PI25DT));
}/*if*/
///////////////////////////////////
//////////Todos los demás/////////
else{
	dest=0;
	MPI_Send(&sum_local,1,MPI_DOUBLE,dest,tag,MPI_COMM_WORLD);
}
//////////////////////////////////
////////////////rank = 0/////////
MPI_Finalize();
return 0;
}
