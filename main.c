#include <stdio.h>
#include <stdlib.h>

void mostrar_menu() {
    printf("\n========================================\n");
    printf("      PROGRAMA ADMIN EN LINUX\n");
    printf("========================================\n");
    printf("1. Administrador de Tareas (Procesos)\n");
    printf("2. Shell de Archivos\n");
    printf("3. Ejecucion de Comandos\n");
    printf("4. Gestion de Respaldos\n");
    printf("5. Analizador de Bash\n");
    printf("6. Cola de Descargas\n");
    printf("0. Salir\n");
    printf("========================================\n");
    printf("Seleccione una opcion: ");
}

int main() {
    int opcion;

    do {
        mostrar_menu();
        if (scanf("%d", &opcion) != 1) {
            // Limpiar el buffer si el usuario ingresa una letra
            while(getchar() != '\n'); 
            opcion = -1;
        }

        switch(opcion) {
            case 1:
                printf("\n[!] Modulo de Procesos en construccion...\n");
                break;
            case 2:
                printf("\n[!] Modulo de Archivos en construccion...\n");
                break;
            case 3:
                printf("\n[!] Modulo de Comandos en construccion...\n");
                break;
            case 4:
                printf("\n[!] Modulo de Respaldos en construccion...\n");
                break;
            case 5:
                printf("\n[!] Analizador de Bash en construccion...\n");
                break;
            case 6:
                printf("\n[!] Cola de descargas en construccion...\n");
                break;
            case 0:
                printf("\nSaliendo del programa...\n");
                break;
            default:
                printf("\n[Error] Opcion invalida. Intente de nuevo.\n");
        }
    } while(opcion != 0);

    return 0;
}
