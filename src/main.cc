
#include "gpu.hh"
#include "pad.hh"

extern "C" {
//#include <stdio.h>
//#include <stdlib.h>
#include <malloc.h>
}

GPU* gpu;

void callback(void) {
    //printf("gpu!\n");
    //gpu->Swap();
}

extern unsigned long *__heap_start;

int main()
{
#ifdef PSX
    InitHeap(__heap_start, 1024*32);
#endif
    gpu = new GPU();
        
    Rect2D screen = { 256, 240 };
    gpu->Init(&screen, 0);
    gpu->SetCallback(callback);
    gpu->Start();    
}