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
    evento *ev;
    int a, b;
    while (((ev = fprio_retira(LEF, &a, &b)) != NULL) && (ev->tempo < M->t_final)){
        M->relogio = ev->tempo;
        switch(ev->tipo){
            case CODIGOCHEGA:
                eventoChega(ev, LEF);
                break;

            case CODIGOESPERA:
                eventoEspera(ev, LEF);
                break;
            
            case CODIGODESISTE:
                eventoDesiste(ev, M, LEF);
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
        }
        free(ev);
}
    destruirEntidades(M);
    LEF = fprio_destroi(LEF);

    return (0) ;
}
