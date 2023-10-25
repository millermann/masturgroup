#include <stdio.h>
#include <stdlib.h>
#include "combo.h"
#include "pedido.h"
#include "lista.h"

int var_glob_vend_id;

void cargar_pedido(pedido *ped_ing)
{
    char str_ing1[strsize], str_ing2[strsize];

    printf("\n - Ing. solamente el nombre: "); scanf("%s", str_ing1);
    printf("\n - Ing. el apellido: "); scanf("%s", str_ing2);
    set_nombre(ped_ing, str_ing1, str_ing2);
    //generar un string aleatorio para pedido_id
    set_vend_id(ped_ing, var_glob_vend_id);
    

}

int main()
{
    
    printf("\n sapeeee");
    return 0;
}
