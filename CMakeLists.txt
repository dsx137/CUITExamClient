cmake_minimum_required(VERSION 3.5) # CMake install : https://cmake.org/download/
project(exam_cuit LANGUAGES CXX)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "$ENV{Qt}/6.4.2/mingw_64") # Qt Kit Dir

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets) # Qt COMPONENTS
aux_source_directory(./src srcs)
qt_standard_project_setup()

# Specify MSVC UTF-8 encoding   
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(${PROJECT_NAME}
    WIN32 # If you need a terminal for debug, please comment this statement 
    ${srcs} 
) 
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets) # Qt5 Shared Library

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)