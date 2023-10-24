#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <malloc.h>
#include "pedido.h"

//todavia no se si andan

typedef struct nod{
    pedido vipd;
    struct nod *next;
};

typedef struct nod nodo;

typedef struct{
    nodo *acceso;
    nodo *cur;
    nodo *aux;
}lista_pedidos;

int isOos(lista_pedidos lista_ing)
{
    if (lista_ing.cur==NULL) return 1;
    else return 0;
}

int isEmpty(lista_pedidos lista_ing)
{
    if (lista_ing.acceso == NULL) return 1;
    else return 0;
}

int isFull(void)
{
    if (malloc(sizeof(nodo))==NULL) return 1;
    else return 0;
}

void init_lista(lista_pedidos *lista)
{
    lista->acceso = NULL;
    lista->cur = NULL;
    lista->aux = NULL;
}

void reset_lista(lista_pedidos *lista)
{
    lista->cur = lista->acceso;
    lista->aux = lista->acceso;
}

void insert_lista(lista_pedidos *lista, pedido pedido_ing)
{
    nodo *nuevo_nodo = (nodo*)malloc(sizeof(nodo));
    copy_pedido(&nuevo_nodo->vipd, pedido_ing);

    if (isEmpty(*lista) == 1 && isOos(*lista) == 1)
    {
        lista->acceso = nuevo_nodo;
        lista->cur = nuevo_nodo;
    }
    else
    {
        if (isOos(*lista) == 1)
        {
            lista->aux->next = nuevo_nodo;
            lista->cur = nuevo_nodo;
        }
        else
        {
            lista->aux->next = nuevo_nodo;
            nuevo_nodo->next = lista->cur;
            lista->cur = nuevo_nodo;
        }
    }
}

pedido *copy_lista(lista_pedidos lista)
{
    return &lista.cur->vipd;
}

void supress_lista(lista_pedidos *lista)
{
    if (lista->acceso == lista->cur) //equiv a aux == NULL
    {
        free((void*)lista->cur);
        free((void*)lista->acceso);
        lista->cur = NULL;
        lista->acceso = NULL;
    }
    else
    {
        lista->aux->next = lista->cur->next;
        free((void*)lista->cur);
        lista->cur = lista->aux->next;
    }
}

void forward_lista(lista_pedidos *lista)
{
    lista->aux = lista->cur;
    lista->cur = lista->aux->next;
}

#endif // LISTA_H_INCLUDED
