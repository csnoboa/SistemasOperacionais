#include "queue.h"

//------------------------------------------------------------------------------
// Insere um elemento no final da fila.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - o elemento deve existir
// - o elemento nao deve estar em outra fila
void queue_append (queue_t **queue, queue_t *elem) {
    // Para fila vazia
    if (*queue == NULL){
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
}

//------------------------------------------------------------------------------
// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila

int queue_size (queue_t *queue) {
    queue_t* primeiro;
    queue_t* aux;
    int tamanho = 0;
    primeiro = queue;
    aux = primeiro;

    do {
        tamanho++;
        aux = aux->next;
    }while(aux->next != primeiro);
}