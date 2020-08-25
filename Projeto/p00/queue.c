#include "queue.h"
#include <stdio.h>

//------------------------------------------------------------------------------
// Insere um elemento no final da fila.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - o elemento deve existir
// - o elemento nao deve estar em outra fila
void queue_append (queue_t **queue, queue_t *elem) {
    // Para fila vazia
    if (elem->prev != NULL && elem->next != NULL){
        ; // elemento de outra lista
    }
    else if (*queue == NULL){
        *queue = elem;
        elem->next = elem;
        elem->prev = elem;
    }
    //Para fila não vazia
    else {
        queue_t* primeiro;
        queue_t* ultimo;
        primeiro = *queue;
        ultimo = primeiro->prev;
        primeiro->prev = elem;
        ultimo->next = elem;
        elem->next = primeiro;
        elem->prev = ultimo;
    }
};

//------------------------------------------------------------------------------
// Remove o elemento indicado da fila, sem o destruir.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - a fila nao deve estar vazia
// - o elemento deve existir
// - o elemento deve pertencer a fila indicada
// Retorno: apontador para o elemento removido, ou NULL se erro
queue_t *queue_remove (queue_t **queue, queue_t *elem) {
    queue_t* aux;
    queue_t* anterior;
    queue_t* proximo;
    aux = *queue;
    int tamanho, i =0;

    tamanho = queue_size(*queue);

    while(aux != elem ){
        aux = aux->next;
        i++;
        if (tamanho < i){
            return NULL;
        }
    }
    if ( tamanho > 1){
        anterior = aux->prev;
        proximo = aux->next;

        anterior->next = proximo;
        proximo->prev = anterior;
        if (*queue == elem){
            *queue = proximo;
        }
    }    
    else{
        *queue = NULL;
    }

    elem->next = NULL;
    elem->prev = NULL;

    return elem;
};


//------------------------------------------------------------------------------
// Percorre a fila e imprime na tela seu conteúdo. A impressão de cada
// elemento é feita por uma função externa, definida pelo programa que
// usa a biblioteca. Essa função deve ter o seguinte protótipo:
//
// void print_elem (void *ptr) ; // ptr aponta para o elemento a imprimir

void queue_print (char *name, queue_t *queue, void print_elem (void*) ){
    printf("%s: [", name);
    queue_t* aux;
    int tamanho, i, anterior, proximo;
    tamanho = queue_size(queue) - 1;
    aux = queue;
    for(i = 0; i < tamanho+1; i++){
        
        if (i == 0){
            anterior = tamanho - i;
        }
        else{
            anterior = i-1;
            printf(" ");
        }

        if (i == tamanho){
            proximo = 0;
        }
        else{
            proximo = i+1;
        }
        printf("%d<%d>%d", anterior, i, proximo);
        aux = aux->next;
    }
    printf("]\n");
} ;


//------------------------------------------------------------------------------
// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila
int queue_size (queue_t *queue) {
    queue_t* primeiro;
    queue_t* aux;
    int tamanho = 0;
    primeiro = queue;
    aux = primeiro;
    
    if (queue == NULL){
        return tamanho;
    }
    tamanho ++;
    while(aux->next != primeiro) {
        tamanho++;
        aux = aux->next;
    }

    return tamanho;
};
