cmake_minimum_required(VERSION 3.13) # 3.13 for target_link_options and add_compile_definitions

# Set project name.
project(I3T VERSION 2.1.0)

# Required packages.
find_package(OpenGL REQUIRED)

# Customizable options.
option(BUILD_TESTS "Build test programs" ON)

if (NOT CMAKE_BUILD_TYPE)
    message(WARNING 'Configuration is not set, using Debug as default. If you want to set custom configuration, run cmake with "-DCMAKE_BUILD_TYPE=Release" for example.')
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif()

# Version number to file config.h in build directory.
configure_file(Source/ConfigVersion.h.in Source/ConfigVersion.h)

# Specifies where build system should find header files.
# TODO -> MH use target_include_directories.
include_directories(Source/)
include_directories(Dependencies/assimp/include)
include_directories(Dependencies/glfw/include)
include_directories(Dependencies/glm)
include_directories(Dependencies/jsonlib/single_include)
include_directories(Dependencies/imgui)
include_directories(Dependencies/imgui_markdown)
include_directories(Dependencies/magic_enum/include)
include_directories(Dependencies/pgr-framework/include)
include_directories(Dependencies/picoc)
include_directories(Dependencies/portable-file-dialogs)
include_directories(Dependencies/spdlog/include)
include_directories(Dependencies/yaml-cpp/include)
include_directories($(OPENGL_INCLUDE_DIRS))

# ========== BUILD CONFIGURATION ==============================
# ---------- Setup compiler parameters. ------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_BUILD_TYPE MATCHES "ReleaseStandalone")
    add_compile_definitions(I3T_RELEASE_STANDALONE) 
endif()

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_compile_definitions(I3T_DEBUG) 
endif()

add_definitions(-DI3T_PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

if (MSVC)
    # Enable multi-processor compilation for MSVC.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
    message(STATUS "MSVC: CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
    message(STATUS "MSVC: CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")

    set(CMAKE_SHARED_LINKER_FLAGS  /MANIFEST:NO)
endif()

# Link C++17 filesystem library.
if (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lstdc++fs")
endif()

# ---------- Set VS specific properties. -----------------------
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set(I3T_OUT_DIR "${CMAKE_SOURCE_DIR}/Binaries/")
    set(I3T_LOGS_DIR "${I3T_OUT_DIR}${CMAKE_BUILD_TYPE}/logs")
else()
    set(I3T_OUT_DIR "${CMAKE_SOURCE_DIR}/Binaries/${CMAKE_BUILD_TYPE}/")
    set(I3T_LOGS_DIR "${I3T_OUT_DIR}logs")
endif()

# ---------- Setup output directories and compiler params. -----
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
        "${I3T_OUT_DIR}"
        CACHE PATH
        "Single Directory for all Libraries"
        )

# Setup the Executable output Directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
        "${I3T_OUT_DIR}"
        CACHE PATH
        "Single Directory for all Executables."
        )

# Setup the Executable output Directory.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
        "${I3T_OUT_DIR}"
        CACHE PATH
        "Single Directory for all static libraries."
        )
# =============================================================

# ========== Set project specific parameters. =================
set(CONFIGURATION_POSTFIX "")

set(ASSIMP_BUILD_TESTS False CACHE BOOL "Assimp should not build build tests.")

set(YAML_BUILD_SHARED_LIBS Off)

set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT CACHE INTERNAL FALSE)
set(ASSIMP_BUILD_3DS_IMPORTER ON)
set(ASSIMP_BUILD_COLLADA_IMPORTER ON)
set(ASSIMP_BUILD_FBX_IMPORTER ON)
set(ASSIMP_BUILD_OBJ_IMPORTER ON)
set(ASSIMP_BUILD_PLY_IMPORTER ON)

set(ASSIMP_BUILD_ASSIMP_TOOLS CACHE INTERNAL FALSE)

set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT CACHE INTERNAL FALSE)
set(ASSIMP_BUILD_FBX_EXPORTER ON)
set(ASSIMP_BUILD_OBJ_EXPORTER ON)
set(ASSIMP_BUILD_PLY_EXPORTER ON)
# =============================================================

