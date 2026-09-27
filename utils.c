#include "memalloc.h"
#include <stdio.h>
#include <stdlib.h>

const char *ErrorCode_to_text(ErrorCode err) {
    switch (err) {
        case ERR_OUT_OF_MEMEORY: return "FATAL: OUT OF HEAP MEMORY!\n";
        case ERR_INVALID_ADDRESS: return "Not a valid pointer.\nYou must use the pointer returned by my_malloc()!\n";
    }
    return "Unknown Error.";
}