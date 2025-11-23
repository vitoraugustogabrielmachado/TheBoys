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
#include "simulacao.h"
#include "entidades.h"
// seus #defines vão aqui

// minimize o uso de variáveis globais

// programa principal
int main (){
    mundo *M = criaMundo();
    struct fprio_t *LEF = fprio_cria();  
    if(!LEF)
        return(0);
    if(!inicializaEntidade(M, LEF))
        return(0);
    simulacao(M, LEF);
    destruirEntidades(M);
    LEF = fprio_destroi(LEF);

    return (0) ;
}
