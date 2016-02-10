#include "ContadorPalavrasCorrigeErros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //String funções
#include <unistd.h> //GETOPT()
#include <ctype.h>  //isalpha() isspace()
#define MAXTAM 200

void PegaArgumentos(int argc, char **argv, char ** entrada, char ** saida){
    const char *vetorOpcoes = "i:o:O:I:";
    char opcoes;
    while ((opcoes = getopt(argc, argv, vetorOpcoes)) != EOF){
        switch (opcoes){
        case 'i':
            *entrada = optarg;
            break;
        case 'I':
            *entrada = optarg;
            break;
        case 'o':
            *saida = optarg;
            break;
        case 'O':
            *saida = optarg;
            break;
        default:
            break;
        }
    }
}

void FLVazia(TipoLista *Lista){
    Lista->Primeiro = (Apontador) malloc(sizeof(Celula));
    Lista->Ultimo = Lista->Primeiro;
    Lista->Primeiro->Prox = NULL;
}

int Vazia(TipoLista Lista){
    return (Lista.Primeiro == Lista.Ultimo);
}

void Insere(TipoItem x, TipoLista *Lista){
    Lista->Ultimo->Prox = (Apontador) malloc(sizeof(Celula));
    Lista->Ultimo = Lista->Ultimo->Prox;
    Lista->Ultimo->Item = x;
    Lista->Ultimo->Prox = NULL;
}

void Imprime(char * saida, TipoLista Lista){
    FILE *fpSaida;
    fpSaida=fopen(saida, "w");
    Apontador Aux;
    Aux = Lista.Primeiro->Prox;
    while (Aux != NULL){
        fprintf(fpSaida, "%s %d\n", Aux->Item.palavra, Aux->Item.numVezes);
        Aux = Aux->Prox;
    }
    fprintf(fpSaida,"FIM\n");
    fclose(fpSaida);
}

char * StrLower(char * s){
        char *t = s;
        if (!s)
            return 0;

        while (*t!='\0'){
                if(*t>='A' && *t<='Z')
                    *t = *t + ('a' - 'A');
                t++;
        }
        return s;
}

char ConverteAcento(char acentuado){
    if(((acentuado<=-93 && acentuado>=-96)) || (acentuado<=-125 && acentuado>=-128))
        acentuado='a';
    if(((acentuado<=-86 && acentuado>=-88)) || (acentuado<=-118 && acentuado>=-120))
        acentuado='e';
    if(((acentuado<=-82 && acentuado>=-84)) || (acentuado<=-118 && acentuado>=-120))
        acentuado='i';
    if(((acentuado<=-75 && acentuado>=-78)) || (acentuado<=-107 && acentuado>=-110))
        acentuado='o';
    if(((acentuado<=-69 && acentuado>=-71)) || (acentuado<=-101 && acentuado>=-103))
        acentuado='u';
    if(acentuado==-89 || acentuado==-121)
        acentuado='c';
    return acentuado;
}

void Tokeniza(char * palavra, char * palavraCorrigida){
    char * token=NULL;
    int i=0;
    char limites[] = ",.!?:;_";

    while(palavra[i]!='\0'){
        if(palavra[i]==-61){
            palavra[i]='?';//Insere interrogação no caracter que indica o "erro", no tratamento de token seguinte o "?" é retirado, normalizando a palavra
            palavra[i+1]=ConverteAcento(palavra[i+1]);
            i++;
        }
    i++;
    }

    token=strtok(palavra, limites);//Retira sinais de pontuação considerando apenas o final da palavra
    while(token!=NULL){
        strcat(palavraCorrigida, token);//Concatena a parte retirada para palavraCorrigida
        token=strtok(NULL, limites);
    }
}

void TabelaASC(int tabelaAsc[256]){
    int i;
    for(i=0;i<256;i++){
        tabelaAsc[i]=i;
    }
}

int StrCmpOrdem (const char *p1, const char *p2, int tabelaAsc[256]){
    register const unsigned char *s1 = (const unsigned char *) p1;
    register const unsigned char *s2 = (const unsigned char *) p2;
    unsigned char c1, c2;

    do{
        c1 = *s1++;
        c2 = *s2++;

        if (c1 == '\0')
            return tabelaAsc[c1] - tabelaAsc[c2];
    } while (c1 == c2);

        return tabelaAsc[c1] - tabelaAsc[c2];
}

