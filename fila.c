#include "fila.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct fila_t *fila_cria (){
    struct fila_t *aux = malloc(sizeof(struct fila_t));
    if(!aux)
        return(NULL);
    return(aux);
}
struct fila_t *fila_destroi (struct fila_t *f){
    struct fila_nodo_t *atual = f->prim;
    while(atual){
        struct fila_nodo_t *aux = atual->prox;
        free(atual);
        atual = aux;
    }
    free(f);
    f = NULL;
    return(NULL); //SE EU CRIAR O ITEM POR MALLOC EU VOU PRECISAR DESTRUIR O ITEM TBM
}
int fila_insere (struct fila_t *f, void *item){
    if(!f || !item)
        return(-1);
    struct fila_nodo_t *atual = f->prim;
    while(atual){ //procura se ja existe o item
        if(item == atual->item)
        return(-1);
        atual = atual->prox;
    }
    struct fila_nodo_t *aux = malloc(sizeof(struct fila_nodo_t));
    aux->item = item;
    aux->prox = NULL;
    f->fim->prox = aux;
    f->fim = aux;
    return(f->num++);
}
void *fila_retira (struct fila_t *f){
    if(!f || !f->num)
        return(NULL);
    struct fila_nodo_t *aux = f->prim;
    f->prim = aux->prox;
    void *it = aux->item;
    free(aux);
    aux = NULL;
    return(it);
        
}
int fila_tamanho (struct fila_t *f){
    if(!f)
        return(-1);
    return(f->num);
}
void fila_imprime (struct fila_t *f);