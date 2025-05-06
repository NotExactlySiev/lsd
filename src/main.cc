
#include "gpu.hh"
#include "pad.hh"

#include <stdio.h>
#include <stdlib.h>

GPU* gpu;

void callback(void) {
    printf("gpu!\n");
    gpu->Swap();
}

extern unsigned long *__heap_start;

int main()
{
    InitHeap(__heap_start, 1024*32);
    
    gpu = new GPU();
        
    Rect2D screen = { 256, 240 };
    gpu->Init(&screen, 0);
    gpu->SetCallback(callback);
    gpu->Start();    
}