#include <stdio.h>
#include <string.h>

#include "memalloc.h"

int main(void) {
    printf("Demonstrating custom memory allocator:\n\n");

    /* 1. my_malloc */
    char *str = (char *)my_malloc(32 * sizeof(char));
    if (str != NULL) {
        strcpy(str, "Hello, Memory Allocator!");
        printf("Allocated string: %s\n", str);

        /* 2. my_realloc */
        str = (char *)my_realloc(str, 64 * sizeof(char));
        if (str != NULL) {
            strcat(str, " Reallocated.");
            printf("After realloc:   %s\n", str);
        }
        my_free(str);
        printf("Freed string buffer.\n\n");
    }

    /* 3. my_calloc */
    size_t count = 5;
    size_t *numbers = (size_t *)my_calloc(count, sizeof(size_t));
    if (numbers != NULL) {
        printf("Allocated calloc array (size %zu):\n", count);
        for (size_t i = 0; i < count; i++) {
            printf("  numbers[%zu] = %zu\n", i, numbers[i]);
            numbers[i] = (i + 1) * 10;
        }
        my_free(numbers);
        printf("Freed numbers array.\n");
    }

    return 0;
}
