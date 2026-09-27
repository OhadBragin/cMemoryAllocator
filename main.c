#include <stdio.h>
#include <stdint.h>
#include "memalloc.h"


int main(void) {
    init_heap();
    char *buf = my_malloc(sizeof(char) * 50);
    my_free(buf);
}
