#include <stdio.h>
#include <stdlib.h>
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

void eventoEspera(evento *ev, struct fprio_t *LEF){
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", ev->tempo, ev->h->id_heroi, ev->b->id_base, fila_tamanho(ev->b->fila_espera));
    fila_insere(ev->b->fila_espera, ev->h->id_heroi); // talvez int tam = isso sirva pra algo dp
    evento *novoEv = criaEvento(CODIGOAVISA, ev->tempo, ev->h, ev->b, ev->mi);
    fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);
}

void eventoDesiste(evento *ev, mundo *M, struct fprio_t *LEF){
    base *destino;
    do{
    destino = M->BASES[aleat(0, M->n_bases - 1)];
    }while(destino->id_base != ev->b->id_base);
    ev->b = destino;

    printf("%6d: DESISTE HEROI %2d BASE %d\n", ev->tempo, ev->h->id_heroi, ev->b->id_base);

    evento *novoEv = criaEvento(CODIGOVIAJA, ev->tempo, ev->h, ev->b, ev->mi);
    fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);
}

void eventoAvisa(evento *ev, mundo *M, struct fprio_t *LEF){
    int idHeroiRetirado;
    printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d)\n", ev->tempo, ev->b->id_base, cjto_card(ev->b->herois_presentes), ev->b->lotacao);
    while((ev->b->lotacao - cjto_card(ev->b->herois_presentes) != 0) && fila_tamanho(ev->b->fila_espera) > 0){
        fila_retira(ev->b->fila_espera, &idHeroiRetirado);
        heroi *h = M->HEROIS[idHeroiRetirado];
        cjto_insere(ev->b->herois_presentes, h->id_heroi);

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", ev->tempo, ev->b->id_base, ev->h->id_heroi);

        evento *novoEv = criaEvento(CODIGOENTRA, ev->tempo, h, ev->b, ev->mi);
        fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);
    }
}
