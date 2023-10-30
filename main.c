#include <stdio.h>
#include <stdlib.h>

#include "combo.h"
#include "pedido.h"
#include "lista.h"

int var_glob_vend_id;

void cargar_pedido(pedido *ped_ing, combo combos_ing[]);
void precarga_combos(combo combos_del_dia[]);
void mostrar_pedido(pedido ped_ing);

int main()
{
    lista_pedidos pedidos; combo combos_del_dia[num_combos];
    mod_vend_id(1);
    init_lista(&pedidos); precarga_combos(combos_del_dia);

    int opcion=-1;


    while(opcion != 0)
    {
        system("cls");
        printf("\n # # # #  M E N U  # # # #\n");
        printf("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
        //funciones de cargar y modificar
        printf("\n  (1) Cargar pedido");
        printf("\n  (n) default");
        printf("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
        //funciones de mostrar
        printf("\n  (2) Mostrar pedido por ID Pedido");
        printf("\n  (n) default");
        printf("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
        //funciones de archivos
        printf("\n  (n) default");
        printf("\n  (0) Salir");
        printf("\n\n + Resp: ");

        scanf("%d", &opcion);

        //control opcion

        switch (opcion)
        {
            case 1: {
                int seguir=-1;

                while (seguir != 0)
                {
                    insert_lista(&pedidos);
                    system("cls");
                    cargar_pedido(copy_lista(pedidos), combos_del_dia);
                    printf("\n - Desea seguir ingresando pedidos (1=si, 0=no): ");
                    scanf("%d", &seguir);
                }
                break;
            }
            case 2: {
                char id_pedido_ing[idsize];
                printf("\n Ingrese id pedido: ");
                scanf("%s", id_pedido_ing);

                if (buscar_x_idped(&pedidos, id_pedido_ing) == 1) mostrar_pedido(*copy_lista(pedidos));
                else printf("\n No se encontro en la base...");

                printf("\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }
        }
    }



    return 0;
}

int buscar_x_idped(lista_pedidos *lista_ing, char id[]) // f-b
{
    reset_lista(lista_ing);
    while(isOos(*lista_ing)!=1)
    {
        if(strcmp(get_pedido_id(*copy_lista(*lista_ing)), id) == 0){
            return 1;
        }
        else forward_lista(lista_ing);
    }
    return 0;
}

int contar_pedidos_vend(lista_pedidos lista_ing, int vend_id_ing, int n) //f-g
{
    // hacer un reset_lista en el main
    if(isOos(lista_ing) == 0)
    {
        if (get_vend_id(*copy_lista(lista_ing)) == vend_id_ing) n+=1;
        forward_lista(&lista_ing);
        return contar_pedidos_vend(lista_ing, vend_id_ing, n);
    }
    else return n;
}

void cargar_pedido(pedido *ped_ing, combo combos_ing[]) // f-a
{
    pedido pre_carga;
    init_pedido(&pre_carga);
    int pedido_confirm=-1;

    while (pedido_confirm != 1)
    {
        int num_ing1, num_ing2, salir_iter=-1; float monto;
        char str_ing1[strsize], str_ing2[strsize];

        printf("\n - Ing. solamente el nombre: "); scanf("%s", str_ing1);
        printf("\n - Ing. el apellido: "); scanf("%s", str_ing2);
        set_nombre(&pre_carga, str_ing1, str_ing2);
        set_pedido_id(&pre_carga, gen_pedido_id());
        set_vend_id(&pre_carga, var_glob_vend_id);
        printf("\n - Ingreso combos: ");

        // deberiamos mostrar una tabla con el id del combo y respectivo stock para todos los combos

        while (salir_iter != 0)
        {
            printf("\n\t + Ing. combo id (0-%d): ", num_combos-1); scanf("%d", &num_ing1);

            //controlar stock

            printf("\n\t + Ing. la cantidad de unidades del combo %d: ", num_ing1); scanf("%d", &num_ing2);
            set_comb_pedidos(&pre_carga, num_ing1, num_ing2);
            printf("\n\t - Desea seguir ingresando combos? (1=si / 0=no): "); scanf("%d", &salir_iter);
        }

        printf("\n - Consume en el local? (1=si / 0=no): "); scanf("%d", &num_ing1);
        set_consum_local(&pre_carga, num_ing1);


        printf("\n - Ingrese forma de pago:");
        printf("\n\t (1) Debito, (2) Credito, (3) QR, (4) Efectivo\n - Resp: "); scanf("%d", &num_ing1);
        set_forma_pago(&pre_carga, num_ing1);

        printf("\n - Ingrese fecha de compra:");
        printf("\n\t + Dia: "); scanf("%d", &num_ing1);

        //controlar dia >= dia actual //se necesita usar time.h

        set_fec_compra_dia(&pre_carga, num_ing1);
        printf("\n\t + Mes: "); scanf("%d", &num_ing1);

        //controlar mes >= mes actual

        set_fec_compra_mes(&pre_carga, num_ing1);
        printf("\n\t + Anio: "); scanf("%d", &num_ing1);

        //controlar anio >= anio actual

        set_fec_compra_anio(&pre_carga, num_ing1);

        printf("\n - Se entrego el pedido? (1=si / 0=no): "); scanf("%d", &num_ing1);
        set_entregado(&pre_carga, num_ing1);

        printf("\n - Posee cupon de descuento? (1=si / 0=no): "); scanf("%d", &num_ing1);
        set_cup_descuento(&pre_carga, num_ing1);

        monto = calcular_subtotal_combos(combos_ing, get_comb_pedidos(pre_carga), get_cup_descuento(pre_carga));

        set_subtotal(&pre_carga, monto);

        if (get_consum_local(pre_carga) == 0) monto += costo_delivery;
        set_total(&pre_carga, monto);

        printf("\n -------------------------");
        printf("\n Pedido ID: %s", get_pedido_id(pre_carga));
        printf("\n - Subtotal: %.2f", get_subtotal(pre_carga));
        if (get_consum_local(pre_carga) == 0) printf("\n + Costo Delivery: %d", costo_delivery);
        printf("\n -------------------------");
        printf("\n - Total: %.2f", get_total(pre_carga));

        printf("\n Desea confirmar el pedido?... (1=si / 0=no): ");
        scanf("%d", &pedido_confirm);
        if (pedido_confirm == 1) copy_pedido(ped_ing, pre_carga);
    }
    printf("\n - Se ha cargado el pedido...\n");
}

void precarga_combos(combo combos_del_dia[])
{
    FILE *archivo_combos = fopen("menu.txt", "r");
    int id_combo_scan;
    char separador[4];

    while(feof(archivo_combos) == 0)
    {
        fscanf(archivo_combos, "%d ", &id_combo_scan);
        combos_del_dia[id_combo_scan].id_combo = id_combo_scan;
        fscanf(archivo_combos, "%[^\n]s", combos_del_dia[id_combo_scan].descripcion);
        fscanf(archivo_combos, "%d ", &combos_del_dia[id_combo_scan].stock);
        fscanf(archivo_combos, "%f ", &combos_del_dia[id_combo_scan].preciounit);
        fscanf(archivo_combos, "%d ", &combos_del_dia[id_combo_scan].descuento);
        fscanf(archivo_combos, "%s ", separador);
    }

    fclose(archivo_combos);
}

void mostrar_pedido(pedido ped_ing)
{
    printf("\n + Pedido ID: %s", get_pedido_id(ped_ing));
    printf("\n    + Fecha de compra: %d/%d/%d", get_fec_compra_dia(ped_ing), get_fec_compra_mes(ped_ing), get_fec_compra_anio(ped_ing));
    printf("\n    + Nombre: %s %s", get_nomb(ped_ing), get_ape(ped_ing));
    printf("\n    + Vendedor ID: %d", get_vend_id(ped_ing));

    //mostrar comb pedidos

    printf("\n    + Consume en el local: ");
    if(get_consum_local(ped_ing) == 1) printf("si");
    else printf("no");

    printf("\n\t + Cupon de descuento: ");
    if(get_cup_descuento(ped_ing) == 1) printf("si");
    else printf("no");

    printf("\n\t + Metodo de pago: ");
    switch(get_forma_pago(ped_ing))
    {
        case 1:{
            printf("Debito."); break;
        }
        case 2:{
            printf("Credito."); break;
        }
        case 3:{
            printf("QR."); break;
        }
        case 4:{
            printf("Efectivo."); break;
        }
    }
    printf("\n -------------------------");
    printf("\n + Subtotal: %.2f", get_subtotal(ped_ing));
    if (get_consum_local(ped_ing) == 0) printf("\n + Costo Delivery: %d", costo_delivery);
    printf("\n -------------------------");
    printf("\n - Total: %.2f", get_total(ped_ing));
}

int mod_estado_pedido(lista_pedidos *lista_ing, char id_pedido[], int estado_ing) //f-i
{
    int encontrado=-1;
    reset_lista(lista_ing);

    encontrado = buscar_x_idped(lista_ing, id_pedido);
    if (encontrado==1)
    {
        set_entregado(copy_lista(*lista_ing), estado_ing);
        return 0;
    }
    else return 1;
}

int mod_form_pago_pedido(lista_pedidos *lista_ing, char id_pedido[], int formpago_ing) //f-j
{
    int encontrado=-1;
    reset_lista(lista_ing);

    encontrado = buscar_x_idped(lista_ing, id_pedido);
    if (encontrado==1)
    {
        set_forma_pago(copy_lista(*lista_ing), formpago_ing);
        return 0;
    }
    else return 1;
}

int mod_nombre_pedido(lista_pedidos *lista_ing, char id_pedido[], char nomb[], char ape[]) //f-k
{
    int encontrado=-1;
    reset_lista(lista_ing);

    encontrado = buscar_x_idped(lista_ing, id_pedido);
    if (encontrado==1)
    {
        set_nombre(copy_lista(*lista_ing), nomb, ape);
        return 0;
    }
    else return 1;
}

void mod_precioystock_combo(combo combos_ing[], int id_combo, float precio, int stock) //f-o
{
    cargaprecio(&combos_ing[id_combo], precio);
    cargastock(&combos_ing[id_combo], stock);
}

int mod_vend_id(int accion) // sirve para f-t // 1=cargar, 2=sobrescrib.
{
    switch(accion)
    {
        case 1:{
            FILE *fp = fopen("vend_id.txt", "r"); // capaz sea mejor abrirlo en w+
            if (fp == NULL) return 1;
            else fscanf(fp, "%d", &var_glob_vend_id);
            fclose(fp);
            break;
        }
        case 2:{
            FILE *fp = fopen("vend_id.txt", "w");
            if (fp == NULL) return 1;
            else fprintf(fp, "%d", var_glob_vend_id);
            fclose(fp);
        }
    }
    return 0;
}
