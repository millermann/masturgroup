#include <stdio.h>
#include <stdlib.h>

#include "combo.h"
#include "pedido.h"
#include "lista.h"

int var_glob_vend_id;
int boo6=0;
combo combos_del_dia[num_combos];

time_t ahora;
struct tm *fecha_actual ;

void cargar_pedido(lista_pedidos *lista_ing);
void precarga_combos(combo combos_del_dia[]);
void mostrar_pedido(pedido ped_ing);


void empleado_del_mes(lista_pedidos l,int mes){
    pedido aux;
    int cont_vend1=0,cont_vend2=0,cont_vend3=0,auxd;
    reset_lista(&l);
    while(!isOos(l))
    {
        aux=copy_lista(l);
        if(get_fec_compra_mes(aux)==mes)
        {
            switch(get_vend_id(aux))
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
    if((cont_vend1>cont_vend2)&&cont_vend1>cont_vend3)
    {
        printf("El empleado 1 es el que mas vendio en el mes\n");
        auxd=1;
    }
    else
    {
        if((cont_vend2>cont_vend1)&&cont_vend2>cont_vend3)
        {
            printf("El empleado 2 es el que mas vendio en el mes\n");
            auxd=2;
        }
        else
        {
            printf("El empleado 3 es el que mas vendio en el mes\n");
            auxd=3;
        }
    }
    if((cont_vend1==cont_vend2)&&(cont_vend1==cont_vend3))
    {
        printf("Los tres empleados tienen la misma cantidad de pedidos realizados\n");

    }
    else
    {
        if(cont_vend1==cont_vend2 && auxd!=3 )
        {
            printf("¡Empleado 1 y empleado 2 son los empleados del mes!\n");
        }
        else
        {
            if(cont_vend3==cont_vend2 && auxd!=1)
            {
                printf("¡Empleado 2 y empleado 3 son los empleados del mes!\n");
            }
            else
            {
                if(cont_vend1==cont_vend3 && auxd!=2)
                {
                    printf("¡Empleado 1 y empleado 3 son los empleados del mes!\n");
                }
            }
        }
    }

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

void muestra_combos_sin_stock(combo combos_ing[]){
    int i,boo=0;
    printf("Combos sin stock:\n");
    for(i=0;i<num_combos;i++){
        if(muestrastock(combos_ing[i])==0){

            printf("\n   ---------------------------");
            printf("\n     ");
            printf("id: %d | %s", i, muestradescripcion(combos_ing[i]));
            printf("\n     ");
            printf("stock: %d | ", get_combo_stock(combos_ing, i));
            printf("precio: %.2f | ", muestraprecio(combos_ing[i]));
            printf("descuento: ");
            if (muestradescuento(combos_ing[i]) == 1) printf("si");
            else printf("no");

            boo=1;
        }
    }
    if(boo!=1)
        printf(" No hay combos sin stock\n");
    system("pause");
    system("cls");
}

void modifica_precio_y_stock_de_combo(combo arr[]){
    int id,stock;
    float precio;
    do{
        printf("    Ingrese id del combo que quiere modificar: ");
        scanf(" %d",&id);
    }while(id<0||id>9);
    do{
    printf("    Ingrese nuevo precio: ");
    scanf(" %f",&precio);
    cargaprecio(&arr[id],precio);
    }while(precio<0);

    do{
    printf("    Ingrese nuevo stock: ");
    scanf(" %d",&stock);
    cargastock(&arr[id],stock);
    }while(stock<0);
}

void muestra_combos(combo arr[]){
    int i;
    for(i=0;i<10;i++){
        printf("   -Id: %d\n",i);
        printf("   -Descripcion: %s.\n",muestradescripcion(arr[i]));
        printf("   -Stock: %d.\n",muestrastock(arr[i]));
        printf("   -Precio: %.2f.\n",muestraprecio(arr[i]));
        if(muestradescuento(arr[i])==1)
            printf("   -Tiene descuento.\n");
        else
            printf("   -No tiene descuento.\n");
        printf("\n\n");
    }
    system("pause");
    system("cls");
}

void mostrar_pecio_y_stock(combo arr[]){// f-q
    int id;
    do{
        printf("    Ingrese id del combo que quiere modificar: ");
        scanf(" %d",&id);
    }while(id<0||id>9);
    system("cls");
    printf("   -Id: %d\n",id);
    printf("   -Stock: %d.\n",muestrastock(arr[id]));
    printf("   -Precio: %.2f.\n",muestraprecio(arr[id]));
    system("pause");
    system("cls");
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
                if(get_fec_compra_mes(arr[j])>get_fec_compra_mes(arr[j+1])){
                    aux=arr[j];
                    arr[j]=arr[j+1];
                    arr[j+1]=aux;
                }
            }
        }

        for(n=0;n<cont;n++){
            mostrar_pedido(arr[n]);
        }
        if(cont==0)
            printf("No hay pedidos en ese mes.\n");

    system("pause");
    system("cls");
}

void mostrar_pedido_por_vendedor(lista_pedidos lista_ing, int vend_id_ing){// f-f
    // hacer un reset_lista en el main
    if (isOos(lista_ing)!=1){
        if (get_vend_id(copy_lista(lista_ing)) == vend_id_ing){
            printf("\n    ###################################");
            printf("\n     + Pedido ID: %s", get_pedido_id(copy_lista(lista_ing)));
            printf("\n    -----------------------------------");
            printf("\n     - Total: %.2f", get_total(copy_lista(lista_ing)));
            printf("\n    -----------------------------------\n");
             boo6=1;
        }
        forward_lista(&lista_ing);
        mostrar_pedido_por_vendedor(lista_ing, vend_id_ing);
    }
}

void muestra_ped_no_entregados(lista_pedidos l){// f-n //poner debajo de las otras funciones
    int boo=0;
    if(!isEmpty(l)){
        reset_lista(&l);
        printf("Pedidos no entregados: \n");
        while(!isOos(l)){
            if(get_entregado(copy_lista(l))==0){
                mostrar_pedido(copy_lista(l));
                boo=1;
            }
            forward_lista(&l);
        }
        if(boo==0)
            printf("\nNo hay pedidos sin entregar\n");
        else
            printf("\nTodos los pedidos sin entregar mostrados\n");
    }
    else printf("No hay pedidos\n");
    system("pause");
    system("cls");
}

void modificar_nombre_por_id(lista_pedidos *l,char n[]){
    int boo=0;
    if(!isEmpty(*l)){
        char str_ing1[strsize], str_ing2[strsize];
        pedido aux;
        reset_lista(l);

        while(!isOos(*l)){
            aux=copy_lista(*l);
            if(strcmp(get_pedido_id(aux),n)==0){
                supress_lista(l);

                printf("\n - Ing. solamente el nombre: "); scanf("%s", str_ing1);
                printf("\n - Ing. el apellido: "); scanf("%s", str_ing2);

                set_nombre(&aux, str_ing1, str_ing2);

                insert_lista(l,aux);

                boo=1;
            }
            forward_lista(l);
        }
        if(boo==0)
            printf("No hay pedidos con ese id\n");
        else printf("Pedido modificado\n");

    }
    else printf("Lista vacía!\n");
}

void modificar_estado_por_id(lista_pedidos *l,char n[]){
    int boo=0;
    if(!isEmpty(*l)){
        pedido aux;
        reset_lista(l);

        while(!isOos(*l)){
            aux=copy_lista(*l);
            if(strcmp(get_pedido_id(aux),n)==0){
                supress_lista(l);

                set_entregado(&aux,1);

                insert_lista(l,aux);

                boo=1;
            }
            forward_lista(l);
        }
        if(boo==0)
            printf("No hay pedidos con ese id\n");
        else printf("Pedido modificado\n");
    }
    else printf("Lista vacía!\n");
}

int main()
{
    int opcion=-1, check_resp=0, mes;
    char nomb[strsize], id_pedido[idsize];
    lista_pedidos pedidos;
    if (mod_vend_id(1) == 1){
        printf("\n\a # No se pudo recuperar el ID del ultimo vendedor, por favor ingrese el ID de vendedor: ");
        fflush(stdin);
        check_resp = scanf("%d", &var_glob_vend_id);
        while (var_glob_vend_id > 3 || var_glob_vend_id < 1 || check_resp != 1){
            printf("\n\a # ID no valido (1 <= ID <= 3)...");
            printf("\n Por favor ingrese el ID de vendedor: ");
            fflush(stdin);
            check_resp = scanf("%d", &var_glob_vend_id);
        }
        mod_vend_id(2);
        printf("\n # ID cargado correctamente.");
        printf("\n\n - Pulse para continuar..."); fflush(stdin); getchar();
    }

    init_lista(&pedidos);
    precarga_combos(combos_del_dia);

    ahora = time(NULL);
    fecha_actual = localtime(&ahora);


    while (opcion != 0)
    {
        system("cls");
        printf("\n # # # #  M E N U  # # # #\n");
        printf("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
        //funciones de cargar y modificar
        printf("\n  (1) Cargar pedido");
        printf("\n  (2) Cambiar al vendedor");
        printf("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
        //funciones de mostrar
        printf("\n  (3) Mostrar pedido por ID Pedido");
        printf("\n  (4) Mostrar pedidos cargados.");
        printf("\n  (5) Mostrar pedidos por mes.");
        printf("\n  (6) Mostrar pedidos por nombre.");
        printf("\n  (7) Mostrar pedidos segun vendedor.");
        printf("\n  (8) Mostrar pedidos no entregados.");
        printf("\n  (9) Mostrar todos los combos.");
        printf("\n  (10) Mostrar precio y stock de un combo por idcombo.");
        printf("\n  (11) Mostrar los combos sin stock.");
        printf("\n  (12) Informar cual es el vendedor que realizo mas pedidos en el mes.");

        printf("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
        //funciones de modificaciones
        printf("\n  (13) Modificar el estado del pedido a entregado por idpedido.");
        printf("\n  (14) Modificar la forma de pago de un pedido según su idpedido.");
        printf("\n  (15) Modificar nombre de un pedido según su idpedido");
        printf("\n  (16) Modificar precio y stock del combo según idcombo");


        printf("\n - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - ");
        //funciones de archivos
        printf("\n  (17) Cargar pedidos por archivo");
        printf("\n  (18) Anular y exportar pedido por ID Pedido");
        printf("\n  (19) Exportar pedido segun forma de pago");
        printf("\n  (0) Salir");
        printf("\n\n + Resp: ");
        fflush(stdin);
        scanf("%d", &opcion);

        //control opcion

        switch (opcion)
        {
            case 1: { // f-a
                cargar_pedido(&pedidos);
                break;
            }

            case 2: { // f-t
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
                break;
            }

            case 3: { // f-c
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

            case 4: { // f-adic
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

            case 5:// f-d, Mostrar todos los pedidos por mes, no anda
                system("cls");
                if(!isEmpty(pedidos)){
                do{ printf("Ingrese numero de mes que desea mostrar: ");
                    scanf(" %d",&mes);
                }while(mes<1||mes>12);
                system("cls");
                muestra_por_mes(pedidos,mes);
                }
                else
                    printf("\n # No hay pedidos cargados en la base...");
                break;

            case 6:// f-e, Mostrar todos los pedidos por nombre.
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

            case 7://NO se si está bien hecho //Mostrar todos los pedidos de un vendedor
                system("cls");
                if(!isEmpty(pedidos)){
                    int id_vendedor=-1;
                    boo6=0;
                    reset_lista(&pedidos);
                    do{
                    printf("Ingrese Id del vendedor: "); //quizas mostrar los id
                    scanf(" %d",&id_vendedor);
                    if(id_vendedor<1 || id_vendedor>3){
                        printf("Id ingresado incorrecto!\n");
                        system("pause");
                        system("cls");
                    }
                    }while(id_vendedor<1 || id_vendedor>3);
                    system("cls");

                    mostrar_pedido_por_vendedor(pedidos,id_vendedor);
                    if(boo6==0)
                        printf("No hay pedidos con ese vendedor\n");

                }
                else
                    printf("\n # No hay pedidos cargados en la base...");

                system("pause");
                system("cls");
                break;

            case 8: //Mostrar todos los pedidos no entregados
                system("cls");
                if(!isEmpty(pedidos)){
                muestra_ped_no_entregados(pedidos);
                }
                else
                    printf("\n # No hay pedidos cargados en la base...");
                break;

            case 9://Mostrar todos los combos.
                system("cls");
                mostrar_combos(combos_del_dia);
                system("pause");
                system("cls");
                break;

            case 10://Mostrar precio y stock de un combo por idcombo
                system("cls");
                mostrar_pecio_y_stock(combos_del_dia);
                break;

            case 11://Mostrar los combos sin stock.
            system("cls");
                muestra_combos_sin_stock(combos_del_dia);
                break;

            case 12://funcion del chiche a terminar//Informar cual es el vendedor que realizo mas pedidos en el mes.
                system("cls");
                if(!isEmpty(pedidos)){

                    do{
                        printf("Ingrese el mes interesado a saber: ");
                        scanf(" %d",&mes);
                        if(mes<1 || mes>12){
                        printf("Mes ingresado incorrecto!\n");
                        system("pause");
                        system("cls");
                    }
                    } while(mes<1||mes>12);
                    empleado_del_mes(pedidos,mes);
                }
                else
                    printf("\n # No hay pedidos cargados en la base...");
                system("pause");
                system("cls");
                break;

            case 13://Modificar el estado del pedido a entregado por idpedido.");
                system("cls");
                if(!isEmpty(pedidos)){
                    printf("Ingrese el ID del pedido: ");
                    scanf(" %s",id_pedido);
                    system("cls");

                    modificar_estado_por_id(&pedidos,id_pedido);

                }
                    printf("\n # No hay pedidos cargados en la base...");
                break;

            case 14://Modificar la forma de pago de un pedido según su idpedido.
                 system("cls");
                if(!isEmpty(pedidos)){
                int form_pago;
                do{
                    printf("\n - ingrese la forma de pago. ");
                    printf("\n (1) Debito, (2) Credito, (3) QR, (4) Efectivo");
                    printf("\n + Resp: "); scanf("%d", &form_pago);
                }while (form_pago != 1 && form_pago != 2 && form_pago != 3 && form_pago != 4 );
                system("cls");
                printf("Ingrese el ID del pedido: ");
                scanf(" %s",id_pedido);
                system("cls");

                if(mod_form_pago_pedido(&pedidos,id_pedido,form_pago)==0)
                    printf("Forma de pago modificada\n");
                else
                    printf("No se encuentra un pedido con ese ID.\n");
                }
                else
                    printf("\n # No hay pedidos cargados en la base...");
                system("pause");
                system("cls");
                //NO La entendi a la funcion
                break;

            case 15:{//Modificar nombre de un pedido según su idpedido
                system("cls");
                if(!isEmpty(pedidos)){
                    printf("Ingrese el ID del pedido: ");
                    scanf(" %s",id_pedido);
                    system("cls");

                    modificar_nombre_por_id(&pedidos,id_pedido);

                }
                    printf("\n # No hay pedidos cargados en la base...");
                break;
            }
            case 16:{//Modificar precio y stock del combo según idcombo
                    system("cls");
                    modifica_precio_y_stock_de_combo(combos_del_dia);

                break;
            }

            case 17: {
                import_pedidos(&pedidos);
                break;
            }

            case 18: { // f-l, Anular y exportar por id pedido
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

            case 19:{ // f-ll, Exportar pedido por form_pago
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

void cargar_pedido(lista_pedidos *lista_ing) // f-a
{
    pedido pre_carga;
    init_pedido(&pre_carga);
    int pedido_confirm=-1, check_resp=0;

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
            printf("\n # # # #   C A R G A R   D A T O S   # # # #");
            printf("\n\n            ");
            printf("# # # # #  M E N U  # # # # #");
            mostrar_combos(combos_del_dia);
            printf("\n\n   + Ingrese ID del combo: ");
            check_resp = scanf("%d", &num_ing1);
            while (num_ing1 < 0 || num_ing1 > (num_combos-1) || check_resp != 1){
                printf("\n\a   # ID no valido...");
                printf("\n\n   + Ingrese ID del combo: ");
                fflush(stdin);
                check_resp = scanf("%d", &num_ing1);
            }

            printf("\n   + Ingrese la cantidad de unidades del combo %d: ", num_ing1);
            check_resp = scanf("%d", &num_ing2);
            while (check_resp != 1){
                printf("\n\t\a # Respuesta no valida...");
                printf("\n   + Ingrese la cantidad de unidades del combo %d: ", num_ing1);
                fflush(stdin);
                check_resp = scanf("%d", &num_ing2);
            }

            if (num_ing2 > muestrastock(combos_del_dia[num_ing1])){
                printf("\n   # No hay stock suficiente...");
            }
            else set_comb_pedidos(&pre_carga, num_ing1, num_ing2);

            printf("\n - Desea seguir ingresando combos? (1=si / 0=no): ");
            check_resp = scanf("%d", &salir_iter);
            while (salir_iter != 1 && salir_iter != 0 || check_resp != 1){
                printf("\n\a # Respuesta no valida...");
                printf("\n - Desea seguir ingresando combos? (1=si / 0=no): ");
                fflush(stdin);
                check_resp = scanf("%d", &salir_iter);
            }
        }
        system("cls");
        printf(" # # # #   C A R G A R   D A T O S   # # # #\n");

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
                printf("\n - Ingrese fecha de compra:");
                printf("\n\t + Dia: "); check_resp = scanf("%d", &num_ing1);
                while (num_ing1 < fecha_actual->tm_mday || num_ing1 > 31 || check_resp != 1){
                    printf("\n\a  # Dia ingresado no valido...");
                    printf("\n\t + Dia: ");
                    fflush(stdin);
                    check_resp = scanf("%d", &num_ing1);
                }
                set_fec_compra_dia(&pre_carga, num_ing1);

                printf("\n\t + Mes: "); check_resp = scanf("%d", &num_ing1);
                while (num_ing1 < (fecha_actual->tm_mon)+1 || num_ing1 > 12 || check_resp != 1){
                    printf("\n\a # Mes ingresado no valido...");
                    printf("\n\t + Mes: ");
                    fflush(stdin);
                    check_resp = scanf("%d", &num_ing1);
                }
                set_fec_compra_mes(&pre_carga, num_ing1);

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
            actualizar_combos_stock(combos_del_dia, get_comb_pedidos(pre_carga));
            printf("\n - Se ha cargado el pedido...\n");
        }
        system("cls");
        printf(" # # # #   C A R G A R   D A T O S   # # # #\n");
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

            fprintf(fp, "Subtotal: %.2f \n", get_subtotal(copy_lista(lista_ing)));

            switch (get_consum_local(copy_lista(lista_ing))){
                case 1:{
                    fprintf(fp, "CONSUMIO EN EL LOCAL \n"); break;
                }
                case 0:{
                    fprintf(fp, "PIDIO DELIVERY\n");
                }
            }

            fprintf(fp, "Total: %.2f \n", get_total(copy_lista(lista_ing)));
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
        printf("\n    DIA MES ANIO");
        printf("\n    ENTREGADO");
        printf("\n    CUPON_DESCUENTO");
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
            if (actualizar_combos_stock(combos_del_dia, get_comb_pedidos(nuevo_pedido)) == 0){
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

void precarga_combos(combo combos_del_dia[]) // f-enie
{
    FILE *archivo_combos = fopen("menu.txt", "r");
    int id_combo_scan;
    char separador[strsize];

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

void mostrar_comb_pedidos(int comb_pedidos[], int cup_descuento){
    int i; float mont_indiv_desc=0;
    for (i=0; i<num_combos; i++){
        if (comb_pedidos[i] > 0){
            printf("\n    -----------------------------------");
            printf("\n     ");
            printf("ID: %d | %s", i, muestradescripcion(combos_del_dia[i]));
            if (muestradescuento(combos_del_dia[i]) == 1 && cup_descuento == 1) printf(" (15% OFF)");
            printf("\n     ");
            printf("Unidades: %d | ", comb_pedidos[i]);
            printf("Precio Unid.: %.2f", muestraprecio(combos_del_dia[i]));

            if (muestradescuento(combos_del_dia[i]) == 1 && cup_descuento == 1){
                mont_indiv_desc = (muestraprecio(combos_del_dia[i]) * comb_pedidos[i]);
                mont_indiv_desc -= ((val_descuento*mont_indiv_desc)/100);
                printf(" | Precio total + descuent.: %.2f", mont_indiv_desc);
            }
            else printf(" | Precio total: %.2f", (muestraprecio(combos_del_dia[i]) * comb_pedidos[i]));
        }
    }
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
    printf("\n     + Subtotal: %.2f", get_subtotal(ped_ing));
    if (get_consum_local(ped_ing) == 0) printf("\n     + Costo Delivery: %d", costo_delivery);
    printf("\n    -----------------------------------");
    printf("\n     - Total: %.2f", get_total(ped_ing));
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

int mod_estado_pedido(lista_pedidos *lista_ing, char id_pedido[], int estado_ing) // f-i
{
    pedido aux;

    int encontrado=-1;
    reset_lista(lista_ing);

    encontrado = buscar_x_idped(lista_ing, id_pedido);
    if (encontrado==1)
    {
        aux = copy_lista(*lista_ing);

        set_entregado(&aux, estado_ing);

        supress_lista(lista_ing);
        insert_lista(lista_ing, aux);

        return 0;
    }
    else return 1;
}

int mod_form_pago_pedido(lista_pedidos *lista_ing, char id_pedido[], int formpago_ing) // f-j
{
    pedido aux;

    int encontrado=-1;
    reset_lista(lista_ing);

    encontrado = buscar_x_idped(lista_ing, id_pedido);
    if (encontrado==1)
    {
        aux = copy_lista(*lista_ing);

        set_forma_pago(&aux, formpago_ing);

        supress_lista(lista_ing);
        insert_lista(lista_ing, aux);

        return 0;
    }
    else return 1;
}

int mod_nombre_pedido(lista_pedidos *lista_ing, char id_pedido[], char nomb[], char ape[]) // f-k
{
    pedido aux;

    int encontrado=-1;
    reset_lista(lista_ing);

    encontrado = buscar_x_idped(lista_ing, id_pedido);
    if (encontrado==1)
    {
        aux = copy_lista(*lista_ing);

        set_nombre(&aux, nomb, ape);

        supress_lista(lista_ing);
        insert_lista(lista_ing, aux);

        return 0;
    }
    else return 1;
}

void mod_precioystock_combo(int id_combo, float precio, int stock) // f-o
{
    cargaprecio(&combos_del_dia[id_combo], precio);
    cargastock(&combos_del_dia[id_combo], stock);
}

int mod_vend_id(int accion) // f-t //implementar en el main // 1=cargar, 2=sobrescrib.
{
    switch(accion)
    {
        case 1:{
            FILE *fp = fopen("vend_id.data", "r"); // capaz sea mejor abrirlo en w+
            if (fp == NULL) return 1;
            else fscanf(fp, "%d", &var_glob_vend_id);
            fclose(fp);
            break;
        }
        case 2:{
            FILE *fp = fopen("vend_id.data", "w");
            if (fp == NULL) return 1;
            else fprintf(fp, "%d", var_glob_vend_id);
            fclose(fp);
        }
    }
    return 0;
}
