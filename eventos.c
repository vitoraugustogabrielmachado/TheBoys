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

void eventoChega(int tempo, evento *ev, struct fprio_t *LEF){ //quando tiver terminado ver se nn fica melhor mudar o nome das variaveis pra nn repetir mt
    ev->h->baseLocal = ev->b->id_base;
    int espera;


    if(ev->b->lotacao == cjto_card(ev->b->herois_presentes) && fila_tamanho(ev->b->fila_espera) == 0)
        espera = 1;
    else 
        espera = ((ev->h->paciencia) > (10 * fila_tamanho(ev->b->fila_espera))); //isso ta certo? nn precisa de um parenteses por fora?

    if (espera){
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n", tempo, ev->h->id_heroi, ev->b->id_base, cjto_card(ev->b->herois_presentes), ev->b->lotacao);
        ev->tipo = CODIGOESPERA;
        fprio_insere(LEF, ev, ev->tipo, ev->tempo);
    }else{
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n", tempo, ev->h->id_heroi, ev->b->id_base, cjto_card(ev->b->herois_presentes), ev->b->lotacao);
        ev->tipo = CODIGODESISTE;
        fprio_insere(LEF, ev, ev->tipo, ev->tempo);
    }
    
}

void eventoEspera(evento *ev, struct fprio_t *LEF){
 
    fila_insere(ev->b->fila_espera, ev->h->id_heroi); // talvez int tam = isso sirva pra algo dp
 
    
    ev->tipo = CODIGOAVISA;

    fprio_insere(LEF, ev, ev->tipo, ev->tempo);

}

