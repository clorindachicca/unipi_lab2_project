//   ____________________________________________________________________________________________________________________________________________________
// 
//   CONTIENE LE IMPLEMENTAZIONI DELLE FUNZIONI
//   1) mr_attr_init
//   2) mr_attr_set_mapper_threads
//   3) mr_attr_set_reducer_threads
//   4) mr_attr_set_queue_size
//   5) mr_attr_set_log_file
//   6) mr_attr_destroy
//   ____________________________________________________________________________________________________________________________________________________

#include <errno.h>         // per utilizzare la variabile globale errno
#include "mr.h"            // per utilizzare il tipo mr_attr_t
#include "mr_settings.h"   // per utilizzare le macro di default MR_DEFAULT_XXX

// - tutte le funzioni controllano che attr sia un puntatore valido
// - le funzioni mr_attr_set_mapper_threads, mr_attr_set_reducer_threads e mr_attr_set_queue_size controllano che l'input n sia diverso da 0
//   - non controllano n < 0 perchè il tipo size_t equivale a long unsigned int e non assume mai valori negativi

int mr_attr_init (mr_attr_t* attr) {
    if (attr == NULL) {
        errno = EINVAL;
        return 1;
    }
    attr->mapper_threads = MR_DEFAULT_MAPPER_THREADS;
    attr->reducer_threads = MR_DEFAULT_REDUCER_THREADS;
    attr->queue_size = MR_DEFAULT_QUEUE_SIZE;
    attr->log_file = NULL;
    return 0;
}

int mr_attr_set_mapper_threads (mr_attr_t* attr, size_t n) {
    if ((attr == NULL) || (n == 0)) {
        errno = EINVAL;
        return 1;
    }
    attr->mapper_threads = n;
    return 0;
}

int mr_attr_set_reducer_threads (mr_attr_t* attr, size_t n) {
    if ((attr == NULL) || (n == 0)) {
        errno = EINVAL;
        return 1;
    }
    attr->reducer_threads = n;
    return 0;
}

int mr_attr_set_queue_size (mr_attr_t* attr, size_t n) {
    if ((attr == NULL) || (n == 0)) {
        errno = EINVAL;
        return 1;
    }
    attr->queue_size = n;
    return 0;
}

int mr_attr_set_log_file (mr_attr_t* attr, const char* path) {
    if (attr == NULL) {
        errno = EINVAL;
        return 1;
    }
    // se path è pari a NULL allora anche attr->log_file rimane pari a NULL
    attr->log_file = path;
    return 0;
}

int mr_attr_destroy (mr_attr_t* attr) {
    if (attr == NULL) {
        errno = EINVAL;
        return 1;
    }
    // la struct *attr perde ogni informazione sulla computazione MapReduce a cui è associata
    attr->mapper_threads = 0;
    attr->reducer_threads = 0;
    attr->queue_size = 0;
    attr->log_file = NULL;
    return 0;
}
