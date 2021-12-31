#include <windows.h>

bool isRunning = true;

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    
    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY: {
            isRunning = false;
        } break;

        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }

            
    };
    
    return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    // Create Window Class
    WNDCLASS windows_class = {};
    windows_class.style = CS_HREDRAW | CS_VREDRAW;
    windows_class.lpszClassName = "Game Window Class";
    windows_class.lpfnWndProc = window_callback;

    // Register Class
    RegisterClass(&windows_class);

    HWND window = CreateWindow(windows_class.lpszClassName, "My First Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);

    while(isRunning) {
        // Input
        MSG message;
        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        // Simulate

        // Render
    }

    return 0;
}