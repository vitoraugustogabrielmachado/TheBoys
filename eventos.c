#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

//calculo a distancia entre dois pontos de maneira geral
int dist(int a[], int b[]){
    int distancia_1 = (a[0] - b[0]) * (a[0] - b[0]);
    int distancia_2 = (a[1] - b[1]) * (a[1] - b[1]);
    return(sqrt(distancia_1 + distancia_2));
}

void eventoChega(evento *ev, struct fprio_t *LEF){ 
    int espera;
    if(ev->h->status){ //ignora se o heroi estiver morto
        ev->h->id_baseAtual = ev->b->id_base; //atualiza a base atual do heroi
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
}

void eventoEspera(evento *ev, struct fprio_t *LEF){
    if(ev->h->status){ //testa se o heroi está vivo
        printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", ev->tempo, ev->h->id_heroi, ev->b->id_base, fila_tamanho(ev->b->fila_espera));
        fila_insere(ev->b->fila_espera, ev->h->id_heroi); 

        if(fila_tamanho(ev->b->fila_espera) > ev->b->fila_max) //isso serve so para as estatisticas
            ev->b->fila_max = fila_tamanho(ev->b->fila_espera);
        
        evento *novoEv = criaEvento(CODIGOAVISA, ev->tempo, ev->h, ev->b, ev->mi);
        fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);
    }
}

void eventoDesiste(evento *ev, struct fprio_t *LEF){
    if(ev->h->status){ //testa se o heroi está vivo
        printf("%6d: DESISTE HEROI %2d BASE %d\n", ev->tempo, ev->h->id_heroi, ev->b->id_base);
        evento *novoEv = criaEvento(CODIGOVIAJA, ev->tempo, ev->h, ev->b, ev->mi);
        fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);
    }
}

void eventoAvisa(evento *ev, mundo *M, struct fprio_t *LEF){
    int idHeroiRetirado;
    printf("%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA ", ev->tempo, ev->b->id_base, cjto_card(ev->b->herois_presentes), ev->b->lotacao);
    printf("[ ");
    if(ev->b->fila_espera && fila_tamanho(ev->b->fila_espera))
        fila_imprime(ev->b->fila_espera);
    printf(" ]\n");
    //a primeira parte do while testa se a base ja nn esta lotada e a segunda testa se a fila de espera é maior que zero, pq se nao for ele nn deve admitir mais herois
    while((ev->b->lotacao - cjto_card(ev->b->herois_presentes) != 0) && fila_tamanho(ev->b->fila_espera) > 0){
        fila_retira(ev->b->fila_espera, &idHeroiRetirado);
        heroi *h = M->HEROIS[idHeroiRetirado];
        cjto_insere(ev->b->herois_presentes, h->id_heroi);

        printf("%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", ev->tempo, ev->b->id_base, h->id_heroi);

        evento *novoEv = criaEvento(CODIGOENTRA, ev->tempo, h, ev->b, ev->mi);
        fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);
    }
}

void eventoEntra(evento *ev, struct fprio_t *LEF){
    if(ev->h->status){ //testa se o heroi está vivo
        int TPB = 15 + ev->h->paciencia * aleat(1, 20); 

        evento *novoEv = criaEvento(CODIGOSAI, ev->tempo + TPB, ev->h, ev->b, ev->mi);
        printf("%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %6d\n", ev->tempo, ev->h->id_heroi, ev->b->id_base, cjto_card(ev->b->herois_presentes), ev->b->lotacao, novoEv->tempo);
        fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);
    }
}

void eventoSai(evento *ev, struct fprio_t *LEF){
    if(ev->h->status){ //testa se o heroi morreu
        if(cjto_retira(ev->b->herois_presentes, ev->h->id_heroi) == -1)
            abort();

        printf("%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", ev->tempo, ev->h->id_heroi, ev->b->id_base, cjto_card(ev->b->herois_presentes), ev->b->lotacao);

        evento *novoEv = criaEvento(CODIGOVIAJA, ev->tempo, ev->h, ev->b, ev->mi);
        fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);

        evento *novoEv2 = criaEvento(CODIGOAVISA, ev->tempo, ev->h, ev->b, ev->mi);
        fprio_insere(LEF, novoEv2, novoEv2->tipo, novoEv2->tempo);
    }
}

