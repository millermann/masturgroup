#include <stdio.h>
#include <stdlib.h>

#include "pedido.h"
#include "lista.h"


int main()
{
    pedido x1;
    char str1[25], str2[25];
    lista_pedidos pedidos;
    init_lista(&pedidos);

    scanf("%s %s", &str1, &str2);
    set_nombre(&x1, str1, str2);
    printf(" pedido x1: %s %s", get_nomb(x1), get_ape(x1));

    insert_lista(&pedidos, x1);

    printf("\n dentro de la lista: %s %s", get_nomb(*copy_lista(pedidos)), get_ape(*copy_lista(pedidos)));


    return 0;
}
