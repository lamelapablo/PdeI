#include <stdio.h>
#include <gmp.h>
#include <time.h>

int main() {
    // Inicializar variables
    mpz_t result;
    mpz_init(result);

    // Registrar el tiempo de inicio
    clock_t start_time = clock();

    // Calcular el factorial de 1000
    mpz_fac_ui(result, 1000);

    // Registrar el tiempo de finalización
    clock_t end_time = clock();

    // Calcular el tiempo transcurrido en segundos y milisegundos
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    double time_taken_ms = time_taken * 1000;

    // Imprimir el resultado y el tiempo transcurrido
    printf("Factorial de 1000: ");
    mpz_out_str(stdout, 10, result);
    printf("\n");
    printf("Tiempo de ejecucion: %.2f segundos (%.2f milisegundos)\n", time_taken, time_taken_ms);

    // Liberar memoria
    mpz_clear(result);

    return 0;
}
