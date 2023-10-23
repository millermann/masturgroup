#ifndef PEDIDO_H_INCLUDED
#define PEDIDO_H_INCLUDED

#define strsize 25

int vend_id_glob = 777; // var.global

typedef struct {int dia; int mes; int anio}; fecha

typedef struct{
    char nomb[strsize];
    char ape[strsize];
    char pedido_id[strsize];
    int vend_id;
    int comb_pedido[10];
    int forma_pago;    // 1-deb, 2-cred, 3-QR, 4-efec
    float subtotal;
    int consum_local;
    float total;
    fecha
}

#endif // PEDIDO_H_INCLUDED
