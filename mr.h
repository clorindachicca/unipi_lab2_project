//   ____________________________________________________________________________________________________________________________________________________
//
//   CONTIENE I PROTOTIPI DELLE FUNZIONI   [[ PARTE DEL PROGRAMMA            |   FILE DI IMPLEMENTAZIONE ]]
//   1) mr_attr_init ...................... [ PARAMETRI MAP REDUCE           |   mr_attr.c               ]
//   2) mr_attr_set_mapper_threads ........ [ PARAMETRI MAP REDUCE           |   mr_attr.c               ]
//   3) mr_attr_set_reducer_threads ....... [ PARAMETRI MAP REDUCE           |   mr_attr.c               ]
//   4) mr_attr_set_queue_size ............ [ PARAMETRI MAP REDUCE           |   mr_attr.c               ]
//   5) mr_attr_set_log_file .............. [ PARAMETRI MAP REDUCE           |   mr_attr.c               ]
//   6) mr_attr_destroy ................... [ PARAMETRI MAP REDUCE           |   mr_attr.c               ]
//   7) mr_create ......................... [ MAP REDUCE                     |   libmr.c                 ]
//   8) mr_start .......................... [ MAP REDUCE                     |   libmr.c                 ]
//   9) mr_destroy ........................ [ MAP REDUCE                     |   libmr.c                 ]
//
//   CONTIENE I TIPI                       [[ PARTE DEL PROGRAMMA            ]]
//   1) mr_attr_t ......................... [ PARAMETRI MAP REDUCE           ]
//   2) mr_file_line_t .................... [ PROCESSO PRINCIPALE -> MAPPER  ]
//   3) mr_mapper_t ....................... [ MAPPER                         ]
//   4) mr_value_t ........................ [ MAPPER -> REDUCER              ]
//   5) mr_emit_pair_t .................... [ MAPPER -> REDUCER              ]
//   6) mr_reducer_t ...................... [ REDUCER                        ]
//   7) mr_emit_result_t .................. [ REDUCER -> PROCESSO PRINCIPALE ]
//   8) mr_t .............................. [ MAP REDUCE                     ]
//   ____________________________________________________________________________________________________________________________________________________

#ifndef MR_HEADER
#define MR_HEADER

// libreria stddef.h della C Standard Library: per utilizzare il tipo size_t
#include <stddef.h>
// libreria sys/types.h della C Standard Library: per utilizzare il tipo ssize_t
#include <sys/types.h>



// NOTE: 7 DA TOGLIERE (reader_main, mr_pair_header_t, readn, writen, mapper_worker_main, reducer_worker_main, mr_result_t)
//       ORDINARE DICHIARAZIONI E TIPI COME NELL'HEADER DEL FILE



// PARAMETRI MAP REDUCE ------------------------------------------------------------------------------------------------------------------------------

// mr_attr_t: tipo di struct che memorizza i parametri di una singola computazione MapReduce
// 1) mapper_threads: numero di thread del processo mapper
// 2) reducer_threads: numero di thread del processo reducer
// 3) queue_size: dimensione dei buffer FIFO condivisi utilizzati dai thread del processo mapper e del processo reducer
// 4) log_file: nome del file di log
// - ogni campo ha un valore di default (MR_DEFAULT_MAPPER_THREADS, MR_DEFAULT_REDUCER_THREADS, MR_DEFAULT_QUEUE_SIZE, MR_DEFAULT_LOG_FILE)
//   definito in mr_settings.h, utilizzato se l'utente non inizializza esplicitamente la struct con le funzioni mr_attr_set_xxx
typedef struct {
    size_t mapper_threads;
    size_t reducer_threads;
    size_t queue_size;
    const char* log_file;
} mr_attr_t;

// mr_attr_init: funzione per inizializzare una struct di tipo mr_attr_t con i valori di default
int mr_attr_init (mr_attr_t* attr);

// mr_attr_set_xxx: funzioni per inizializzare i singoli campi di una struct di tipo mr_attr_t
int mr_attr_set_mapper_threads (
    mr_attr_t* attr,
    size_t n
);

int mr_attr_set_reducer_threads (
    mr_attr_t* attr,
    size_t n
);

int mr_attr_set_queue_size (
    mr_attr_t* attr,
    size_t n
);

int mr_attr_set_log_file (
    mr_attr_t* attr,
    const char* path
);

// mr_attr_destroy: funzione per rendere una struct di tipo mr_attr_t
// 1) anonima: cioè senza nessuna informazione sulla computazione MapReduce alla quale era associata
// 2) non riutilizzabile: cioè rendere necessario re-inizializzarla, chiamando di nuovo la funzione mr_attr_init, prima di riutilizzarla
int mr_attr_destroy (mr_attr_t* attr);



// PROCESSI "MAPPER" E "REDUCER" ---------------------------------------------------------------------------------------------------------------------

