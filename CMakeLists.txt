cmake_minimum_required(VERSION 3.23)
project(VisualLean LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # compiler-specific-extensions-off

# header-only/static core lib
add_library(core INTERFACE)
target_include_directories(core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/core)

# FRONTENDS

# tui:
add_executable(tui frontends/main_tui.cpp)
target_link_libraries(tui PRIVATE core)
#target_link_libraries(tui PRIVATE core ftxui::ftxui)  # FetchContent below

# gen/native dev frontend
add_executable(visual_lean frontends/main.cpp)
target_link_libraries(visual_lean PRIVATE core)

# examples 
add_executable(nat_game examples/nat_game.cpp)
target_link_libraries(nat_mage PRIVATE core)

# WASM target (emscripten)
if(EMSCRIPTEN)
    add_executable(wasm frontends/main_wasm.cpp)
    target_link_libraries(wasm PRIVATE core)

    set_target_properties(wasm PROPERTIES 
        SUFFIX ".html"
        LINK_FLAGS "-s USE_WEBGL2=1 \
                    -s WASM=1 \
                    -s ALLOW_MEMORY_GROWTH=1 \
                    -s EXPORT_NAME='VisualLean' \
                    --shell-file ${CMAKE_SOURCE_DIR}/shell.html" 
    )
endif()

#if(EMSCRIPTEN)
#  add_executable(wasm frontends/main_wasm.cpp)
#  target_link_libraries(wasm PRIVATE core)
#  set_target_properties(wasm PROPERTIES SUFFIX ".html"
#    LINK_FLAGS "-s USE_WEBGL2=1 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 --shell-file ${CMAKE_SOURCE_DIR}/shell.html")
#endif()

# Third-party (ImGui, FTXUI via FetchContent)
#include(FetchContent)
#FetchContent_Declare(ftxui ...)  # Add as needed