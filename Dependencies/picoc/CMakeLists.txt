cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(picoc)

file(GLOB_RECURSE PICOC_SOURCE
    "*.h"
    "*.cpp"
)
set_source_files_properties(${PICOC_SOURCE} PROPERTIES LANGUAGE "CXX")

add_library(picoc STATIC ${PICOC_SOURCE})
