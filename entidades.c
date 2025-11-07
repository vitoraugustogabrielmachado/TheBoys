#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conjunto.h"
#include "fila.h"
#include "fprio.h"
#include "entidades.h"
#include "eventos.h"

#define TEMPOMAX 525600
#define TAMANHOMAX 20000



struct item_t *criaItem(int c, int t){
    struct item_t *item = malloc(sizeof(struct item_t));
    if(!item)
        abort();
    item->codigoEvento = c;
    item->tempoEvento = t;
    return(item);
}

void destroiItem(struct item_t *I){

}

mundo *criaMundo(){
    mundo *M = malloc(sizeof(missao));
    if(!M)
        abort();
    M->t_inicial = 0;
    M->t_final = TEMPOMAX;
    M->tamanho = TAMANHOMAX;
    M->n_habilidades = 10;
    M->n_herois = M->n_habilidades * 5;
    M->n_bases = M->n_herois / 5;
    M->n_missoes = M->t_final / 100;
    M->n_compostosV = M->n_habilidades * 3;
    M->relogio = M->t_inicial;
    return(M);
}

int aleat(int min, int max){
    return(min + rand() % (max - min + 1));
}

missao *criaMissoes(mundo *M, struct fprio_t *LEF, int i){
    missao *MI = malloc(sizeof(missao)); //liberar esse maloc
    if(!MI)
        abort();
    MI->id_missao = i; 
    MI->local[1] = aleat(0, M->tamanho - 1);
    MI->local[2] = aleat(0 ,M->tamanho - 1);
    MI->hab_necessarias = cjto_aleat(aleat(6, 10), M->n_habilidades);
    MI->tempoMissao = aleat(0, M->t_final);
    struct item_t *item = item_cria(CODIGOMISSAO, MI->tempoMissao); 
    int controle = fprio_insere(LEF, item, item->codigoEvento, item->tempoEvento);
    if(!controle)
        return;
    return(MI);
    //criar e inserir o evento missao(MI.tempoMissao, MI)
}

heroi *criaHerois(mundo *M, struct fprio_t *LEF, int i){
    heroi *H = malloc(sizeof(heroi)); //liberar esse maloc
    if(!H)
        abort();
    H->id_heroi = i;
    H->hab_pertencentes = cjto_aleat(aleat(1, 3), M->n_habilidades);
    H->experiencia = 0;
    H->paciencia = aleat(0, 100);
    H->tempoHeroi = aleat(0, 4320);
    H->velocidade = aleat(50, 5000);
    H->baseLocal = -1; //talvez seja isso? pq nn faz sentido ele ter base por enquanto ate ele chegar em alguma
    struct item_t *item = item_cria(CODIGOCHEGA, H->tempoHeroi); 
    item->h = H;

    base *destino = M->bases[aleat(0, M->n_bases - 1)];
    item->b = destino;
    //liberar isso, como todos os eventos vao ter itens com eles 
    int controle = fprio_insere(LEF, item, item->codigoEvento, item->tempoEvento); //o define nn devia passar para ca?
    if(!controle)
        return;
    return(H);
    //criar e inserir o evento chega(H.tempoHeroi, H, H.baseLocal)
}

base *criaBases(mundo *M, int i){
    base *B = malloc(sizeof(base)); //liberar esse maloc
    if(!B)
        abort();
    B->id_base = i;
    B->local[1] = aleat(0, M->tamanho - 1);
    B->local[2] = aleat(0, M->tamanho - 1);
    B->lotacao = aleat(3, 10);
    B->herois_presentes = cjto_cria(B->lotacao);
    B->fila_espera = fila_cria();
    return(B);
}

//retorna -1 se falhou
//retorna 1 em caso de sucesso
int inicializaEntidade(mundo *M, struct fprio_t *LEF){
    int i;
    
    base **bases = malloc(M->n_bases * sizeof(base*)); //tenho q liberar todos os mallocs
    if(!bases)
        return(-1);
    for(i = 0; i < M->n_bases; i++)
        bases[i] = criaBases(M, i);
    
    heroi **herois = malloc(M->n_herois * sizeof(heroi*));
    if(!herois)
        return(-1);
    for(i = 0; i < M->n_herois; i++)
        herois[i] = criaHerois(M, LEF, i);
    
    missao **missoes = malloc(M->n_missoes * sizeof(missao*));
    if(!missoes)
        return(-1);
    for(i = 0; i < M->n_missoes; i++)
        missoes[i] = criaMissoes(M, LEF, i); 
    return(1);
    //os eventos tem q ser do tipo fpnodo_t
    //ev = lef->tipo 
    //tempo = lef->prio
    //lef = lef->prox
    //switch ev e executa o evento 
    //ev = lef->tipo (= lef->prox-> tipo) e assim fica em loop
}