int LeArquivo(char * entrada, int tabelaAsc[256], TipoLista * palavras){
    FILE *fpEntrada;
    int i, ascAux, numPalavras;
    char aux[2], confere[MAXTAM], palavraCorrigida[MAXTAM];
    TipoItem item;

    fpEntrada=fopen(entrada, "r");
    fscanf(fpEntrada, "%s", confere);

    if(strcasecmp(confere, "ORDEM")==0){
        for(i=0;i<26;i++){
            fscanf(fpEntrada, "%s", aux);
            StrLower(aux);
            ascAux=ConverteCharAsc(aux);
            tabelaAsc[ascAux]=i+97;//POSICAO CORERETA NA TABELA AUX PASSADA
        }

        fscanf(fpEntrada, "%s", confere);// LE "TEXTO" E GUARDA EM CONFERE
        numPalavras=0;
        while (fscanf(fpEntrada, "%s", confere) != EOF){
                strcpy(palavraCorrigida,"");//ESVAZIA PALAVRACORRIGIDA
                Tokeniza(confere, palavraCorrigida);
                if(palavraCorrigida[strlen(palavraCorrigida)-1]=='-' && strlen(palavraCorrigida)>1){//Se o último caracter for hífen e a palavra não for "-"
                    if(fscanf(fpEntrada, "%s", confere) != EOF){
                        Tokeniza(confere, palavraCorrigida);
                    }
                }
                StrLower(palavraCorrigida);
                strcpy(item.palavra, palavraCorrigida);//item.palavra=confere
                item.numVezes=1;
                Insere(item, palavras);//Cria celula para a palavra;
                numPalavras++;//NUMERO DE PALAVRAS INSERIDAS NA LISTA
        }//WHILE
    } else {//LOGO A PRIMEIRA PALAVRA SERÁ TEXTO
        fscanf(fpEntrada, "%s", confere);
        numPalavras=0;//Ja foi inserida a primeira palavra
        while(strcasecmp(confere, "ORDEM")!=0){
            strcpy(palavraCorrigida,"");//ESVAZIA PALAVRACORRIGIDA
                Tokeniza(confere, palavraCorrigida);
                if(palavraCorrigida[strlen(palavraCorrigida)-1]=='-' && strlen(palavraCorrigida)>1){//Se o último caracter for hífen e a palavra não for "-"
                    fscanf(fpEntrada, "%s", confere);//Le a prox palavra
                    if(strcasecmp(confere, "ORDEM")!=0){
                        Tokeniza(confere, palavraCorrigida);
                    }
                }
                StrLower(palavraCorrigida);
                strcpy(item.palavra, palavraCorrigida);//item.palavra=confere
                item.numVezes=1;
                Insere(item, palavras);//Cria celula para a palavra;
                numPalavras++;//NUMERO DE PALAVRAS INSERIDAS NA LISTA

            if(strcmp(confere, "ORDEM")!=0)
                fscanf(fpEntrada, "%s", confere);
        }//WHILE
        for(i=0;i<26;i++){
            fscanf(fpEntrada, "%s", aux);
            StrLower(aux);
            ascAux=ConverteCharAsc(aux);
            tabelaAsc[ascAux]=i+97;//POSICAO CORERETA NA TABELA AUX PASSADA
        }
    }//ELSE
    return numPalavras;
    fclose(fpEntrada);
}

void Particao(int Esq, int Dir, int *i, int *j, Apontador * ponteiros, int tabelaAsc[256]){
    char x[MAXTAM], w[MAXTAM];
    *i = Esq; *j = Dir;

    strcpy(x, ponteiros[(*i + *j)/2]->Item.palavra);
    do{
        while (StrCmpOrdem(x, ponteiros[*i]->Item.palavra, tabelaAsc) > 0) (*i)++;
        while (StrCmpOrdem(x, ponteiros[*j]->Item.palavra, tabelaAsc) < 0) (*j)--;
            if (*i <= *j){
                strcpy(w, ponteiros[*i]->Item.palavra);
                strcpy(ponteiros[*i]->Item.palavra, ponteiros[*j]->Item.palavra);
                strcpy(ponteiros[*j]->Item.palavra, w);
                (*i)++; (*j)--;
            }
    } while (*i <= *j);
}

void Ordena(int Esq, int Dir, Apontador * ponteiros, int tabelaAsc[256]){
    int i, j;
    Particao(Esq, Dir, &i, &j, ponteiros, tabelaAsc);
    if (Esq < j) Ordena(Esq, j, ponteiros, tabelaAsc);
    if (i < Dir) Ordena(i, Dir, ponteiros, tabelaAsc);
}

void QuickSort(Apontador * ponteiros, int *n, int tabelaAsc[256]){
    Ordena(0, *n, ponteiros, tabelaAsc);
}

int ConverteCharAsc(char * caracter){
    int asc;
    asc=caracter[0];
    return asc;
}

void OrdenaLista(Apontador * ponteiros, TipoLista palavras, int tabelaAsc[256], TipoLista * resultado){
    int i=0, j=0;
    Apontador aux;
    TipoItem item;

    aux=palavras.Primeiro->Prox;
    while(aux!=NULL){
        ponteiros[i]=aux;
        i++;
        aux=aux->Prox;
    }

    i--;//PASSAR A CASA FINAL DO VETOR CERTA

    QuickSort(ponteiros, &i, tabelaAsc);

    while(i>=0){
        strcpy(item.palavra, ponteiros[j]->Item.palavra);
        item.numVezes=1;
        if(strcmp(resultado->Ultimo->Item.palavra, item.palavra)==0)
            resultado->Ultimo->Item.numVezes++;
        else
            Insere(item, resultado);
        i--;
       j++;
    }
}
