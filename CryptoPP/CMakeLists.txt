cmake_minimum_required(VERSION 3.5) 
project(CryptoPP)
set(CMAKE_ASM_SOURCE_FILE_EXTENSIONS asm)
enable_language(ASM_MASM)  # 对于汇编文件使用 MASM 编译器

include_directories("include")

file(GLOB_RECURSE CryptoPP_H "include\\*.h")
file(GLOB_RECURSE CryptoPP_SRC "src\\*.cpp" LANGUAGE CXX)
file(GLOB CryptoPP_ASM_SRC "asm\\*.asm" LANGUAGE ASM_MASM)

add_library(CryptoPP STATIC ${CryptoPP_H} ${CryptoPP_SRC} ${CryptoPP_ASM_SRC})


# 生成静态库
set_target_properties(CryptoPP PROPERTIES OUTPUT_NAME "cryptopp")
set_target_properties(CryptoPP PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(CryptoPP PROPERTIES VERSION 8.2.0 SOVERSION 8)