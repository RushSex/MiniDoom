# examples/CMakeLists.txt — все примеры книги по главам.
# Сгенерирован из папок примеров; намеренно сломанные примеры
# (ошибки компиляции и компоновки из текста книги) не собираются —
# их список в README.md.

find_package(Threads REQUIRED)

function(doom_flags target)
    target_compile_features(${target} PRIVATE cxx_std_20)
    target_link_libraries(${target} PRIVATE Threads::Threads)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive- /utf-8)
        if(DOOM_WERROR)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic
            -Wshadow -Wconversion -Wsign-conversion)
        if(DOOM_WERROR)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()
endfunction()

# Консольная программа: работает везде.
function(doom_console target)
    add_executable(${target} ${ARGN})
    doom_flags(${target})
endfunction()

# Программа для Windows: окно (GUI) или консоль с <windows.h>.
function(doom_windows target kind)
    if(NOT WIN32)
        return()
    endif()
    if(kind STREQUAL "GUI")
        add_executable(${target} WIN32 ${ARGN})
        if(MINGW)
            target_compile_options(${target} PRIVATE -municode)
            target_link_options(${target} PRIVATE -municode)
        endif()
    else()
        add_executable(${target} ${ARGN})
    endif()
    target_link_libraries(${target} PRIVATE winmm)
    doom_flags(${target})
endfunction()

# Пример с багом «найди сам»: Clang замечает его ещё при компиляции,
# поэтому для него предупреждение не превращаем в ошибку.
function(doom_buggy target)
    if(TARGET ${target} AND NOT MSVC)
        target_compile_options(${target} PRIVATE -Wno-error)
    endif()
endfunction()