void eventoViaja(evento *ev, mundo *M, struct fprio_t *LEF){
    if(ev->h->status){
        base *destino;
        destino = M->BASES[aleat(0, M->n_bases - 1)]; //decide uma base aleatoria para o heroi ir
        int distanciaTotal = dist(ev->b->local, destino->local); //calcula a distancia
        int duracao = distanciaTotal/ ev->h->velocidade; //calcula o tempo

        evento *novoEv = criaEvento(CODIGOCHEGA, ev->tempo + duracao, ev->h, destino, ev->mi);
        fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);

        printf("%6d: VIAJA HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", ev->tempo, novoEv->h->id_heroi, ev->b->id_base, novoEv->b->id_base, distanciaTotal, novoEv->h->velocidade, novoEv->tempo);
    }
}

void eventoMorre(evento *ev, struct fprio_t *LEF){
    if(cjto_retira(ev->b->herois_presentes, ev->h->id_heroi) == -1)
        abort();
    ev->h->status = 0;

    printf("%6d: MORRE HEROI %d MISSAO %d\n", ev->tempo, ev->h->id_heroi, ev->mi->id_missao);

    evento *novoEv = criaEvento(CODIGOAVISA, ev->tempo, ev->h, ev->b, ev->mi);
    fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);
}


void eventoMissao(evento *ev, mundo *M, struct fprio_t *LEF){
    int i, cont, BMP, maiorEXP, encontrado = 0, salvaIdBase, habNecessaria, h, menor;
    int *distBaseMissao = malloc(M->n_bases * sizeof(int));
    if(!distBaseMissao)
        abort();

    printf("%6d: MISSAO %d TENT %d HAB REQ:[ ", ev->tempo, ev->mi->id_missao, ev->mi->tentativas);
    cjto_imprime(ev->mi->hab_necessarias);
    printf("]\n");

    for(i = 0; i < M->n_bases; i++){
        distBaseMissao[i] = dist(ev->mi->local, M->BASES[i]->local);
    } // coloca no vetor todas as distancias possiveis
    
    for(cont = 0; cont < M->n_bases; cont++){ //esse loop serve para testar todas as bases possiveis
        menor = __INT_MAX__;
        salvaIdBase = -1;

        for(i = 0; i < M->n_bases; i++){ //esse loop serve para achar qual é a menor distancia
            if(distBaseMissao[i] < menor){
                menor = distBaseMissao[i];
                salvaIdBase = i;
            }
        }
        if(salvaIdBase == -1) //caso todas as bases tenham falhado e todas as distancias sejam int max, ele quebra aqui para nn dar segfault
            break;

        BMP = 1;
        for(habNecessaria = 0; habNecessaria < M->n_habilidades; habNecessaria++){ //esse loop serve para decidir qual o numero da habilidade que a missao requisita
            if(cjto_pertence(ev->mi->hab_necessarias, habNecessaria)){
                encontrado = 0; 
                for(h = 0; h < M->n_herois; h++){ //esse loop serve para decidir se o heroi h pertence a base mais proxima e logo dps decide se ele possui a habilidade
                    if(cjto_pertence(M->BASES[salvaIdBase]->herois_presentes, h))
                        if(cjto_pertence(M->HEROIS[h]->hab_pertencentes, habNecessaria))
                            encontrado = 1;
                }
                if(!encontrado){ //caso o heroi nn tenha a habilidade ele ja quebra o loop aqui msm, pq nn adianta testar alem disso
                    BMP = 0;
                    break;
                }
            }
        }
        if(BMP)
            break;
        distBaseMissao[salvaIdBase] = __INT_MAX__; //torna a distancia da base anteriormente conhecida como a menor para o maximo possivel, assim nunca mais escolhe ela
    }

    if(BMP == 1){ //caso a base seja a mais proxima e apta para concluir a missao
        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS[", ev->tempo, ev->mi->id_missao, M->BASES[salvaIdBase]->id_base, distBaseMissao[salvaIdBase]);
        cjto_imprime(M->BASES[salvaIdBase]->herois_presentes);
        printf("]\n");

        for(h = 0; h < M->n_herois; h++){
            if(cjto_pertence(M->BASES[salvaIdBase]->herois_presentes, h)){ //loop para acrescentar experiencia dos herois e imprimir suas habilidades
                M->HEROIS[h]->experiencia++;
                printf("%6d: MISSAO %d HAB HEROI %d[", ev->tempo, ev->mi->id_missao, M->HEROIS[h]->id_heroi);
                cjto_imprime(M->HEROIS[h]->hab_pertencentes);
                printf("]\n");
            }
        }
        M->BASES[salvaIdBase]->quantidadeMissoes++; //serve para a estatistica

        /*esse bloco de linhas logo abaixo serve para juntar todas as habilidades da base e imprimir
        poderia fazer isso no loop acima, mas ocupa MUITO espaço atoa, entao decidi colocar aqui*/
        struct cjto_t *resultadoUniao = cjto_cria(M->n_habilidades);
        for(cont = 0; cont < M->n_bases; cont++){
                for(h = 0; h < M->n_herois; h++){
                    if(cjto_pertence(M->BASES[salvaIdBase]->herois_presentes, h)){
                        struct cjto_t *tmp = cjto_uniao(resultadoUniao, M->HEROIS[h]->hab_pertencentes);
                        cjto_destroi(resultadoUniao);
                        resultadoUniao = tmp;
                    }
                }
        }

        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS:[", ev->tempo, ev->mi->id_missao, M->BASES[salvaIdBase]->id_base);
        cjto_imprime(resultadoUniao);
        printf("]\n");
        cjto_destroi(resultadoUniao);
    }
    else if(M->n_compostosV > 0 && ev->tempo % 2500 == 0){
        maiorEXP = M->HEROIS[0]->experiencia;
        int indiceMaiorEXP = 0;
        M->n_compostosV--;
        for(h = 0; h < M->n_herois; h++){ //procura o heroi com mais experiencia
            if(M->HEROIS[h]->experiencia > maiorEXP && M->HEROIS[h]->status == 1){
                maiorEXP = M->HEROIS[h]->experiencia;
                indiceMaiorEXP = h;
            }
        } 

        //mata o heroi com mais exp
        evento *novoEv = criaEvento(CODIGOMORRE, ev->tempo, M->HEROIS[indiceMaiorEXP], M->BASES[M->HEROIS[indiceMaiorEXP]->id_baseAtual], ev->mi);
        fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);

        printf("%6d: MISSAO %d CUMPRIDA HEROI %d USOU COMPOSTO V\n", ev->tempo, ev->mi->id_missao, novoEv->h->id_heroi);            

        salvaIdBase = M->HEROIS[indiceMaiorEXP]->id_baseAtual;
        M->BASES[salvaIdBase]->quantidadeMissoes++;

        for(h = 0; h < M->n_herois; h++){ //acrescenta exp para todos os herois na base
            if(cjto_pertence(M->BASES[salvaIdBase]->herois_presentes, h))
                M->HEROIS[h]->experiencia++;
        }
    }
    else{
        ev->mi->tentativas++;
        printf("%6d: MISSAO %d IMPOSSIVEL\n", ev->tempo, ev->mi->id_missao);
        evento *novoEv = criaEvento(CODIGOMISSAO, ev->tempo + 24*60, ev->h, ev->b, ev->mi);
        fprio_insere(LEF, novoEv, novoEv->tipo, novoEv->tempo);
    }
    free(distBaseMissao);
}
