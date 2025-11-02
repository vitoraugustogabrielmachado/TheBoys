#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conjunto.h"
#include "fila.h"
#include "fprio.h"
#include "entidades.h"

void inicializaMundo(mundo *M){
    M->t_inicial = 0;
    M->t_final = 525600;
    M->tamanho = 20000;
    M->n_habilidades = 10;
    M->n_herois = M->n_habilidades * 5;
    M->n_bases = M->n_herois / 5;
    M->n_missoes = M->t_final / 100;
    M->n_compostosV = M->n_habilidades * 3;
}

int aleat(int min, int max){
    return(min + rand() % (max - min + 1));
}

int idAleat(int min, int max){
    //desenvolver ela pra gerar aleatorios e conferir se ja foram usados
}

missao inicializaMissoes(missao *MI, mundo *M, struct frpio_t *LEF){
    MI->id_missao = aleat(0, M->n_missoes - 1); //VERIFICAR SE JA FOI REPETIDO
    MI->local[1] = aleat(0, M->tamanho - 1);
    MI->local[2] = aleat(0 ,M->tamanho - 1);
    MI->hab_necessarias = cjto_aleat(aleat(6, 10), M->n_habilidades);
    MI->tempoMissao = aleat(0, M->t_final);
    void *item; //isso eh temporario so pra conseguir utilizar a funçao
    int controle = fprio_insere(LEF, item, 1, MI->tempoMissao);
    if(!controle)
        return;
    //criar e inserir o evento missao(MI.tempoMissao, MI)
}

heroi inicializaHerois(heroi *H, mundo *M, struct fprio_t *LEF){
    H->id_heroi = aleat(0, M->n_herois - 1);
    H->hab_pertencentes = cjto_aleat(aleat(1, 3), M->n_habilidades);
    H->experiencia = 0;
    H->paciencia = aleat(0, 100);
    H->tempoHeroi = aleat(0, 4320);
    H->velocidade = aleat(50, 5000);
    H->baseLocal = aleat(0, M->n_bases);
    void *item; //isso eh temporario so pra conseguir utilizar a funçao
    int controle = fprio_insere(LEF, item, 2, H->tempoHeroi);
    if(!controle)
        return;
    //criar e inserir o evento chega(H.tempoHeroi, H, H.baseLocal)
}

base inicializaBases(base *B, mundo *M){
    B->id_base = aleat(0, M->n_bases - 1);
    B->local[1] = aleat(0, M->tamanho - 1);
    B->local[2] = aleat(0, M->tamanho - 1);
    B->lotacao = aleat(3, 10);
    B->herois_presentes = cjto_cria(B->lotacao);
    B->fila_espera = fila_cria();
}
//retorna -1 se falhou
//retorna 1 em caso de sucesso
int inicializaEntidade(mundo *M, struct fprio_t *LEF){
    int i;

    base *bases = malloc(M->n_bases * sizeof(base)); //tenho q liberar todos os mallocs
    if(!bases)
        return(-1);
    for(i = 0; i < M->n_bases; i++)
        bases[i] = inicializaBases(&bases[i], M);
    
    heroi *herois = malloc(M->n_herois * sizeof(heroi));
    if(!herois)
        return(-1);
    for(i = 0; i < M->n_herois; i++)
        herois[i] = inicializaHerois(&herois[i], M, LEF);
    
    missao *missoes = malloc(M->n_missoes * sizeof(missao));
    if(!missoes)
        return(-1);
    for(i = 0; i < M->n_missoes; i++)
        missoes[i] = inicializaMissoes(&missoes[i], M, LEF);
    return(1);
    //os eventos tem q ser do tipo fpnodo_t
    //ev = lef->tipo 
    //tempo = lef->prio
    //lef = lef->prox
    //switch ev e executa o evento 
    //ev = lef->tipo (= lef->prox-> tipo) e assim fica em loop
}