// main_win32.cpp — окно из главы 13; новые клавиши и карта
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <mmsystem.h>

#include "canvas.hpp"
#include "game.hpp"
#include "hud.hpp"
#include "input.hpp"
#include "level.hpp"
#include "map.hpp"

#include <algorithm>
#include <array>
#include <chrono>

#ifdef _MSC_VER
#pragma comment(lib, "winmm.lib")
#endif

namespace {

doom::Canvas g_canvas(640, 400);
HWND g_hwnd = nullptr;
std::array<bool, 256> g_down{};
bool g_click = false;
int g_mouseDX = 0;
bool g_captured = false;

void captureMouse() {
    if (g_captured) {
        return;
    }
    RECT rc;
    GetClientRect(g_hwnd, &rc);
    auto* corners = reinterpret_cast<POINT*>(&rc);
    MapWindowPoints(g_hwnd, nullptr, corners, 2);
    ClipCursor(&rc);
    ShowCursor(FALSE);
    g_captured = true;
}

void releaseMouse() {
    if (!g_captured) {
        return;
    }
    ClipCursor(nullptr);
    ShowCursor(TRUE);
    g_captured = false;
}

void present(HDC dc) {
    RECT rc;
    GetClientRect(g_hwnd, &rc);
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
    PatBlt(dc, 0, 0, cw, y, BLACKNESS);
    PatBlt(dc, 0, y + h, cw, ch - y - h, BLACKNESS);
    PatBlt(dc, 0, y, x, h, BLACKNESS);
    PatBlt(dc, x + w, y, cw - x - w, h, BLACKNESS);
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

void onRawInput(LPARAM lp) {
    RAWINPUT raw{};
    UINT size = sizeof(raw);
    const auto handle = reinterpret_cast<HRAWINPUT>(lp);
    const UINT got = GetRawInputData(handle, RID_INPUT, &raw, &size,
                                     sizeof(RAWINPUTHEADER));
    if (got == static_cast<UINT>(-1)) {
        return;
    }
    if (raw.header.dwType == RIM_TYPEMOUSE && g_captured) {
        g_mouseDX += raw.data.mouse.lLastX;
    }
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wp,
                            LPARAM lp) {
    switch (msg) {
    case WM_KEYDOWN:
        g_down[static_cast<std::size_t>(wp & 0xFF)] = true;
        if (wp == VK_ESCAPE) {
            releaseMouse();
        }
        return 0;
    case WM_KEYUP:
        g_down[static_cast<std::size_t>(wp & 0xFF)] = false;
        return 0;
    case WM_LBUTTONDOWN:
        if (g_captured) {
            g_click = true;
        } else {
            captureMouse();
        }
        return 0;
    case WM_INPUT:
        onRawInput(lp);
        break;
    case WM_ACTIVATEAPP:
        if (wp == FALSE) {
            releaseMouse();
            g_down.fill(false);
        }
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        const HDC dc = BeginPaint(hwnd, &ps);
        present(dc);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_ERASEBKGND:
        return 1;
    case WM_DESTROY:
        releaseMouse();
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

doom::Input collectInput() {
    const auto key = [](int vk) {
        return g_down[static_cast<std::size_t>(vk)];
    };
    doom::Input in;
    in.forward = key('W') || key(VK_UP);
    in.back = key('S') || key(VK_DOWN);
    in.strafeLeft = key('A');
    in.strafeRight = key('D');
    in.turnLeft = key(VK_LEFT);
    in.turnRight = key(VK_RIGHT);
    in.run = key(VK_SHIFT);
    in.fire = g_click;
    in.mouseDX = g_mouseDX;
    g_click = false;
    g_mouseDX = 0;
    return in;
}

bool createWindow(HINSTANCE instance, int show) {
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = windowProc;
    wc.hInstance = instance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.lpszClassName = L"MiniDoomWindow";
    if (RegisterClassExW(&wc) == 0) {
        return false;
    }
    RECT rect{0, 0, 1280, 800};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    g_hwnd = CreateWindowExW(
        0, wc.lpszClassName, L"MINI DOOM", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left,
        rect.bottom - rect.top, nullptr, nullptr, instance, nullptr);
    if (g_hwnd == nullptr) {
        return false;
    }
    RAWINPUTDEVICE mouse{};
    mouse.usUsagePage = 0x01;
    mouse.usUsage = 0x02;
    mouse.hwndTarget = g_hwnd;
    RegisterRawInputDevices(&mouse, 1, sizeof(mouse));
    ShowWindow(g_hwnd, show);
    return true;
}

} // namespace

int WINAPI wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE,
                    _In_ PWSTR, _In_ int show) {
    if (!createWindow(instance, show)) {
        return 1;
    }
    timeBeginPeriod(1);
    doom::Game game{doom::Map{doom::kLevel}};
    using Clock = std::chrono::steady_clock;
    using Seconds = std::chrono::duration<double>;
    auto last = Clock::now();
    auto fpsStart = last;
    int frames = 0;
    int fps = 0;                    // показанное значение FPS
    bool running = true;
    while (running) {
        MSG msg;
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
            }
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        const auto now = Clock::now();
        const double dt =
            std::min(Seconds(now - last).count(), 0.05);
        last = now;
        ++frames;
        if (now - fpsStart >= std::chrono::seconds(1)) {
            fps = frames;           // обновляем раз в секунду,
            frames = 0;             // иначе цифры мельтешат
            fpsStart = now;
        }

        game.update(collectInput(), dt);
        game.render(g_canvas);
        doom::drawHud(g_canvas, game, fps);
        const HDC dc = GetDC(g_hwnd);
        present(dc);
        ReleaseDC(g_hwnd, dc);
        Sleep(1);
    }
    timeEndPeriod(1);
    return 0;
}
