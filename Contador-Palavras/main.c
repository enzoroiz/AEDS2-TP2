#include <stdio.h>
#include <stdlib.h>
#include "ContadorPalavrasCorrigeErros.h"

int main(int argc, char ** argv){
    int tabelaAsc[256], numPalavras;
    char *entrada=NULL, *saida=NULL;
    TipoLista palavras, resultado;

    FLVazia(&resultado);
    FLVazia(&palavras);
    PegaArgumentos(argc, argv, &entrada, &saida);
    TabelaASC(tabelaAsc);
    numPalavras=LeArquivo(entrada, tabelaAsc, &palavras);
    Apontador ponteiros[numPalavras];
    OrdenaLista(ponteiros, palavras, tabelaAsc, &resultado);
    Imprime(saida, resultado);
    return 0;
}

