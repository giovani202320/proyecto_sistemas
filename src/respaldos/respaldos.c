#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "../../include/respaldos.h"

// Funcion auxiliar para copiar binariamente cualquier tipo de archivo
void copiar_archivo(const char *origen, const char *destino) {
    FILE *fo = fopen(origen, "rb");
    if (fo == NULL) {
        printf("\033[0;31m[Error] No se pudo abrir el archivo origen '%s'.\033[0m\n", origen);
        return;
    }
    
    FILE *fd = fopen(destino, "wb");
    if (fd == NULL) {
        printf("\033[0;31m[Error] No se pudo crear el archivo destino '%s'.\033[0m\n", destino);
        fclose(fo);
        return;
    }

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), fo)) > 0) {
        fwrite(buffer, 1, bytes, fd);
    }

    fclose(fo);
    fclose(fd);
    printf("\033[0;32m[Exito] Archivo guardado como: %s\033[0m\n", destino);
}

void crear_respaldo(const char *nombre) {
    // Validamos que el archivo original exista antes de intentar respaldarlo
    if (access(nombre, F_OK) != 0) {
        printf("\033[0;31m[Error] El archivo '%s' no existe en este directorio.\033[0m\n", nombre);
        return;
    }

    // Obtenemos la hora actual del sistema
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    char nombre_respaldo[512];
    // Formato: nombre_original_AÑO_MES_DIA_HORA_MIN_SEG.bak
    snprintf(nombre_respaldo, sizeof(nombre_respaldo), "%s_%04d%02d%02d_%02d%02d%02d.bak",
             nombre, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    
    printf("\nGenerando respaldo...\n");
    copiar_archivo(nombre, nombre_respaldo);
}

void restaurar_respaldo(const char *nombre_bak, const char *nombre_original) {
    if (access(nombre_bak, F_OK) != 0) {
        printf("\033[0;31m[Error] El respaldo '%s' no existe.\033[0m\n", nombre_bak);
        return;
    }
    printf("\nRestaurando version...\n");
    // Al copiar el .bak sobre el nombre original, lo sobreescribe/restaura
    copiar_archivo(nombre_bak, nombre_original);
}

void modulo_respaldos() {
    int opcion;
    char archivo_orig[256];
    char archivo_bak[256];

    do {
        printf("\n========================================\n");
        printf("         GESTION DE RESPALDOS\n");
        printf("========================================\n");
        printf("1. Crear respaldo de un archivo\n");
        printf("2. Restaurar archivo desde un respaldo\n");
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
                printf("\nIngrese el nombre del archivo a respaldar:\n> ");
                if (fgets(archivo_orig, sizeof(archivo_orig), stdin) != NULL) {
                    archivo_orig[strcspn(archivo_orig, "\n")] = 0;
                    if (strlen(archivo_orig) > 0) crear_respaldo(archivo_orig);
                }
                break;
            case 2:
                printf("\nIngrese el nombre exacto del archivo de respaldo (ej. archivo_2026...bak):\n> ");
                if (fgets(archivo_bak, sizeof(archivo_bak), stdin) != NULL) {
                    archivo_bak[strcspn(archivo_bak, "\n")] = 0;
                    
                    printf("Ingrese el nombre con el que desea restaurarlo (nombre original):\n> ");
                    if (fgets(archivo_orig, sizeof(archivo_orig), stdin) != NULL) {
                        archivo_orig[strcspn(archivo_orig, "\n")] = 0;
                        if (strlen(archivo_bak) > 0 && strlen(archivo_orig) > 0) {
                            restaurar_respaldo(archivo_bak, archivo_orig);
                        }
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
