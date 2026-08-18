#ifndef MR_INTERNAL_STATE
#define MR_INTERNAL_STATE

// libreria stddef.h della C Standard Library: per utilizzare il tipo size_t
#include <stddef.h>
// libreria mr.h: per utilizzare i tipi mr_mapper_t e mr_reducer_t
#include "mr.h"

// mr_state_t: tipo che descrive lo stato di una singola computazione MapReduce con 3 possibili valori interi
typedef enum {
    MR_READY,        // MR_READY equivale a 0
    MR_RUNNING,      // MR_RUNNING equivale a 1
    MR_DESTROYED     // MR_DESTROYED equivale a 2
} mr_state_t;

// mr: struct che descrive una singola computazione MapReduce
// - passata come primo input alla funzione mr_crate
struct mr {
    // parametri di MapReduce: dalla struct attr in input alla funzione mr_create
    size_t mapper_threads;
    size_t reducer_threads;
    size_t queue_size;
    char* log_file;
    // parametri di MapReduce: dai terz'ultimi input della funzione mr_create
    mr_mapper_t mapper;
    mr_reducer_t reducer;
    void* user_arg;
    // stato di MapReduce
    mr_state_t state;
};

#endif
