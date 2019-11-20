#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "mpi.h"


int main(int argc, char* argv[]) 
{
  double *x, *y, *local_x, *local_y;
  int    res, n_local; 
  double  dot, local_dot,sum_local;
  int    inicio,size, rank, i, n;
  
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
///////////////// Rank = 0 ////////////////////////////////////////////  
  if (rank == 0) {
    printf("Tamaño del vector ");
     scanf("%d", &n);
    x = (double *) calloc(n, sizeof(double));
    y = (double *) calloc(n, sizeof(double));
		for (i=0; i<n; i++) 
		{
			x[i] = 1.0;  y[i] = 1.0;
		}
    }
  //////////////////// Todos //////////////////////////////////////////
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
  n_local = n/size;
  local_x = (double *) calloc(n_local, sizeof(double));
  local_y = (double *) calloc(n_local, sizeof(double));
  
  MPI_Scatter(x, n_local, MPI_DOUBLE, local_x, n_local, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Scatter(y, n_local, MPI_DOUBLE, local_y, n_local, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  
  
  local_dot = 0.0;
  for (i = 0; i < n_local; i++){
    local_dot += local_x[i] * local_y[i];
  }
  free(local_x);
  free(local_y); 
  //////////////////////////Rank = 0 //////////////////////////////////
  if (rank==0){
  res=n%size;
  printf("residuo: %d\n",res);
  
	if(res != 0)
		{
			sum_local=0.0;
			inicio=(size)*(n/size);
	    
		for(i=inicio;i<n;i++)
		{
			sum_local += x[i]*y[i];
			}
				printf("inicio : %d, n : %d, sum_local : %.2f\n",inicio,n,sum_local);
		}
	else sum_local=0.0;
	
  }
 /////////////////  Todos  ////////////////////////////////////////////   
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Reduce(&local_dot, &dot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  
  //////////////////  Rank = 0 ////////////////////////////////////////
  if (rank == 0){
	  printf("sum_local: %.2f, dot : %.2f\n",sum_local,dot);
	  dot+=sum_local;
    printf("El producto punto es %.2f \n", dot);
    
  }
 ///////////////////  Todos  /////////////////////////////////////////// 
  MPI_Finalize();
 return 0;
}  


