#include <windows.h>
#include <wingdi.h>

bool isRunning = true;

struct RenderState {
    void* memory;  
    int height, width;

    BITMAPINFO bitmapInfo;   
};

struct ImageState {
    HDC DC;
    HBITMAP bitmap;
    HBITMAP oldBitmap;
};

RenderState renderState;
ImageState imageState;

void loadImage(const char* pathname) {
    imageState.DC = CreateCompatibleDC(NULL);

    imageState.bitmap = (HBITMAP)LoadImageA(
        NULL, pathname,
        IMAGE_BITMAP, 0, 0,
        LR_DEFAULTSIZE | LR_LOADFROMFILE
    );

    imageState.oldBitmap = (HBITMAP)SelectObject(imageState.DC, imageState.bitmap);
}

void cleanImage() {
    SelectObject(imageState.DC, imageState.oldBitmap);
    DeleteObject(imageState.bitmap);
    DeleteDC(imageState.DC);
}

void drawImage(HDC screen) {
    BitBlt(screen, 0, 0, renderState.width, renderState.height, imageState.DC, 0, 0, SRCCOPY);
}

LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    
    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY: {
            isRunning = false;
            DeleteObject(imageState.bitmap);
            PostQuitMessage(0);
        } break;

        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            renderState.width = rect.right - rect.left;
            renderState.height = rect.bottom - rect.top;

            int bufferSize = renderState.width*renderState.height*sizeof(unsigned int);

            if (renderState.memory)
                VirtualFree(renderState.memory, 0, MEM_RELEASE);
            renderState.memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            renderState.bitmapInfo.bmiHeader.biSize = sizeof(renderState.bitmapInfo.bmiHeader);
            renderState.bitmapInfo.bmiHeader.biWidth = renderState.width;
            renderState.bitmapInfo.bmiHeader.biHeight = renderState.height;
            renderState.bitmapInfo.bmiHeader.biPlanes = 1;
            renderState.bitmapInfo.bmiHeader.biBitCount = 32;
            renderState.bitmapInfo.bmiHeader.biCompression = BI_RGB;
        } break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC screen = BeginPaint(hwnd,&ps);
            drawImage(screen);
            EndPaint(hwnd,&ps);
        } break;

        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }

            
    };
    
    return result;
}

HWND createWindow(HINSTANCE inst) {
    // Create Window Class
    WNDCLASSEX windowsClass = {0};
    windowsClass.cbSize = sizeof(WNDCLASSEX);
    windowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowsClass.hInstance = inst;
    windowsClass.style = CS_HREDRAW | CS_VREDRAW;
    windowsClass.lpszClassName = "Game Window Class";
    windowsClass.lpfnWndProc = wndProc;

    // Register Class
    RegisterClassEx(&windowsClass);

    int style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    RECT rc = {0,0,renderState.width, renderState.height};      // desired rect
    AdjustWindowRect(&rc,style,FALSE);

    // Create Window
    return CreateWindow
    (
    windowsClass.lpszClassName,
    "My Window",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    CW_USEDEFAULT, CW_USEDEFAULT,
    1280, 720, 0, 0, inst, 0
    );
    
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    HWND window = createWindow(hInstance);

    HDC hdc = GetDC(window);

    while(isRunning) {
        // Input
        MSG message;
        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        // Simulate
        unsigned int* pixel = (unsigned int*)renderState.memory;
        for(int y = 0; y < renderState.height; y++) {
            for(int x = 0; x < renderState.width; x++) {
                *pixel++ = 0xff00ff*x + 0xff00ff*y;
            }
        }

        // Render
        StretchDIBits
        (
            hdc, 0, 0, renderState.width, renderState.height,
            0, 0, renderState.width, renderState.height, renderState.memory,
            &renderState.bitmapInfo, DIB_RGB_COLORS, SRCCOPY
        );
    }

    return 0;
}