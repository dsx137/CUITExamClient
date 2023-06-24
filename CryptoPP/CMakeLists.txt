cmake_minimum_required(VERSION 3.5) 
project(CryptoPP LANGUAGES CXX ASM_MASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS asm)
enable_language(CXX ASM_MASM)

set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (ASFLAGS64 "/nologo /D_M_X64 /W3 /Cx /Zi")

include_directories(include)

aux_source_directory(${PROJECT_SOURCE_DIR}/src libsrcs)
file(GLOB libasms ${PROJECT_SOURCE_DIR}/*.asm LANGUAGE ASM_MASM)

set_source_files_properties(${libasms} PROPERTIES COMPILE_FLAGS "${ASFLAGS64}")

add_library(${PROJECT_NAME} STATIC ${libsrcs} ${libasms})