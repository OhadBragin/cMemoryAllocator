#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "memalloc.h"


int main(void) {
    char *buf = my_malloc(sizeof(char) * 20);
    my_free(buf);
    size_t *buf3 = my_calloc(5, sizeof(size_t));
    buf = my_malloc(sizeof(char) * 20);
    strcpy(buf, "Hello, World!");
}
