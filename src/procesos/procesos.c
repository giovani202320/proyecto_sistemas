#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <signal.h> // Nueva libreria para mandar senales (kill)
#include "../../include/procesos.h"

int es_numero(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

void listar_procesos() {
    struct dirent *de;
    DIR *dr = opendir("/proc");
    FILE *fp;
    // FIX: Aumentamos el tamano del buffer a 512 para evitar el warning
    char ruta_comm[512]; 
    char nombre_proceso[256];

    if (dr == NULL) {
        printf("\033[0;31m[Error] No se pudo abrir /proc.\033[0m\n");
        return;
    }

    printf("\n--- Lista de Procesos Activos ---\n");
    printf("%-10s %-20s\n", "PID", "Nombre");
    printf("-----------------------------------\n");

    int count = 0;
    while ((de = readdir(dr)) != NULL && count < 20) {
        if (de->d_type == DT_DIR && es_numero(de->d_name)) {
            snprintf(ruta_comm, sizeof(ruta_comm), "/proc/%s/comm", de->d_name);
            
            fp = fopen(ruta_comm, "r");
            if (fp != NULL) {
                if (fgets(nombre_proceso, sizeof(nombre_proceso), fp) != NULL) {
                    nombre_proceso[strcspn(nombre_proceso, "\n")] = 0;
                    printf("%-10s %-20s\n", de->d_name, nombre_proceso);
                    count++;
                }
                fclose(fp);
            }
        }
    }
    closedir(dr);
    printf("... (Mostrando los primeros 20 procesos)\n");
}

void finalizar_proceso(int pid) {
    // La funcion kill envia una senal al proceso. SIGKILL (9) lo fuerza a cerrar.
    if (kill(pid, SIGKILL) == 0) {
        printf("\n[Exito] Se envio la senal de finalizacion al PID %d.\n", pid);
    } else {
        printf("\n\033[0;31m[Error] No se pudo finalizar el PID %d. (El proceso no existe o faltan permisos de root)\033[0m\n", pid);
    }
}

void modulo_procesos() {
    int opcion;
    int pid_objetivo;

    do {
        printf("\n========================================\n");
        printf("       ADMINISTRADOR DE TAREAS\n");
        printf("========================================\n");
        printf("1. Listar procesos\n");
        printf("2. Finalizar proceso (Kill)\n");
        printf("0. Regresar al menu principal\n");
        printf("========================================\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            while(getchar() != '\n');
            opcion = -1;
        }
        while(getchar() != '\n');

        switch(opcion) {
            case 1:
                listar_procesos();
                break;
           case 2:
                printf("\nIngrese el PID del proceso a finalizar:\n> ");
                if (scanf("%d", &pid_objetivo) == 1) {
                    if (pid_objetivo <= 0) {
                        printf("\033[0;31m[Error] El PID debe ser mayor a 0. ¡No te auto-elimines!\033[0m\n");
                    } else if (pid_objetivo <= 10) {
                        printf("\033[0;31m[Error] Operacion denegada. Esos son procesos criticos del sistema.\033[0m\n");
                    } else {
                        finalizar_proceso(pid_objetivo);
                    }
                } else {
                    printf("\033[0;31m[Error] PID invalido.\033[0m\n");
                }
                while(getchar() != '\n'); // Limpiar buffer despues del scanf
                break;
	    case 0:
                printf("\nRegresando al menu principal...\n");
                break;
            default:
                printf("\n\033[0;31m[Error] Opcion invalida.\033[0m\n");
        }
    } while(opcion != 0);
}
