#ifndef SIMULACAO_T
#define SIMULACAO_T

#include "fila.h"
#include "fprio.h"
#include "entidades.h"
#include "eventos.h"

void simulacao(mundo *M, struct fprio_t *LEF);

void imprimir_estatisticas(mundo *M);
#endif