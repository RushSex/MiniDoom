# CMakeLists.txt — как собрать MINI DOOM на любой машине
cmake_minimum_required(VERSION 3.25)
project(MiniDoom LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)       # чистый C++20, без диалектов
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # для clang-tidy и редакторов

option(DOOM_SANITIZE "AddressSanitizer + UBSan" OFF)

# Ядро игры: ни строчки про Windows. Его используют игра и тесты.
add_library(doom_core STATIC
    ai.cpp app.cpp art.cpp canvas.cpp fire.cpp game.cpp hud.cpp
    levels.cpp map.cpp mixer.cpp monsters.cpp noise.cpp paint.cpp
    pathfind.cpp raycast.cpp sprites.cpp synth.cpp text.cpp
    textures.cpp topview.cpp view3d.cpp weapons.cpp)
target_include_directories(doom_core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Строгость — для всех, кто подключает ядро (PUBLIC).
if(MSVC)
    target_compile_options(doom_core PUBLIC /W4 /WX /permissive- /utf-8)
else()
    target_compile_options(doom_core PUBLIC -Wall -Wextra -Wpedantic
        -Wshadow -Wconversion -Wsign-conversion -Werror)
endif()

if(DOOM_SANITIZE)
    if(MSVC)
        target_compile_options(doom_core PUBLIC /fsanitize=address)
    else()
        target_compile_options(doom_core PUBLIC
            -fsanitize=address,undefined -fno-sanitize-recover=all)
        target_link_options(doom_core PUBLIC
            -fsanitize=address,undefined)
    endif()
endif()

# Уровни лежат рядом с исходниками; копируем их туда, где запуск.
file(GLOB DOOM_LEVELS CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.lvl)

if(WIN32)
    add_executable(minidoom WIN32 main_win32.cpp audio_win32.cpp)
    target_link_libraries(minidoom PRIVATE doom_core winmm)
    if(MINGW)                       # wWinMain у MinGW — по ключу
        target_compile_options(minidoom PRIVATE -municode)
        target_link_options(minidoom PRIVATE -municode)
    endif()
    add_custom_command(TARGET minidoom POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DOOM_LEVELS}
                $<TARGET_FILE_DIR:minidoom>)
endif()

# Тесты: одна программа, каждый тест — свой аргумент.
enable_testing()
add_executable(doom_tests test.cpp)
target_link_libraries(doom_tests PRIVATE doom_core)
file(COPY ${DOOM_LEVELS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
foreach(name IN ITEMS levels chaos shots)
    add_test(NAME ${name} COMMAND doom_tests ${name})
endforeach()
