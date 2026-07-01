#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "../../include/descargas.h"

#define MAX_DESCARGAS 10

typedef struct {
    char nombre_archivo[256];
    int progreso; // De 0 a 100
    int estado;   // 0: Vacio, 1: En cola, 2: Descargando, 3: Completado
} Descarga;

// Variables globales para la cola y el hilo
Descarga cola[MAX_DESCARGAS];
pthread_t hilo_descargas;
pthread_mutex_t mutex_cola = PTHREAD_MUTEX_INITIALIZER;
int hilo_iniciado = 0;

// Esta es la funcion que vivira en segundo plano (El trabajador)
void* procesar_descargas(void* arg) {
    (void)arg; // Evitar warning de parametro no usado
    
    while (1) {
        int indice_actual = -1;

        // Buscamos el primer archivo en cola de forma segura (usando el candado Mutex)
        pthread_mutex_lock(&mutex_cola);
        for (int i = 0; i < MAX_DESCARGAS; i++) {
            if (cola[i].estado == 1) { // Si esta "En cola"
                indice_actual = i;
                cola[i].estado = 2; // Cambiamos a "Descargando"
                break;
            }
        }
        pthread_mutex_unlock(&mutex_cola);

        // Si encontramos un archivo, simulamos su descarga
        if (indice_actual != -1) {
            for (int p = 0; p <= 100; p += 20) {
                pthread_mutex_lock(&mutex_cola);
                cola[indice_actual].progreso = p;
                pthread_mutex_unlock(&mutex_cola);
                sleep(1); // Simulamos el tiempo de red (1 segundo por cada 20%)
            }
            // Marcamos como completado
            pthread_mutex_lock(&mutex_cola);
            cola[indice_actual].estado = 3; 
            pthread_mutex_unlock(&mutex_cola);
        } else {
            // Si no hay nada en cola, el hilo descansa medio segundo antes de volver a revisar
            usleep(500000); 
        }
    }
    return NULL;
}

void agregar_a_cola(const char* archivo) {
    pthread_mutex_lock(&mutex_cola);
    for (int i = 0; i < MAX_DESCARGAS; i++) {
        // CORRECCION: Solo buscamos espacios 100% vacios
        if (cola[i].estado == 0) { 
            strncpy(cola[i].nombre_archivo, archivo, 255);
            cola[i].progreso = 0;
            cola[i].estado = 1;
            printf("\n\033[0;32m[Evento] Archivo '%s' agregado a la cola de descargas.\033[0m\n", archivo);
            pthread_mutex_unlock(&mutex_cola);
            return;
        }
    }
    pthread_mutex_unlock(&mutex_cola);
    printf("\n\033[0;31m[Error] La cola de descargas esta llena.\033[0m\n");
}
void mostrar_progreso() {
    printf("\n--- Estado de las Descargas ---\n");
    int activas = 0;
    
    pthread_mutex_lock(&mutex_cola);
    for (int i = 0; i < MAX_DESCARGAS; i++) {
        if (cola[i].estado != 0) {
            activas++;
            printf("Archivo: %-20s | Progreso: [", cola[i].nombre_archivo);
            
            // Dibujar la barra de progreso
            int barras = cola[i].progreso / 10;
            for(int b=0; b<10; b++) {
                if(b < barras) printf("#");
                else printf("-");
            }
            
            printf("] %3d%% | Estado: ", cola[i].progreso);
            if (cola[i].estado == 1) printf("En cola\n");
            else if (cola[i].estado == 2) printf("\033[0;33mDescargando...\033[0m\n");
            else if (cola[i].estado == 3) printf("\033[0;32mCompletado\033[0m\n");
        }
    }
    pthread_mutex_unlock(&mutex_cola);

    if (activas == 0) {
        printf("No hay descargas en el historial.\n");
    }
}

void modulo_descargas() {
    // Iniciamos el hilo de descargas en segundo plano solo la primera vez que entramos
    if (!hilo_iniciado) {
        memset(cola, 0, sizeof(cola)); // Limpiamos la memoria del arreglo
        if (pthread_create(&hilo_descargas, NULL, procesar_descargas, NULL) != 0) {
            printf("\033[0;31m[Error] No se pudo iniciar el motor de descargas.\033[0m\n");
            return;
        }
        hilo_iniciado = 1;
    }

    int opcion;
    char buffer[256];

    do {
        printf("\n========================================\n");
        printf("          COLA DE DESCARGAS\n");
        printf("========================================\n");
        printf("1. Agregar nueva descarga (Simulacion)\n");
        printf("2. Ver progreso y eventos\n");
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
                printf("\nIngrese un nombre o URL simulada para descargar:\n> ");
                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    buffer[strcspn(buffer, "\n")] = 0;
                    if (strlen(buffer) > 0) agregar_a_cola(buffer);
                }
                break;
            case 2:
                mostrar_progreso();
                break;
            case 0:
                printf("\nRegresando al menu principal...\n");
                break;
            default:
                printf("\n\033[0;31m[Error] Opcion invalida.\033[0m\n");
        }
    } while(opcion != 0);
}
