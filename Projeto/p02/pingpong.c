#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include "pingpong.h"

ucontext_t ContextPing, ContextPong, ContextMain;

// Inicializa o sistema operacional; deve ser chamada no inicio do main()
void pingpong_init () {
    
    setvbuf (stdout, 0, _IONBF, 0) ;
};

// Cria uma nova tarefa. Retorna um ID> 0 ou erro.
int task_create (task_t *task, void (*start_func)(void *),void *arg){
    makecontext (task, (void*)(*start_func), 1, arg);

} ;	

// alterna a execução para a tarefa indicada
int task_switch (task_t *task) {
    swapcontext (&ContextMain, task);
};

// Termina a tarefa corrente, indicando um valor de status encerramento
void task_exit (int exitCode) {

};

// retorna o identificador da tarefa corrente (main eh 0)
int task_id () {

};