#include "fprio.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct fprio_t *fprio_cria (){
  struct fprio_t *aux = malloc(sizeof(struct fprio_t));
  if(!aux)
    return(NULL);
  memset(aux, 0, sizeof(struct fprio_t));
  return(aux);
}

int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio){
  if(!f || !item)
    return(-1);
  struct fpnodo_t *atual = f->prim;
  while(atual){ //procura se ja existe o item
    if(item == atual->item)
      return(-1);
    atual = atual->prox;
  }

  struct fpnodo_t *ant = NULL;
  atual = f->prim;
  while(atual && atual->prio <= prio){
    ant = atual;
    atual = atual->prox;
  }
  struct fpnodo_t *inse = malloc(sizeof(struct fpnodo_t));
  if(!inse)
    return(-1);
  inse->item = item;
  inse->prio = prio;
  inse->tipo = tipo;

  if(!fprio_tamanho(f)){ //se for lista vazia
    f->prim = inse;
    inse->prox = NULL;
    return(f->num++);
  }
  if(!atual){ //se a prio inserida for a maior de todas
    ant->prox = inse;
    inse->prox = NULL;
    return(f->num++);
  }
  if(!ant){ //se a prio do primeiro for maior q a prio inserida
    inse->prox = f->prim;
    f->prim = inse;
    return(f->num++);
  }
  inse->prox = atual;
  ant->prox = inse;
  return(f->num++);
}

int fprio_tamanho (struct fprio_t *f){
  if (!f || f->num < 0)
    return(-1);
  return(f->num);
}

void fprio_imprime (struct fprio_t *f){
  struct fpnodo_t *aux = f->prim;
  if(!fprio_tamanho(f))
    return;
  for(int i = 1; i < f->num; i++){
    printf("(%d %d) ", aux->tipo, aux->prio);
    aux = aux->prox;
  }
  printf("(%d %d)", aux->tipo, aux->prio); //imprime o ultimo elemento
}

void *fprio_retira (struct fprio_t *f, int *tipo, int *prio){
  if(!f || fprio_tamanho(f) == 0 || !tipo || !prio) //testa todos os casos de serem nulls
    return(NULL); 
  struct fpnodo_t *aux = f->prim;
  f->prim = aux->prox; //retira sempre o primeiro
  *tipo = aux->tipo;
  *prio = aux->prio;
  f->num--;
  void *item = aux->item; // salva antes de liberar
  free(aux);
  return (item);
}

struct fprio_t *fprio_destroi (struct fprio_t *f){
  struct fpnodo_t *atual = f->prim;
  while(atual){
    struct fpnodo_t *aux = atual->prox;
    free(atual->item);
    free(atual);
    atual = aux;
  }
  free(f);
  f = NULL;
  return(NULL);
}