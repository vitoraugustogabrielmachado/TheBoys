#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "eventos.h"
#include "entidades.h"
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"

evento *criaEvento(int tipo, int tempo, heroi *h, base *b, missao *mi){
    evento *ev = malloc(sizeof(evento));
    if(!ev)
        abort();
    ev->tipo = tipo;
    ev->tempo = tempo;
    ev->h = h;
    ev->b = b;
    ev->mi = mi;
    return(ev);
}

void eventoChega(evento *ev, struct fprio_t *LEF){ //quando tiver terminado ver se nn fica melhor mudar o nome das variaveis pra nn repetir mt
    int espera;

    if(ev->h->status){
        if(ev->b->lotacao == cjto_card(ev->b->herois_presentes) && fila_tamanho(ev->b->fila_espera) == 0)
            espera = 1;
        else 
            espera = ((ev->h->paciencia) > (10 * fila_tamanho(ev->b->fila_espera))); //isso ta certo? nn precisa de um parenteses por fora?

        if (espera){
            printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n", ev->tempo, ev->h->id_heroi, ev->b->id_base, cjto_card(ev->b->herois_presentes), ev->b->lotacao);
            evento *novoEv = criaEvento(CODIGOESPERA, ev->tempo, ev->h, ev->b, ev->mi);
            fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);
        }else{
            printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n", ev->tempo, ev->h->id_heroi, ev->b->id_base, cjto_card(ev->b->herois_presentes), ev->b->lotacao);
            evento *novoEv = criaEvento(CODIGODESISTE, ev->tempo, ev->h, ev->b, ev->mi);
            fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);
        }
    }
    
}

void eventoEspera(evento *ev, struct fprio_t *LEF){
    if(ev->h->status){
        printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", ev->tempo, ev->h->id_heroi, ev->b->id_base, fila_tamanho(ev->b->fila_espera));
        fila_insere(ev->b->fila_espera, ev->h->id_heroi); // talvez int tam = isso sirva pra algo dp
        evento *novoEv = criaEvento(CODIGOAVISA, ev->tempo, ev->h, ev->b, ev->mi);
        fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);
    }
}

void eventoDesiste(evento *ev, struct fprio_t *LEF){
    if(ev->h->status){
        printf("%6d: DESISTE HEROI %2d BASE %d\n", ev->tempo, ev->h->id_heroi, ev->b->id_base);
        evento *novoEv = criaEvento(CODIGOVIAJA, ev->tempo, ev->h, ev->b, ev->mi);
        fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);
    }
}

void eventoAvisa(evento *ev, mundo *M, struct fprio_t *LEF){
    int idHeroiRetirado;
    printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA ", ev->tempo, ev->b->id_base, cjto_card(ev->b->herois_presentes), ev->b->lotacao);
    printf("[ ");
    if(ev->b->fila_espera && fila_tamanho(ev->b->fila_espera))
        fila_imprime(ev->b->fila_espera);
    printf(" ]\n");

    while((ev->b->lotacao - cjto_card(ev->b->herois_presentes) != 0) && fila_tamanho(ev->b->fila_espera) > 0){
        fila_retira(ev->b->fila_espera, &idHeroiRetirado);
        heroi *h = M->HEROIS[idHeroiRetirado];
        cjto_insere(ev->b->herois_presentes, h->id_heroi);

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", ev->tempo, ev->b->id_base, h->id_heroi);

        evento *novoEv = criaEvento(CODIGOENTRA, ev->tempo, h, ev->b, ev->mi);
        fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);
    }
}

void eventoEntra(evento *ev, struct fprio_t *LEF){
    if(ev->h->status){
        int TPB = 15 + ev->h->paciencia * aleat(1, 20); 
        evento *novoEv = criaEvento(CODIGOSAI, ev->tempo + TPB, ev->h, ev->b, ev->mi);
        printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %6d\n", ev->tempo, ev->h->id_heroi, ev->b->id_base, cjto_card(ev->b->herois_presentes), ev->b->lotacao, novoEv->tempo);
        fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);
    }
}

void eventoSai(evento *ev, struct fprio_t *LEF){
    if(ev->h->status){
        if(cjto_retira(ev->b->herois_presentes, ev->h->id_heroi) == -1)
            abort();

        printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", ev->tempo, ev->h->id_heroi, ev->b->id_base, cjto_card(ev->b->herois_presentes), ev->b->lotacao);

        evento *novoEv = criaEvento(CODIGOVIAJA, ev->tempo, ev->h, ev->b, ev->mi);
        fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);

        evento *novoEv2 = criaEvento(CODIGOAVISA, ev->tempo, ev->h, ev->b, ev->mi);
        fprio_insere(LEF, novoEv2, novoEv2->tipo, novoEv2->tempo);
    }
}

void eventoViaja(evento *ev, mundo *M, struct fprio_t *LEF){
    if(ev->h->status){
        base *destino;
        destino = M->BASES[aleat(0, M->n_bases - 1)];
        
        int distancia_1 = (ev->b->local[0] - destino->local[0]) * (ev->b->local[0] - destino->local[0]);
        int distancia_2 = (ev->b->local[1] - destino->local[1]) * (ev->b->local[1] - destino->local[1]);
        int distanciaTotal = sqrt(distancia_1 + distancia_2);
        int duracao = distanciaTotal/ ev->h->velocidade;

        evento *novoEv = criaEvento(CODIGOCHEGA, ev->tempo + duracao, ev->h, destino, ev->mi);
        fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);

        printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", ev->tempo, novoEv->h->id_heroi, ev->b->id_base, novoEv->b->id_base, distanciaTotal, novoEv->h->velocidade, novoEv->tempo);
    }
}

void eventoMorre(evento *ev, struct fprio_t *LEF){
    if(!cjto_retira(ev->b->herois_presentes, ev->h->id_heroi))
        abort();
    ev->h->status = 0;

    evento *novoEv = criaEvento(CODIGOAVISA, ev->tempo, ev->h, ev->b, ev->mi);
    fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);
}

int dist(evento *ev, base *b){
    int distancia_1 = (b->local[0] - ev->mi->local[0]) * (b->local[0] - ev->mi->local[0]);
    int distancia_2 = (b->local[1] - ev->mi->local[1]) * (b->local[1] - ev->mi->local[1]);
    return(sqrt(distancia_1 + distancia_2));
}



void eventoMissao(evento *ev, mundo *M, struct fprio_t *LEF){
    int i, menor = __INT_MAX__, distBaseMissao[M->n_bases];
    interseccao = cjto_cria();
    for(i = 0; i < M->n_bases; i++){
        distBaseMissao[i] = dist(ev, M->BASES[i]);
    } //coloca no vetor todas as distancias possiveis
    for(i = 0; i < M->n_bases; i++){
        if(menor > distBaseMissao[i]){
            cjto_inter(M->BASES[i]->herois_presentes, ev->mi->hab_necessarias);
        }
    }

}

