// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DAINF UTFPR
// Versão 1.0 -- Março de 2015
//
// Estruturas de dados internas do sistema operacional


// ALUNO: CAIQUE SALVADOR NOBOA
// RA: 190494


#ifndef __DATATYPES__
#define __DATATYPES__

#include <sys/time.h>
#include "queue.h"
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h> 


enum status_t { 
  PRONTA, SUSPENSA, TERMINADO 
  };


// Estrutura que define uma tarefa
typedef struct task_t
{
    struct task_t *prev, *next, *pai;
    int tid, prioEstatica, prioDinamica, ticks;
    bool tarefaDeUsuario;
    void *stack;
    enum status_t status;
    ucontext_t context;
    unsigned int ativacoes;
    unsigned int tempoGlobal; 
    unsigned int tempoLocal;
    unsigned int tempoTotalProcessador; 
} task_t ;

// estrutura que define um semáforo
typedef struct
{
  // preencher quando necessário
} semaphore_t ;

// estrutura que define um mutex
typedef struct
{
  // preencher quando necessário
} mutex_t ;

// estrutura que define uma barreira
typedef struct
{
  // preencher quando necessário
} barrier_t ;

// estrutura que define uma fila de mensagens
typedef struct
{
  // preencher quando necessário
} mqueue_t ;

#endif
