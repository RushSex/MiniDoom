// main_win32.cpp — игровой цикл: события, время, мир, кадр
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <mmsystem.h>

#include "canvas.hpp"

#include <algorithm>
#include <chrono>
#include <format>

#ifdef _MSC_VER
#pragma comment(lib, "winmm.lib")   // timeBeginPeriod живёт тут
#endif

namespace {

doom::Canvas g_canvas(640, 400);

struct Box {                        // «игрок» пока — квадрат
    double x = 100.0;
    double y = 180.0;
    double vx = 220.0;              // скорость, пикселей в секунду
    double vy = 140.0;
    int size = 40;
};

// Отскок: вылетели за край — вернуться и развернуться.
void bounce(double& pos, double& vel, double max) {
    if (pos < 0.0) {
        pos = 0.0;
        vel = -vel;
    } else if (pos > max) {
        pos = max;
        vel = -vel;
    }
}

void update(Box& b, double dt) {
    b.x += b.vx * dt;               // путь = скорость × время
    b.y += b.vy * dt;
    bounce(b.x, b.vx, g_canvas.width() - b.size);
    bounce(b.y, b.vy, g_canvas.height() - b.size);
}

void render(const Box& b) {
    g_canvas.clear(doom::rgb(20, 20, 28));
    g_canvas.fillRect(static_cast<int>(b.x), static_cast<int>(b.y),
                      b.size, b.size, doom::rgb(230, 70, 20));
}

void present(HWND hwnd, HDC dc) {
    RECT rc;
    GetClientRect(hwnd, &rc);
    const int cw = rc.right;
    const int ch = rc.bottom;
    int w = cw;
    int h = cw * g_canvas.height() / g_canvas.width();
    if (h > ch) {
        h = ch;
        w = ch * g_canvas.width() / g_canvas.height();
    }
    const int x = (cw - w) / 2;
    const int y = (ch - h) / 2;
    PatBlt(dc, 0, 0, cw, y, BLACKNESS);           // поля — только
    PatBlt(dc, 0, y + h, cw, ch - y - h, BLACKNESS);  // вокруг
    PatBlt(dc, 0, y, x, h, BLACKNESS);            // картинки,
    PatBlt(dc, x + w, y, cw - x - w, h, BLACKNESS);   // без мигания

    BITMAPINFO bmi{};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = g_canvas.width();
    bmi.bmiHeader.biHeight = -g_canvas.height();
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    SetStretchBltMode(dc, COLORONCOLOR);
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
        return 1;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

HWND createWindow(HINSTANCE instance, int show) {
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = windowProc;
    wc.hInstance = instance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.lpszClassName = L"MiniDoomWindow";
    if (RegisterClassExW(&wc) == 0) {
        return nullptr;
    }
    RECT rect{0, 0, 1280, 800};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    const HWND hwnd = CreateWindowExW(
        0, wc.lpszClassName, L"MINI DOOM", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left,
        rect.bottom - rect.top, nullptr, nullptr, instance, nullptr);
    if (hwnd != nullptr) {
        ShowWindow(hwnd, show);
    }
    return hwnd;
}

} // namespace

int WINAPI wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE,
                    _In_ PWSTR, _In_ int show) {
    const HWND hwnd = createWindow(instance, show);
    if (hwnd == nullptr) {
        return 1;
    }
    timeBeginPeriod(1);             // Sleep(1) спит ~1 мс, а не 15

    using Clock = std::chrono::steady_clock;
    auto last = Clock::now();
    auto fpsStart = last;
    int frames = 0;
    Box box;
    bool running = true;
    while (running) {
        MSG msg;                    // 1. разобрать ВСЕ сообщения
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
            }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        const auto now = Clock::now();   // 2. сколько прошло
        using Seconds = std::chrono::duration<double>;
        double dt = Seconds(now - last).count();
        last = now;
        dt = std::min(dt, 0.05);    // после паузы не телепортируемся

        update(box, dt);            // 3. мир
        render(box);                // 4. кадр
        const HDC dc = GetDC(hwnd);
        present(hwnd, dc);
        ReleaseDC(hwnd, dc);

        ++frames;                   // 5. счётчик кадров в заголовок
        if (now - fpsStart >= std::chrono::seconds(1)) {
            const auto title =
                std::format(L"MINI DOOM — {} FPS", frames);
            SetWindowTextW(hwnd, title.c_str());
            frames = 0;
            fpsStart = now;
        }
        Sleep(1);                   // отдать процессор другим
    }
    timeEndPeriod(1);
    return 0;
}
