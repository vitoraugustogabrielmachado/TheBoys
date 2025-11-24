#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "eventos.h"
#include "entidades.h"
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"

void imprimir_estatisticas(mundo *M){
    int i;
    float porcentual, taxaMissao = 0.0;
    printf("**************************************************************************\n");
    printf("                     ESTATISTICAS DO FIM DO MUNDO\n");
    printf("**************************************************************************\n\n");
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
    printf("\n");
    for(i = 0; i < M->n_bases; i++){
        printf("BASE %d LOT %d FILA MAX %d MISSOES %d\n", M->BASES[i]->id_base, M->BASES[i]->lotacao, M->BASES[i]->fila_max, M->BASES[i]->quantidadeMissoes);
        taxaMissao += M->BASES[i]->quantidadeMissoes;
    }
    printf("\n");
    porcentual = (taxaMissao/M->n_missoes) * 100;
    printf("TAXA DE CONCLUSÃO DE MISSÕES: %.0f/%d (%.1f%%)\n", taxaMissao, M->n_missoes, porcentual);

    int tentativaMax = 1;
    float mediaTentativas = 0.0;
    for(i = 0; i < M->n_missoes; i++){
        mediaTentativas += M->MISSOES[i]->tentativas;
        if(M->MISSOES[i]->tentativas > tentativaMax)
            tentativaMax = M->MISSOES[i]->tentativas;
    }
    mediaTentativas /= M->n_missoes;
    printf("TENTATIVAS/MISSAO: MIN 1, MAX %d, MEDIA %.1f\n", tentativaMax, mediaTentativas);
    
    float qntdMortos = 0.0;
    float taxaMortalidade;      
    for(i = 0; i < M->n_herois; i++){
        if(M->HEROIS[i]->status == 0)
            qntdMortos += 1.0;
    }
    taxaMortalidade = qntdMortos/M->n_herois;
    printf("TAXA DE MORTALIDADE %.1f%%\n", taxaMortalidade * 100);
}

void simulacao(mundo *M, struct fprio_t *LEF){
    evento *ev;
    int a, b, eventosTratados = 0; //ver um jeito de tirar esse a b
    while (((ev = fprio_retira(LEF, &a, &b)) != NULL)){
        M->relogio = ev->tempo;
        if(M->relogio >= M->t_final)
            break;
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
    printf("%6d: FIM DO MUNDO\n\n", M->relogio);
    imprimir_estatisticas(M);
    printf("EVENTOS TRATADOS: %d\n", eventosTratados);
    free(ev);
}