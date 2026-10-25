// main_win32.cpp — окно из главы 27; уровни из файлов рядом с exe
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <mmsystem.h>

#include "app.hpp"
#include "audio.hpp"
#include "canvas.hpp"
#include "input.hpp"
#include "mixer.hpp"
#include "synth.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <exception>
#include <filesystem>
#include <cwchar>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#ifdef _MSC_VER
#pragma comment(lib, "winmm.lib")
#endif

namespace {

doom::Canvas g_canvas(640, 400);
HWND g_hwnd = nullptr;
std::array<bool, 256> g_down{};
bool g_fire = false;            // левая кнопка зажата
int g_weapon = 0;               // нажали 1 или 2
bool g_tab = false;             // Tab нажали в этом кадре
bool g_use = false;             // E или пробел
bool g_confirm = false;         // Enter
bool g_pause = false;           // Esc
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
        if ((lp & (1 << 30)) == 0) {    // не автоповтор
            g_tab = g_tab || wp == VK_TAB;
            g_use = g_use || wp == 'E' || wp == VK_SPACE;
            g_confirm = g_confirm || wp == VK_RETURN;
            if (wp == '1' || wp == '2') {
                g_weapon = static_cast<int>(wp - '0');
            }
        }
        if (wp == VK_ESCAPE && (lp & (1 << 30)) == 0) {
            g_pause = true;
        }
        return 0;
    case WM_KEYUP:
        g_down[static_cast<std::size_t>(wp & 0xFF)] = false;
        return 0;
    case WM_LBUTTONDOWN:
        if (g_captured) {
            g_fire = true;
        } else {
            captureMouse();
        }
        return 0;
    case WM_LBUTTONUP:
        g_fire = false;
        return 0;
    case WM_INPUT:
        onRawInput(lp);
        break;
    case WM_ACTIVATEAPP:
        if (wp == FALSE) {
            releaseMouse();
            g_down.fill(false);
            g_fire = false;
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
    in.fire = g_fire || key(VK_CONTROL);
    in.weapon = g_weapon;
    in.map = g_tab;
    in.use = g_use;
    in.confirm = g_confirm;
    in.pause = g_pause;
    in.mouseDX = g_mouseDX;
    g_weapon = 0;
    g_tab = false;
    g_use = false;
    g_confirm = false;
    g_pause = false;
    g_mouseDX = 0;
    return in;
}

// Папка, где лежит наш .exe. Не путать с текущей папкой процесса!
std::filesystem::path exeDir() {
    std::wstring path(MAX_PATH, L'\0');
    const DWORD n = GetModuleFileNameW(
        nullptr, path.data(), static_cast<DWORD>(path.size()));
    path.resize(n);
    return std::filesystem::path(path).parent_path();
}

// «-warp 2» в командной строке — сразу на второй уровень, как
// в Doom. Возвращает номер уровня с единицы или 0, если ключа нет.
long warpLevel(std::wstring_view cmd) {
    constexpr std::wstring_view key = L"-warp ";
    const auto pos = cmd.find(key);
    if (pos == std::wstring_view::npos) {
        return 0;
    }
    return std::wcstol(cmd.data() + pos + key.size(), nullptr, 10);
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
                    _In_ PWSTR cmdLine, _In_ int show) {
    std::vector<doom::Level> levels;
    try {
        levels = doom::loadLevels(exeDir());
    } catch (const std::exception& e) {    // нет уровней — нет игры
        MessageBoxA(nullptr, e.what(), "MINI DOOM", MB_ICONERROR);
        return 1;
    }
    if (!createWindow(instance, show)) {
        return 1;
    }
    timeBeginPeriod(1);
    const doom::SoundBank bank;         // синтез всех звуков
    doom::Mixer mixer(bank);
    doom::AudioOut audio(mixer);        // отсюда звучит второй поток
    doom::App app(std::move(levels));
    if (const long warp = warpLevel(cmdLine); warp > 0) {
        app.warp(static_cast<std::size_t>(warp - 1));
    }
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

        app.update(collectInput(), dt);
        for (const doom::SoundEvent& e : app.takeSounds()) {
            mixer.play(e.id, e.volume);
        }
        if (app.mode() != doom::Mode::Playing) {
            releaseMouse();         // в меню мышь нужна системе
        }
        app.render(g_canvas, fps);
        const HDC dc = GetDC(g_hwnd);
        present(dc);
        ReleaseDC(g_hwnd, dc);
        Sleep(1);
    }
    timeEndPeriod(1);
    return 0;
}
