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
    struct fprio_t *LEF = fprio_cria(); //destruir isso 
    if(!LEF)
        return(0);
    if(!inicializaEntidade(M, LEF))
        return(0);
    struct fpnodo_t *ev;
    //int a, b;
    do{
        //tenho q retirar os eventos tbm
        ev = LEF->prim;
        M->relogio = ev->prio; //ver isso 
        switch (ev->tipo){
        case CODIGOCHEGA: eventoChega(ev->prio, ev->item, LEF);
          /* code */
          break;
       
        
        default:
          break;
        }
        //fprio_retira(LEF, &a, &b); //ver isso
        LEF->prim = LEF->prim->prox;
    }while (ev->tipo != CODIGOFIM);//eh assim um do while?
  /*codigo da missao = 1
  codigo do evento chega = 2*/
    // executar o laço de simulação
    destruirEntidades(M);
    LEF = fprio_destroi(LEF);
    // destruir o mundo

    return (0) ;
}
