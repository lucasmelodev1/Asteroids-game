#include <windows.h>

struct RenderState {
    void* memory;  
    int height, width;

    BITMAPINFO bitmapInfo;   
};

RenderState renderState;