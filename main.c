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
        printf("\n  (3) Mostrar todo los pedidos cargados.");
        printf("\n  (n) default");
        printf("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
        //funciones de archivos
        printf("\n  (4) Cargar pedidos por archivo");
        printf("\n  (5) Anular y exportar pedido por ID Pedido");
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
                    // si no si la precarga no se hace la lista quedaria con datos nodos vacios //rehacer
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

                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 3: { // error // cuando se carga 1 pedido, luego se borra, y se importan los pedidos de un archivo // entra en bucle
                reset_lista(&pedidos);
                system("cls");
                if (isEmpty(pedidos)!=1){
                    while(isOos(pedidos) != 1){
                        mostrar_pedido(*copy_lista(pedidos));
                        forward_lista(&pedidos);
                    }
                }
                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 4: {
                import_pedidos(&pedidos);
                break;
            }

            case 5: {
                char id_pedido_ing[idsize];
                printf("\n Ingrese id pedido: ");
                scanf("%s", id_pedido_ing);

                if (buscar_x_idped(&pedidos, id_pedido_ing) == 1){

                    export_pedido(*copy_lista(pedidos));
                    supress_lista(&pedidos);
                    printf("\n Pedido %s anulado y exportado...", id_pedido_ing);
                }
                else printf("\n No se encontro en la base...");

                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
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

void export_pedido(pedido ped_ing) // para f-l
{
    int i;
    FILE *fp = fopen("pedidos_export.txt", "a");


    fprintf(fp, "%s \n", get_pedido_id(ped_ing));
    fprintf(fp, "%d \n", get_vend_id(ped_ing));
    fprintf(fp, "%d %d %d \n", get_fec_compra_dia(ped_ing), get_fec_compra_mes(ped_ing), get_fec_compra_anio(ped_ing));
    fprintf(fp, "%s %s\n", get_nomb(ped_ing), get_ape(ped_ing));
    for (i=0; i<num_combos; i++){
        fprintf(fp, "%d ", get_comb_pedidos(ped_ing)[i]);
    }
    fprintf(fp, "\n");
    fprintf(fp, "%d \n", get_consum_local(ped_ing));
    fprintf(fp, "%d \n", get_cup_descuento(ped_ing));
    fprintf(fp, "%.2f \n", get_subtotal(ped_ing));
    fprintf(fp, "%.2f \n", get_total(ped_ing));
    fprintf(fp, "%d \n", get_forma_pago(ped_ing));
    fprintf(fp, "%d ", get_entregado(ped_ing));
    fprintf(fp, "\n---\n");
    fclose(fp);
}

void import_pedidos(lista_pedidos *lista_ing) // f-ll
{
    FILE *pedidos_importados;
    char nomb_del_archivo[30];

    system("cls");
    printf("\n # # #   I M P O R T A R   P E D I D O S   # # #\n");

    printf("\n - Introduzca el nombre completo del archivo (ej: ARCHIVO_DE_EJEMPLO.txt)");
    printf("\n    * Puede introducir \"ayuda\" para ver mas informacion... ");

    printf("\n\n - Resp: ");
    scanf("%s", nomb_del_archivo);

    if (strcmp(nomb_del_archivo, "ayuda") == 0){
        system("cls");
        printf("\n\a # # #   A Y U D A   # # #\n");
        printf("\n no le den bola todavia, todavia no se en que orden leer y guardarlos.");
        printf("\n * Coloque el archivo en dentro de la carpeta del programa...\n");
        printf("\n * El nombre del archivo no debe contener ningun espacio...\n");
        printf("\n * El archivo de tener el siguiente formato:");
        printf("\n    ---");
        printf("\n    PEDIDO_ID");
        printf("\n    VEND_ID");
        printf("\n    DIA MES ANIO");
        printf("\n    NOMBRE APELLIDO");
        printf("\n    UNIDADES_DEL_COMBO ");
        printf("\n    CONSUM_LOCAL");
        printf("\n    CUPON_DESCUENTO");
        printf("\n    SUBTOTAL");
        printf("\n    TOTAL");
        printf("\n    FORMA_DE_PAGO");
        printf("\n    ENTREGADO");
        printf("\n\n - Introduzca el nombre completo del archivo (ej: ARCHIVO_DE_EJEMPLO.txt)");
        printf("\n\n - Resp: ");
        scanf("%s", nomb_del_archivo);
    }

    if (fopen(nomb_del_archivo, "r")==NULL){
        printf("\n\a # No se encontro el archivo...");
    }
    else{
        pedido nuevo_pedido;
        pedidos_importados = fopen(nomb_del_archivo, "r");
        reset_lista(lista_ing); // necesario?

        char pedido_id[idsize], nomb[strsize], ape[strsize], separador[4];
        int vend_id, comb_pedidos[num_combos], consum_local, cup_descuento, forma_pago, entregado, i;
        float subtotal, total;
        fecha fec_compra;

        printf("\n # # #   I M P O R T A R   S E L E C C I O N   # # #\n");
        printf("\n - Desde: %s", nomb_del_archivo);

        while (feof(pedidos_importados)==0){
            init_pedido(&nuevo_pedido);
            fscanf(pedidos_importados, "%s ", pedido_id);
            fscanf(pedidos_importados, "%d ", &vend_id);
            fscanf(pedidos_importados, "%d %d %d ", &fec_compra.dia, &fec_compra.mes, &fec_compra.anio);
            fscanf(pedidos_importados, "%s %s ", nomb, ape);

            for (i=0; i<num_combos; i++){
                fscanf(pedidos_importados, "%d ", &comb_pedidos[i]);
            }

            fscanf(pedidos_importados, "%d ", &consum_local);
            fscanf(pedidos_importados, "%d ", &cup_descuento);
            fscanf(pedidos_importados, "%f ", &subtotal);
            fscanf(pedidos_importados, "%f ", &total);
            fscanf(pedidos_importados, "%d ", &forma_pago);

            fscanf(pedidos_importados, "%d ", &entregado);
            fscanf(pedidos_importados, "%s ", separador);

            set_pedido_id(&nuevo_pedido, pedido_id);
            set_vend_id(&nuevo_pedido, vend_id);
            set_fec_compra_dia(&nuevo_pedido, fec_compra.dia);
            set_fec_compra_mes(&nuevo_pedido, fec_compra.mes);
            set_fec_compra_anio(&nuevo_pedido, fec_compra.anio);
            set_nombre(&nuevo_pedido, nomb, ape);
            for (i=0; i<num_combos; i++){
                set_comb_pedidos(&nuevo_pedido, i, comb_pedidos[i]);
            }
            set_consum_local(&nuevo_pedido, consum_local);
            set_cup_descuento(&nuevo_pedido, cup_descuento);
            set_subtotal(&nuevo_pedido, subtotal);
            set_total(&nuevo_pedido, total);
            set_forma_pago(&nuevo_pedido, forma_pago);
            set_entregado(&nuevo_pedido, entregado);

            insert_listaypedido(lista_ing, nuevo_pedido);

            printf("\n\n\a # Datos Importados:\n");
            mostrar_pedido(*copy_lista(*lista_ing));
            printf("\n");
        }
        fclose(pedidos_importados);
    }
    printf("\n\n - Pulse una tecla para volver al menu...");
    fflush(stdin);
    getchar();
}

void precarga_combos(combo combos_del_dia[]) // f-�
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
     printf("\n -----------------------------------");
    printf("\n + Pedido ID: %s", get_pedido_id(ped_ing));
    printf("\n    + Vendedor ID: %d", get_vend_id(ped_ing));
    printf("\n    + Fecha de compra: %d/%d/%d", get_fec_compra_dia(ped_ing), get_fec_compra_mes(ped_ing), get_fec_compra_anio(ped_ing));
    printf("\n    + Nombre: %s %s", get_nomb(ped_ing), get_ape(ped_ing));

    //mostrar comb pedidos

    printf("\n    + Consume en el local: ");
    if(get_consum_local(ped_ing) == 1) printf("si");
    else printf("no");

    printf("\n    + Cupon de descuento: ");
    if(get_cup_descuento(ped_ing) == 1) printf("si");
    else printf("no");

    printf("\n -----------------------------------");
    printf("\n + Subtotal: %.2f", get_subtotal(ped_ing));
    if (get_consum_local(ped_ing) == 0) printf("\n + Costo Delivery: %d", costo_delivery);
    printf("\n -----------------------------------");
    printf("\n - Total: %.2f", get_total(ped_ing));
    printf("\n -----------------------------------");
    printf("\n + Metodo de pago: ");
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
    printf("\n + Estado: ");
    if (get_entregado(ped_ing) == 1) printf("Entregado.");
    else printf("No Entregado.");
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
