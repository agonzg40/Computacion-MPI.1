#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

#define N 10

int main(int argc, char *argv[])
{

    int size, rank, i, from, to, ndat, part, tag, sumaTotal;
    MPI_Status info;

    // Inicializo MPI
    MPI_Init(&argc, &argv);

    // Calculo cuántos procesos hay
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Calculo qué proceso soy
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *matriz, *matriz_results, *matriz_result_sum;
    if (rank == 0)
    {

        // Creacion matriz puntero 5X5 con valores aleatorios entre 0 y 100
        srand(time(NULL));

        matriz = (int *)malloc(N * N * sizeof(int *));
        for (int i = 0; i < N * N; i++)
        {
            //*(matriz + i) = rand() % 100;
            *(matriz + i) = 1;
        }

        for (i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                printf("%d ", *(matriz + i * N + j));
            }
            printf("\n");
        }
    }
    // Distribuyo la matriz a partes iguales
    ndat = (N*N) / size;
    matriz_results = (int *)malloc(ndat * sizeof(int *));

    // Envio los valores con scatter
    MPI_Scatter(&matriz[0], ndat, MPI_INT, &matriz_results[0], ndat, MPI_INT, 0, MPI_COMM_WORLD);

    //Sincronizar todos los procesos en este punto
    MPI_Barrier(MPI_COMM_WORLD);

    /*printf("Proceso %d: recibe los datos: \n", rank);
    for (i = 0; i < ndat; i++)
    {
        for (int j = 0; j < ndat; j++)
        {
            printf("%d ", *(matriz_results + i * ndat + j));
        }
        printf("\n");
    }*/

    //Suma de todos los elementos
    int suma = 0;
    for (i = 0; i < ndat; i++)
    {
        suma += *(matriz_results + i);
    }

    // Imprimo la suma
    printf("Proceso %d: suma = %d \n", rank, suma);

    //Todos envian la suma que han calculado
    matriz_results = (int *)malloc(size * sizeof(int *));
    MPI_Allreduce(&suma, &sumaTotal, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    //Sincronizar todos los procesos en este punto
    MPI_Barrier(MPI_COMM_WORLD);

    //Calculo el valor medio
    int aux = 0;
    float valorMedio = 0.0;
    valorMedio = (float)sumaTotal / (float)(N*N);

    //Imprimo el valor medio
    printf("Proceso %d: media = %f \n", rank, valorMedio);

    /*for(i = 0; i<ndat; i++){

    }*/


    // Cierro MPI
    MPI_Finalize();

    return 0;
}