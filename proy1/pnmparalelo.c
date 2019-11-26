#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

 
int main(int argc, char* argv[]) {
    // Revisar argumento
    if (argc != 2){
        printf("Uso: pnmserial <imagen pnm>\n");
        return 0;
    }

    char* filename = argv[1];
    char str[255];
    FILE *fp;
    FILE *fpGray;
    FILE *fpNeg;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("No se pudo abrir archivo %s",filename);
        return 1;
    }

    // Lectura del numero magico
    if (fscanf(fp, "%s", str) == 1){
        if (str[0] != '#'){
            if (strcmp(str, "P3") != 0){
                printf("Imagen debe ser a color y en formato texto\n");
                return 1;
            }
        } else {
            // Ignorar comentario
            fgets(str, 255, fp);
        }
    } else {
        printf("Error en formato de imagen\n");
        return 1;
    }

    int sizeX, sizeY, maxVal;
    int rank,size,n_local,remainder,TAG=1,dest=0,source,sourcefpGray,sourcefpNeg,n,**bufr,**bufb,**bufg;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Lectura de tamaño y valor máximo
    if(rank==0){
    
    int arr[3];
    for (int i = 0; i < 3; ){
        if (fscanf(fp, "%s", str) == 1){
            if (str[0] == '#'){
                // ignorar comentario
                fgets(str, 255, fp);
            } else {
                sscanf(str, "%d", &arr[i]);
                i++;
            }
        } else {
            printf("Error en formato de imagen\n");
            return 1;
             }
        
        }
    
    sizeX = arr[0];
    sizeY = arr[1];
    maxVal = arr[2];
    
    // No se declaran dinamcamente porque no se necesita modificacion posterior en tamaño
    // Memoria alojada en stack
    int r[sizeX][sizeY];
    int g[sizeX][sizeY];
    int b[sizeX][sizeY];

    //int gray[sizeX][sizeY];

    // Header archivo escala grises
    char *filename_dot = strchr(filename, '.');
    int offset = filename_dot - filename;
    filename[offset] = '\0';
    FILE* fpGray = fopen(strcat(filename, "-gray.pnm"), "w");
    if (fp == NULL){
        printf("No se pudo crear archivo %s", strcat(filename, "-gray.pnm"));
        return 1;
    }
    fprintf(fpGray, "P2\n%d %d\n%d\n", sizeX, sizeY, maxVal);

    // Header archivo negativo
    filename[offset] = '\0';
    FILE* fpNeg = fopen(strcat(filename, "-neg.pnm"), "w");
    if (fp == NULL){
        printf("No se pudo crear archivo %s", strcat(filename, "-neg.pnm"));
        return 1;
    }
    fprintf(fpNeg, "P3\n%d %d\n%d\n", sizeX, sizeY, maxVal);
    bufr = (double *) calloc(n, sizeof(double));
    bufg = (double *) calloc(n, sizeof(double));
    bufb = (double *) calloc(n, sizeof(double));
    MPI_Send(r,1,MPI_INT,dest,TAG,MPI_COMM_WORLD);
    MPI_Send(g,1,MPI_INT,dest,TAG,MPI_COMM_WORLD);
    MPI_Send(b,1,MPI_INT,dest,TAG,MPI_COMM_WORLD);
   // MPI_Send(&fpGray,1,MPI_INT,sourcefpGray,MPI_ANY_TAG,MPI_COMM_WORLD);
    //MPI_Send(fpNeg,1,MPI_FILE,sourcefpNeg,MPI_ANY_TAG,MPI_COMM_WORLD);
}
/////////////////INICIA PROCESOS PRARALELO///////////////////////
    // Lectura de valores rgb
    for (int x = 0; x < sizeX; x++){
        for (int y = 0; y < sizeY; y++){
            //para la division de los elementos entre los hilos
            n_local=sizeY/size;
            remainder=sizeY/size;
            for(int i=(n_local*rank)+1; i<=((n_local*rank)+n_local); ++i){
                source=i;

             MPI_Recv(bufr,1,MPI_INT,source,TAG,MPI_COMM_WORLD,&status);
             MPI_Recv(bufg,1,MPI_INT,source,TAG,MPI_COMM_WORLD,&status);
             MPI_Recv(bufb,1,MPI_INT,source,TAG,MPI_COMM_WORLD,&status);
           // MPI_Recv(&fpGray,1,MPI_INT,source,TAG,MPI_COMM_WORLD,&status);
                if (fscanf(fp, " %d %d %d", bufr[x][y],bufg[x][y], bufb[x][y]) == 3){
                 // Conversion a grises
                 //gray[x][y] = (r[x][y] + g[x][y] + b[x][y]) / 3;
                    fprintf(fpGray, "%d\n", (bufr[x][y] + bufg[x][y] + bufb[x][y]) / 3);

                 // Conversion a negativo
                 fprintf(fpNeg, "%d %d %d\n", maxVal-bufr[x][y], maxVal-bufg[x][y], maxVal-bufb[x][y]);
                } else {
                    printf("Error en formato de archivo\n");
                }
                ///////////////Para el remanente///////////////7
            if(rank==size-1 && remainder!=0){
                for(int i=(n_local*size)+1;i<=sizeX;++i){
                    // Conversion a grises
                 //gray[x][y] = (r[x][y] + g[x][y] + b[x][y]) / 3;
                    fprintf(fpGray, "%d\n", (bufr[x][y] + bufg[x][y] + bufb[x][y]) / 3);

                 // Conversion a negativo
                 fprintf(fpNeg, "%d %d %d\n", maxVal-bufr[x][y], maxVal-bufg[x][y], maxVal-bufb[x][y]);
                }
            }
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
//}
    MPI_Finalize();
    fclose(fp);
    fclose(fpGray);
    fclose(fpNeg);
    return 0;
}