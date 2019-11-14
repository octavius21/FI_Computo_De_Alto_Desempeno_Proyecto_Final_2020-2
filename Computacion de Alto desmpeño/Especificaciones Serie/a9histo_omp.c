
/*histo_omp.c **********************************************************************************/ 
#include <omp.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/time.h> 
#define N 10 /* luego 6000 Orden de la matriz*/ 
#define NG 10 /* luego 256 Valores de cada pixel*/ 
int main () 
{ 
   struct timeval t0, t1; 
   double tej; 

 int IMA[N][N], histo[NG],histo1[NG], B[N], C[N]; 
 int i=0, j=0, hmin=0, imin=0, spm = 0, x=0; //tid

// Inicializacion de variables (aleatorio) 
 for(i=0; i<N; i++) 
   for(j=0; j<N; j++)
       IMA[i][j] = rand() % NG; 

for(i=0; i<NG; i++)  histo[i] = 0; 
 for(i=0; i<NG; i++)  histo1[i] = 0; 

   // toma de tiempos 
       gettimeofday(&t0, 0); 

        // 1. Calculo del histograma de IMA 
 #pragma omp parallel firstprivate(histo1,j)
 {       
	 for(i=0; i<NG; i++)  histo1[i] = 0; 
	 for(i=0; i<NG; i++)  histo[i] = 0; 
 #pragma omp for
 for (i=0; i<N; i++) 
       for (j=0; j<N; j++) 
            histo1[IMA[i][j]] ++;
	
 #pragma omp critical	
 		{
			for(i=0;i<N;i++) histo[i]+=histo1[i]; 
		}
}//cierra región paralela
///////////////////////////////////////////////////////////////////////
// 2. Busqueda del minimo del histograma 
        hmin = N*N+1; 
 #pragma omp parallel shared(hmin,imin)
 {
     #pragma omp for
     for (i=0; i<NG; i++)
      
          if (hmin > histo[i]) 
      #pragma omp critical
      {
			if (hmin > histo[i]) 
           { 
			 hmin = histo[i];
			 imin = i; 
		    } 
	   }
}//región paralela 2

///////////////////////////////////////////////////////////////////////
//3.
// Calculo de B, C y SPM 


#pragma omp parallel 
{
for(i=0; i<N; i++)  B[i] = 0; 
for(i=0; i<N; i++)  C[i] = 0;
	
 #pragma omp for firstprivate(j) firstprivate(x) reduction(+:spm)
 for (i=0; i<N; i++) 
  { 
      j = 0; 
      x = 0; 
       while ((IMA[i][j] != imin) && (j<N)) 
       { 
         x = x + IMA[i][j] ; 
         j++; 
         } 
 
    B[i] = x; 
    C[i] = j;
 
    spm = spm + j; 
  
 } 
}//cierre de region 3

// toma de tiempos 
 gettimeofday(&t1, 0); 

// Imprimir resultados 
printf("\n Histograma \n"); 
    for(i=0; i<10; i++) printf("%5d",i); 
    printf("\n"); 
  
   for(i=0; i<NG; i++) printf("%5d",histo[i]); 
    printf("\n hmin = %d imin = %d\n", hmin, imin); 

     printf("\n Vector B \n"); 
    for(i=0;i<NG;i++) printf(" %3d", B[i]); 

   printf("\n Vector C \n"); 
 for(i=0;i<NG;i++) printf(" %3d", C[i]); 

 printf("\n SPM = %d\n\n", spm); 
tej = (t1.tv_sec - t0.tv_sec) + (t1.tv_usec - t0.tv_usec) / 1e6; 
printf("\n T. ejec. (serie) = %1.3f ms \n\n", tej*1000); 
}

