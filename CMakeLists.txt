# MINI DOOM — все примеры книги и итоговая игра одной сборкой.
cmake_minimum_required(VERSION 3.25)
project(MiniDoomBook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(DOOM_WERROR "Предупреждения в примерах глав считать ошибками" OFF)

enable_testing()

# Примеры глав 1–28 и приложений: каждый — отдельная программа.
add_subdirectory(examples)

# Итоговая игра из главы 29 со своими тестами (levels, chaos, shots).
add_subdirectory(examples/ch29/doom)
