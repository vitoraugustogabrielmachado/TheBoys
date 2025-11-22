// programa principal do projeto "The Boys - 2024/2"
// Autor: Vitor Augusto Gabriel Machado, GRR 20252066

// seus #includes vão aqui
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fprio.h"
#include "fila.h"
#include "conjunto.h"
#include "eventos.h"
#include "entidades.h"
// seus #defines vão aqui

// minimize o uso de variáveis globais
void imprimir_estatisticas(mundo *M){
    int i, taxaMissao = 0;
    for(i = 0; i < M->n_herois; i++){
        if(M->HEROIS[i]->status){
            printf("HEROI %d VIVO PAC %d VEL %d EXP %d HABS [", M->HEROIS[i]->id_heroi, M->HEROIS[i]->paciencia, M->HEROIS[i]->velocidade, M->HEROIS[i]->experiencia);
            cjto_imprime(M->HEROIS[i]->hab_pertencentes);
            printf("]\n");
        }
        else{
            printf("HEROI %d MORTO PAC %d VEL %d EXP %d HABS [", M->HEROIS[i]->id_heroi, M->HEROIS[i]->paciencia, M->HEROIS[i]->velocidade, M->HEROIS[i]->experiencia);
            cjto_imprime(M->HEROIS[i]->hab_pertencentes);
            printf("]\n");
        }
    }
    for(i = 0; i < M->n_bases; i++){
        printf("BASE %d LOT %d FILA MAX %d MISSOES %d\n", M->BASES[i]->id_base, M->BASES[i]->lotacao, M->BASES[i]->fila_max, M->BASES[i]->quantidadeMissoes);
        taxaMissao += M->BASES[i]->quantidadeMissoes;
    }
    printf("TAXA DE CONCLUSÃO DE MISSÕES: %d", taxaMissao/M->n_missoes);
}

// programa principal
int main (){
    mundo *M = criaMundo();
    struct fprio_t *LEF = fprio_cria(); 
    if(!LEF)
        return(0);
    if(!inicializaEntidade(M, LEF))
        return(0);
    evento *ev;
    int a, b, eventosTratados = 0; //ver um jeito de tirar esse a b
    while (((ev = fprio_retira(LEF, &a, &b)) != NULL) && (M->relogio < M->t_final)){
        M->relogio = ev->tempo;
        switch(ev->tipo){
            case CODIGOCHEGA:
                eventoChega(ev, LEF);
                break;

            case CODIGOESPERA:
                eventoEspera(ev, LEF);
                break;
            
            case CODIGODESISTE:
                eventoDesiste(ev, LEF);
                break;
              
            case CODIGOAVISA:
                eventoAvisa(ev, M, LEF);
                break;

            case CODIGOENTRA:
                eventoEntra(ev, LEF);
                break;

            case CODIGOSAI:
                eventoSai(ev, LEF);
                break;

            case CODIGOVIAJA:
                eventoViaja(ev, M, LEF);
                break;

            case CODIGOMORRE:   
                eventoMorre(ev, LEF);
                break;

            case CODIGOMISSAO:
                eventoMissao(ev, M, LEF);
                break;
        }
        free(ev);
        eventosTratados++;
}
    imprimir_estatisticas(M);
    printf("EVENTOS TRATADOS: %d\n", eventosTratados);
    free(ev);
    destruirEntidades(M);
    LEF = fprio_destroi(LEF);

    return (0) ;
}
