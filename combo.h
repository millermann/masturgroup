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

void init_combos_default(combo menu_combos[]){
    int i;
    for (i=0; i<num_combos; i++){
        menu_combos[i].id_combo = i;
        strcpy(menu_combos[i].descripcion, "DEFAULT");
        menu_combos[i].stock = 0;
        menu_combos[i].preciounit = 0;
        menu_combos[i].descuento = 0;
    }
}

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




#endif // COMBO_H_INCLUDE
