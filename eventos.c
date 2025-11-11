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
        struct item_t *item = criaItem(CODIGOESPERA, tempo);
        fprio_insere(LEF, item, 0, item->tempoEvento);
    }else{
        struct item_t *item = criaItem(CODIGODESISTE, tempo); //sera q isso nn ocupa o dobro de espaço na memoria?
        fprio_insere(LEF, item, 0, item->tempoEvento);
    }
    
}

void eventoEspera(int tempo, struct item_t *item, struct fprio_t *LEF){
    fila_insere(item->b->fila_espera, item->h); // talvez int tam = isso sirva pra algo dps
    struct item_t *i = criaItem(CODIGOAVISA, tempo);
    fprio_insere(LEF, i, 0, i->tempoEvento);
}