# ======== Project dependencies which should be compiled. =====
add_subdirectory(Dependencies/assimp)
add_subdirectory(Dependencies/glfw)
add_subdirectory(Dependencies/picoc)
add_subdirectory(Dependencies/spdlog)
add_subdirectory(Dependencies/yaml-cpp)
# =============================================================

# ======== Macros and functions. ==============================
macro(copy_shared_libs target_project from_path)
    add_custom_command(TARGET ${target_project} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${from_path}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endmacro()
# =============================================================

# ======== Source groups ======================================
# Add ImGui source files.
set(I3T_IMGUI_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/misc/cpp/imgui_stdlib.cpp)

file(GLOB_RECURSE I3T_SOURCE
    "Source/*.h"
    "Source/*.hpp"
    "Source/*.cpp"
    "Dependencies/pgr-framework/src/*"
    "Dependencies/pgr-framework/include/*"
)
list(REMOVE_ITEM I3T_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Source/Main.cpp")

list(APPEND I3T_SOURCE "${I3T_IMGUI_SOURCE}")
# =============================================================

# ======== Configuration for building tests.  =================
# if (BUILD_TESTS)
if (True)
    set(TEST_PROJECT_NAME I3TTest)

    message("Building tests...")

    include_directories(${CMAKE_SOURCE_DIR}/Dependencies/gtest/googlemock/include)
    include_directories(${CMAKE_SOURCE_DIR}/Dependencies/gtest/googletest/include)

    include_directories(${CMAKE_SOURCE_DIR}/Test)

    add_subdirectory(Dependencies/gtest)

    file(GLOB_RECURSE TEST_SOURCE
            "Test/*.cpp"
            "Test/*.h"
            "Test/*.hpp"
    )

    add_executable(${TEST_PROJECT_NAME} ${TEST_SOURCE} ${I3T_SOURCE})

    target_link_libraries(
            ${TEST_PROJECT_NAME}
            ${OPENGL_LIBRARIES}
            gtest
            gtest_main
            assimp
            picoc
            glfw
            yaml-cpp)

    set_target_properties(gtest PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${I3T_OUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${I3T_OUT_DIR}"
    )
    set_target_properties(gtest_main PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${I3T_OUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${I3T_OUT_DIR}"
    )

    target_compile_definitions(${TEST_PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<gl_core_4_4.h> GL_GLEXT_PROTOTYPES=1 _CRT_SECURE_NO_WARNINGS)
endif()
# =============================================================

# ===== Create target executable from I3T source files. =======
add_executable(${PROJECT_NAME} ${I3T_SOURCE} Source/Main.cpp)
# =============================================================

# Set VS_DEBUGGER_WORKING_DIRECTORY
set_target_properties(
        ${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${I3T_OUT_DIR}/${CMAKE_BUILD_TYPE}")

# ==== Organize files into groups in Visual Studio based on directories (ideal goal)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ==== Target parameters. =====================================
# Tell ImGui which header file contains OpenGL functions signatures.
target_compile_definitions(${PROJECT_NAME} PUBLIC IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<gl_core_4_4.h> GL_GLEXT_PROTOTYPES=1 _CRT_SECURE_NO_WARNINGS)

# Link I3T with following binaries.
target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
        ${OPENGL_LIBRARIES}
        assimp
        glfw
        picoc
        spdlog
        yaml-cpp
)

# target_link_options(${PROJECT_NAME} PUBLIC $<$<CXX_COMPILER_ID:MSVC>:/FORCE:MULTIPLE>)
# =============================================================

# ==== Post build commands. ===================================
# Create logs/ directory.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${I3T_LOGS_DIR}")

set_target_properties(assimp PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${I3T_OUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${I3T_OUT_DIR}")

if (CMAKE_BUILD_TYPE MATCHES "ReleaseStandalone")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/Data" "${I3T_OUT_DIR}Data")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/config.yml" "${I3T_OUT_DIR}config.yml")
endif()
# =============================================================
