#ifndef _IMAGE_H
#define _IMAGE_H
#include <windows.h>
#include "render.h"

class ImageState {
    public:
        HDC DC;
        HBITMAP bitmap;
        HBITMAP oldBitmap;

    public:
        void loadImage(const char* pathname);
        void drawImage(HDC screen, RenderState renderState);
        void cleanImage();
};

#endif