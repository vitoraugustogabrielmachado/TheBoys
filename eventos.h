#ifndef EVENTOS_T
#define EVENTOS_T

#include "entidades.h"
/*includes aqui*/

#define CODIGOCHEGA 1
#define CODIGOMISSAO 2 
#define CODIGOESPERA 3
#define CODIGODESISTE 4 
#define CODIGOAVISA 5
#define CODIGOENTRA 6
#define CODIGOSAI 7
#define CODIGOVIAJA 8
#define CODIGOMORRE 9
#define CODIGOFIM 10

typedef struct eventos_t{
    int tempo;
    int tipo;
    heroi *h;
    base *b;
    missao *mi;
}evento;

evento *criaEvento(int tipo, int tempo, heroi *h, base *b, missao *mi);

void eventoChega(int tempo, evento *ev, struct fprio_t *LEF);

void eventoMissao();

void eventoEspera(evento *ev, struct fprio_t *LEF);

void eventoDesiste();

void eventoAvisa();

void eventoEntra();

void eventoSai();

void eventoViaja();

void eventoMorre();

void eventoFim();

#endif