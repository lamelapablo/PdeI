#include <stdio.h>
#include <time.h>

// Función para medir el tiempo de ejecución de otra función
double medir_tiempo(void (*func)()) {
    clock_t inicio, fin;
    double tiempo;

    inicio = clock(); // Marca de tiempo inicial

    func(); // Llamada a la función cuyo tiempo de ejecución queremos medir

    fin = clock(); // Marca de tiempo final

    // Cálculo del tiempo transcurrido en segundos
    tiempo = ((double)(fin - inicio)) / CLOCKS_PER_SEC;

    return tiempo;
}

// Función de ejemplo que se utilizará para medir su tiempo de ejecución
void funcion_ejemplo() {
    int i;
    for (i = 0; i < 1000; i++) {
        // Simulación de una tarea que lleva tiempo
    }
}

void funcion_ejemplo2() {
    int i;
    for (i = 0; i < 100000; i++) {
        // Simulación de una tarea que lleva tiempo
    }
}

void funcion_ejemplo3() {
    int i;
    for (i = 0; i < 100000000; i++) {
        // Simulación de una tarea que lleva tiempo
    }
}


int main() {
    double tiempo, tiempo2, tiempo3;

    // Medir el tiempo de ejecución de la función de ejemplo
    tiempo = medir_tiempo(funcion_ejemplo);
    tiempo2 = medir_tiempo(funcion_ejemplo2);
    tiempo3 = medir_tiempo(funcion_ejemplo3);

    // Imprimir el tiempo medido
    printf("Tiempo de ejecucion: %.6f segundos\n", tiempo);
    printf("Tiempo de ejecucion: %.6f segundos\n", tiempo2);
    printf("Tiempo de ejecucion: %.6f segundos\n", tiempo3);

    return 0;
}
