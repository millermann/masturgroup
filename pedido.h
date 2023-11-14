#ifndef PEDIDO_H_INCLUDED
#define PEDIDO_H_INCLUDED

#include <string.h>
#include <time.h>

#define strsize 25
#define idsize 8
#define num_combos 10
#define costo_delivery 500

typedef struct {int dia; int mes; int anio;} fecha;

typedef struct{
    char pedido_id[idsize];
    int vend_id;
    char nomb[strsize];
    char ape[strsize];
    int comb_pedidos[num_combos];
    int consum_local;           //1=si 0=no
    int cup_descuento;          //1=si
    float subtotal;             //total precio*unidad - el descuento
    float total;                //subtotal + imp de delivery
    int forma_pago;             // 1-deb, 2-cred, 3-QR, 4-efec
    fecha fec_compra;           //no es un tda, se puede usar sin getters/setters en el main (dia, mes, anio)
    int entregado;              //1-si, 0-no
}pedido;

void init_pedido(pedido *ped_ing)
{
    int i;
    strcpy(ped_ing->nomb, "DEFAULT");
    strcpy(ped_ing->ape, "DEFAULT");
    strcpy(ped_ing->pedido_id, "DEFAULT");
    ped_ing->vend_id = 0;

    for (i=0; i<=num_combos; i++) //por alguna extraña razon cuando num_comb = 9 siempre comb_pedidos[9]==1
    {
        ped_ing->comb_pedidos[i] = 0;
    }
    ped_ing->subtotal = 0;
    ped_ing->consum_local = 1;
    ped_ing->cup_descuento = 0;
    ped_ing->total = 0;
    ped_ing->fec_compra.dia = 0;
    ped_ing->fec_compra.mes = 0;
    ped_ing->fec_compra.anio = 0;
    ped_ing->entregado = 0;
}
/* GETTERS */
char *get_nomb(pedido ped_ing)
{
    char *aux=(char*)malloc(sizeof(char)*strsize);
    strcpy(aux, ped_ing.nomb);
    return aux;
}

char *get_ape(pedido ped_ing)
{
    char *aux=(char*)malloc(sizeof(char)*strsize);
    strcpy(aux, ped_ing.ape);
    return aux;
}
    // es necesaria funcion get del nombre completo??
char *get_pedido_id(pedido ped_ing)
{
    char *aux = (char*)malloc(sizeof(char)*idsize);
    strcpy(aux, ped_ing.pedido_id);
    aux[idsize]='\0';
    return aux;
}

int get_vend_id(pedido ped_ing)
{
    return ped_ing.vend_id;
}

int *get_comb_pedidos(pedido ped_ing)
{
    int *aux = (int*)malloc(sizeof(int)*num_combos);
    memcpy(aux, ped_ing.comb_pedidos, sizeof(int)*num_combos); // clona arrays
    return aux;
}

int get_forma_pago(pedido ped_ing)
{
    return ped_ing.forma_pago;
}

float get_subtotal(pedido ped_ing)
{
    return ped_ing.subtotal;
}

int get_consum_local(pedido ped_ing)
{
    return ped_ing.consum_local;
}

int get_cup_descuento(pedido ped_ing)
{
    return ped_ing.cup_descuento;
}

float get_total(pedido ped_ing)
{
    return ped_ing.total;
}

int get_fec_compra_dia(pedido ped_ing)
{
    return ped_ing.fec_compra.dia;
}

int get_fec_compra_mes(pedido ped_ing)
{
    return ped_ing.fec_compra.mes;
}

int get_fec_compra_anio(pedido ped_ing)
{
    return ped_ing.fec_compra.anio;
}

int get_entregado(pedido ped_ing)
{
    return ped_ing.entregado;
}
/* SETTERS */
void set_nombre(pedido *ped_ing, char nomb_ing[], char ape_ing[])
{
    strcpy(ped_ing->nomb, nomb_ing);
    strcpy(ped_ing->ape, ape_ing);
}

void set_pedido_id(pedido *ped_ing, char pedido_id_ing[])
{
    strcpy(ped_ing->pedido_id, pedido_id_ing);
}

void set_comb_pedidos(pedido *ped_ing, int tipo_combo, int cant_ing)
{
    ped_ing->comb_pedidos[tipo_combo] = cant_ing;
}

void set_forma_pago(pedido *ped_ing, int forma_pago_ing)
{
    ped_ing->forma_pago = forma_pago_ing;
}

void set_subtotal(pedido *ped_ing, float subtotal_ing)
{
    ped_ing->subtotal = subtotal_ing;
}

void set_cup_descuento(pedido *ped_ing, int cup_ing)
{
    ped_ing->cup_descuento = cup_ing;
}

void set_consum_local(pedido *ped_ing, int consum_local_ing)
{
    ped_ing->consum_local = consum_local_ing;
}

void set_total(pedido *ped_ing, float total_ing)
{
    ped_ing->total = total_ing;
}
    // puede ser util un get/set fecha completa
void set_fec_compra_dia(pedido *ped_ing, int dia_ing)
{
    ped_ing->fec_compra.dia = dia_ing;
}

void set_vend_id(pedido *ped_ing, int vend_id_ing)
{
    ped_ing->vend_id = vend_id_ing;
}

void set_fec_compra_mes(pedido *ped_ing, int mes_ing)
{
    ped_ing->fec_compra.mes = mes_ing;
}

void set_fec_compra_anio(pedido *ped_ing, int anio_ing)
{
    ped_ing->fec_compra.anio = anio_ing;
}

void set_entregado(pedido *ped_ing, int entregado_ing)
{
    ped_ing->entregado = entregado_ing;
}

void copy_pedido(pedido *ped1, pedido ped2) // fun. adic.
{
    *ped1=ped2;
}

#endif // PEDIDO_H_INCLUDED
