#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

FILE* openFile(char* filename, char* option);
char* appendToFilename(char* filename, char* str);
char* strupr(char *str);

 
int main(int argc, char* argv[]) {
    // Revisar argumento
    if (argc != 4){
        printf("Uso: hillserial <n(dimension de matriz)> <archivo-matriz> <archivo-mensaje>\n");
        return 0;
    }

    int n = argv[1];
    char str[255];

    // Abre archivo matriz
    FILE* fp = openFile(argv[2], "r");
    int A[n][n];
    // Lee matriz de archivo
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n ; j++){
            fscanf("%d", A[i][j]);
        }
    }
    fclose(fp);

    // Abre archivo alfabeto
    FILE* fp = openFile("alfabeto.txt", "r");

    // Revisa tamaño del alfabeto
    int sizeAlph = 0;
    for(;  fgets(str, 255, fp) == 1; sizeAlph++);
    int alph[sizeAlph][2];

    // Lee archivo alfabeto
    for (int i = 0; i < sizeAlph; i++){
        fscanf(fp, "%c %d", alph[i][0], alph[i][1]);
    }
    fclose(fp);

    // Abre archivo a cifrar
    FILE* fpMsg = openFile(argv[3], "r");

    // Abre archivo cifrado
    char* filename = appendToFilename(argv[3], "-crypt");
    FILE* fpCrypt = openFile(filename, "w");

    // Lee palabras sin espacios
    while (fscanf(fpMsg, "%254[a-zA-Z]", str) == 1){
            strupr(str);
    }

    

    fclose(fp);
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

// Agrega una cadena a un nombre de archivo sin modificar su extension AUN NO FUNCIONA
char* appendToFilename(char* filename, char* str){
    char* result = strcpy(result, filename);
    char *extension = strrchr(result, '.');
    int offset = extension - result;
    result[offset] = '\0';
    strcat(result, str);
    strcat(result, extension);
    return result;
}

// Cambia letras en una cadena a MAYUSCULAS
char* strupr(char *str){
  unsigned char *p = (unsigned char *)str;

  while (*p) {
     *p = toupper((unsigned char)*p);
      p++;
  }

  return str;
}