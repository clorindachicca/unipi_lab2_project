//   ____________________________________________________________________________________________________________________________________________________
//
//   CONTIENE LE MACRO                     [[ PARTE DEL PROGRAMMA           ]]
//   1) MR_DEFAULT_MAPPER_THREADS ......... [ PARAMETRI MAP REDUCE          ]
//   2) MR_DEFAULT_REDUCER_THREADS ........ [ PARAMETRI MAP REDUCE          ]
//   3) MR_DEFAULT_QUEUE_SIZE ............. [ PARAMETRI MAP REDUCE          ]
//   4) MR_DEFAULT_LOG_FILE ............... [ PARAMETRI MAP REDUCE          ]
//   5) MR_MAX_FILE_NAME_SIZE ............. [ PROCESSO PRINCIPALE -> MAPPER ]
//   6) MR_MAX_LINE_SIZE .................. [ PROCESSO PRINCIPALE -> MAPPER ]
//   7) MR_MAX_TOKEN_SIZE ................. [ MAPPER -> REDUCER             ]
//   8) MR_MAX_VALUE_SIZE ................. [ MAPPER -> REDUCER             ]
//   9) MR_MAX_RESULT_SIZE ................ [ REDUCER                       ]
//   ____________________________________________________________________________________________________________________________________________________

#ifndef MR_SETTINGS_H
#define MR_SETTINGS_H

#define MR_CHILD_PROCESSES_COUNT ((size_t) 2);

// 1) numero di default di thread worker del processo mapper: 8
// 2) numero di default di thread worker del processo reducer: 8
// 3) capacità di default dei buffer FIFO del framework: 128
// 4) nome di default del file di log del framework: mr.log
#define MR_DEFAULT_MAPPER_THREADS ((size_t) 8)
#define MR_DEFAULT_REDUCER_THREADS ((size_t) 8)
#define MR_DEFAULT_QUEUE_SIZE ((size_t) 128)
#define MR_DEFAULT_LOG_FILE "mr.log"



// dimensione massima in byte di
// 1) un nome logico di un file: 4 KB
// 2) una linea logica: 1 MB
// 3) un token logico: 4 KB
// 4) un valore associato a un token: 1 MB
// 5) un risultato: 1 MB
// - la dimensione massima, nei casi logici 1-2-3, non include il carattere di terminazione '\0' (che deve essere aggiunto successivamente dal framework)
#define MR_MAX_FILE_NAME_SIZE ((size_t) (4 * 1024))
#define MR_MAX_LINE_SIZE ((size_t) (1 * 1024 * 1024)) 
#define MR_MAX_TOKEN_SIZE ((size_t) (4 * 1024))
#define MR_MAX_VALUE_SIZE ((size_t) (1 * 1024 * 1024))
#define MR_MAX_RESULT_SIZE ((size_t) (1 * 1024 * 1024))



#endif
