#ifndef ENTIDADES_T
#define ENTIDADES_T

#include "conjunto.h"
#include "fila.h"
#include "fprio.h"

typedef struct heroi_t{
    int id_heroi; 
    struct cjto_t *hab_pertencentes; 
    int experiencia; 
    int paciencia; 
    int velocidade; 
    int tempoHeroi;
    int baseLocal;
}heroi;

typedef struct mundo_t{
    int t_inicial; //fixado em 0
    int t_final; //fixado em 525600
    int tamanho; //fixado em 20000
    int n_habilidades; //fixado em 10
    int n_herois; //n_habilidades * 5
    int n_bases; //n_herois / 5
    int n_missoes; //t_final / 100
    int n_compostosV; //n_habilidades * 3
}mundo;

typedef struct missao_t{
    int id_missao; //aleatorio de 0 a n_missoes - 1
    int local[2]; //aleatorio de 0 a tamanho - 1
    struct cjto_t *hab_necessarias; //de 6 a 10 de forma aleatoria
    int tempoMissao;//precisa colocar o tempo na struct?
}missao;

typedef struct base_t{
    int id_base; //aleatorio de 0 a n_bases - 1
    int local[2]; //aleatorio de 0 a tamanho - 1 
    int lotacao; //aleatorio de 3 a 10 
    struct cjto_t *herois_presentes; //de inicio tem q estar vazio
    struct fila_t *fila_espera; //fila vazia de inicio
}base;

void inicializaMundo(mundo *M);

missao inicializaMissoes(missao *MI, mundo *M, struct fprio_t *LEF);

heroi inicializaHerois(heroi *H, mundo *M, struct fprio_t *LEF);

base inicializaBases(base *B, mundo *M);

int inicializaEntidade(mundo *M, struct fprio_t *LEF);


//talvez alterar o tempo dos eventos por referencia?

#endif