#include "fila.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct fila_t *fila_cria (){
    struct fila_t *aux = malloc(sizeof(struct fila_t));
    aux->prim = NULL;
    aux->ult = NULL;
    aux->num = 0;  
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
int fila_insere (struct fila_t *f, int item){
    if(!f)
        return(0);

    struct fila_nodo_t *aux = malloc(sizeof(struct fila_nodo_t));
    aux->item = item;
    aux->prox = NULL;
    if (!f->num){
        f->prim = aux;
        f->ult = aux;
    }else{
        f->ult->prox = aux;
        f->ult = aux;
    }
    f->num++;
    return(1);
}
int fila_retira (struct fila_t *f, int *item){
    if(!f || !f->num)
        return(0);
    struct fila_nodo_t *aux = f->prim;
    f->prim = aux->prox;
    *item = aux->item;
    free(aux);
    aux = NULL;
    f->num--;
    return(1);
        
}
int fila_tamanho (struct fila_t *f){
    if(!f)
        return(-1);
    return(f->num);
}
void fila_imprime (struct fila_t *f){
    struct fila_nodo_t *aux = f->prim;
    for(int i = 1; i < f->num; i++){
        printf("%d ", aux->item);
        aux = aux->prox;
    }
    printf("%d", aux->item);
}