// SPOSTARE reader_main: codice del thread lettore dallo standard input, cioè thread scrittore sul buffer FIFO condiviso
// - implementazione uguale sia per il processo mapper sia per il processo reducer
static int reader_main (void* arg);

// SPOSTARE readn: funzione che legge un intero messaggio logico, chiamando più volte la system call read sul buffer buf
ssize_t readn (int fd, void* buf, size_t n);

// SPOSTARE writen: funzione che scrive un intero messaggio logico, chiamando più volte la system call write sul buffer buf
ssize_t writen (int fd, const void* buf, size_t n);



// PROCESSO "MAPPER" ---------------------------------------------------------------------------------------------------------------------------------

// SPOSTARE mapper_worker_main: thread lettore dal buffer FIFO condiviso
static int mapper_worker_main (void* arg);

// SPOSTARE mr_pair_header_t: tipo delle coppie (token, value) prodotte dai thread del processo mapper
typedef struct {
    int token_len;     // lunghezza in byte del token (escludendo il carattere di terminazione '\0', che deve essere riaggiunto dal processo reducer)
    int value_len;     // lunghezza in byte del valore opaco
} mr_pair_header_t;

// mr_emit_pair_t: tipo della funzione utilizzata da un thread del processo mapper per inviare al processo reducer una coppia (token, value)
// - se value_size == 0, allora value può essere NULL
typedef int (*mr_emit_pair_t) (
    const char* token,     // token: stringa che termina con '\0'
    const void* value,     // value: variabile di tipo opaco
    size_t value_size,     // value_size: dimensione in byte di value
    void* emit_arg         // emit_arg: input di tipo opaco della funzione
);



// FUNZIONE "MAPPER" ---------------------------------------------------------------------------------------------------------------------------------

// mr_file_line_t: tipo dell'input della funzione "mapper", rappresenta una riga logica
typedef struct {
    // nome e lunghezza in byte del file
    const char* file_name;         // non termina necessariamente con '\n' o '\0'
    size_t file_name_length;
    // indice a partire da 1, nome e lunghezza in byte della linea del file
    unsigned long int line_number;
    const char* line;              // non termina necessariamente con '\n' o '\0'
    size_t line_length;
} mr_file_line_t;

// mr_value_t: tipo opaco di value in un output <token, value> della funzione "mapper"
// - value viene considerato solo come un insieme "data" di byte di lunghezza "size"
typedef struct {
    const void* data;
    size_t size;
} mr_value_t;

// mr_mapper_t: tipo della funzione mapper
typedef int (*mr_mapper_t) (
    const mr_file_line_t* line,
    mr_emit_pair_t emit,
    void* emit_arg,
    void* user_arg
);



// PROCESSO "REDUCER" --------------------------------------------------------------------------------------------------------------------------------

// SPOSTARE reducer_worker_main: thread lettore dal buffer FIFO condiviso
static int reducer_worker_main (void* arg);

// mr_emit_result_t: tipo della funzione utilizzata da un thread del processo reducer per inviare al processo principale un output
// - se result_size == 0, allora result può essere NULL
typedef int (*mr_emit_result_t) (
    const char* token,      // token: stringa che termina con '\0'
    const void* result,     // result: variabile di tipo opaco
    size_t result_size,     // result_size: dimensione in byte di result
    void* emit_arg          // emit_arg: input di tipo opaco della funzione
);



// FUNZIONE "REDUCER" --------------------------------------------------------------------------------------------------------------------------------

// SPOSTARE mr_result_t: tipo opaco dell'output della funzione "reducer"
// - viene inviato al processo principale
typedef struct {
    const void* result;
    size_t size;            // size: dimensione in byte di result
} mr_result_t;

// mr_reducer_t: tipo della funzione reducer
typedef int (*mr_reducer_t) (
    const char* token,
    const mr_value_t* values,
    size_t values_count,
    mr_emit_result_t emit,
    void* emit_arg,
    void* user_arg
);



// MAP REDUCE ----------------------------------------------------------------------------------------------------------------------------------------

// mr_t: tipo opaco di una singola computazione MapReduce,
//       che ne nasconde l'implementazione aumentando la modularità del framework
typedef struct mr* mr_t;

// mr_create: funzione per allocare e inizializzare una computazione MapReduce
int mr_create (
    mr_t* mr,                  // mr: identificatore della computazione MapReduce
    const mr_attr_t* attr,     // attr: parametri di MapReduce
    mr_mapper_t mapper,        // mapper: funzione mapper
    mr_reducer_t reducer,      // reducer: funzione reducer
    void* user_arg             // input utente
);

// mr_start: funzione per eseguire una computazione MapReduce
int mr_start (
    mr_t mr,                     // mr: identificatore della computazione MapReduce
    const char* input_path,      // file o directory in input
    const char* output_path      // log dove memorizzare l'output
);

// mr_destroy: funzione per deallocare una computazione MapReduce
int mr_destroy (mr_t mr);



#endif
