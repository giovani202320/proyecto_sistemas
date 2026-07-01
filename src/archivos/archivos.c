#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../../include/archivos.h"

void listar_archivos() {
    struct dirent *de;
    struct stat info;
    DIR *dr = opendir("."); // Abre el directorio actual

    if (dr == NULL) {
        printf("\033[0;31m[Error] No se pudo abrir el directorio actual.\033[0m\n");
        return;
    }

    printf("\n--- Contenido del Directorio ---\n");
    printf("%-20s %-10s\n", "Nombre", "Bytes");
    printf("--------------------------------\n");

    while ((de = readdir(dr)) != NULL) {
        // Obtenemos las estadisticas del archivo
        stat(de->d_name, &info);
        printf("%-20s %-10ld\n", de->d_name, info.st_size);
    }
    closedir(dr);
}

void eliminar_archivo(const char *nombre) {
    char confirmacion;
    
    // Mostramos una advertencia en amarillo
    printf("\n\033[0;33m[Atencion] ¿Estas seguro que deseas eliminar permanentemente el archivo '%s'? (s/n): \033[0m", nombre);
    
    // Leemos un solo caracter
    if (scanf(" %c", &confirmacion) == 1) {
        while(getchar() != '\n'); // Limpiamos el buffer por si el usuario escribe mas cosas
        
        if (confirmacion == 's' || confirmacion == 'S') {
            // Recien aqui procedemos a borrar
            if (unlink(nombre) == 0) {
                printf("\n[Exito] Archivo '%s' eliminado correctamente.\n", nombre);
            } else {
                printf("\n\033[0;31m[Error] No se pudo eliminar '%s'. (Asegurate de que no sea una carpeta y exista)\033[0m\n", nombre);
            }
        } else {
            printf("\nOperacion cancelada. Tu archivo esta a salvo.\n");
        }
    }
}
void modulo_archivos() {
    int opcion;
    char buffer[256];

    do {
        printf("\n========================================\n");
        printf("       SHELL DE ARCHIVOS\n");
        printf("========================================\n");
        printf("1. Listar archivos y tamaños\n");
        printf("2. Eliminar un archivo\n");
        printf("0. Regresar al menu principal\n");
        printf("========================================\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            while(getchar() != '\n');
            opcion = -1;
        }
        while(getchar() != '\n'); // Limpiar el salto de linea que deja scanf

        switch(opcion) {
            case 1:
                listar_archivos();
                break;
            case 2:
                printf("\nIngrese el nombre exacto del archivo a eliminar:\n> ");
                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    buffer[strcspn(buffer, "\n")] = 0; // Quitar el enter
                    if (strlen(buffer) > 0) {
                        eliminar_archivo(buffer);
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
