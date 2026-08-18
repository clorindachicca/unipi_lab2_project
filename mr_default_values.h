#ifndef MR_DEFAULT_VALUES_H
#define MR_DEFAULT_VALUES_H

#include <stddef.h>

#define MR_MAX_FILE_NAME_SIZE ((size_t)4096)
#define MR_MAX_LINE_SIZE      ((size_t)(16U * 1024U * 1024U))
#define MR_MAX_TOKEN_SIZE     ((size_t)(64U * 1024U))
#define MR_MAX_VALUE_SIZE     ((size_t)(64U * 1024U * 1024U))
#define MR_MAX_RESULT_SIZE    ((size_t)(64U * 1024U * 1024U))

#define MR_DEFAULT_MAPPER_THREADS  ((size_t)1)
#define MR_DEFAULT_REDUCER_THREADS ((size_t)1)
#define MR_DEFAULT_QUEUE_SIZE      ((size_t)64)
#define MR_DEFAULT_LOG_FILE        "mr.log"

#endif
