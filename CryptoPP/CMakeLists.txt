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
file(GLOB libasms ${PROJECT_SOURCE_DIR}/asm/*.asm LANGUAGE ASM_MASM)

foreach(libasm IN LISTS libasms)
    get_filename_component(libasm_basename ${libasm} NAME_WE)
    set(obj_file ${CMAKE_CURRENT_BINARY_DIR}/${libasm_basename}.obj)
    set_source_files_properties(${libasm} PROPERTIES COMPILE_FLAGS "/Fo${obj_file} ${ASFLAGS64}")
endforeach()

add_library(${PROJECT_NAME} STATIC ${libsrcs} ${libasms})