# Автотест главы: ядро игры без Windows + test.cpp. Запуск — ctest.
function(doom_test target dir)
    add_executable(${target} ${ARGN})
    doom_flags(${target})
    set(run ${CMAKE_CURRENT_BINARY_DIR}/run/${target})
    file(MAKE_DIRECTORY ${run})
    file(GLOB levels ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/*.lvl)
    if(levels)
        file(COPY ${levels} DESTINATION ${run})
    endif()
    add_test(NAME ${target} COMMAND ${target} WORKING_DIRECTORY ${run})
endfunction()

# --- app
doom_console(app_cpp20 app/cpp20.cpp)
doom_console(app_vectors app/vectors.cpp)

# --- ch00
doom_console(ch00_listing_demo ch00/listing_demo.cpp)

# --- ch01
doom_console(ch01_ans_frame ch01/ans_frame.cpp)
doom_console(ch01_ans_loading ch01/ans_loading.cpp)
doom_console(ch01_ex_what ch01/ex_what.cpp)
doom_console(ch01_hello ch01/hello.cpp)
doom_console(ch01_title ch01/title.cpp)

# --- ch02
doom_console(ch02_ans_ammo ch02/ans_ammo.cpp)
doom_console(ch02_ans_bug ch02/ans_bug.cpp)
doom_console(ch02_ans_percent ch02/ans_percent.cpp)
doom_console(ch02_ans_time ch02/ans_time.cpp)
doom_console(ch02_calc ch02/calc.cpp)
doom_console(ch02_cast ch02/cast.cpp)
doom_console(ch02_constants ch02/constants.cpp)
doom_console(ch02_division ch02/division.cpp)
doom_console(ch02_ex_bug ch02/ex_bug.cpp)
doom_console(ch02_ex_what ch02/ex_what.cpp)
doom_console(ch02_floats ch02/floats.cpp)
doom_console(ch02_memory ch02/memory.cpp)
doom_console(ch02_overflow ch02/overflow.cpp)
doom_console(ch02_sizes ch02/sizes.cpp)
doom_console(ch02_vars ch02/vars.cpp)

# --- ch03
doom_console(ch03_ans_bar ch03/ans_bar.cpp)
doom_console(ch03_ans_code ch03/ans_code.cpp)
doom_console(ch03_ans_ticker ch03/ans_ticker.cpp)
doom_console(ch03_cin_fail ch03/cin_fail.cpp)
doom_console(ch03_compare ch03/compare.cpp)
doom_console(ch03_duel ch03/duel.cpp)
doom_console(ch03_ex_what ch03/ex_what.cpp)
doom_console(ch03_for ch03/for.cpp)
doom_console(ch03_health ch03/health.cpp)
doom_console(ch03_logic ch03/logic.cpp)
doom_console(ch03_random ch03/random.cpp)
doom_console(ch03_weapon ch03/weapon.cpp)
doom_console(ch03_while ch03/while.cpp)

# --- ch04
doom_console(ch04_ans_clamp ch04/ans_clamp.cpp)
doom_console(ch04_ans_percent ch04/ans_percent.cpp)
doom_console(ch04_ans_static ch04/ans_static.cpp)
doom_console(ch04_bar ch04/bar.cpp)
doom_console(ch04_byref ch04/byref.cpp)
doom_console(ch04_ex_what ch04/ex_what.cpp)
doom_console(ch04_heal ch04/heal.cpp)
doom_console(ch04_overload ch04/overload.cpp)
doom_console(ch04_duel ch04/duel/combat.cpp ch04/duel/main.cpp)

# --- ch05
doom_console(ch05_ans_best ch05/ans_best.cpp)
doom_console(ch05_ans_count ch05/ans_count.cpp)
doom_console(ch05_array ch05/array.cpp)
doom_console(ch05_ex_bug ch05/ex_bug.cpp)
doom_console(ch05_ex_what ch05/ex_what.cpp)
doom_console(ch05_map ch05/map.cpp)
doom_console(ch05_oob ch05/oob.cpp)
doom_console(ch05_rogue ch05/rogue.cpp)
doom_console(ch05_strings ch05/strings.cpp)
doom_console(ch05_vector ch05/vector.cpp)

# --- ch06
doom_console(ch06_ans_sort ch06/ans_sort.cpp)
doom_console(ch06_ans_weapons ch06/ans_weapons.cpp)
doom_console(ch06_byref ch06/byref.cpp)
doom_console(ch06_ex_bug ch06/ex_bug.cpp)
doom_console(ch06_ex_what ch06/ex_what.cpp)
doom_console(ch06_kind ch06/kind.cpp)
doom_console(ch06_padding ch06/padding.cpp)
doom_console(ch06_player ch06/player.cpp)
doom_console(ch06_pos ch06/pos.cpp)
doom_console(ch06_rogue ch06/rogue/main.cpp ch06/rogue/world.cpp)

# --- ch07
doom_console(ch07_ans_ammo ch07/ans_ammo.cpp)
doom_console(ch07_ex_what ch07/ex_what.cpp)
doom_console(ch07_health ch07/health.cpp)
doom_console(ch07_open_struct ch07/open_struct.cpp)
doom_console(ch07_raii ch07/raii.cpp)
doom_console(ch07_rogue ch07/rogue/game.cpp ch07/rogue/main.cpp)

# --- ch08
doom_console(ch08_ans_factory ch08/ans_factory.cpp)
doom_console(ch08_dangling ch08/dangling.cpp)
doom_console(ch08_ex_bug ch08/ex_bug.cpp)
doom_buggy(ch08_ex_bug)
doom_console(ch08_ex_what ch08/ex_what.cpp)
doom_console(ch08_leak ch08/leak.cpp)
doom_console(ch08_null_deref ch08/null_deref.cpp)
doom_console(ch08_pointers ch08/pointers.cpp)
doom_console(ch08_unique ch08/unique.cpp)
doom_console(ch08_hunt_hunt ch08/hunt/hunt.cpp)
doom_console(ch08_hunt_hunt_fixed ch08/hunt/hunt_fixed.cpp)
doom_console(ch08_hunt_hunt_v2 ch08/hunt/hunt_v2.cpp)
doom_console(ch08_hunt_hunt_v3 ch08/hunt/hunt_v3.cpp)

# --- ch09
doom_console(ch09_ex_what ch09/ex_what.cpp)
doom_console(ch09_wide ch09/wide.cpp)
doom_windows(ch09_esc GUI ch09/esc/main_win32.cpp)
doom_windows(ch09_noquit GUI ch09/noquit/main_win32.cpp)
doom_windows(ch09_window GUI ch09/window/main_win32.cpp)

# --- ch10
doom_console(ch10_color ch10/color.cpp)
doom_console(ch10_index ch10/index.cpp)
doom_console(ch10_oob ch10/oob.cpp)
doom_windows(ch10_flipped GUI ch10/flipped/canvas.cpp ch10/flipped/main_win32.cpp ch10/flipped/scene.cpp)
doom_windows(ch10_noredraw GUI ch10/noredraw/canvas.cpp ch10/noredraw/main_win32.cpp ch10/noredraw/scene.cpp)
doom_windows(ch10_pixels GUI ch10/pixels/canvas.cpp ch10/pixels/main_win32.cpp ch10/pixels/scene.cpp)
doom_test(ch10_pixels_test ch10/pixels ch10/pixels/canvas.cpp ch10/pixels/scene.cpp ch10/pixels/test.cpp)

# --- ch11
doom_console(ch11_clamp ch11/clamp.cpp)
doom_console(ch11_dt ch11/dt.cpp)
doom_windows(ch11_loop GUI ch11/loop/canvas.cpp ch11/loop/main_win32.cpp)

# --- ch12
doom_windows(ch12_input GUI ch12/input/canvas.cpp ch12/input/game.cpp ch12/input/main_win32.cpp)
doom_test(ch12_input_test ch12/input ch12/input/canvas.cpp ch12/input/game.cpp ch12/input/test.cpp)

# --- ch13
doom_console(ch13_glyph ch13/glyph.cpp)
doom_console(ch13_mirror ch13/mirror.cpp)
doom_windows(ch13_text GUI ch13/text/canvas.cpp ch13/text/game.cpp ch13/text/hud.cpp ch13/text/main_win32.cpp ch13/text/text.cpp)
doom_test(ch13_text_test ch13/text ch13/text/canvas.cpp ch13/text/game.cpp ch13/text/hud.cpp ch13/text/text.cpp ch13/text/test.cpp)

# --- ch14
doom_console(ch14_compass ch14/compass.cpp)
doom_console(ch14_ex_bug ch14/ex_bug.cpp)
doom_console(ch14_ex_what ch14/ex_what.cpp)
doom_console(ch14_radians ch14/radians.cpp)
doom_windows(ch14_top GUI ch14/top/canvas.cpp ch14/top/game.cpp ch14/top/hud.cpp ch14/top/main_win32.cpp ch14/top/map.cpp ch14/top/text.cpp ch14/top/topview.cpp)
doom_test(ch14_top_test ch14/top ch14/top/canvas.cpp ch14/top/game.cpp ch14/top/hud.cpp ch14/top/map.cpp ch14/top/text.cpp ch14/top/topview.cpp ch14/top/test.cpp)

# --- ch15
doom_console(ch15_ex_bug ch15/ex_bug.cpp)
doom_console(ch15_ex_what ch15/ex_what.cpp)
doom_console(ch15_trace ch15/trace.cpp)
doom_windows(ch15_rays GUI ch15/rays/canvas.cpp ch15/rays/game.cpp ch15/rays/hud.cpp ch15/rays/main_win32.cpp ch15/rays/map.cpp ch15/rays/raycast.cpp ch15/rays/text.cpp ch15/rays/topview.cpp)
doom_test(ch15_rays_test ch15/rays ch15/rays/canvas.cpp ch15/rays/game.cpp ch15/rays/hud.cpp ch15/rays/map.cpp ch15/rays/raycast.cpp ch15/rays/text.cpp ch15/rays/topview.cpp ch15/rays/test.cpp)

# --- ch16
doom_console(ch16_ex_bug ch16/ex_bug.cpp)
doom_console(ch16_ex_what ch16/ex_what.cpp)
doom_windows(ch16_fisheye GUI ch16/fisheye/canvas.cpp ch16/fisheye/game.cpp ch16/fisheye/hud.cpp ch16/fisheye/main_win32.cpp ch16/fisheye/map.cpp ch16/fisheye/raycast.cpp ch16/fisheye/text.cpp ch16/fisheye/topview.cpp ch16/fisheye/view3d.cpp)
doom_test(ch16_fisheye_test ch16/fisheye ch16/fisheye/canvas.cpp ch16/fisheye/game.cpp ch16/fisheye/hud.cpp ch16/fisheye/map.cpp ch16/fisheye/raycast.cpp ch16/fisheye/text.cpp ch16/fisheye/topview.cpp ch16/fisheye/view3d.cpp ch16/fisheye/test.cpp)
doom_windows(ch16_walls GUI ch16/walls/canvas.cpp ch16/walls/game.cpp ch16/walls/hud.cpp ch16/walls/main_win32.cpp ch16/walls/map.cpp ch16/walls/raycast.cpp ch16/walls/text.cpp ch16/walls/topview.cpp ch16/walls/view3d.cpp)
doom_test(ch16_walls_test ch16/walls ch16/walls/canvas.cpp ch16/walls/game.cpp ch16/walls/hud.cpp ch16/walls/map.cpp ch16/walls/raycast.cpp ch16/walls/text.cpp ch16/walls/topview.cpp ch16/walls/view3d.cpp ch16/walls/test.cpp)

# --- ch17
doom_console(ch17_ex_bug ch17/ex_bug.cpp)
doom_console(ch17_ex_what ch17/ex_what.cpp)
doom_console(ch17_ascii ch17/ascii/main.cpp ch17/ascii/noise.cpp)
doom_windows(ch17_tex GUI ch17/tex/canvas.cpp ch17/tex/game.cpp ch17/tex/hud.cpp ch17/tex/main_win32.cpp ch17/tex/map.cpp ch17/tex/noise.cpp ch17/tex/raycast.cpp ch17/tex/text.cpp ch17/tex/textures.cpp ch17/tex/topview.cpp ch17/tex/view3d.cpp)
doom_test(ch17_tex_test ch17/tex ch17/tex/canvas.cpp ch17/tex/game.cpp ch17/tex/hud.cpp ch17/tex/map.cpp ch17/tex/noise.cpp ch17/tex/raycast.cpp ch17/tex/text.cpp ch17/tex/textures.cpp ch17/tex/topview.cpp ch17/tex/view3d.cpp ch17/tex/test.cpp)

# --- ch18
doom_console(ch18_ex_bug ch18/ex_bug.cpp)
doom_console(ch18_ex_what ch18/ex_what.cpp)
doom_windows(ch18_floor GUI ch18/floor/canvas.cpp ch18/floor/game.cpp ch18/floor/hud.cpp ch18/floor/main_win32.cpp ch18/floor/map.cpp ch18/floor/noise.cpp ch18/floor/raycast.cpp ch18/floor/text.cpp ch18/floor/textures.cpp ch18/floor/topview.cpp ch18/floor/view3d.cpp)
doom_test(ch18_floor_test ch18/floor ch18/floor/canvas.cpp ch18/floor/game.cpp ch18/floor/hud.cpp ch18/floor/map.cpp ch18/floor/noise.cpp ch18/floor/raycast.cpp ch18/floor/text.cpp ch18/floor/textures.cpp ch18/floor/topview.cpp ch18/floor/view3d.cpp ch18/floor/test.cpp)

# --- ch19
doom_windows(ch19_doors GUI ch19/doors/canvas.cpp ch19/doors/game.cpp ch19/doors/hud.cpp ch19/doors/main_win32.cpp ch19/doors/map.cpp ch19/doors/noise.cpp ch19/doors/raycast.cpp ch19/doors/text.cpp ch19/doors/textures.cpp ch19/doors/topview.cpp ch19/doors/view3d.cpp)
doom_test(ch19_doors_test ch19/doors ch19/doors/canvas.cpp ch19/doors/game.cpp ch19/doors/hud.cpp ch19/doors/map.cpp ch19/doors/noise.cpp ch19/doors/raycast.cpp ch19/doors/text.cpp ch19/doors/textures.cpp ch19/doors/topview.cpp ch19/doors/view3d.cpp ch19/doors/test.cpp)

# --- ch20
doom_console(ch20_ex_bug ch20/ex_bug.cpp)
doom_console(ch20_ex_what ch20/ex_what.cpp)
doom_windows(ch20_nozbuf GUI ch20/nozbuf/art.cpp ch20/nozbuf/canvas.cpp ch20/nozbuf/game.cpp ch20/nozbuf/hud.cpp ch20/nozbuf/main_win32.cpp ch20/nozbuf/map.cpp ch20/nozbuf/noise.cpp ch20/nozbuf/raycast.cpp ch20/nozbuf/sprites.cpp ch20/nozbuf/text.cpp ch20/nozbuf/textures.cpp ch20/nozbuf/topview.cpp ch20/nozbuf/view3d.cpp)
doom_test(ch20_nozbuf_test ch20/nozbuf ch20/nozbuf/art.cpp ch20/nozbuf/canvas.cpp ch20/nozbuf/game.cpp ch20/nozbuf/hud.cpp ch20/nozbuf/map.cpp ch20/nozbuf/noise.cpp ch20/nozbuf/raycast.cpp ch20/nozbuf/sprites.cpp ch20/nozbuf/text.cpp ch20/nozbuf/textures.cpp ch20/nozbuf/topview.cpp ch20/nozbuf/view3d.cpp ch20/nozbuf/test.cpp)
doom_windows(ch20_sprites GUI ch20/sprites/art.cpp ch20/sprites/canvas.cpp ch20/sprites/game.cpp ch20/sprites/hud.cpp ch20/sprites/main_win32.cpp ch20/sprites/map.cpp ch20/sprites/noise.cpp ch20/sprites/raycast.cpp ch20/sprites/sprites.cpp ch20/sprites/text.cpp ch20/sprites/textures.cpp ch20/sprites/topview.cpp ch20/sprites/view3d.cpp)
doom_test(ch20_sprites_test ch20/sprites ch20/sprites/art.cpp ch20/sprites/canvas.cpp ch20/sprites/game.cpp ch20/sprites/hud.cpp ch20/sprites/map.cpp ch20/sprites/noise.cpp ch20/sprites/raycast.cpp ch20/sprites/sprites.cpp ch20/sprites/text.cpp ch20/sprites/textures.cpp ch20/sprites/topview.cpp ch20/sprites/view3d.cpp ch20/sprites/test.cpp)

# --- ch21
doom_console(ch21_ascii ch21/ascii.cpp)
doom_console(ch21_ex_bug ch21/ex_bug.cpp)
doom_console(ch21_ex_what ch21/ex_what.cpp)
doom_windows(ch21_monsters GUI ch21/monsters/art.cpp ch21/monsters/canvas.cpp ch21/monsters/game.cpp ch21/monsters/hud.cpp ch21/monsters/main_win32.cpp ch21/monsters/map.cpp ch21/monsters/monsters.cpp ch21/monsters/noise.cpp ch21/monsters/paint.cpp ch21/monsters/raycast.cpp ch21/monsters/sprites.cpp ch21/monsters/text.cpp ch21/monsters/textures.cpp ch21/monsters/topview.cpp ch21/monsters/view3d.cpp)
doom_test(ch21_monsters_test ch21/monsters ch21/monsters/art.cpp ch21/monsters/canvas.cpp ch21/monsters/game.cpp ch21/monsters/hud.cpp ch21/monsters/map.cpp ch21/monsters/monsters.cpp ch21/monsters/noise.cpp ch21/monsters/paint.cpp ch21/monsters/raycast.cpp ch21/monsters/sprites.cpp ch21/monsters/text.cpp ch21/monsters/textures.cpp ch21/monsters/topview.cpp ch21/monsters/view3d.cpp ch21/monsters/test.cpp)

# --- ch22
doom_console(ch22_ex_bug ch22/ex_bug.cpp)
doom_console(ch22_ex_what ch22/ex_what.cpp)
doom_console(ch22_hitscan ch22/hitscan.cpp)
doom_windows(ch22_guns GUI ch22/guns/art.cpp ch22/guns/canvas.cpp ch22/guns/game.cpp ch22/guns/hud.cpp ch22/guns/main_win32.cpp ch22/guns/map.cpp ch22/guns/monsters.cpp ch22/guns/noise.cpp ch22/guns/paint.cpp ch22/guns/raycast.cpp ch22/guns/sprites.cpp ch22/guns/text.cpp ch22/guns/textures.cpp ch22/guns/topview.cpp ch22/guns/view3d.cpp ch22/guns/weapons.cpp)
doom_test(ch22_guns_test ch22/guns ch22/guns/art.cpp ch22/guns/canvas.cpp ch22/guns/game.cpp ch22/guns/hud.cpp ch22/guns/map.cpp ch22/guns/monsters.cpp ch22/guns/noise.cpp ch22/guns/paint.cpp ch22/guns/raycast.cpp ch22/guns/sprites.cpp ch22/guns/text.cpp ch22/guns/textures.cpp ch22/guns/topview.cpp ch22/guns/view3d.cpp ch22/guns/weapons.cpp ch22/guns/test.cpp)

# --- ch23
doom_console(ch23_ex_bug ch23/ex_bug.cpp)
doom_console(ch23_ex_what ch23/ex_what.cpp)
doom_windows(ch23_ai GUI ch23/ai/ai.cpp ch23/ai/art.cpp ch23/ai/canvas.cpp ch23/ai/game.cpp ch23/ai/hud.cpp ch23/ai/main_win32.cpp ch23/ai/map.cpp ch23/ai/monsters.cpp ch23/ai/noise.cpp ch23/ai/paint.cpp ch23/ai/raycast.cpp ch23/ai/sprites.cpp ch23/ai/text.cpp ch23/ai/textures.cpp ch23/ai/topview.cpp ch23/ai/view3d.cpp ch23/ai/weapons.cpp)
doom_test(ch23_ai_test ch23/ai ch23/ai/ai.cpp ch23/ai/art.cpp ch23/ai/canvas.cpp ch23/ai/game.cpp ch23/ai/hud.cpp ch23/ai/map.cpp ch23/ai/monsters.cpp ch23/ai/noise.cpp ch23/ai/paint.cpp ch23/ai/raycast.cpp ch23/ai/sprites.cpp ch23/ai/text.cpp ch23/ai/textures.cpp ch23/ai/topview.cpp ch23/ai/view3d.cpp ch23/ai/weapons.cpp ch23/ai/test.cpp)

# --- ch24
doom_console(ch24_bfs ch24/bfs.cpp)
doom_console(ch24_ex_bug ch24/ex_bug.cpp)
doom_console(ch24_ex_what ch24/ex_what.cpp)
doom_windows(ch24_path GUI ch24/path/ai.cpp ch24/path/art.cpp ch24/path/canvas.cpp ch24/path/game.cpp ch24/path/hud.cpp ch24/path/main_win32.cpp ch24/path/map.cpp ch24/path/monsters.cpp ch24/path/noise.cpp ch24/path/paint.cpp ch24/path/pathfind.cpp ch24/path/raycast.cpp ch24/path/sprites.cpp ch24/path/text.cpp ch24/path/textures.cpp ch24/path/topview.cpp ch24/path/view3d.cpp ch24/path/weapons.cpp)
doom_test(ch24_path_test ch24/path ch24/path/ai.cpp ch24/path/art.cpp ch24/path/canvas.cpp ch24/path/game.cpp ch24/path/hud.cpp ch24/path/map.cpp ch24/path/monsters.cpp ch24/path/noise.cpp ch24/path/paint.cpp ch24/path/pathfind.cpp ch24/path/raycast.cpp ch24/path/sprites.cpp ch24/path/text.cpp ch24/path/textures.cpp ch24/path/topview.cpp ch24/path/view3d.cpp ch24/path/weapons.cpp ch24/path/test.cpp)

# --- ch25
doom_console(ch25_ex_bug ch25/ex_bug.cpp)
doom_console(ch25_ex_what ch25/ex_what.cpp)
doom_console(ch25_tunnel ch25/tunnel.cpp)
doom_windows(ch25_fire GUI ch25/fire/ai.cpp ch25/fire/art.cpp ch25/fire/canvas.cpp ch25/fire/game.cpp ch25/fire/hud.cpp ch25/fire/main_win32.cpp ch25/fire/map.cpp ch25/fire/monsters.cpp ch25/fire/noise.cpp ch25/fire/paint.cpp ch25/fire/pathfind.cpp ch25/fire/raycast.cpp ch25/fire/sprites.cpp ch25/fire/text.cpp ch25/fire/textures.cpp ch25/fire/topview.cpp ch25/fire/view3d.cpp ch25/fire/weapons.cpp)
doom_test(ch25_fire_test ch25/fire ch25/fire/ai.cpp ch25/fire/art.cpp ch25/fire/canvas.cpp ch25/fire/game.cpp ch25/fire/hud.cpp ch25/fire/map.cpp ch25/fire/monsters.cpp ch25/fire/noise.cpp ch25/fire/paint.cpp ch25/fire/pathfind.cpp ch25/fire/raycast.cpp ch25/fire/sprites.cpp ch25/fire/text.cpp ch25/fire/textures.cpp ch25/fire/topview.cpp ch25/fire/view3d.cpp ch25/fire/weapons.cpp ch25/fire/test.cpp)

# --- ch26
doom_console(ch26_ex_bug ch26/ex_bug.cpp)
doom_console(ch26_ex_what ch26/ex_what.cpp)
doom_console(ch26_locked ch26/locked.cpp)
doom_console(ch26_race ch26/race.cpp)
doom_console(ch26_shapes ch26/shapes.cpp)
doom_windows(ch26_sound GUI ch26/sound/ai.cpp ch26/sound/art.cpp ch26/sound/audio_win32.cpp ch26/sound/canvas.cpp ch26/sound/game.cpp ch26/sound/hud.cpp ch26/sound/main_win32.cpp ch26/sound/map.cpp ch26/sound/mixer.cpp ch26/sound/monsters.cpp ch26/sound/noise.cpp ch26/sound/paint.cpp ch26/sound/pathfind.cpp ch26/sound/raycast.cpp ch26/sound/sprites.cpp ch26/sound/synth.cpp ch26/sound/text.cpp ch26/sound/textures.cpp ch26/sound/topview.cpp ch26/sound/view3d.cpp ch26/sound/weapons.cpp)
doom_test(ch26_sound_test ch26/sound ch26/sound/ai.cpp ch26/sound/art.cpp ch26/sound/canvas.cpp ch26/sound/game.cpp ch26/sound/hud.cpp ch26/sound/map.cpp ch26/sound/mixer.cpp ch26/sound/monsters.cpp ch26/sound/noise.cpp ch26/sound/paint.cpp ch26/sound/pathfind.cpp ch26/sound/raycast.cpp ch26/sound/sprites.cpp ch26/sound/synth.cpp ch26/sound/text.cpp ch26/sound/textures.cpp ch26/sound/topview.cpp ch26/sound/view3d.cpp ch26/sound/weapons.cpp ch26/sound/test.cpp)

# --- ch27
doom_console(ch27_ex_bug ch27/ex_bug.cpp)
doom_console(ch27_ex_what ch27/ex_what.cpp)
doom_windows(ch27_screens GUI ch27/screens/ai.cpp ch27/screens/app.cpp ch27/screens/art.cpp ch27/screens/audio_win32.cpp ch27/screens/canvas.cpp ch27/screens/fire.cpp ch27/screens/game.cpp ch27/screens/hud.cpp ch27/screens/main_win32.cpp ch27/screens/map.cpp ch27/screens/mixer.cpp ch27/screens/monsters.cpp ch27/screens/noise.cpp ch27/screens/paint.cpp ch27/screens/pathfind.cpp ch27/screens/raycast.cpp ch27/screens/sprites.cpp ch27/screens/synth.cpp ch27/screens/text.cpp ch27/screens/textures.cpp ch27/screens/topview.cpp ch27/screens/view3d.cpp ch27/screens/weapons.cpp)
doom_test(ch27_screens_test ch27/screens ch27/screens/ai.cpp ch27/screens/app.cpp ch27/screens/art.cpp ch27/screens/canvas.cpp ch27/screens/fire.cpp ch27/screens/game.cpp ch27/screens/hud.cpp ch27/screens/map.cpp ch27/screens/mixer.cpp ch27/screens/monsters.cpp ch27/screens/noise.cpp ch27/screens/paint.cpp ch27/screens/pathfind.cpp ch27/screens/raycast.cpp ch27/screens/sprites.cpp ch27/screens/synth.cpp ch27/screens/text.cpp ch27/screens/textures.cpp ch27/screens/topview.cpp ch27/screens/view3d.cpp ch27/screens/weapons.cpp ch27/screens/test.cpp)

# --- ch28
doom_console(ch28_ex_bug ch28/ex_bug.cpp)
doom_console(ch28_ex_what ch28/ex_what.cpp)
doom_windows(ch28_levels GUI ch28/levels/ai.cpp ch28/levels/app.cpp ch28/levels/art.cpp ch28/levels/audio_win32.cpp ch28/levels/canvas.cpp ch28/levels/fire.cpp ch28/levels/game.cpp ch28/levels/hud.cpp ch28/levels/levels.cpp ch28/levels/main_win32.cpp ch28/levels/map.cpp ch28/levels/mixer.cpp ch28/levels/monsters.cpp ch28/levels/noise.cpp ch28/levels/paint.cpp ch28/levels/pathfind.cpp ch28/levels/raycast.cpp ch28/levels/sprites.cpp ch28/levels/synth.cpp ch28/levels/text.cpp ch28/levels/textures.cpp ch28/levels/topview.cpp ch28/levels/view3d.cpp ch28/levels/weapons.cpp)
doom_test(ch28_levels_test ch28/levels ch28/levels/ai.cpp ch28/levels/app.cpp ch28/levels/art.cpp ch28/levels/canvas.cpp ch28/levels/fire.cpp ch28/levels/game.cpp ch28/levels/hud.cpp ch28/levels/levels.cpp ch28/levels/map.cpp ch28/levels/mixer.cpp ch28/levels/monsters.cpp ch28/levels/noise.cpp ch28/levels/paint.cpp ch28/levels/pathfind.cpp ch28/levels/raycast.cpp ch28/levels/sprites.cpp ch28/levels/synth.cpp ch28/levels/text.cpp ch28/levels/textures.cpp ch28/levels/topview.cpp ch28/levels/view3d.cpp ch28/levels/weapons.cpp ch28/levels/test.cpp)

# --- ch29
doom_console(ch29_ex_bug ch29/ex_bug.cpp)
doom_console(ch29_ex_what ch29/ex_what.cpp)
