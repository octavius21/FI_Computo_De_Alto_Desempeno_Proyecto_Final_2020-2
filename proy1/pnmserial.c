#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* openFile(char* filename, char* option);
 
int main(int argc, char* argv[]){
    // Revisar argumento
    if (argc < 2){
        printf("Uso: pnmserial <imagen pnm>\n");
        return 0;
    }

    char* filename = argv[1];
    char str[255];
    FILE *fp;

    fp = openFile(argv[1], "r");

    // Lectura del numero magico
    if (fscanf(fp, "%254s", str) == 1){
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
    // Lectura de tamaño y valor máximo
    int arr[3];
    for (int i = 0; i < 3; ){
        if (fscanf(fp, "%254s", str) == 1){
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
    strcat(filename, "-gray.pnm");
    FILE* fpGray = openFile(filename, "w");
    fprintf(fpGray, "P2\n%d %d\n%d\n", sizeX, sizeY, maxVal);

    // Header archivo negativo
    filename[offset] = '\0';
    strcat(filename, "-neg.pnm");
    FILE* fpNeg = openFile(filename, "w");
    fprintf(fpNeg, "P3\n%d %d\n%d\n", sizeX, sizeY, maxVal);

    // Lectura de valores rgb
    for (int x = 0; x < sizeX; x++){
        for (int y = 0; y < sizeY; y++){
            if (fscanf(fp, " %d %d %d", &r[x][y], &g[x][y], &b[x][y]) == 3){
                // Conversion a grises
                //gray[x][y] = (r[x][y] + g[x][y] + b[x][y]) / 3;
                fprintf(fpGray, "%d\n", (r[x][y] + g[x][y] + b[x][y]) / 3);

                // Conversion a negativo
                fprintf(fpNeg, "%d %d %d\n", maxVal-r[x][y], maxVal-g[x][y], maxVal-b[x][y]);
            } else {
                printf("Error en formato de archivo\n");
            }
        }
    }
    fclose(fp);
    fclose(fpGray);
    fclose(fpNeg);
    return 0;
}

// Devuelve puntero FILE abierto. Si falla termina el programa
FILE* openFile(char* filename, char* option){
    FILE* fp = fopen(filename, option);
    if (fp == NULL){
        printf("No se pudo abrir archivo %s", filename);
        exit(1);
    }
    return fp;
}
