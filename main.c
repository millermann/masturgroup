#include <stdio.h>
#include <stdlib.h>

#include "combo.h"
#include "pedido.h"
#include "lista.h"

int var_glob_vend_id;

void cargar_pedido(pedido *ped_ing, combo combos_ing[])
{
    pedido pre_carga;
    int pedido_confirm=-1;

    while (pedido_confirm != 1)
    {
        int num_ing1, num_ing2, salir_iter=-1; float monto;
        char str_ing1[strsize], str_ing2[strsize];

        system("cls");
        printf("\n - Ing. solamente el nombre: "); scanf("%s", str_ing1);
        printf("\n - Ing. el apellido: "); scanf("%s", str_ing2);
        set_nombre(&pre_carga, str_ing1, str_ing2);

        // falta generar un string aleatorio para pedido_id

        set_vend_id(&pre_carga, var_glob_vend_id);

        printf("\n - Ingreso combos: ");
        // deberiamos hacer una tabla con el id del combo y respectivo stock
        while (salir_iter != 0)
        {
            printf("\n\t + Ing. combo id (0-%d): ", num_combos); scanf("%d", &num_ing1);
            //controlar stock
            printf("\n\t + Ing. la cantidad de unidades del combo %d: ", num_ing1); scanf("%d", &num_ing2);
            set_comb_pedidos(&pre_carga, num_ing1, num_ing2);
            printf("\n\t - Desea seguir ingresando combos? (1=si / 0=no): "); scanf("%d", &salir_iter);
        }

        printf("\n - Consume en el local? (1=si / 0=no): "); scanf("%d", &num_ing1);
        set_consum_local(&pre_carga, num_ing1);

        printf("\n - Posee cupon de descuento? (1=si / 0=no): "); scanf("%d", &num_ing1);

        printf("\n - Ingrese forma de pago:");
        printf("\n\t (1) Debito, (2) Credito, (3) QR, (4) Efectivo\n - Resp: "); scanf("%d", &num_ing1);
        set_forma_pago(&pre_carga, num_ing1);

        printf("\n - Ingrese fecha de compra:"); // reemplazable com time.h
        printf("\n\t + Dia: "); scanf("%d", &num_ing1);
        set_fec_compra_dia(&pre_carga, num_ing1);
        printf("\n\t + Mes: "); scanf("%d", &num_ing1);
        set_fec_compra_mes(&pre_carga, num_ing1);
        printf("\n\t + Anio: "); scanf("%d", &num_ing1);
        set_fec_compra_anio(&pre_carga, num_ing1);

        printf("\n - Se entrego el pedido? (1=si / 0=no): "); scanf("%d", &num_ing1);
        set_entregado(&pre_carga, num_ing1);

        monto = calcular_subtotal_combos(combos_ing, get_comb_pedidos(pre_carga), num_ing1);
        set_subtotal(&pre_carga, monto);

        if (get_consum_local == 0) monto -= costo_delivery;
        set_total(&pre_carga, monto);

        printf("\n -------------------------");
        printf("\n + Subtotal: %.2f", get_subtotal(pre_carga));
        if (get_consum_local(pre_carga) == 0) printf("\n + Costo Delivery: %d", costo_delivery);
        printf("\n -------------------------");
        printf("\n - Total: %.2f", get_total(pre_carga));

        printf("\n Desea confirmar el pedido?... (1=si / 0=no): ");
        scanf("%d", &pedido_confirm);
        if (pedido_confirm == 1) copy_pedido(ped_ing, pre_carga);
    }
    printf("\n - Se ha cargado el pedido...");
}

int main()
{
    lista_pedidos pedidos; combo combos_del_dia[num_combos];

    init_lista(&pedidos);
    insert_lista(&pedidos);

    cargar_pedido(copy_lista(pedidos), combos_del_dia);
    printf("\n sapeeee");
    return 0;
}
