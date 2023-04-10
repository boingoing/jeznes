cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_TOOLCHAIN_FILE toolchain/cc65-toolchain.cmake)

project(jeznes C ASM)

set (SOURCES
  ${PROJECT_SOURCE_DIR}/crt0.s
  ${PROJECT_SOURCE_DIR}/jeznes.c)

add_executable (jeznes ${SOURCES})

set_target_properties(jeznes PROPERTIES OUTPUT_NAME "jeznes" SUFFIX ".nes")

target_compile_options(jeznes PRIVATE -Oirs --target nes --add-source)

target_link_options(jeznes PRIVATE nes.lib -C ${PROJECT_SOURCE_DIR}/nrom_32k_vert.cfg -Ln jeznes.labels.txt --dbgfile jeznes.dbg)
