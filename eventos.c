#include <stdio.h>
#include "eventos.h"
#include "entidades.h"
#include "fila.h"
#include "fprio.h"
#include "conjunto.h"

void eventoChega(int tempo, struct item_t *item, struct fprio_t *LEF){ //quando tiver terminado ver se nn fica melhor mudar o nome das variaveis pra nn repetir mt
    item->h->baseLocal = item->b->id_base;
    int espera;


    if(item->b->lotacao == cjto_card(item->b->herois_presentes) && fila_tamanho(item->b->fila_espera) == 0)
        espera = 1;
    else 
        espera = ((item->h->paciencia) > (10 * fila_tamanho(item->b->fila_espera))); //isso ta certo? nn precisa de um parenteses por fora?

    if (espera){
        struct item_t *item2 = criaItem(CODIGOESPERA, tempo);
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n", tempo, item->h->id_heroi, item->b->id_base, cjto_card(item->b->herois_presentes), item->b->lotacao);
        item2->h = item->h;
        item2->b = item->b;
        fprio_insere(LEF, item2, item2->codigoEvento, item2->tempoEvento);
    }else{
        struct item_t *item2 = criaItem(CODIGODESISTE, tempo); //sera q isso nn ocupa o dobro de espaço na memoria?
        printf("%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n", tempo, item->h->id_heroi, item->b->id_base, cjto_card(item->b->herois_presentes), item->b->lotacao);
        item2->h = item->h;
        item2->b = item->b;
        fprio_insere(LEF, item2, item2->codigoEvento, item2->tempoEvento);
    }
    
}

void eventoEspera(int tempo, struct item_t *item, struct fprio_t *LEF){
 
    fila_insere(item->b->fila_espera, item->h->id_heroi); // talvez int tam = isso sirva pra algo dp
 
    
    struct item_t *i = criaItem(CODIGOAVISA, tempo);

    fprio_insere(LEF, i, i->codigoEvento, i->tempoEvento);

}

