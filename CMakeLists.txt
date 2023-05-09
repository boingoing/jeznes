cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_TOOLCHAIN_FILE toolchain/cc65-toolchain.cmake)

find_program (CLANG_TIDY_EXE NAMES "clang-tidy")
if (CLANG_TIDY_EXE)
  set (CMAKE_C_CLANG_TIDY clang-tidy)
endif ()

set (CMAKE_C_STANDARD 11)

project(jeznes C ASM)

include_directories (${PROJECT_SOURCE_DIR}/src)

set (SOURCES
  ${PROJECT_SOURCE_DIR}/src/crt0.s
  ${PROJECT_SOURCE_DIR}/src/jeznes.c)

add_executable (jeznes ${SOURCES})

set_target_properties(jeznes PROPERTIES OUTPUT_NAME "jeznes" SUFFIX ".nes")

target_compile_options(jeznes PRIVATE -Oirs --target nes --add-source)

target_link_options(jeznes PRIVATE nes.lib -C ${PROJECT_SOURCE_DIR}/nrom_32k_vert.cfg -Ln jeznes.labels.txt --dbgfile jeznes.dbg)
