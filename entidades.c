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


mundo *criaMundo(){ //o tipo dele eh diferente pq ele ta no programa principal
    mundo *M = malloc(sizeof(mundo));
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
    //colocar o vetor de bases. heróis e missões aq
    base **BASES = malloc(M->n_bases * sizeof(base*));
    if (!BASES)
        abort();
    missao **MISSOES = malloc(M->n_missoes * sizeof(missao*));
    if(!MISSOES)
        abort();
    heroi **HEROIS = malloc(M->n_herois * sizeof(heroi*));
    if(!HEROIS)
        abort();
    M->BASES = BASES;
    M->HEROIS = HEROIS;
    M->MISSOES = MISSOES;
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
    MI->local[0] = aleat(0, M->tamanho - 1);
    MI->local[1] = aleat(0 ,M->tamanho - 1);
    MI->hab_necessarias = cjto_aleat(aleat(6, 10), M->n_habilidades);
    MI->tempoMissao = aleat(0, M->t_final);
    evento *ev = criaEvento(CODIGOMISSAO, MI->tempoMissao, NULL, NULL, MI);
    if(fprio_insere(LEF, ev, ev->tipo, ev->tempo) == -1)
        abort();
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
    H->status = 1;
    //talvez seja isso? pq nn faz sentido ele ter base por enquanto ate ele chegar em alguma
    base *destino = M->BASES[aleat(0, M->n_bases - 1)];

    evento *ev = criaEvento(CODIGOCHEGA, H->tempoHeroi, H, destino, NULL);
     //perguntar se precisa disso explicar que
    //essa ideia veio do fato do evento desiste/otro precisa q o heroi va para outra base
    //mas agora que eu fiz o vetor de bases e herois no mundo, nao daria pra resolver o problema
    //do evento com o mundo?
    //liberar isso, como todos os eventos vao ter itens com eles 
     //o define nn devia passar para ca?
    if(fprio_insere(LEF, ev, ev->tipo, ev->tempo) == -1)
        abort();
    return(H);
    //criar e inserir o evento chega(H.tempoHeroi, H, H.baseLocal)
}

base *criaBases(mundo *M, int i){
    base *B = malloc(sizeof(base)); //liberar esse maloc
    if(!B)
        abort();
    B->id_base = i;
    B->local[0] = aleat(0, M->tamanho - 1);
    B->local[1] = aleat(0, M->tamanho - 1);
    B->lotacao = aleat(3, 10);
    B->herois_presentes = cjto_cria(B->lotacao);
    B->fila_espera = fila_cria();
    return(B);
}

//retorna -1 se falhou, talvez mudar o tipo de retorno
//retorna 1 em caso de sucesso
int inicializaEntidade(mundo *M, struct fprio_t *LEF){
    int i;
    for(i = 0; i < M->n_bases; i++)
        M->BASES[i] = criaBases(M, i);
    
    
    for(i = 0; i < M->n_herois; i++)
        M->HEROIS[i] = criaHerois(M, LEF, i);
    
    for(i = 0; i < M->n_missoes; i++)
        M->MISSOES[i] = criaMissoes(M, LEF, i);

    return(1);
    //os eventos tem q ser do tipo fpnodo_t
    //ev = lef->tipo 
    //tempo = lef->prio
    //lef = lef->prox
    //switch ev e executa o evento 
    //ev = lef->tipo (= lef->prox-> tipo) e assim fica em loop
}

void destruirMissoes(missao *MI){
    MI->hab_necessarias = cjto_destroi(MI->hab_necessarias);
    free(MI);
    MI = NULL;
}

void destruirBases(base *B){    
    B->fila_espera = fila_destroi(B->fila_espera);
    B->herois_presentes = cjto_destroi(B->herois_presentes);
    free(B);
    B = NULL;
}

void destruirHerois(heroi *H){
    H->hab_pertencentes = cjto_destroi(H->hab_pertencentes);
    free(H);
    H = NULL;
}

void destruirEntidades(mundo *M){
    int i;

    for(i = 0; i < M->n_missoes; i++)
        destruirMissoes(M->MISSOES[i]);

    for(i = 0; i < M->n_bases; i++)
        destruirBases(M->BASES[i]);
    
    for(i = 0; i < M->n_herois; i++)
        destruirHerois(M->HEROIS[i]);
    
    free(M->BASES);
    free(M->HEROIS);
    free(M->MISSOES);
    M->BASES = NULL;
    M->HEROIS = NULL;
    M->MISSOES = NULL;

    free(M);
    M = NULL;
} 
//destruir as coisas primeiro, depois pensar nos eventos