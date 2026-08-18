//   ____________________________________________________________________________________________________________________________________________________
//   
//   CONTIENE LE MACRO
//   1) MR_LOG_MAIN_PROCESS_NAME
//   2) MR_LOG_MAPPER_PROCESS_NAME
//   3) MR_LOG_REDUCER_PROCESS_NAME
//   4) MR_LOG_THREAD_NAME
//   5) MR_LOG_LOGGER_THREAD_NAME
//   6) MR_LOG_SEMAPHORE_NAME_SIZE
//   
//   CONTIENE I TIPI
//   1) mr_log_t
//   
//   CONTIENE I PROTOTIPI DELLE FUNZIONI
//   1) mr_log_init
//   2) mr_log_child
//   3) mr_logger_thread_start
//   4) mr_log_event
//   5) mr_log_destroy
//   ____________________________________________________________________________________________________________________________________________________

#ifndef MR_LOG_H
#define MR_LOG_H

#include <stddef.h>      // per utilizzare il tipo size_t
#include <stdbool.h>     // per utilizzare il tipo bool
#include <stdio.h>       // per utilizzare la funzione snprintf
#include <sys/types.h>   // per utilizzare il tipo pid_t
#include <threads.h>     // per utilizzare il tipo thrd_t
#include <semaphore.h>   // per utilizzare il tipo sem_t

// nomi di default dei processi di una computazione MapReduce
#define MR_LOG_MAIN_PROCESS_NAME "MAIN PROCESS"
#define MR_LOG_MAPPER_PROCESS_NAME "MAPPER PROCESS"
#define MR_LOG_REDUCER_PROCESS_NAME "REDUCER PROCESS"

/*// SIZE_T_AS_STRING_SIZE: numero massimo di caratteri necessari per rappresentare un valore di tipo size_t come stringa
// - {(#byte del tipo size_t) * (8 bit/byte) * ceil[log_10(2)]} + (#1 byte per il carattere di terminazione'\0')
// - nella macro MR_LOG_THREAD_NAME: se sizeof(str) è pari a (7 + SIZE_T_AS_STRING_SIZE), allora la macro MR_LOG_THREAD_NAME non fallisce mai
#define SIZE_T_AS_STRING_SIZE ((int) (sizeof(size_t) * 8 * 0.30103) + 1)*/

// MR_LOG_THREAD_NAME: nome di default di un thread generico (con indice) in un processo di una computazione MapReduce
#define MR_LOG_THREAD_NAME "THREAD %d"

// MR_LOG_LOGGER_THREAD_NAME: nome di default del thread logger nel processo principale
#define MR_LOG_LOGGER_THREAD_NAME "LOGGER THREAD"

// MR_LOG_SEMAPHORE_NAME_SIZE: dimensione massima in byte di un nome di un semaforo in una struct di tipo mr_log_t
#define MR_LOG_SEMAPHORE_NAME_SIZE ((size_t) 64)

// mr_log_t: tipo di struct che memorizza le informazioni di logging di una singola computazione MapReduce
typedef struct {

    // pipe di log, file di log e metadati della struct
    int read_side_pipe;                          // pipe di log in lettura: utilizzata solo dal thread logger del processo principale
    int write_side_pipe;                         // pipe di log in scrittura: utilizzata da tutti i processi e da tutti i thread
    int fd;                                      // descrittore del file di log
    pid_t owner_process;                         // PID del processo principale
    bool initialized;                            // indica se la struct è stata inizializzata

    // thread logger del processo principale
    thrd_t logger_thread;                        // descrittore del thread logger del processo principale
    bool logger_thread_started;                  // indica se il thread logger è stato creato
    int logger_thread_result;                    // valore in output del thread logger del processo principale

    // sincronizzazione del logging
    char sem_name[MR_LOG_SEMAPHORE_NAME_SIZE];   // nome del semaforo di sincronizzazione del logging
    sem_t* sem;                                  // semaforo con nome che sincronizza il logging tra i processi
    
} mr_log_t;

// mr_log_init: funzione per inizializzare una struct "log" di tipo mr_log_t e aprire un file di log "path"
// - se il file "path" non esiste: viene creato
// - se il file "path" esiste già: la scrittura inizia dalla fine del file
// - quindi si utilizza la modalità append+: uno stesso file di log può essere utilizzato per tenere traccia di più computazioni MapReduce
//   eseguite in modo sequenziale (ma non concorrente / parallelo)
int mr_log_init (mr_log_t* log, const char* path);

// mr_log_child: funzione per rendere utilizzabile una struct di tipo mr_log_t sia dal processo mapper sia dal processo reducer
// - chiude file e pipe inutili al processo mapper e al processo reducer
int mr_log_child (mr_log_t* log);

// mr_logger_thread_start: funzione per creare il thread logger del processo principale
int mr_log_logger_thread_start (mr_log_t* log);

// mr_log_write_record: funzione per scrivere un record sulla pipe di log
// - la scrittura effettiva del record sul file di log è gestita dal thread logger del processo principale, che è in lettura sulla pipe di log
int mr_log_write_record (
    const mr_log_t* log,
    const char* process_name,
    const char* thread_name,
    const char* event_name,
    const char* message
);

// mr_log_destroy: funzione per rendere una struct di tipo mr_log_t
// 1) anonima: cioè senza nessuna informazione sulla computazione MapReduce alla quale era associata
// 2) non riutilizzabile: cioè rendere necessario re-inizializzarla prima di riutilizzarla, chiamando di nuovo la funzione mr_log_init
int mr_log_destroy (mr_log_t* log);

#endif
