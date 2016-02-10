#include <stdio.h>
#include <stdlib.h>
#define MAXTAM 200
typedef struct {
    char palavra[MAXTAM];
    int numVezes;
} TipoItem;

typedef struct Celula_str *Apontador;

typedef struct Celula_str {
    TipoItem Item;
    Apontador Prox;
} Celula;

typedef struct {
    Apontador Primeiro, Ultimo;
} TipoLista;

void PegaArgumentos(int argc, char **, char **, char **);
void FLVazia(TipoLista *);
int Vazia(TipoLista);
void Insere(TipoItem, TipoLista *);
void Imprime(char *, TipoLista);
char * StrLower(char *);
char ConverteAcento(char);
void Tokeniza(char *, char *);
void TabelaASC(int *);
int StrCmpOrdem (const char *, const char *, int *);
int LeArquivo(char *, int *, TipoLista *);
void Particao(int, int, int *, int *, Apontador *, int *);
void Ordena(int , int, Apontador *, int *);
void QuickSort(Apontador *, int *, int *);
int ConverteCharAsc(char *);
void OrdenaLista(Apontador *, TipoLista , int *, TipoLista *);
