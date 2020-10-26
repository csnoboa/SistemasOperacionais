// ALUNO: CAIQUE SALVADOR NOBOA
// RA: 190494

#include "pingpong.h"
#define PRIO_DEFAULT 0
#define AGING -1
#define TICKS 20
#define SCHEDULER_PRIO true

int ultimaID = 0, userTasks = 0;
unsigned int mSegundos = 0;

task_t tarefaMain;
task_t dispatcher;
task_t *tarefaAtual;
task_t *tarefasProntas;
task_t *tarefasSuspensas;

struct itimerval tempo;
struct sigaction acao;

void tratar_tick() {
    if (tarefaAtual->tarefaDeUsuario == true) 
    {
        tarefaAtual->ticks--;
        if (tarefaAtual->ticks < 1) 
        {
            task_yield();
        }
    }
    mSegundos++;
}

void temporizador() {
    acao.sa_handler = tratar_tick;
    sigemptyset (&acao.sa_mask) ;
    acao.sa_flags = 0 ;
    if (sigaction (SIGALRM, &acao, 0) < 0)
    {
        perror ("Erro sigaction: ") ;
        exit (1) ;
    }
    tempo.it_value.tv_sec  = 0 ;      
    tempo.it_value.tv_usec = 1000 ;      
    tempo.it_interval.tv_sec  = 0 ;   
    tempo.it_interval.tv_usec = 1000 ;   
    if (setitimer (ITIMER_REAL, &tempo, 0) < 0)
    {
        perror ("Erro setitimer: ") ;
        exit (1) ;
    }
}

void task_setprio(task_t *task, int prioridade) {
    #ifdef DEBUG
        printf("task_setprio: %d está mudando para a prioridade %d \n", task->tid, prioridade);
    #endif //DEBUG print
    if (prioridade < -20) 
    { 
        prioridade = -20; 
    }
    if (prioridade > 20) 
    { 
        prioridade = 20; 
    }
    if (task == NULL)
    {
        tarefaAtual->prioDinamica = prioridade; 
        tarefaAtual->prioEstatica = prioridade;
    }
    else
    {
        task->prioDinamica = prioridade; 
        task->prioEstatica = prioridade;
    }
}

int task_getprio(task_t *task) {
    int prioEstatica;
    if (task == NULL)
    {
        prioEstatica =  tarefaAtual->prioEstatica;
    }
    else
    {
        prioEstatica = task->prioEstatica;
    }
    return prioEstatica;
}
unsigned int systime() {
    return mSegundos;
}

task_t* scheduler() 
{   
    if(SCHEDULER_PRIO){
        // Algoritimo para varrer a lista de tarefas e retornar a que tem menor numero de prioridade
        int baixaPrioridade = 21;
        task_t *primeira = tarefasProntas;
        task_t *aux = tarefasProntas;
        task_t *tarefaRetorno = tarefasProntas;
        if (aux->prioDinamica < baixaPrioridade) 
        {
            baixaPrioridade = aux->prioDinamica;
            tarefaRetorno = aux;
        }
        aux->prioDinamica += AGING; 
        aux = aux->next;
        while(aux != primeira) 
        {
            if (aux->prioDinamica < baixaPrioridade) {
                baixaPrioridade = aux->prioDinamica;
                tarefaRetorno = aux;
            }
            aux->prioDinamica += AGING;
            aux = aux->next;
        }
        tarefaRetorno->prioDinamica = tarefaRetorno->prioEstatica;
        return tarefaRetorno;
    }
    else{
        // algoritimo first come, fist served 
        // pega o primeiro da lista de tarefas
         return tarefasProntas; 
    }
}

void dispatcher_body(void *arg) {
    #ifdef DEBUG
        printf("dispatcher_body. \n");
    #endif //DEBUG print
    while (userTasks > 0) {
        #ifdef DEBUG
            printf("userTasks:  %d\n", userTasks);
        #endif //DEBUG print
        task_t *proximaTarefa = scheduler();
        #ifdef DEBUG
            printf("dispatcher_body. proximaTarefa: %d \n", proximaTarefa->tid);
        #endif //DEBUG print
        if (proximaTarefa != NULL) 
        {
            task_switch(proximaTarefa);
            if(proximaTarefa->status == TERMINADO)
            {
                // #ifdef DEBUG
                //     printf("status == TERMINADO \n");
                // #endif //DEBUG print
                queue_remove((queue_t **) &tarefasProntas, (queue_t *) proximaTarefa);
                userTasks--;
                free(proximaTarefa->stack);
            }
            if(proximaTarefa != NULL){
                if(proximaTarefa->status == PRONTA)
                {
                    // #ifdef DEBUG
                    //     printf("status == PRONTA \n");
                    // #endif //DEBUG print
                    queue_remove((queue_t **) &tarefasProntas, (queue_t *) proximaTarefa);
                    queue_append((queue_t**) &tarefasProntas, (queue_t*) proximaTarefa);               
                }
            }           
        }
    }
    tarefaAtual->status = TERMINADO;
    printf("Task %d exit: running time: %u ms, cpu time: %u ms, %d acticvations\n", tarefaAtual->tid, mSegundos - tarefaAtual->tempoGlobal, tarefaAtual->tempoTotalProcessador, tarefaAtual->ativacoes );
    task_switch(&tarefaMain); // como se fosse um task_exit() direto para a tarefaMain
}

