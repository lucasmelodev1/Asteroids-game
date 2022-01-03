#include "image.h"
#include "render.h"

#include <windows.h>

void ImageState::loadImage(const char* pathname) {
    DC = CreateCompatibleDC(NULL);

    bitmap = (HBITMAP)LoadImageA(
        NULL, pathname,
        IMAGE_BITMAP, 0, 0,
        LR_DEFAULTSIZE | LR_LOADFROMFILE
    );

    oldBitmap = (HBITMAP)SelectObject(DC, bitmap);
};

void ImageState::cleanImage() {
    SelectObject(DC, oldBitmap);
    DeleteObject(bitmap);
    DeleteDC(DC);
};

void ImageState::drawImage(HDC screen, RenderState renderState) {
    BitBlt(screen, 0, 0, renderState.width, renderState.height, DC, 0, 0, SRCCOPY);
};