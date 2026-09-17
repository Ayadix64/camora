cmake_minimum_required(VERSION 3.16)
project(camora)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(glfw3 3.4 REQUIRED)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


add_executable(camora src/main.cpp
glad/glad.c
ticktackto/src/core.c ticktackto/src/utils.c ticktackto/src/basics.c ticktackto/src/render.c ticktackto/src/text.c ticktackto/src/ui.c
)
#add_subdirectory(build/)

target_link_libraries(camora ${OpenCV_LIBS} glfw3)
