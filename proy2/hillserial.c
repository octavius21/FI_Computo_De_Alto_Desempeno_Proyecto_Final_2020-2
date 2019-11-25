/*
 *  Daniel Ponce de León Súchil
 *  Luis Octavio Gómez de la Cruz
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <omp.h>

struct Alphabet{
    char simbol;
    int value;
};

// Devuelve puntero FILE abierto. Si falla termina el programa
FILE* openFile(char* filename, char* option);

// Devuelve el respectivo valor numerico de c segun el alfabeto
int alphCharToInt(char c, struct Alphabet alph[], int sizeAlph);

// Davuelve el caracter c correspondiente al numero val segun alfabeto
char alphIntToChar(int val, struct Alphabet alph[], int sizeAlph);

// Multiplica mat[n][n] * vect[n] = res[n]
void multiply(int n, int mat[n][n], int vect[n], int res[n]);

// Args: n, path-matriz, path-mensaje
int main(int argc, char* argv[]) {
    // Revisar argumento
    if (argc != 4){
        printf("Uso: hillserial <n(dimension de matriz)> <archivo-matriz> <archivo-mensaje>\n");
        return 0;
    }

    int n = atoi(argv[1]);
    char str[255];

    // Abre archivo matriz
    FILE* fp = openFile(argv[2], "r");
    int matA[n][n];
    // Lee matriz de archivo
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n ; j++){
            fscanf(fp, "%d", &matA[i][j]);
        }
    }
    fclose(fp);

    // Abre archivo alfabeto
    fp = openFile("alfabeto.txt", "r");

    // Revisa tamaño del alfabeto
    int sizeAlph = 0;
    for(;  fgets(str, 255, fp) != NULL; sizeAlph++);
    fclose(fp);
    
    // Crea alfabeto
    struct Alphabet alph[sizeAlph];

    // Lee archivo alfabeto
    fp = openFile("alfabeto.txt", "r");
    for (int i = 0; feof(fp) == 0; i++){
        fscanf(fp, "%c %d\n", &alph[i].simbol, &alph[i].value);
    }
    fclose(fp);

    // Abre archivo a cifrar
    fp = openFile(argv[3], "r");

    // Abre archivo cifrado
    FILE* fpWr = openFile("treated.txt", "w");

    // Lee palabras caracteres alfabeticos y convierte a MAYUSCULAS
    char c;
    while (feof(fp) == 0){
        c = fgetc(fp);
        if (isalpha(c)){
            fputc(toupper(c), fpWr);
        }
    }
    fclose(fp);
    fclose(fpWr);

    // Abrir archivo intermedio y destino
    fp = openFile("treated.txt", "r");
    fpWr = openFile ("encrypted.txt", "w");
    int vectX[n];
    int vectY[n];
    int loop = 1;
    // Lee del archivo y agrupa en n caracteres
    for (int i = 0; loop; i++){
        // No se ha llegado a EOF
        if (feof(fp) == 0){
            c = fgetc(fp);
            // Guarda valor del caracter leido segun alfabeto
            vectX[i] = alphCharToInt(c, alph, sizeAlph);

        // Llego EOF y no termino agrupacion
        } else if(i < (n-1)){
            // se iteran elementos restantes
            for (; i < n; i++){
                vectX[i] = 23; // X
                // Se hace ultima transformacion y se deja de iterar
                loop = 0;
            }
        }

        // Se termino de agrupar vector
        if (i >= (n-1)){
            // Transformación lineal a vectY
            multiply(n, matA, vectX, vectY);
            // Escribir vectY a char en archivo destino
            for (int j = 0; j < n; j++){
                // Convierte valor num a char segun alfabeto
                vectY[j] = vectY[j] % sizeAlph;
                c = alphIntToChar(vectY[j], alph, sizeAlph);
                fprintf(fpWr, "%c", c);
            }
            i = 0;
        }
    }
    remove("treated.txt");
    fclose(fp);
    fclose(fpWr);
    return 0;
}


FILE* openFile(char* filename, char* option){
    FILE* fp = fopen(filename, option);
    if (fp == NULL){
        printf("No se pudo abrir archivo %s", filename);
        exit(1);
    }
    return fp;
}

int alphCharToInt(char c, struct Alphabet alph[], int sizeAlph){
    for(int i = 0; i < sizeAlph; i++){
            if (c == alph[i].simbol){
                return alph[i].value;
            }
        }
}

char alphIntToChar(int val, struct Alphabet alph[], int sizeAlph){
    for(int i = 0; i < sizeAlph; i++){
            if (val == alph[i].value){
                return alph[i].simbol;
            }
        }
}

void multiply(int n, int mat[n][n], int vect[n], int res[n]){ 
    int i, j, k; 
    for (i = 0; i < n; i++) 
    {
        res[i] = 0;
        for (j = 0; j < n; j++) 
        {
            res[i] += mat[i][j] * vect[j]; 
        } 
    } 
}
