#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define N 20
typedef uint64_t Fila;

void escribir_tablero(FILE* f, Fila* grid, int gen) {
    fprintf(f, "Generación %d\n", gen);
    for (int i = 0; i < N; i++) {
        for (int j = N - 1; j >= 0; j--) {
            fputc(((grid[i] >> j) & 1) ? 'O' : '.', f);
        }
        fputc('\n', f);
    }
    fputc('\n', f);
}

void siguiente_generacion(Fila* actual, Fila* siguiente) {
    for (int i = 0; i < N; i++) {
        siguiente[i] = 0;
        for (int j = 0; j < N; j++) {
            int vecinos = 0;
            for (int di = -1; di <= 1; di++) {
                int ni = i + di;
                if (ni < 0 || ni >= N) continue;
                for (int dj = -1; dj <= 1; dj++) {
                    if (di == 0 && dj == 0) continue;
                    int nj = j + dj;
                    if (nj < 0 || nj >= N) continue;
                    vecinos += (actual[ni] >> nj) & 1;
                }
            }
            int viva = (actual[i] >> j) & 1;
            if ((viva && (vecinos == 2 || vecinos == 3)) || (!viva && vecinos == 3)) {
                siguiente[i] |= (1ULL << j);
            }
        }
    }
}

void copiar(Fila* dst, Fila* src) {
    for (int i = 0; i < N; i++) {
        dst[i] = src[i];
    }
}

int leer_tablero(const char* nombre, Fila* grid) {
    FILE* f = fopen(nombre, "r");
    if (!f) {
        perror("Error al abrir el archivo");
        return 0;
    }

    char linea[64];
    for (int i = 0; i < N; i++) {
        if (!fgets(linea, sizeof(linea), f)) {
            fprintf(stderr, "Error: archivo tiene menos de 20 filas.\n");
            fclose(f);
            return 0;
        }

        int len = strlen(linea);
        if (linea[len - 1] == '\n') linea[len - 1] = '\0';

        if (strlen(linea) != N) {
            fprintf(stderr, "Error: fila %d no tiene 20 caracteres.\n", i + 1);
            fclose(f);
            return 0;
        }

        Fila fila = 0;
        for (int j = 0; j < N; j++) {
            if (linea[j] == 'O') {
                fila |= (1ULL << (N - 1 - j));
            } else if (linea[j] != '.') {
                fprintf(stderr, "Error: caracter inválido en fila %d, columna %d: '%c'\n", i + 1, j + 1, linea[j]);
                fclose(f);
                return 0;
            }
        }
        grid[i] = fila;
    }

    fclose(f);
    return 1;
}

int main() {
    char nombre[] = "tablero.txt";

    Fila actual[N] = {0};
    Fila siguiente[N] = {0};

    if (!leer_tablero(nombre, actual)) {
        return 1;
    }

    int generaciones;
    printf("Ingrese el numero de generaciones a simular: ");
    scanf("%d", &generaciones);

    FILE* archivo_salida = fopen("vida.txt", "w");
    if (!archivo_salida) {
        perror("No se pudo abrir archivo de salida");
        return 1;
    }

    for (int gen = 0; gen < generaciones; gen++) {
        escribir_tablero(archivo_salida, actual, gen);

        siguiente_generacion(actual, siguiente);
        copiar(actual, siguiente);
    }

    fclose(archivo_salida);
    return 0;
}