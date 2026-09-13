#include <windows.h>

#pragma comment(lib, "user32.lib")

const char CLASS_NAME[] = "HelloWorld500";

int windowCount = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT rect;
        GetClientRect(hwnd, &rect);

        // 800 строк Hello World
        rect.top = 5;

        for (int i = 0; i < 800; ++i)
        {
            RECT line = rect;
            line.top = 5 + i * 18;
            line.bottom = line.top + 18;

            DrawTextA(
                hdc,
                "Hello World",
                -1,
                &line,
                DT_CENTER | DT_SINGLELINE
            );
        }

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_CLOSE:
        // Закрывается только это конкретное окно
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        --windowCount;

        if (windowCount == 0)
            PostQuitMessage(0);

        return 0;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    LPSTR,
    int nCmdShow
)
{
    WNDCLASSA wc = {};

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassA(&wc))
    {
        MessageBoxA(
            nullptr,
            "Failed to register window class.",
            "Error",
            MB_OK | MB_ICONERROR
        );

        return 1;
    }

    // Создаём 500 отдельных окон
    for (int i = 0; i < 500; ++i)
    {
        int x = 20 + (i % 10) * 35;
        int y = 20 + (i % 10) * 35;

        HWND hwnd = CreateWindowExA(
            0,
            CLASS_NAME,
            "Hello World",
            WS_OVERLAPPEDWINDOW,
            x,
            y,
            500,
            300,
            nullptr,
            nullptr,
            hInstance,
            nullptr
        );

        if (hwnd)
        {
            ++windowCount;

            ShowWindow(hwnd, nCmdShow);
            UpdateWindow(hwnd);
        }
    }

    if (windowCount == 0)
    {
        MessageBoxA(
            nullptr,
            "No windows were created.",
            "Error",
            MB_OK | MB_ICONERROR
        );

        return 1;
    }

    MSG msg = {};

    while (GetMessageA(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return 0;
}
