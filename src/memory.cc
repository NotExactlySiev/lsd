#include <stdlib.h>
#include <stdio.h>

void* operator new(size_t sz) {
    return malloc(sz);
}

void operator delete(void* sz) {
    return free(sz);
}