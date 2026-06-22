#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/comandos.h"

#define MAX_HISTORIAL 100
#define MAX_LONGITUD_COMANDO 256

// Variables globales para este módulo
char historial[MAX_HISTORIAL][MAX_LONGITUD_COMANDO];
int contador_historial = 0;

void agregar_historial(const char *comando) {
    if (contador_historial < MAX_HISTORIAL) {
        strncpy(historial[contador_historial], comando, MAX_LONGITUD_COMANDO - 1);
        historial[contador_historial][MAX_LONGITUD_COMANDO - 1] = '\0';
        contador_historial++;
    }
}

void mostrar_historial() {
    printf("\n--- Historial de Comandos ---\n");
    if (contador_historial == 0) {
        printf("El historial esta vacio.\n");
        return;
    }
    for (int i = 0; i < contador_historial; i++) {
        printf("%d: %s\n", i + 1, historial[i]);
    }
}

void ejecutar_comando(const char *comando) {
    char buffer[1024];
    char comando_completo[MAX_LONGITUD_COMANDO + 30];
    
    // Redirigir stderr (canal 2) a un archivo temporal para capturar los errores aparte
    snprintf(comando_completo, sizeof(comando_completo), "%s 2> /tmp/errores_cmd.txt", comando);

    // popen abre un proceso y nos permite leer su salida estándar
    FILE *fp = popen(comando_completo, "r");
    if (fp == NULL) {
        printf("\033[0;31m[Error] No se pudo invocar el proceso.\033[0m\n");
        return;
    }

    printf("\n--- Salida del Comando ---\n");
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }
    pclose(fp);

    // Leer el archivo temporal para ver si hubo errores
    FILE *f_err = fopen("/tmp/errores_cmd.txt", "r");
    if (f_err != NULL) {
        fseek(f_err, 0, SEEK_END);
        long tam = ftell(f_err); // Verificar si el archivo tiene peso
        if (tam > 0) {
            rewind(f_err);
            printf("\n--- Errores del Comando ---\n");
            while (fgets(buffer, sizeof(buffer), f_err) != NULL) {
                printf("\033[0;31m%s\033[0m", buffer); // \033[0;31m imprime texto en rojo
            }
        }
        fclose(f_err);
        remove("/tmp/errores_cmd.txt"); // Limpiar la basura
    }
    
    agregar_historial(comando);
}

void modulo_comandos() {
    char comando[MAX_LONGITUD_COMANDO];
    int opcion;

    do {
        printf("\n========================================\n");
        printf("       MODULO DE COMANDOS LINUX\n");
        printf("========================================\n");
        printf("1. Ejecutar nuevo comando\n");
        printf("2. Ver historial de comandos\n");
        printf("0. Regresar al menu principal\n");
        printf("========================================\n");
        printf("Seleccione una opcion: ");
        
        if (scanf("%d", &opcion) != 1) {
            while(getchar() != '\n'); 
            opcion = -1;
        }
        
        while(getchar() != '\n'); // Limpiar el buffer para fgets

        switch(opcion) {
            case 1:
                printf("\nIngrese el comando (ej. ls -la, pwd, cat archivo_falso):\n> ");
                if (fgets(comando, sizeof(comando), stdin) != NULL) {
                    comando[strcspn(comando, "\n")] = 0; // Quitar el salto de linea
                    if (strlen(comando) > 0) {
                        ejecutar_comando(comando);
                    }
                }
                break;
            case 2:
                mostrar_historial();
                break;
            case 0:
                printf("\nRegresando al menu principal...\n");
                break;
            default:
                printf("\n\033[0;31m[Error] Opcion invalida.\033[0m\n");
        }
    } while(opcion != 0);
}
