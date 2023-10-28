#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(void)
{
    int numero, i; char pepe[7];

    int M=48, N=90;
    numero = rand();


    srand(time(0));
    for (i=0; i<=id_siz; i++)
    {
        pepe[i]=rand()% (N-M+1) + M;
    }
    printf("%s ", pepe);

    return 0;
}
