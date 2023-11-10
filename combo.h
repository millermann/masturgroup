#ifndef COMBO_H_INCLUDED
#define COMBO_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include "pedido.h"

#define descrip_size 100
#define val_descuento 15
typedef struct{
    int id_combo;
    char descripcion[descrip_size];
    int stock;
    float preciounit;
    int descuento;
} combo;

void cargaid(combo *comb_ing, int ide)
{
    (*comb_ing).id_combo=ide;
}

int muestraid(combo comb_ing)
{
    return comb_ing.id_combo;
}

void cargadescripcion(combo *comb_ing, char descr[])
{
    strcpy((*comb_ing).descripcion, descr);
}

char *muestradescripcion(combo comb_ing)
{
    char *aux = (char*)malloc(sizeof(char)*descrip_size);
    strcpy(aux, comb_ing.descripcion);
    return aux;
}

void cargastock(combo *comb_ing, int Stock)
{
    (*comb_ing).stock=Stock;
}

int muestrastock(combo comb_ing)
{
    return comb_ing.stock;
}

int get_combo_stock(combo combos[], int id_combo)
{
    return combos[id_combo].stock;
}

void cargaprecio(combo *comb_ing, float prec)
{
    (*comb_ing).preciounit=prec;
}

float muestraprecio(combo comb_ing)
{
    return comb_ing.preciounit;
}

void cargadescuento(combo *comb_ing, int desc)
{
    (*comb_ing).descuento = desc;
}

int muestradescuento(combo comb_ing)
{
    return comb_ing.descuento;
}

float calcular_subtotal_combos(combo combos_ing[], int comb_pedidos[], int cupon_descuento)
{
    int i; float monto_indiv, suma_total=0;

    for (i=0; i<num_combos; i++)
    {
        monto_indiv = 0;

        monto_indiv = ((combos_ing[i].preciounit) * (comb_pedidos[i]));
        if (cupon_descuento == 1){
            if (combos_ing[i].descuento == 1){
                monto_indiv -= ((val_descuento*monto_indiv)/100);
            }
        }
        suma_total += monto_indiv;
    }
    return suma_total;
}

int actualizar_combos_stock(combo menu_combos[], int comb_pedidos[]){
    int i, stock_actual=0;

    for (i=0; i<num_combos; i++){ // controla q en todas las unidades pedidos < stock del combo
        if ((muestrastock(menu_combos[i]) - comb_pedidos[i])<0) return 1;
    }

    for (i=0; i<num_combos; i++){
        stock_actual = (muestrastock(menu_combos[i]) - comb_pedidos[i]);
        cargastock(&menu_combos[i], stock_actual);
    }
    return 0;
}
#endif // COMBO_H_INCLUDE
