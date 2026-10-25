// main_win32.cpp — окно, которое показывает холст
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include "canvas.hpp"
#include "scene.hpp"

namespace {

doom::Canvas g_canvas(640, 400);      // кадр 640 × 400

// Растягивает холст на всё окно с сохранением пропорций.
void present(HWND hwnd, HDC dc) {
    RECT rc;
    GetClientRect(hwnd, &rc);
    const int cw = rc.right;
    const int ch = rc.bottom;
    int w = cw;
    int h = cw * g_canvas.height() / g_canvas.width();
    if (h > ch) {                     // окно слишком широкое
        h = ch;
        w = ch * g_canvas.width() / g_canvas.height();
    }
    const int x = (cw - w) / 2;
    const int y = (ch - h) / 2;
    PatBlt(dc, 0, 0, cw, ch, BLACKNESS);   // чёрные поля

    BITMAPINFO bmi{};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = g_canvas.width();
    bmi.bmiHeader.biHeight = g_canvas.height();   // забыли минус
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    SetStretchBltMode(dc, COLORONCOLOR);   // без размытия
    StretchDIBits(dc, x, y, w, h, 0, 0, g_canvas.width(),
                  g_canvas.height(), g_canvas.data(), &bmi,
                  DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wp,
                            LPARAM lp) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        const HDC dc = BeginPaint(hwnd, &ps);
        present(hwnd, dc);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_ERASEBKGND:
        return 1;                     // фон рисуем сами
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

} // namespace

int WINAPI wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE,
                    _In_ PWSTR, _In_ int show) {
    doom::drawScene(g_canvas);        // рисуем один раз заранее

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;  // перерисовать целиком
    wc.lpfnWndProc = windowProc;
    wc.hInstance = instance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.lpszClassName = L"MiniDoomWindow";
    if (RegisterClassExW(&wc) == 0) {
        return 1;
    }
    RECT rect{0, 0, 1280, 800};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    const HWND hwnd = CreateWindowExW(
        0, wc.lpszClassName, L"MINI DOOM", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left,
        rect.bottom - rect.top, nullptr, nullptr, instance, nullptr);
    if (hwnd == nullptr) {
        return 1;
    }
    ShowWindow(hwnd, show);

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return static_cast<int>(msg.wParam);
}
