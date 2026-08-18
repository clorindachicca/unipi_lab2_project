//   ____________________________________________________________________________________________________________________________________________________
//   
//   CONTIENE LE IMPLEMENTAZIONI DELLE FUNZIONI
//   1) mr_threads_to_errno
//   ____________________________________________________________________________________________________________________________________________________

#include <threads.h>
#include <errno.h>
#include "mr_errors.h"

int mr_threads_to_errno (int threads_error_code) {
    switch (threads_error_code) {
        case thrd_success:
            return 0;
        case thrd_nomem:
            return ENOMEM;
        case thrd_timedout:
            return ETIMEDOUT;
        case thrd_busy:
            return EBUSY;
        case thrd_error:
        default:
            return EIO;
    }
}
