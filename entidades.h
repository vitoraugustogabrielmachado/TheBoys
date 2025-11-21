#ifndef ENTIDADES_T
#define ENTIDADES_T

#include "conjunto.h"
#include "fila.h"
#include "fprio.h"



typedef struct heroi_t{
    int id_heroi; 
    int id_baseAtual;
    struct cjto_t *hab_pertencentes; 
    int experiencia; 
    int paciencia; 
    int velocidade; 
    int tempoHeroi;
    int status;
}heroi;

typedef struct missao_t{
    int id_missao; //aleatorio de 0 a n_missoes - 1
    int local[2]; //aleatorio de 0 a tamanho - 1
    struct cjto_t *hab_necessarias; //de 6 a 10 de forma aleatoria
    int tempoMissao;//precisa colocar o tempo na struct?
    int tentativas;
}missao;

typedef struct base_t{
    int id_base; //aleatorio de 0 a n_bases - 1
    int local[2]; //aleatorio de 0 a tamanho - 1 
    int lotacao; //aleatorio de 3 a 10 
    struct cjto_t *herois_presentes; //de inicio tem q estar vazio
    struct fila_t *fila_espera; //fila vazia de inicio
}base;

typedef struct mundo_t{
    int t_inicial; //fixado em 0
    int t_final; //fixado em 525600
    int tamanho; //fixado em 20000
    int n_habilidades; //fixado em 10
    int n_herois; //n_habilidades * 5
    heroi **HEROIS;
    int n_bases; //n_herois / 5
    base **BASES;
    int n_missoes; //t_final / 100
    missao **MISSOES;
    int n_compostosV; //n_habilidades * 3
    int relogio;
}mundo;

mundo *criaMundo();

missao *criaMissoes(mundo *M, struct fprio_t *LEF, int i);

heroi *criaHerois(mundo *M, struct fprio_t *LEF, int i);

base *criaBases(mundo *M, int i);

int aleat(int min, int max);

struct item_t *criaItem(int c, int t);

int inicializaEntidade(mundo *M, struct fprio_t *LEF);

void destruirEntidades(mundo *M); //ver isso dps se os parametros estao corretos

void destruirBases(base *bases);

void destruirHerois(heroi *herois);

void destruirMissoes(missao *missoes);

#endif
//talvez alterar o tempo dos eventos por referencia?
//problemas que eu tive: fila estava usando campos q nn tinham sido inicializados
// e os itens nn estavam sendo destruidos dentro da funcao fprio destroi
