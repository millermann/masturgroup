#include <stdio.h>
#include <stdlib.h>

#include "combo.h"
#include "pedido.h"
#include "lista.h"

int var_glob_vend_id;
char nomb_archiv_menu[strsize];

combo combos_del_dia[num_combos];

time_t ahora;
struct tm *fecha_actual ;

int buscar_x_idped(lista_pedidos *lista_ing, char id[]);
void cargar_pedido(lista_pedidos *lista_ing);
int contar_pedidos_vend(lista_pedidos lista_ing, int vend_id_ing, int n);
void empleado_del_mes(lista_pedidos l, int mes);
void export_pedido(pedido ped_ing, char nombre_del_archivo[]);
int export_pedidosxformpago(lista_pedidos lista_ing, int form_pago);
void import_pedidos(lista_pedidos *lista_ing);
void modificar_estado_por_id(lista_pedidos *l, char n[]);
void mod_form_pago_pedido(lista_pedidos *lista_ing, int formpago_ing);
void modificar_nombre_por_id(lista_pedidos *l, char n[]);
void modifica_precio_y_stock_de_combo(combo arr[]);
int mod_vend_id(void);
void mostrar_comb_pedidos(int comb_pedidos[], int cup_descuento);
void mostrar_combos(combo combos_ing[]);
void mostrar_precio_y_stock(combo arr[]);
void mostrar_pedido(pedido ped_ing);
void mostrar_pedido_por_vendedor(lista_pedidos lista_ing, int vend_id_ing,int *x);
void muestra_combos_sin_stock(combo combos_ing[]);
void muestra_ped_no_entregados(lista_pedidos l);
int muestra_pedxnomb(lista_pedidos l, char c[]);
void muestra_por_mes(lista_pedidos l, int mes);
int precarga_combos(combo combos_del_dia[], char nombre_del_archivo[]);
int user_data(int opcion);

// funciones corregidas
char *gen_pedido_id(void)
{
    int i, min_val, max_val;
    char id_gen[idsize];
    char *aux = (char*)malloc(sizeof(char)*idsize);

    srand(time(0));

    min_val=65, max_val=90; // 65=A y 90=Z
    for (i=0; i<=3; i++)
    {
        id_gen[i] = (rand() % (max_val-min_val+1) + min_val);
    }

    min_val=48, max_val=57; // 48=0 y 57=9
    for (i=4; i<=idsize; i++)
    {
        id_gen[i] = (rand() % (max_val-min_val+1) + min_val);
    }

    id_gen[idsize]='\0';

    strcpy(aux, id_gen);
    return aux;
}

float calcular_subtotal_combos(combo combos_ing[], int comb_pedidos[], int cupon_descuento)
{
    int i; float monto_indiv, suma_total=0;

    for (i=0; i<num_combos; i++)
    {
        monto_indiv = 0;

        monto_indiv = (muestraprecio(combos_ing[i]) * (comb_pedidos[i]));
        if (cupon_descuento == 1){
            if (muestradescuento(combos_ing[i]) == 1){
                monto_indiv -= ((val_descuento*monto_indiv)/100);
            }
        }
        suma_total += monto_indiv;
    }
    return suma_total;
}

int actualizar_combos_stock(combo menu_combos[], int comb_pedidos[], int opcion) // 1= quitar stock, 2=devolver stock
{
    int i, stock_actual=0;

    if (opcion == 1){
        for (i=0; i<num_combos; i++){ // controla q en todas las unidades pedidos < stock del combo
            if ((muestrastock(menu_combos[i]) - comb_pedidos[i]) < 0) return 1;
        }
        for (i=0; i<num_combos; i++){
            stock_actual = (muestrastock(menu_combos[i]) - comb_pedidos[i]);
            cargastock(&menu_combos[i], stock_actual);
        }
    }

    else{
        for (i=0; i<num_combos; i++){
            stock_actual = (muestrastock(menu_combos[i]) + comb_pedidos[i]);
            cargastock(&menu_combos[i], stock_actual);
        }
    }
    return 0;
}

