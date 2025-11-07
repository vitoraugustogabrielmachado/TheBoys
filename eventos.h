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

void eventoChega(int tempo, struct item_t *item, struct fprio_t *LEF);

void eventoMissao();

void eventoEspera();

void eventoDesiste();

void eventoDesiste();

void eventoAvisa();

void eventoEntra();

void eventoSai();

void eventoViaja();

void eventoMorre();

void eventoFim();
