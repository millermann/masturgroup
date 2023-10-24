#ifndef PEDIDO_H_INCLUDED
#define PEDIDO_H_INCLUDED

#include <string.h>

#define strsize 25
#define num_combos 10

int vend_id_glob = 777; // ira main.c o aca?

typedef struct {int dia; int mes; int anio;} fecha;

typedef struct{
    char nomb[strsize];
    char ape[strsize];
    char pedido_id[strsize];
    int vend_id;
    int comb_pedidos[num_combos];
    int forma_pago;     // 1-deb, 2-cred, 3-QR, 4-efec
    float subtotal;     //total de todo lo q se vendio - el descuento
    int consum_local;   //1=si 0=no
    float total;        //subtotal + imp de consum local
    fecha fec_compra;
    int entregado;      //1-si, 0-no
}pedido;

void init_pedido(pedido *ped_ing) // hay q usar los setters dentro del init??
{
    int i;
    strcpy(ped_ing->nomb, "DEFAULT");
    strcpy(ped_ing->ape, "DEFAULT");
    strcpy(ped_ing->pedido_id, "DEFAULT");
    ped_ing->vend_id = vend_id_glob;

    for (i=0; i<=num_combos; i++) ped_ing->comb_pedidos[i] = 0;

    ped_ing->subtotal = 0;
    ped_ing->consum_local = 1;
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
    aux = ped_ing.nomb;
    return aux;
}

char *get_ape(pedido ped_ing)
{
    char *aux=(char*)malloc(sizeof(char)*strsize);
    aux = ped_ing.ape;
    return aux;
}
    // es necesaria funcion get del nombre completo??
int get_vend_id(pedido ped_ing)
{
    return ped_ing.vend_id;
}

int *get_comb_pedidos(pedido ped_ing)
{
    return ped_ing.comb_pedidos; //se necesita un aux como con los char?
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

void set_vend_id(pedido *ped_ing, int vend_id_ing) //    necesaria?
{
    ped_ing->vend_id = vend_id_ing;
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

void set_consum_local(pedido *ped_ing, int consum_local_ing)
{
    ped_ing->consum_local = consum_local_ing;
}

void set_total(pedido *ped_ing, float total_ing)
{
    ped_ing->total = total_ing;
}
    // puede ser util un get fecha completa
void set_fec_compra_dia(pedido *ped_ing, int dia_ing)
{
    ped_ing->fec_compra.dia = dia_ing;
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
#endif // PEDIDO_H_INCLUDED
