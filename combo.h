#ifndef COMBO_H_INCLUDED
#define COMBO_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <malloc.h>

#define descrip_size 30

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
