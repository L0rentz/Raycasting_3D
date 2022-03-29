cmake_minimum_required(VERSION 3.10)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB_RECURSE SRC "main.cpp" "src/*.cpp" "src/*.hpp")

include_directories("src")
include_directories("SFML-2.5.1/include")

# set the project name
project(Raycasting3D)

unset(CMAKE_IMPORT_LIBRARY_SUFFIX)

# add the executable
add_executable(raycasting3d ${SRC})

# link_directories(SFML-2.5.1/lib)
target_link_directories(raycasting3d PRIVATE "SFML-2.5.1/lib")
target_link_libraries(raycasting3d sfml-graphics sfml-window sfml-system sfml-audio)
target_compile_definitions(raycasting3d PRIVATE _CRT_SECURE_NO_WARNINGS)

unset(CMAKE_IMPORT_LIBRARY_SUFFIX)

add_custom_command(
    TARGET raycasting3d POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/SFML-2.5.1/bin/sfml-audio-2.dll" "${CMAKE_SOURCE_DIR}/build/debug/sfml-audio-2.dll"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/SFML-2.5.1/bin/sfml-graphics-2.dll" "${CMAKE_SOURCE_DIR}/build/debug/sfml-graphics-2.dll"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/SFML-2.5.1/bin/sfml-window-2.dll" "${CMAKE_SOURCE_DIR}/build/debug/sfml-window-2.dll"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/SFML-2.5.1/bin/sfml-system-2.dll" "${CMAKE_SOURCE_DIR}/build/debug/sfml-system-2.dll"
)