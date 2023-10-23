#ifndef COMBO_H_INCLUDED
#define COMBO_H_INCLUDED

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int id_combo;
    char Descripcion[30];
    int stock;
    float PrecioU;
    int descuento;
} Combo;

void cargaID(Combo *c,int ide)
{
    (*c).id_combo=ide;
}

int muestraid(Combo c)
{
    return c.id_combo;
}

void cargadescrip(Combo *c,char descr[])
{
    strcpy((*c).Descripcion,descr);
}

char *muestradescr(Combo c)
{
    return c.Descripcion;
}

void cargastock(Combo *c,int Stock)
{
    (*c).stock=Stock;
}

int muestrastock(Combo c)
{
    return c.stock;
}

void cargaprecio(Combo *c,float prec)
{
    (*c).PrecioU=prec;
}

float muestraprec(Combo c)
{
    return c.PrecioU;
}

void cargaprecio(Combo *c,int desc)
{
    (*c).descuento=desc;
}

int muestraprec(Combo c)
{
    return c.descuento;
}

#endif // COMBO_H_INCLUDE
