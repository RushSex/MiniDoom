// main_win32.cpp — первое окно MINI DOOM
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

namespace {

// Оконная процедура: Windows зовёт её на каждое событие окна.
LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wp,
                            LPARAM lp) {
    switch (msg) {
    case WM_KEYDOWN:                      // нажата клавиша
        if (wp == VK_ESCAPE) {
            DestroyWindow(hwnd);          // закрываем окно
            return 0;
        }
        break;
    case WM_PAINT: {                      // окно просит перерисовку
        PAINTSTRUCT ps;
        const HDC dc = BeginPaint(hwnd, &ps);
        const auto black =
            static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
        FillRect(dc, &ps.rcPaint, black);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:                      // окно закрыли
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);   // всё прочее
}

} // namespace

int WINAPI wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE,
                    _In_ PWSTR, _In_ int show) {
    WNDCLASSEXW wc{};                     // 1. описываем класс окна
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = windowProc;
    wc.hInstance = instance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.lpszClassName = L"MiniDoomWindow";
    if (RegisterClassExW(&wc) == 0) {
        return 1;
    }

    RECT rect{0, 0, 1280, 800};           // 2. размер картинки
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    const HWND hwnd = CreateWindowExW(
        0, wc.lpszClassName, L"MINI DOOM", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left,
        rect.bottom - rect.top, nullptr, nullptr, instance, nullptr);
    if (hwnd == nullptr) {
        return 1;
    }
    ShowWindow(hwnd, show);

    MSG msg{};                            // 3. цикл сообщений
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return static_cast<int>(msg.wParam);
}
