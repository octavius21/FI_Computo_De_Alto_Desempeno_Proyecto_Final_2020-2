#include <stdio.h>

int main(int argc, char *argv[])
{
int a[5][5]={{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5}};
int b[5][2]={{2,2},{2,2},{2,2},{2,2},{2,2}};
int c[5][2]={{0,0},{0,0},{0,0},{0,0},{0,0}};

int i,j,k;

for(i=0;i<5;i++){
	printf("\n");
	for(j=0;j<5;j++){
	printf(" a[%d][%d]=%d \t",i,j,a[i][j]);
	}
}

printf("\n"); 

for (i=0;i<5;i++){
 	for(j=0;j<2;j++){
	  for(k=0;k<5;k++){
	c[i][j]+=a[i][k]*b[k][j];
	}
    }
}


for(i=0;i<5;i++){
printf("\n");
	for(j=0;j<2;j++){
	printf("c[%d][%d]=%d\t",i,j,c[i][j]);
	}
}

return 0;
}
