cmake_minimum_required(VERSION 3.15) # 3.13 for target_link_options and add_compile_definitions
                                     # 3.15 to fix a "overriding /W3 with /W4" bug

# Set project name.
project(I3T VERSION 2.1.0)

# Required packages.
find_package(OpenGL REQUIRED)

# Version number to file config.h in build directory.
configure_file(Source/ConfigVersion.h.in Source/ConfigVersion.h)

# Specifies where build system should find header files.
include_directories(Source/)
include_directories(Dependencies/assimp/include)
include_directories(Dependencies/assimp/contrib/rapidjson/include)
include_directories(Dependencies/glfw/include)
include_directories(Dependencies/glm)
include_directories(Dependencies/jsonlib/single_include)
include_directories(Dependencies/imgui)
include_directories(Dependencies/imgui_markdown)
include_directories(Dependencies/lua/lua)
include_directories(Dependencies/magic_enum/include)
include_directories(Dependencies/pgr-framework/include)
include_directories(Dependencies/portable-file-dialogs)
include_directories(Dependencies/sol2/include)
include_directories(Dependencies/spdlog/include)
include_directories(Dependencies/yaml-cpp/include)
include_directories(Dependencies/ImGuizmo/ImGuizmo)
include_directories(${OPENGL_INCLUDE_DIRS})

# ========== BUILD CONFIGURATION ==============================
# ---------- Setup compiler parameters. ------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ==== Organize files into groups in Visual Studio based on directories (ideal goal)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (MSVC)
    # Enable multi-processor compilation for MSVC.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
    message(STATUS "MSVC: CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
    message(STATUS "MSVC: CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")

    set(CMAKE_SHARED_LINKER_FLAGS /MANIFEST:NO)
endif()

# Link C++17 filesystem library.
if (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lstdc++fs -Wno-restrict")
endif()

# ---------- Set VS specific properties. -----------------------
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set(I3T_OUT_DIR "${CMAKE_SOURCE_DIR}/Binaries/")
else()
    set(I3T_OUT_DIR "${CMAKE_SOURCE_DIR}/Binaries/${CMAKE_BUILD_TYPE}/")
endif()

# ---------- Setup output directories and compiler params. -----
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
        "${I3T_OUT_DIR}"
        CACHE PATH
        "Single Directory for all Libraries")

# Setup the Executable output Directory.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
        "${I3T_OUT_DIR}"
        CACHE PATH
        "Single Directory for all Executables.")

# Setup the Executable output Directory.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
        "${I3T_OUT_DIR}"
        CACHE PATH
        "Single Directory for all static libraries.")
# =============================================================

# ========== Set project specific parameters. =================
set(CONFIGURATION_POSTFIX "")

set(ASSIMP_BUILD_TESTS False CACHE BOOL "Assimp should not build build tests.")

set(YAML_BUILD_SHARED_LIBS Off)

set(ASSIMP_BUILD_ZLIB ON)

set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT CACHE INTERNAL FALSE)
set(ASSIMP_BUILD_3DS_IMPORTER ON)
set(ASSIMP_BUILD_COLLADA_IMPORTER ON)
set(ASSIMP_BUILD_FBX_IMPORTER ON)
set(ASSIMP_BUILD_GLTF_IMPORTER ON)
set(ASSIMP_BUILD_OBJ_IMPORTER ON)
set(ASSIMP_BUILD_PLY_IMPORTER ON)

set(ASSIMP_BUILD_ASSIMP_TOOLS CACHE INTERNAL FALSE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT CACHE INTERNAL FALSE)

#  Project dependencies which should be compiled.
add_subdirectory(Dependencies/assimp)
add_subdirectory(Dependencies/glfw)
add_subdirectory(Dependencies/lua)
add_subdirectory(Dependencies/sol2)
add_subdirectory(Dependencies/spdlog)
add_subdirectory(Dependencies/yaml-cpp)
add_subdirectory(Dependencies/ImGuizmo)
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
set(TEST_PROJECT_NAME I3TTest)

file(GLOB_RECURSE TEST_SOURCE
        "Test/*.cpp"
        "Test/*.h"
        "Test/*.hpp")

add_executable(${TEST_PROJECT_NAME} ${TEST_SOURCE} ${I3T_SOURCE})

target_include_directories(${TEST_PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Dependencies/gtest/googlemock/include)
target_include_directories(${TEST_PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/Dependencies/gtest/googletest/include)

target_include_directories(${TEST_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Test)

add_subdirectory(Dependencies/gtest)

set_target_properties(gtest PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${I3T_OUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${I3T_OUT_DIR}")

set_target_properties(gtest_main PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${I3T_OUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${I3T_OUT_DIR}")

target_link_libraries(
        ${TEST_PROJECT_NAME}
        ${OPENGL_LIBRARIES}
        gtest
        gtest_main
        assimp
        glfw
        lua
        sol2
        yaml-cpp
        ImGuizmo
)

target_compile_definitions(${TEST_PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:I3T_DEBUG>)

target_compile_definitions(${TEST_PROJECT_NAME} PUBLIC
        IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<gl_core_4_4.h> GL_GLEXT_PROTOTYPES=1 _CRT_SECURE_NO_WARNINGS)
# =============================================================

# ===== Create target executable from I3T source files. =======
add_executable(${PROJECT_NAME} ${I3T_SOURCE} Source/Main.cpp)

# Set VS_DEBUGGER_WORKING_DIRECTORY
set_target_properties(
        ${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:I3T_DEBUG>)

# Tell ImGui which header file contains OpenGL functions signatures.
target_compile_definitions(${PROJECT_NAME}
        PUBLIC IMGUI_IMPL_OPENGL_LOADER_CUSTOM=<gl_core_4_4.h> GL_GLEXT_PROTOTYPES=1 _CRT_SECURE_NO_WARNINGS)

# Link I3T with following binaries.
target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
        ${OPENGL_LIBRARIES}
        assimp
        glfw
        lua
        sol2
        spdlog
        yaml-cpp
        ImGuizmo
)

set_target_properties(assimp PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${I3T_OUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${I3T_OUT_DIR}")

# =============================================================