void pingpong_init() {
    setvbuf(stdout, 0, _IONBF, 0); // dado pelo prof
    //Criação da tarefa Main
    ultimaID = 0;
    tarefaMain.tid = ultimaID;
    ultimaID++;
    tarefaAtual = &tarefaMain;
    // Criação da tarefa dispatcher
    task_create(&dispatcher, &dispatcher_body, NULL);
    dispatcher.pai = &tarefaMain; // Como padrão, na hora de criar uma tarefa, o pai é o dispatcher, mas no caso do dispatcher, é a main
    dispatcher.tarefaDeUsuario = false;
    queue_remove((queue_t **) &tarefasProntas, (queue_t *) &dispatcher);  // dispatcher não estará na fila de tarefas
    userTasks--;
    temporizador();
}

// Função para criar uma nova tarefa
// int task_create(task_t *task, void (*start_func)(void), void *arg) {
int task_create(task_t *task, void (*start_func)(void*), void *arg) {
    // Criar context do p01
    ucontext_t contexto;
    getcontext(&contexto);
    char *stack = malloc(32768) ;
    if (stack)
    {
        contexto.uc_stack.ss_size = 32768;
        contexto.uc_stack.ss_sp = stack ;
        contexto.uc_link = 0;
        contexto.uc_stack.ss_flags = 0;
    }
    makecontext(&contexto, start_func, 1, arg);
    // Fim do criar context
    queue_append((queue_t**) &tarefasProntas, (queue_t*) task);
    userTasks++;
    task->context = contexto;
    task->status = PRONTA;
    task->prioEstatica = PRIO_DEFAULT;
    task->prioDinamica = PRIO_DEFAULT;
    task->tarefaDeUsuario = true;
    task->ticks = TICKS;
    task->tid = ultimaID;
    ultimaID++;
    task->stack = contexto.uc_stack.ss_sp;
    task->pai = &dispatcher; //quando for criado o dispatcher, o pai dele será trocado depois
    task->tempoGlobal = mSegundos;
    task->tempoLocal = mSegundos;
    task->ativacoes = 0;
    task->tempoTotalProcessador = 0;
    #ifdef DEBUG
        printf("task_create: %d está sendo criada. \n", task->tid);
    #endif //DEBUG print
    return task->tid;
}

//Função para troca de tarefa
int task_switch(task_t *task) {
    #ifdef DEBUG
        printf("task_switch:  %d -> %d\n", tarefaAtual->tid, task->tid);
    #endif //DEBUG print
    task_t *tarefaAnterior = tarefaAtual;
    tarefaAtual = task;
    tarefaAnterior->tempoTotalProcessador += mSegundos - tarefaAnterior->tempoLocal; // Tempo de processador é o tempo que ficou ativa a tarefa
    tarefaAtual->ativacoes++; 
    tarefaAtual->tempoLocal = mSegundos; //tempo local é sempre o tempo que a tarefa foi ativada pela ultima vez (para poder contar no tempo de processador)
    tarefaAtual->ticks = TICKS;
    if (swapcontext(&tarefaAnterior->context, &task->context) == 0) 
    {
        return 0;
    }
    else 
    {
        perror("Erro na troca de contextos");
        return -1;
    }   
}

//Função para encerrar uma tarefa
void task_exit(int exitCode) {
    #ifdef DEBUG
        printf("task_exit:  %d sendo encerrada. \n", task_id());
    #endif //DEBUG print
    printf("Task %d exit: running time: %u ms, cpu time: %u ms, %d acticvations\n", tarefaAtual->tid, mSegundos - tarefaAtual->tempoGlobal, tarefaAtual->tempoTotalProcessador, tarefaAtual->ativacoes );
    tarefaAtual->status = TERMINADO;
    // queue_remove((queue_t **) &tarefasProntas, (queue_t *) tarefaAtual);
    // userTasks--;
    // free(tarefaAtual->stack);   
    task_yield();
}

int task_id() {
    // #ifdef DEBUG
    //     printf("task_id \n");
    // #endif //DEBUG print
    return tarefaAtual->tid;
}

void task_yield() {
    #ifdef DEBUG
        printf("task_yield. \n");
    #endif //DEBUG print  
    task_switch(&dispatcher);
}

void task_suspend(task_t *task, task_t **queue) {
    #ifdef DEBUG
        printf("task_suspend: %d está sendo suspensa. \n", task->tid);
    #endif //DEBUG print   
    if (task == NULL) 
    { 
        task=tarefaAtual; 
    }
    if (queue == NULL) 
    { 
        return; 
    }
    queue_append((queue_t **) &tarefasSuspensas, (queue_t *) task);
    queue_remove((queue_t **) &tarefasProntas, (queue_t *) task);
    userTasks--;
    task->status = SUSPENSA;
}

void task_resume(task_t *task) {
    #ifdef DEBUG
        printf("task_resume: %d está voltando. \n", task->tid);
    #endif //DEBUG print
    queue_append((queue_t **) &tarefasProntas, (queue_t *) task);
    userTasks++;
    queue_remove((queue_t **) &tarefasSuspensas, (queue_t *) task);
    task->status = PRONTA;
}