int main()
{
    int opcion=-1, check_resp=0, mes;
    char nomb[strsize], id_pedido[idsize];
    lista_pedidos pedidos;
    init_lista(&pedidos);
    init_combos_default(combos_del_dia);

    switch (user_data(1)){
        case 1:{
            printf("\n # # # #   E R R O R   # # # #\n");
            printf("\n\a # No se pudo restaurar los datos del programa...");
            printf("\n - Por favor ingrese el ID de vendedor: ");
            fflush(stdin);
            check_resp = scanf("%d", &var_glob_vend_id);
            while (((var_glob_vend_id > 3) || (var_glob_vend_id < 1)) || (check_resp != 1)){
                printf("\n\a # ID no valido (1 <= ID <= 3)...");
                printf("\n Por favor ingrese el ID de vendedor: ");
                fflush(stdin);
                check_resp = scanf("%d", &var_glob_vend_id);
            }
        }
        case 2:{
            system("cls");
            printf("\n # # # #   E R R O R   # # # #\n");
            printf("\n - Por favor ingrese el nombre del archivo que contiene los datos del menu de combos: ");
            scanf("%s", nomb_archiv_menu);
            while (precarga_combos(combos_del_dia, nomb_archiv_menu) == 1)
            {
                system("cls");
                printf("\n # # # #   E R R O R   # # # #\n");
                printf("\n\a # ATENCION: No se ha podido encontrar el archivo que contiene los datos del menu de combos");
                printf("\n\t - Ingrese el nombre de un nuevo archivo que contenga los datos correspondientes");
                printf("\n + Resp.: ");
                scanf("%s", nomb_archiv_menu);
            }
            user_data(2);
            system("cls");
            printf("\n\a # Datos del usuario cargados correctamente.");
            printf("\n\n - Pulse para continuar..."); fflush(stdin); getchar();
        }
    }

    ahora = time(NULL);
    fecha_actual = localtime(&ahora);

    while (opcion != 0)
    {
        system("cls");
        printf("\n # # # # # # # # # # # # # # #  M E N U  # # # # # # # # # # # # # # # #");
        printf("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        printf("\n  +  P E D I D O S");
        printf("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        printf("\n  [1]   Cargar pedido.");
        printf("\n  [2]   Cargar pedidos por archivo.");
        printf("\n  [3]   Modificar nombre de un pedido segun su ID.");
        printf("\n  [4]   Modificar el estado del pedido a entregado por ID.");
        printf("\n  [5]   Modificar la forma de pago de un pedido segun su ID.");
        printf("\n  [6]   Mostrar todos pedidos cargados.");
        printf("\n  [7]   Mostrar pedido por ID.");
        printf("\n  [8]   Mostrar pedidos segun vendedor.");
        printf("\n  [9]   Mostrar pedidos por nombre.");
        printf("\n  [10]  Mostrar pedidos por mes.");
        printf("\n  [11]  Mostrar pedidos no entregados.");
        printf("\n  [12]  Anular y exportar pedido por ID.");
        printf("\n  [13]  Exportar pedido por forma de pago.");
        printf("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        printf("\n  +  C O M B O S");
        printf("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        printf("\n  [14]  Mostrar todos los combos.");
        printf("\n  [15]  Mostrar precio y stock de un combo por ID Combo.");
        printf("\n  [16]  Mostrar los combos sin stock.");
        printf("\n  [17]  Modificar precio y stock del combo segun ID Combo.");
        printf("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        printf("\n  +  V E N D E D O R");
        printf("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
        printf("\n  [18]  Cambiar al vendedor.");
        printf("\n  [19]  Informar cual es el vendedor que realizo mas pedidos en el mes.");
        printf("\n  [20]  Contar pedidos por vendedor.");
        printf("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");

        printf("\n  [0]  Salir.");
        printf("\n\n + Resp.: ");

        fflush(stdin);
        check_resp = scanf("%d", &opcion);
        while(opcion>20 || opcion<0 || check_resp!=1){
            printf("\n\a # Respuesta invalida...");
            printf("\n\n + Resp: ");
            fflush(stdin);
            check_resp = scanf("%d", &opcion);
        }

        switch (opcion)
        {
            case 1:{ // f-a
                cargar_pedido(&pedidos);
                break;
            }

            case 2:{
                import_pedidos(&pedidos);
                break;
            }

            case 3:{ //Modificar nombre de un pedido segun su idpedido
                system("cls");
                printf("\n # # # #   M O D I F I C A R   N O M B R E   P O R   I D   # # # #\n");
                if(!isEmpty(pedidos)){
                    printf("\n - Ingrese el ID del pedido: ");
                    scanf("%s",id_pedido);
                    modificar_nombre_por_id(&pedidos,id_pedido);
                }
                else printf("\n # No hay pedidos cargados en la base...\n");
                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 4:{ //Modificar el estado del pedido a entregado por idpedido.");
                system("cls");
                printf("\n # # # #   M O D I F I C A R   E N T R E G A   P O R   I D   # # # #\n");
                if (!isEmpty(pedidos))
                {
                    printf("\n - Ingrese el ID del pedido: ");
                    scanf("%s", id_pedido);
                    modificar_estado_por_id(&pedidos, id_pedido);
                }
                else printf("\n # No hay pedidos cargados en la base...");
                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 5:{ // Modificar la forma de pago de un pedido segun su idpedido.
                system("cls");
                printf("\n # # # #   M O D I F I C A R   F O R M A   D E   P A G O   P O R   I D   # # # #\n");
                if (!isEmpty(pedidos)){
                    printf("\n - Ingrese el ID del pedido: ");
                    scanf("%s", id_pedido);

                    if (buscar_x_idped(&pedidos, id_pedido) == 1){
                        int form_pago;
                        printf("\n - Ingrese la forma de pago. ");
                        printf("\n (1) Debito, (2) Credito, (3) QR, (4) Efectivo");
                        printf("\n + Resp: ");
                        check_resp = scanf("%d", &form_pago);
                        while (form_pago < 1 || form_pago > 4 || check_resp != 1){
                            printf("\n - Ingrese la forma de pago. ");
                            printf("\n (1) Debito, (2) Credito, (3) QR, (4) Efectivo");
                            printf("\n + Resp: ");
                            fflush(stdin);
                            check_resp = scanf("%d", &form_pago);
                        }
                        mod_form_pago_pedido(&pedidos, form_pago);
                        printf("\n # Forma de pago modificada correctamente.");
                    }
                    else printf("\n\a # El pedido no esta cargado en la base.");
                }
                else printf("\n # No hay pedidos cargados en la base...");

                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 6:{ // f-adic
                system("cls");
                printf("\n # # # #   M O S T R A R   T O D O S   L O S   P E D I D O S   # # # #\n");

                reset_lista(&pedidos);
                if (isEmpty(pedidos) == 1){
                    printf("\n\a # No hay pedidos cargados en la base...");
                }
                else{
                    while(isOos(pedidos) != 1){
                        mostrar_pedido(copy_lista(pedidos));
                        printf("\n");
                        forward_lista(&pedidos);
                    }
                }
                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 7:{ // f-c
                char id_pedido_ing[strsize];
                system("cls");
                printf("\n # # # #   M O S T R A R   P E D I D O   P O R   I D   # # # #\n");
                if (isEmpty(pedidos) == 1){
                    printf("\n # No hay pedidos cargados en la base...");
                    printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                    break;
                }
                printf("\n Ingrese el ID del pedido: ");
                scanf("%s", id_pedido_ing);

                if (buscar_x_idped(&pedidos, id_pedido_ing) == 1) mostrar_pedido(copy_lista(pedidos));
                else printf("\n # El pedido no se encontro en la base...");

                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 8:{ //Mostrar todos los pedidos de un vendedor
                system("cls");
                printf("\n # # # #   M O S T R A R   P E D I D O S   P O R   N O M B R E   # # # #\n");
                if (!isEmpty(pedidos))
                {
                    int id_vendedor = -1;
                    int boo=0 ;
                    reset_lista(&pedidos);

                    printf(" - Ingrese ID del vendedor: "); // quizas mostrar los id
                    check_resp = scanf("%d", &id_vendedor);
                    while (id_vendedor < 1 || id_vendedor > 3 || check_resp != 1){
                        printf("\n\a # ID invalido\n");
                        printf(" - Ingrese ID del vendedor: "); // quizas mostrar los id
                        fflush(stdin);
                        check_resp = scanf("%d", &id_vendedor);
                    }
                    system("cls");
                    mostrar_pedido_por_vendedor(pedidos, id_vendedor,&boo);
                    if (boo == 0)
                        printf("\n # No hay pedidos con ese vendedor\n");
                }
                else printf("\n # No hay pedidos cargados en la base...");

                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 9:{ // f-e, Mostrar todos los pedidos por nombre.
                system("cls");
                printf("\n # # # #   M O S T R A R   P E D I D O S   P O R   N O M B R E   # # # #\n");

                if (isEmpty(pedidos) == 1){
                    printf("\n\a # No hay pedidos cargados en la base...");
                }
                else{
                    printf("\n - Ingrese nombre: ");
                    scanf("%s",nomb);
                    if (muestra_pedxnomb(pedidos, nomb) == 1){
                        printf("\n\a # No se encontro el nombre dentro de los pedidos cargados en la base...");
                    }
                    else{
                        system("cls");
                        printf("\n # # # #   M O S T R A R   P E D I D O S   P O R   N O M B R E   # # # #\n");
                        printf("\n\a # No hay mas pedidos cargados en la base...");
                    }

                }
                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 10:{ // f-d, Mostrar todos los pedidos por mes, no anda
                system("cls");
                printf("\n # # # #   M O S T R A R   P E D I D O S   P O R   M E S   # # # #\n");
                if (!isEmpty(pedidos)){
                    printf("\n - Ingrese el mes: ");
                    fflush(stdin);
                    check_resp = scanf("%d", &mes);
                    while (mes>12 || mes<0 || check_resp!=1){
                        printf("\n\a # Respuesta invalida...");
                        printf("\n - Ingrese el mes: ");
                        fflush(stdin);
                        check_resp = scanf("%d", &mes);
                    }
                    muestra_por_mes(pedidos, mes);
                }
                else printf("\n # No hay pedidos cargados en la base...");

                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 11:{ //Mostrar todos los pedidos no entregados
                system("cls");
                printf("\n # # # #   P E D I D O S   N O   E N T R E G A D O S   # # # #\n");
                if(!isEmpty(pedidos)){
                    muestra_ped_no_entregados(pedidos);
                }
                else printf("\n\a # No hay pedidos cargados en la base...");
                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 12: { // f-l, Anular y exportar por id pedido
                system("cls");
                printf("\n # # # #   A N U L A R   Y   E X P O R T A R   P O R   I D   P E D I D O   # # # #\n");
                if (isEmpty(pedidos) == 1){
                    printf("\n # No hay pedidos cargados en la base...");
                }
                else{ // solo anda en el debug (?)
                    char nombre_archivo[strsize];

                    printf("\n - Ingrese el ID del pedido: ");
                    scanf("%s", id_pedido);
                    if (buscar_x_idped(&pedidos, id_pedido) == 1){
                        mostrar_pedido(copy_lista(pedidos));

                        printf("\n\n - Ingrese el nombre del archivo en donde se va exportar el pedido...");
                        printf("\n\t * El nombre no debe contener espacios. ");
                        printf("\n\t * Puede presionar 1 para guardar en la ruta por defecto. ");
                        printf("\n + Resp.: ");
                        scanf("%s", nombre_archivo);
                        if (strcmp(nombre_archivo, "1") == 0){
                            strcpy(nombre_archivo, "pedidos_export.txt");
                        }
                        export_pedido(copy_lista(pedidos), nombre_archivo);
                        actualizar_combos_stock(combos_del_dia, get_comb_pedidos(copy_lista(pedidos)), 2);
                        supress_lista(&pedidos);
                        system("cls");
                        printf("\n # # # #   A N U L A R   Y   E X P O R T A R   P O R   I D   P E D I D O   # # # #\n");
                        printf("\n # Pedido %s anulado y exportado en %s...", id_pedido, nombre_archivo);
                    }
                    else printf("\n No se encontro en la base...");
                }

                /* anda en el debug y release, por las dudas lo dejo
                else{
                    printf("\n - Ingrese el ID del pedido: ");
                    scanf("%s", id_pedido);
                    if (buscar_x_idped(&pedidos, id_pedido) == 1){
                        mostrar_pedido(copy_lista(pedidos));
                        printf("\n - Ingrese sape: ");
                        export_pedido(copy_lista(pedidos), "pedidos_export.txt");
                        supress_lista(&pedidos);
                        printf("\n\n Pedido %s anulado y exportado en pedidos_export.txt...", id_pedido);
                    }
                    else printf("\n No se encontro en la base...");
                }
                */

                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 13:{ // f-ll, Exportar pedido por form_pago
                int form_pago, resultado;

                system("cls");
                printf("\n # # # #    E X P O R T A R   P O R   F O R M A   D E   P A G O   # # # #\n");
                if (isEmpty(pedidos) == 1){
                    printf("\n # No hay pedidos cargados en la base...");
                }
                else{
                    printf("\n - Ingrese la forma de pago. ");
                    printf("\n\t (1) Debito, (2) Credito, (3) QR, (4) Efectivo");
                    printf("\n + Resp.: ");
                    check_resp = scanf("%d", &form_pago);
                    while (form_pago < 1 || form_pago > 4 || check_resp != 1){
                        system("cls");
                        printf("\n # # # #    E X P O R T A R   P O R   F O R M A   D E   P A G O   # # # #\n");
                        printf("\n\a # Respuesta no valida...");
                        printf("\n - Ingrese forma de pago:");
                        printf("\n\t (1) Debito, (2) Credito, (3) QR, (4) Efectivo\n - Resp: ");
                        fflush(stdin);
                        check_resp = scanf("%d", &form_pago);
                    }
                    reset_lista(&pedidos);
                    resultado = export_pedidosxformpago(pedidos, form_pago);
                    switch (resultado){
                        case -1:
                            printf("\n\a # Ocurrio un error con el archivo..."); break;
                        case 0:
                            printf("\n # No hay pedidos cargados que correspondan a la forma ingresada..."); break;
                        default:
                            printf("\n # Se exportaron %d pedido/s en el archivo export_pedidos_formpago.txt... ", resultado);
                    }
                }
                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 14:{ //Mostrar todos los combos.
                system("cls");
                printf("\n\n            ");
                printf("# # # # #  M E N U  # # # # #");
                mostrar_combos(combos_del_dia);
                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 15:{ //Mostrar precio y stock de un combo por idcombo
                mostrar_precio_y_stock(combos_del_dia);
                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 16:{ //Mostrar los combos sin stock.
                system("cls");
                printf("\n # # # #   C O M B O S   S I N   S T O C K   # # # #\n");
                muestra_combos_sin_stock(combos_del_dia);
                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 17:{ //Modificar precio y stock del combo segun idcombo
                system("cls");
                printf("\n # # # #   M O D I F I C A   P R E C I O   Y   S T O C K   S E G U N   C O M B O   # # # #\n");
                modifica_precio_y_stock_de_combo(combos_del_dia);
                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 18:{ // f-t
                system("cls");
                printf("\n # # # #   C A M B I A R   V E N D E D O R   # # # #\n");
                printf("\n - ID Vendedor actual: %d", var_glob_vend_id);
                printf("\n\n - Por favor ingrese el ID del nuevo vendedor: ");
                fflush(stdin);
                check_resp = scanf("%d", &var_glob_vend_id);
                while (var_glob_vend_id > 3 || var_glob_vend_id < 1 || check_resp != 1){
                    printf("\n\a # ID no valido (1 <= ID <= 3)...");
                    printf("\n - Por favor ingrese el ID de vendedor: ");
                    fflush(stdin);
                    check_resp = scanf("%d", &var_glob_vend_id);
                }

                if (mod_vend_id() == 0){
                    printf("\n # Vendedor sustituido correctamente...");
                }
                else printf("\n\a # No se pudo acceder a prog_data");

                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();

                /*
                system("cls");
                printf("\n # # # #   C A M B I A R   V E N D E D O R   # # # #\n");
                printf("\n - ID Vendedor actual: %d", var_glob_vend_id);
                printf("\n\n - Por favor ingrese el ID del nuevo vendedor: ");
                fflush(stdin);
                check_resp = scanf("%d", &var_glob_vend_id);
                while (var_glob_vend_id > 3 || var_glob_vend_id < 1 || check_resp != 1){
                    printf("\n\a # ID no valido (1 <= ID <= 3)...");
                    printf("\n - Por favor ingrese el ID de vendedor: ");
                    fflush(stdin);
                    check_resp = scanf("%d", &var_glob_vend_id);
                }

                if (mod_vend_id(2) == 0){
                    printf("\n # Vendedor sustituido correctamente...");
                }
                else printf("\n\a # No se pudo acceder a vend_id.txt");

                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                */
                break;
            }

            case 19:{ //Informar cual es el vendedor que realizo mas pedidos en el mes.
                system("cls");
                printf("\n # # # #   V E N D E D O R   D E L   M E S   # # # #\n");

                if(!isEmpty(pedidos)){
                    printf("\n - Ingrese el mes: ");
                    fflush(stdin);
                    check_resp = scanf("%d", &mes);
                    while (mes<1 || mes>12 || check_resp != 1){
                        printf("\n\a # Respuesta invalida... ");
                        printf("\n - Ingrese el mes: ");
                        fflush(stdin);
                        check_resp = scanf("%d", &mes);
                    }
                    printf("\n -");
                    empleado_del_mes(pedidos,mes);
                }
                else printf("\n # No hay pedidos cargados en la base...\n");
                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;
            }

            case 20:{
                system("cls");
                if(!isEmpty(pedidos)){
                    int total, id;

                    printf("\n - Ingrese el vendedor: ");
                    check_resp = scanf("%d", &id);
                    while (id < 0 || id > 3 || check_resp != 1){
                        printf("\n\a # Respuesta invalida... ");
                        printf("\n - Ingrese el vendedor: ");
                        check_resp = scanf("%d", &id);
                    }
                    system("cls");

                    total=contar_pedidos_vend(pedidos,id,0);

                    printf("\n -Total de pedidos por el vendedor %d: %d.",id,total);
                }
                else printf("\n # No hay pedidos cargados en la base...\n");
                printf("\n\n - Pulse para volver al menu..."); fflush(stdin); getchar();
                break;

            }

        }
    }
    return 7;
}

int buscar_x_idped(lista_pedidos *lista_ing, char id[]) // f-b
{
    reset_lista(lista_ing);
    while(isOos(*lista_ing)!=1)
    {
        if(strcmp(get_pedido_id(copy_lista(*lista_ing)), id) == 0){
            return 1;
        }
        else forward_lista(lista_ing);
    }
    return 0;
}

void cargar_pedido(lista_pedidos *lista_ing) // f-a
{
    pedido pre_carga;
    init_pedido(&pre_carga);
    int pedido_confirm=-1, check_resp=0, fecha_ok=0;

    while (pedido_confirm != 1)
    {
        int num_ing1, num_ing2, salir_iter=-1; float monto;
        char str_ing1[strsize], str_ing2[strsize];

        system("cls");
        printf("\n # # # #   C A R G A R   D A T O S   # # # #\n");
        printf("\n - Ingrese el nombre completo: "); scanf("%s %s", str_ing1, str_ing2);
        set_nombre(&pre_carga, str_ing1, str_ing2);
        set_pedido_id(&pre_carga, gen_pedido_id());
        set_vend_id(&pre_carga, var_glob_vend_id);

        while (salir_iter != 0)
        {
            system("cls");
            printf("\n\n            ");
            printf("# # # # #  M E N U  # # # # #");
            mostrar_combos(combos_del_dia);
            printf("\n\n    + Ingrese ID del combo: ");
            check_resp = scanf("%d", &num_ing1);
            while (num_ing1 < 0 || num_ing1 > (num_combos-1) || check_resp != 1){
                printf("\n\a    # ID no valido...");
                printf("\n    + Ingrese ID del combo: ");
                fflush(stdin);
                check_resp = scanf("%d", &num_ing1);
            }

            printf("\n    + Ingrese la cantidad de unidades del combo %d: ", num_ing1);
            check_resp = scanf("%d", &num_ing2);
            while (num_ing2 < 0 || check_resp != 1){
                printf("\n\a    # Respuesta no valida...");
                printf("\n    + Ingrese la cantidad de unidades del combo %d: ", num_ing1);
                fflush(stdin);
                check_resp = scanf("%d", &num_ing2);
            }

            if (num_ing2 > muestrastock(combos_del_dia[num_ing1])){
                printf("\n\a    # No hay stock suficiente...");
            }
            else set_comb_pedidos(&pre_carga, num_ing1, num_ing2);

            printf("\n\n - Desea seguir ingresando combos? (1=si / 0=no): ");
            check_resp = scanf("%d", &salir_iter);
            while (((salir_iter != 1) && (salir_iter != 0)) || (check_resp != 1)){
                printf("\n\a # Respuesta no valida...");
                printf("\n - Desea seguir ingresando combos? (1=si / 0=no): ");
                fflush(stdin);
                check_resp = scanf("%d", &salir_iter);
            }
        }
        system("cls");
        printf("\n # # # #   C A R G A R   D A T O S   # # # #\n");

        printf("\n - Consume en el local? (1=si / 0=no): ");
        check_resp = scanf("%d", &num_ing1);
        while (num_ing1 != 1 && num_ing1 != 0 || check_resp != 1){
            printf("\n\a # Respuesta no valida...");
            printf("\n - Consume en el local? (1=si / 0=no): ");
            fflush(stdin);
            check_resp = scanf("%d", &num_ing1);
        }
        set_consum_local(&pre_carga, num_ing1);

        printf("\n - Ingrese forma de pago:");
        printf("\n\t (1) Debito, (2) Credito, (3) QR, (4) Efectivo\n - Resp: ");
        check_resp = scanf("%d", &num_ing1);
        while (num_ing1 < 1 || num_ing1 > 4 || check_resp != 1){
            printf("\n\a # Respuesta no valida...");
            printf("\n - Ingrese forma de pago:");
            printf("\n\t (1) Debito, (2) Credito, (3) QR, (4) Efectivo\n - Resp: ");
            fflush(stdin);
            check_resp = scanf("%d", &num_ing1);
        }
        set_forma_pago(&pre_carga, num_ing1);

        printf("\n - Fecha de compra:");
        printf("\n\t (1) Cargar fecha actual, (2) Introducir manualmente\n + Resp: ");
        check_resp = scanf("%d", &num_ing1);
        while (num_ing1 != 1 && num_ing1 != 2 || check_resp != 1){
            printf("\n\a # Respuesta no valida...");
            printf("\n - Fecha de compra:");
            printf("\n\t (1) Cargar automaticamente la fecha actual, (2) Introducirla manualmente\n + Resp: ");
            fflush(stdin);
            check_resp = scanf("%d", &num_ing1);
        }
        switch (num_ing1){
            case 1:{
                set_fec_compra_dia(&pre_carga, fecha_actual->tm_mday);
                set_fec_compra_mes(&pre_carga, fecha_actual->tm_mon+1);
                set_fec_compra_anio(&pre_carga, fecha_actual->tm_year+1900);
                break;
            }
            case 2:{
                while (fecha_ok == 0){
                    printf("\n - Ingrese fecha de compra:");
                    printf("\n\t + Dia: "); check_resp = scanf("%d", &num_ing1);
                    while (num_ing1 < 0 || num_ing1 > 31 || check_resp != 1){
                        printf("\n\a  # Dia ingresado no valido...");
                        printf("\n\t + Dia: ");
                        fflush(stdin);
                        check_resp = scanf("%d", &num_ing1);
                    }

                    printf("\n\t + Mes: "); check_resp = scanf("%d", &num_ing2);
                    while (num_ing2 < (fecha_actual->tm_mon)+1 || num_ing2 > 12 || check_resp != 1){
                        printf("\n\a # Mes ingresado no valido...");
                        printf("\n\t + Mes: ");
                        fflush(stdin);
                        check_resp = scanf("%d", &num_ing2);
                    }

                    if (num_ing2 == (fecha_actual->tm_mon)+1 && num_ing1 < fecha_actual->tm_mday){
                        printf("\n\a # La fecha no puede ser menor a la actual...");
                    }
                    else fecha_ok = 1;
                }
                set_fec_compra_dia(&pre_carga, num_ing1);
                set_fec_compra_mes(&pre_carga, num_ing2);

                printf("\n\t + Anio: "); check_resp = scanf("%d", &num_ing1);
                while (num_ing1 < (fecha_actual->tm_year)+1900 || check_resp != 1){
                    printf("\n\a  # Anio ingresado no valido...");
                    printf("\n\t + Anio: ");
                    fflush(stdin);
                    check_resp = scanf("%d", &num_ing1);
                }
                set_fec_compra_anio(&pre_carga, num_ing1);
            }
        }

        printf("\n - Se entrego el pedido? (1=si / 0=no): ");
        check_resp = scanf("%d", &num_ing1);
        while (num_ing1 != 1 && num_ing1 != 0 || check_resp != 1){
            printf("\n\a # Respuesta no valida...");
            printf("\n - Se entrego el pedido? (1=si / 0=no): ");
            fflush(stdin);
            check_resp = scanf("%d", &num_ing1);
        }
        set_entregado(&pre_carga, num_ing1);

        printf("\n - Posee cupon de descuento? (1=si / 0=no): ");
        check_resp = scanf("%d", &num_ing1);
        while (num_ing1 != 1 && num_ing1 != 0 || check_resp != 1){
            printf("\n\a # Respuesta no valida...");
            printf("\n - Posee cupon de descuento? (1=si / 0=no): ");
            fflush(stdin);
            check_resp = scanf("%d", &num_ing1);
        }
        set_cup_descuento(&pre_carga, num_ing1);

        monto = calcular_subtotal_combos(combos_del_dia, get_comb_pedidos(pre_carga), get_cup_descuento(pre_carga));
        set_subtotal(&pre_carga, monto);

        if (get_consum_local(pre_carga) == 0) monto += costo_delivery;
        set_total(&pre_carga, monto);

        system("cls");
        printf("\n # # # #   C A R G A R   D A T O S   # # # #\n");
        mostrar_pedido(pre_carga);

        printf("\n\n # Desea confirmar el pedido?... (1=si / 0=no): ");
        check_resp = scanf("%d", &pedido_confirm);
        while (pedido_confirm != 1 && pedido_confirm != 0 || check_resp != 1){
            printf("\n\a # Respuesta no valida...");
            printf("\n\n # Desea confirmar el pedido?... (1=si / 0=no): ");
            fflush(stdin);
            check_resp = scanf("%d", &pedido_confirm);
        }

        if (pedido_confirm == 1){
            insert_lista(lista_ing, pre_carga);
            actualizar_combos_stock(combos_del_dia, get_comb_pedidos(pre_carga), 1);
            printf("\n - Se ha cargado el pedido...\n");
        }
        system("cls");
        printf("\n # # # #   C A R G A R   D A T O S   # # # #\n");
        printf("\n - Desea volver al menu?... (1=si / 0=no): ");
        check_resp = scanf("%d", &pedido_confirm);
        while (pedido_confirm != 1 && pedido_confirm != 0 || check_resp != 1){
            printf("\n\a # Respuesta no valida...");
            printf("\n - Desea volver al menu?... (1=si / 0=no): ");
            fflush(stdin);
            check_resp = scanf("%d", &pedido_confirm);
        }
    }
}

int contar_pedidos_vend(lista_pedidos lista_ing, int vend_id_ing, int n) // f-g
{
    // hacer un reset_lista en el main
    if(isOos(lista_ing) == 0)
    {
        if (get_vend_id(copy_lista(lista_ing)) == vend_id_ing) n+=1;
        forward_lista(&lista_ing);
        return contar_pedidos_vend(lista_ing, vend_id_ing, n);
    }
    else return n;
}

void empleado_del_mes(lista_pedidos l, int mes) // f-s
{
    pedido aux;
    int cont_vend1 = 0, cont_vend2 = 0, cont_vend3 = 0;
    reset_lista(&l);
    while (!isOos(l))
    {
        aux = copy_lista(l);
        if (get_fec_compra_mes(aux) == mes)
        {
            switch (get_vend_id(aux))
            {
            case 1:
                    cont_vend1++;
                    forward_lista(&l);
                    break;
            case 2:
                    cont_vend2++;
                    forward_lista(&l);
                    break;
            case 3:
                    cont_vend3++;
                    forward_lista(&l);
                    break;
            }
        }
        else
        {
            forward_lista(&l);
        }
    }

    if ((cont_vend1 == cont_vend2) && (cont_vend1 == cont_vend3))
    {
        printf(" Los tres empleados tienen la misma cantidad de pedidos realizados\n");
    }
    else
    {
        if (cont_vend1 == cont_vend2 && cont_vend1>cont_vend3)
        {
            printf(" Empleado 1 y empleado 2 son los empleados del mes!\n");
        }
        else
        {
            if (cont_vend3 == cont_vend2 && cont_vend3>cont_vend1)
            {
                    printf(" Empleado 2 y empleado 3 son los empleados del mes!\n");
            }
            else
            {
                    if (cont_vend1 == cont_vend3 && cont_vend1>cont_vend2)
                    {
                        printf(" Empleado 1 y empleado 3 son los empleados del mes!\n");
                    }
                    else{

                         if ((cont_vend1 > cont_vend2) && cont_vend1 > cont_vend3)
                        {
                            printf(" El empleado 1 es el que mas vendio en el mes\n");
                        }
                        else
                        {
                            if ((cont_vend2 > cont_vend1) && cont_vend2 > cont_vend3)
                            {
                                printf(" El empleado 2 es el que mas vendio en el mes\n");
                            }
                            else
                            {
                                printf(" El empleado 3 es el que mas vendio en el mes\n");
                            }
                        }
                    }
            }
        }
    }
}

void export_pedido(pedido ped_ing, char nombre_del_archivo[]) // f-l //implementar en el main
{
    int i;
    FILE *fp = fopen(nombre_del_archivo, "a");

    fprintf(fp, "%s %s\n", get_nomb(ped_ing), get_ape(ped_ing));
    fprintf(fp, "%s \n", get_pedido_id(ped_ing));
    fprintf(fp, "%d \n", get_vend_id(ped_ing));
    for (i=0; i<num_combos; i++){
        fprintf(fp, "%d\n", get_comb_pedidos(ped_ing)[i]);
    }
    fprintf(fp, "%d \n", get_forma_pago(ped_ing));
    fprintf(fp, "%.2f \n", get_subtotal(ped_ing));
    fprintf(fp, "%d \n", get_consum_local(ped_ing));
    fprintf(fp, "%.2f \n", get_total(ped_ing));
    fprintf(fp, "%d \n%d \n%d \n", get_fec_compra_dia(ped_ing), get_fec_compra_mes(ped_ing), get_fec_compra_anio(ped_ing));
    fprintf(fp, "%d ", get_entregado(ped_ing));
    fprintf(fp, "\n---\n");
    fclose(fp);
}

int export_pedidosxformpago(lista_pedidos lista_ing, int form_pago) // f-ll //implementar en el main
{
    FILE *fp = fopen("export_pedidos_formpago.txt", "w");
    if (fp == NULL){
        return -1;
    }
    int contador=0, i;

    while(isOos(lista_ing) != 1){
        if (get_forma_pago(copy_lista(lista_ing)) == form_pago){
            fprintf(fp, "Nombre: %s %s\n", get_nomb(copy_lista(lista_ing)), get_ape(copy_lista(lista_ing)));
            fprintf(fp, "ID Pedido: %s \n", get_pedido_id(copy_lista(lista_ing)));
            fprintf(fp, "ID Vendedor: %d \n", get_vend_id(copy_lista(lista_ing)));
            for (i=0; i<num_combos; i++){
                fprintf(fp, "ID COMBO: %d -> unidades: %d\n", i, get_comb_pedidos(copy_lista(lista_ing))[i]);
            }
            fprintf(fp, "Pagado con ");
            switch (get_forma_pago(copy_lista(lista_ing))){
                case 1:{
                    fprintf(fp, "DEBITO \n"); break;
                }
                case 2:{
                    fprintf(fp, "CREDITO \n"); break;
                }
                case 3:{
                    fprintf(fp, "QR \n"); break;
                }
                case 4:{
                    fprintf(fp, "EFECTIVO \n");
                }
            }

            fprintf(fp, "Subtotal: $ %.2f \n", get_subtotal(copy_lista(lista_ing)));

            switch (get_consum_local(copy_lista(lista_ing))){
                case 1:{
                    fprintf(fp, "CONSUMIO EN EL LOCAL \n"); break;
                }
                case 0:{
                    fprintf(fp, "PIDIO DELIVERY\n");
                }
            }

            fprintf(fp, "Total: $ %.2f \n", get_total(copy_lista(lista_ing)));
            fprintf(fp, "Fecha de compra: %d/%d/%d \n", get_fec_compra_dia(copy_lista(lista_ing)), get_fec_compra_mes(copy_lista(lista_ing)), get_fec_compra_anio(copy_lista(lista_ing)));

            fprintf(fp, "Estado: ");
            switch(get_entregado(copy_lista(lista_ing))){
                case 1:{
                    fprintf(fp, "ENTREGADO \n"); break;
                }
                case 0:{
                    fprintf(fp, "NO ENTREGADO \n");
                }
            }
            fprintf(fp, "Cupon de descuento: ");
            if (get_cup_descuento(copy_lista(lista_ing)) == 1) fprintf(fp, "SI \n");
            else fprintf(fp, "no \n");

            fprintf(fp, "---\n");
            contador += 1;
        }
        forward_lista(&lista_ing);
    }

    fclose(fp);
    return contador;
}

void import_pedidos(lista_pedidos *lista_ing) // f-m
{
    FILE *pedidos_importados;
    char nomb_del_archivo[30];

    system("cls");
    printf("\n # # #   I M P O R T A R   P E D I D O S   # # #\n");

    printf("\n - Introduzca el nombre completo del archivo (ej: ARCHIVO_DE_EJEMPLO.txt)");
    printf("\n    * Puede introducir \"ayuda\" para ver mas informacion... ");

    printf("\n\n - Resp: ");
    scanf("%s", nomb_del_archivo);

    // corregir el texto de ayuda

    if (strcmp(nomb_del_archivo, "ayuda") == 0){
        system("cls");
        printf("\n\a # # #   A Y U D A   # # #\n");
        printf("\n * Coloque el archivo en dentro de la carpeta del programa...\n");
        printf("\n * El nombre del archivo no debe contener ningun espacio...\n");
        printf("\n * El archivo de tener el siguiente formato:");
        printf("\n    NOMBRE APELLIDO");
        printf("\n    PEDIDO_ID");
        printf("\n    VEND_ID");
        printf("\n    UNIDADES_DEL_COMBO (Una debajo de la otra)");
        printf("\n    FORMA_DE_PAGO");
        printf("\n    SUBTOTAL");
        printf("\n    CONSUM_LOCAL");
        printf("\n    TOTAL");
        printf("\n    DIA\n    MES\n    ANIO");
        printf("\n    ENTREGADO");
        printf("\n    --- (Separador)");
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

        char pedido_id[idsize], nomb[strsize], ape[strsize], separador[20];
        int vend_id, comb_pedidos[num_combos], consum_local, forma_pago, entregado, cup_descuento = 0, i, ped_no_cargados=0;
        float subtotal, total;

        fecha fec_compra;

        printf("\n # # #   I M P O R T A R   P E D I D O S   # # #\n");
        printf("\n - Desde: %s", nomb_del_archivo);
        printf("\n\n\a # Datos Importados:\n");
        while (feof(pedidos_importados)==0){
            init_pedido(&nuevo_pedido);

            fscanf(pedidos_importados, "%s %s", nomb, ape);
            fscanf(pedidos_importados, "%s ", pedido_id);

            fscanf(pedidos_importados, "%d ", &vend_id);

            for (i=0; i<num_combos; i++){
                fscanf(pedidos_importados, "%d ", &comb_pedidos[i]);
            }

            fscanf(pedidos_importados, "%d ", &forma_pago);
            fscanf(pedidos_importados, "%f ", &subtotal);
            fscanf(pedidos_importados, "%d ", &consum_local);
            fscanf(pedidos_importados, "%f ", &total);

            if (subtotal != calcular_subtotal_combos(combos_del_dia, comb_pedidos, 0)) cup_descuento = 1;
            else cup_descuento = 0;

            fscanf(pedidos_importados, "%d %d %d ", &fec_compra.dia, &fec_compra.mes, &fec_compra.anio);
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
            set_subtotal(&nuevo_pedido, subtotal);
            set_total(&nuevo_pedido, total);
            set_forma_pago(&nuevo_pedido, forma_pago);
            set_entregado(&nuevo_pedido, entregado);
            set_cup_descuento(&nuevo_pedido, cup_descuento);
            /*
            insert_lista(lista_ing, nuevo_pedido);
            mostrar_pedido(copy_lista(*lista_ing));
            */
            if (actualizar_combos_stock(combos_del_dia, get_comb_pedidos(nuevo_pedido), 1) == 0){
                insert_lista(lista_ing, nuevo_pedido);
                mostrar_pedido(copy_lista(*lista_ing));
            }
            else{
                ped_no_cargados += 1;
                printf("\n    #####################################################################################");
                printf("\n    #### Pedido %s no fue cargado. No hay stock suficiente para subir la carga ####", get_pedido_id(nuevo_pedido));
                printf("\n    #####################################################################################");
            }
            printf("\n");
        }
        if (ped_no_cargados > 0) printf("\n\a #### ATENCION: Hay %d pedido/s no cargados, consulte las lineas anteriores... ####", ped_no_cargados);
        fclose(pedidos_importados);
    }
    printf("\n\n - Pulse una tecla para volver al menu...");
    fflush(stdin);
    getchar();
}

void modificar_estado_por_id(lista_pedidos *l, char n[]){ // f-i
    int boo=0;
    pedido aux;
    reset_lista(l);
    while(!isOos(*l)){
        aux=copy_lista(*l);
        if(strcmp(get_pedido_id(aux),n)==0){
            supress_lista(l);
            set_entregado(&aux,1);
            insert_lista(l,aux);
            printf("\n - Se modificado el pedido. ");
            boo=1;
        }
        forward_lista(l);
    }
    if(boo==0) printf("\n\a # No hay pedidos con ese ID");
}

void mod_form_pago_pedido(lista_pedidos *lista_ing, int formpago_ing) // f-j
{
    pedido aux;
    aux = copy_lista(*lista_ing);

    set_forma_pago(&aux, formpago_ing);

    supress_lista(lista_ing);
    insert_lista(lista_ing, aux);
}

void modificar_nombre_por_id(lista_pedidos *l, char n[]){ // f-k
    int boo = 0;
    char str_ing1[strsize], str_ing2[strsize];
    pedido aux;
    reset_lista(l);
    while (!isOos(*l))
    {
        if (strcmp(get_pedido_id(copy_lista(*l)), n) == 0)
        {
            aux = copy_lista(*l);
            supress_lista(l);
            printf("\n - Ingrese el nombre completo: ");
            scanf("%s %s", str_ing1, str_ing2);
            set_nombre(&aux, str_ing1, str_ing2);
            insert_lista(l, aux);
            boo = 1;
        }
        forward_lista(l);
    }
    if (boo == 0) printf("\n\a # El pedido no esta cargado en la base.");
    else printf("\n # Pedido modificado");
}

void modifica_precio_y_stock_de_combo(combo arr[]){ // f-o
    int id, stock, check_resp=0;
    float precio;
    printf("\n - Ingrese ID del combo que quiere modificar: ");
    check_resp = scanf("%d", &id);
    while (id < 0 || id > num_combos || check_resp != 1){
        printf("\n\a # Respuesta invalida... ");
        printf("\n - Ingrese ID del combo que quiere modificar: ");
        fflush(stdin);
        check_resp = scanf("%d", &id);
    }

    printf("\n - Ingrese stock: ");
    check_resp = scanf("%d", &stock);
    while (stock < 0 || check_resp != 1){
        printf("\n\a # Respuesta invalida... ");
        printf("\n - Ingrese stock: ");
        fflush(stdin);
        check_resp = scanf("%d",&stock);
    }
    cargastock(&arr[id],stock);

    printf("\n - Ingrese el nuevo precio de unidad: ");
    check_resp = scanf("%f",&precio);
    while (precio < 0 || check_resp != 1){
        printf("\n\a # Respuesta invalida... ");
        printf("\n - Ingrese nuevo precio de unidad: ");
        fflush(stdin);
        check_resp = scanf("%f",&precio);
    }
    cargaprecio(&arr[id],precio);

    printf("\n # Combo modificado correctamente. ");
}

int mod_vend_id(void){ // f-t
    char nomb_archiv_menu_guardada[strsize]; int basura;
    FILE *fp = fopen("usuario.data", "r+");
    if (fp == NULL) return 1;

    fscanf(fp, "%d\n", &basura);
    fscanf(fp, "%s", nomb_archiv_menu_guardada);

    rewind(fp);
    fprintf(fp,"%d", var_glob_vend_id);
    fprintf(fp,"\n%s", nomb_archiv_menu_guardada);
    fclose(fp);
    return 0;
}

void mostrar_comb_pedidos(int comb_pedidos[], int cup_descuento){
    int i; float mont_indiv_desc=0;
    for (i=0; i<num_combos; i++){
        if (comb_pedidos[i] > 0){
            printf("\n    -----------------------------------");
            printf("\n     ");
            printf("ID: %d | %s", i, muestradescripcion(combos_del_dia[i]));
            if (muestradescuento(combos_del_dia[i]) == 1 && cup_descuento == 1) printf(" (15%% OFF)");
            printf("\n     ");
            printf("Unidades: %d | ", comb_pedidos[i]);
            printf("Precio Unid.: $ %.2f", muestraprecio(combos_del_dia[i]));

            if (muestradescuento(combos_del_dia[i]) == 1 && cup_descuento == 1){
                mont_indiv_desc = (muestraprecio(combos_del_dia[i]) * comb_pedidos[i]);
                mont_indiv_desc -= ((val_descuento*mont_indiv_desc)/100);
                printf(" | Precio total + descuent.: $ %.2f", mont_indiv_desc);
            }
            else printf(" | Precio total: $ %.2f", (muestraprecio(combos_del_dia[i]) * comb_pedidos[i]));
        }
    }
}

void mostrar_combos(combo combos_ing[]){ // f-p
    int i;
    for (i=0; i<num_combos; i++){
        printf("\n   -------------------------------------------------");
        printf("\n     ");
        printf("ID: %d | %s", i, muestradescripcion(combos_ing[i]));
        printf("\n     ");
        printf("Stock: %d | ", get_combo_stock(combos_ing, i));
        printf("Precio: $ %.2f | ", muestraprecio(combos_ing[i]));
        printf("Descuento: ");
        if (muestradescuento(combos_ing[i]) == 1) printf("si");
        else printf("no");
    }
}

void mostrar_precio_y_stock(combo arr[]){// f-q
    int id, check_resp;
    system("cls");
    printf("\n # # # #   P R E C I O   Y   S T O C K   # # # #\n");
    printf("\n - Ingrese el ID del combo: ");
    fflush(stdin);
    check_resp = scanf("%d", &id);
    while(id<0||id>num_combos || check_resp!=1){
        printf("\n\a # Respuesta invalida... ");
        printf("\n - Ingrese el ID del combo: ");
        fflush(stdin);
        check_resp = scanf("%d", &id);
    }
    system("cls");
    printf("\n # # # #   P R E C I O   Y   S T O C K   # # # #\n");
    printf("\n\n ---------------------------------\n");
    printf("   + Id: %d\n",id);
    printf("      - Stock: %d.\n",muestrastock(arr[id]));
    printf("      - Precio: $ %.2f.\n",muestraprecio(arr[id]));
    printf("\n ---------------------------------");
}

void mostrar_pedido(pedido ped_ing)
{
    printf("\n    ###################################");
    printf("\n     + Pedido ID: %s", get_pedido_id(ped_ing));
    printf("\n       + Vendedor ID: %d", get_vend_id(ped_ing));
    printf("\n       + Fecha de compra: %d/%d/%d", get_fec_compra_dia(ped_ing), get_fec_compra_mes(ped_ing), get_fec_compra_anio(ped_ing));
    printf("\n       + Nombre: %s %s", get_nomb(ped_ing), get_ape(ped_ing));

    mostrar_comb_pedidos(get_comb_pedidos(ped_ing), get_cup_descuento(ped_ing));
    printf("\n    -----------------------------------");
    printf("\n     + Consume en el local: ");
    if(get_consum_local(ped_ing) == 1) printf("si");
    else printf("no");

    printf("\n     + Cupon de descuento: ");
    if(get_cup_descuento(ped_ing) == 1) printf("si");
    else printf("no");

    printf("\n    -----------------------------------");
    printf("\n     + Subtotal: $ %.2f", get_subtotal(ped_ing));
    if (get_consum_local(ped_ing) == 0) printf("\n     + Costo Delivery: %d", costo_delivery);
    printf("\n    -----------------------------------");
    printf("\n     - Total: $ %.2f", get_total(ped_ing));
    printf("\n    -----------------------------------");
    printf("\n     + Metodo de pago: ");
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
    printf("\n     + Estado: ");
    if (get_entregado(ped_ing) == 1) printf("Entregado.");
    else printf("No Entregado.");
}

void mostrar_pedido_por_vendedor(lista_pedidos lista_ing, int vend_id_ing,int *x){// f-f
    // hacer un reset_lista en el main
    if (isOos(lista_ing)!=1){
        if (get_vend_id(copy_lista(lista_ing)) == vend_id_ing){
            printf("\n # # # #   M O S T R A R   P E D I D O S   P O R   V E N D E D O R   # # # #\n");
            printf("\n    ###################################");
            printf("\n     + Pedido ID: %s", get_pedido_id(copy_lista(lista_ing)));
            printf("\n    -----------------------------------");
            printf("\n     - Total: $ %.2f", get_total(copy_lista(lista_ing)));
            printf("\n    -----------------------------------\n");
            *x=1;
        }
        forward_lista(&lista_ing);
        mostrar_pedido_por_vendedor(lista_ing, vend_id_ing,x);
    }
}

void muestra_combos_sin_stock(combo combos_ing[]){
    int i,boo=0;
    for (i = 0; i < num_combos; i++){
        if(muestrastock(combos_ing[i])==0){
            printf("\n   -------------------------------------------------");
            printf("\n     ");
            printf("ID: %d | %s", i, muestradescripcion(combos_ing[i]));
            printf("\n     ");
            printf("Stock: %d | ", get_combo_stock(combos_ing, i));
            printf("Precio: $ %.2f | ", muestraprecio(combos_ing[i]));
            printf("Descuento: ");
            if (muestradescuento(combos_ing[i]) == 1) printf("si");
            else printf("no");
            boo=1;
        }
    }
    if(boo!=1) printf("\n # No hay combos sin stock\n");
}

void muestra_ped_no_entregados(lista_pedidos l){// f-n //poner debajo de las otras funciones
    int boo=0;

    reset_lista(&l);
    while(!isOos(l)){
        if(get_entregado(copy_lista(l))==0){
            mostrar_pedido(copy_lista(l));
            printf("\n");
            boo=1;
        }
        forward_lista(&l);
    }
    if(boo==0) printf("\n # No hay pedidos sin entregar\n");
    else printf("\n # Todos los pedidos sin entregar mostrados\n");
}

int muestra_pedxnomb(lista_pedidos l, char c[]){ // f-e
    int boo=0;
    pedido aux;
    reset_lista(&l);
    while(!isOos(l)){
        aux=copy_lista(l);
        if (strcmp(get_nomb(aux),c)==0){
            boo=1;
            system("cls");
            printf("\n # # # #   M O S T R A R   P E D I D O S   P O R   N O M B R E   # # # #\n");
            printf("\n - Nombre: %s\n", c);
            mostrar_pedido(aux);
            printf("\n\n - Pulse para continuar..."); fflush(stdin); getchar();
            forward_lista(&l);
        }
        else forward_lista(&l);
    }
    if (boo == 1) return 0;
    else return 1;
}

void muestra_por_mes(lista_pedidos l,int mes){//D
    pedido aux;
    int cont=0,i=0,j,n;

        reset_lista(&l);

        while(!isOos(l)){
            aux=copy_lista(l);
            if(get_fec_compra_mes(aux)==mes)
                cont++;
            forward_lista(&l);
        }

        pedido *arr=(pedido *)malloc(sizeof(pedido)*cont);

        reset_lista(&l);

        while(!isOos(l)){
            aux=copy_lista(l);
            if(get_fec_compra_mes(aux)==mes){
                arr[i]=aux;
                i++;
            }
            forward_lista(&l);
        }

        for(n=0;n<cont-1;n++){
            for(j=0;j<cont-1;j++){
                if(get_fec_compra_dia(arr[j])>get_fec_compra_dia(arr[j+1])){
                    aux=arr[j];
                    arr[j]=arr[j+1];
                    arr[j+1]=aux;
                }
            }
        }

        for(n=0;n<cont;n++){
            mostrar_pedido(arr[n]);
            printf("\n");
        }
        if(cont==0) printf("\n # No hay pedidos en ese mes.\n");

}

int precarga_combos(combo combos_del_dia[], char nombre_del_archivo[]) // f-enie
{
    FILE *archivo_combos = fopen(nombre_del_archivo, "r");
    if (archivo_combos == NULL){
        return 1;
    }

    char descripcion[descrip_size], separador[strsize];
    int id_combo_scan, stock, descuento;
    float preciounit;


    while(feof(archivo_combos) == 0)
    {
        fscanf(archivo_combos, "%d ", &id_combo_scan);
        fscanf(archivo_combos, "%[^\n]s", descripcion);
        fscanf(archivo_combos, "%d ", &stock);
        fscanf(archivo_combos, "%f ", &preciounit);
        fscanf(archivo_combos, "%d ", &descuento);
        fscanf(archivo_combos, "%s ", separador);

        cargaid(&combos_del_dia[id_combo_scan], id_combo_scan);
        cargadescripcion(&combos_del_dia[id_combo_scan], descripcion);
        cargastock(&combos_del_dia[id_combo_scan], stock);
        cargaprecio(&combos_del_dia[id_combo_scan], preciounit);
        cargadescuento(&combos_del_dia[id_combo_scan], descuento);
    }

    fclose(archivo_combos);
    return 0;
}

int user_data(int opcion){ // 1= restaura, 2=sobreescribe
    if (opcion == 1){
        FILE *fp = fopen("usuario.data", "r");
        if (fp == NULL) return 1;

        fscanf(fp, "%d\n", &var_glob_vend_id);
        fscanf(fp, "%s", nomb_archiv_menu);
        if (precarga_combos(combos_del_dia, nomb_archiv_menu) == 1) return 2;
        fclose(fp);
        return 0;
    }
    else{
        FILE *fp = fopen("usuario.data", "w");
        fprintf(fp, "%d\n%s", var_glob_vend_id, nomb_archiv_menu);
        fclose(fp);
        return 0;
    }
}

