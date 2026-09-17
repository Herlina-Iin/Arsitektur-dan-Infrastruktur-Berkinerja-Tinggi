#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 1000

int main(int argc, char *argv[])
{
    int myid, numprocs;
    int data[MAXSIZE], i, x, low, high, myresult = 0, result = 0;
    char fn[255];
    FILE *fp;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    if (myid == 0) { /* Open input file and initialize data */
        strcpy(fn, getenv("HOME"));
        strcat(fn, "/MPI/rand_data.txt");

        if ((fp = fopen(fn, "r")) == NULL) {
            printf("Can't open the input file: %s\n\n", fn);
            exit(1);
        }

        for(i = 0; i < MAXSIZE; i++)
            fscanf(fp, "%d", &data[i]);
    }

    MPI_Bcast(data, MAXSIZE, MPI_INT, 0, MPI_COMM_WORLD);

    x = MAXSIZE / numprocs;
    low = myid * x;
    high = low + x;

    for(i = low; i < high; i++)
        myresult += data[i];

    printf("I got %d from %d\n", myresult, myid);

    MPI_Reduce(&myresult, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (myid == 0)
        printf("The sum is %d. Herlina Iin Nur Soleha (24/541333/PA/22962)\n", result);

    MPI_Finalize();
}
