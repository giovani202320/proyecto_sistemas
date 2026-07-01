#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/bash_analyzer.h"

void analizar_script(const char *ruta) {
    FILE *fp = fopen(ruta, "r");
    if (fp == NULL) {
        printf("\033[0;31m[Error] No se pudo abrir el archivo '%s'. Verifica la ruta y que sea un archivo de texto.\033[0m\n", ruta);
        return;
    }

    char linea[512];
    int num_linea = 1;
    int contador_variables = 0;
    int contador_ciclos = 0;

    printf("\n--- Analizando script: %s ---\n", ruta);

    while (fgets(linea, sizeof(linea), fp) != NULL) {
        // Ignorar lineas que sean comentarios o esten vacias
        if (linea[0] == '#' || linea[0] == '\n') {
            num_linea++;
            continue;
        }

        // Detectar asignacion de variables (busca '=' pero excluye el comparador '==')
        if (strchr(linea, '=') != NULL && strstr(linea, "==") == NULL) {
            printf("[Linea %d] Variable detectada -> %s", num_linea, linea);
            contador_variables++;
        }

        // Detectar ciclos ('for ' o 'while ')
        if (strstr(linea, "for ") != NULL || strstr(linea, "while ") != NULL) {
            printf("\033[0;33m[Linea %d] Ciclo detectado -> %s\033[0m", num_linea, linea);
            contador_ciclos++;
        }

        num_linea++;
    }

    fclose(fp);
    
    printf("\n--- Resumen del Analisis ---\n");
    printf("Total de variables encontradas: %d\n", contador_variables);
    printf("Total de ciclos encontrados: %d\n", contador_ciclos);
}

void modulo_bash() {
    int opcion;
    char ruta_archivo[256];

    do {
        printf("\n========================================\n");
        printf("         ANALIZADOR DE BASH\n");
        printf("========================================\n");
        printf("1. Analizar un script (.sh)\n");
        printf("0. Regresar al menu principal\n");
        printf("========================================\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            while(getchar() != '\n');
            opcion = -1;
        }
        while(getchar() != '\n'); // Limpiar buffer

        switch(opcion) {
            case 1:
                printf("\nIngrese la ruta del script a analizar (ej. /home/giovani/script.sh o solo script.sh):\n> ");
                if (fgets(ruta_archivo, sizeof(ruta_archivo), stdin) != NULL) {
                    ruta_archivo[strcspn(ruta_archivo, "\n")] = 0; // Quitar el enter
                    if (strlen(ruta_archivo) > 0) {
                        analizar_script(ruta_archivo);
                    }
                }
                break;
            case 0:
                printf("\nRegresando al menu principal...\n");
                break;
            default:
                printf("\n\033[0;31m[Error] Opcion invalida.\033[0m\n");
        }
    } while(opcion != 0);
}
