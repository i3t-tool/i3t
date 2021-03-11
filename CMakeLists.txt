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
include_directories(Source/)
include_directories(Dependencies/assimp/include)
include_directories(Dependencies/jsonlib/single_include)
include_directories(Dependencies/glfw/include)
include_directories(Dependencies/glm)
include_directories(Dependencies/imgui/examples)
include_directories(Dependencies/imgui_node_editor)
include_directories(Dependencies/imgui_node_editor/external/imgui)
include_directories(Dependencies/imgui_markdown)
include_directories(Dependencies/pgr-framework/include)
include_directories(Dependencies/picoc)
include_directories(Dependencies/portable-file-dialogs)
include_directories(Dependencies/spdlog/include)
include_directories(Dependencies/yaml-cpp/include)
include_directories($(OPENGL_INCLUDE_DIRS))

# ========== BUILD CONFIGURATION ==============================
# ---------- Setup compiler parameters. ------------------------
# Target machine must have a C++17 compiler.
# To set in in Visual studio use: /std:c++17
# To set this for a project:
#   1. Right click a project, go to Properties
#   2. Select Configuration Properties > C/C++ > Language
#   3. For C++ Language Standard select "ISO C++17 Standard (/std:c++17)"
set(CMAKE_CXX_STANDARD 17)
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
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
    message(STATUS "MSVC: CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
    message(STATUS "MSVC: CMAKE_C_FLAGS = ${CMAKE_C_FLAGS}")
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
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/external/imgui/imgui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/external/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/external/imgui/imgui_internal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/external/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/external/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/external/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/external/imgui/imconfig.h)

file(GLOB_RECURSE I3T_SOURCE
    "Source/*.h"
    "Source/*.hpp"
    "Source/*.cpp"
    "Dependencies/pgr-framework/src/*"
    "Dependencies/pgr-framework/include/*"
)
list(REMOVE_ITEM I3T_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/Source/Main.cpp")

list(APPEND I3T_SOURCE "${I3T_IMGUI_SOURCE}")

# Add ImGui_node_editor source files.
list(APPEND I3T_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/imgui_node_editor.h)
list(APPEND I3T_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/imgui_node_editor.cpp)
list(APPEND I3T_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/imgui_bezier_math.h)
list(APPEND I3T_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/imgui_canvas.cpp)
list(APPEND I3T_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/imgui_canvas.h)
list(APPEND I3T_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/imgui_extra_math.h)
list(APPEND I3T_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/imgui_node_editor_api.cpp)
list(APPEND I3T_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/imgui_node_editor_internal.h)
list(APPEND I3T_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/crude_json.h)
list(APPEND I3T_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_node_editor/crude_json.cpp)
# =============================================================

# ======== Configuration for building tests.  =================
# if (BUILD_TESTS)
if (True)
    set(TEST_PROJECT_NAME I3TTest)

    message("Building tests...")

    include_directories(${CMAKE_SOURCE_DIR}/Dependencies/gtest/googlemock/include)
    include_directories(${CMAKE_SOURCE_DIR}/Dependencies/gtest/googletest/include)

    add_subdirectory(Dependencies/gtest)

    file(GLOB_RECURSE TEST_SOURCE
            "Test/*.cpp"
            "Test/*.h"
            "Test/*.hpp"
            )

    add_executable(${TEST_PROJECT_NAME} ${TEST_SOURCE} ${I3T_SOURCE})

    target_link_libraries(${TEST_PROJECT_NAME} ${OPENGL_LIBRARIES} gtest gtest_main assimp glfw yaml-cpp)

    set_target_properties(gtest PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${I3T_OUT_DIR})
    set_target_properties(gtest_main PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${I3T_OUT_DIR})

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

# ==== Source files grouping for VS. ==========================
if(FALSE)
	# Simple structure - .h and .cpp together
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${I3T_SOURCE})
else()
	# Better structure - .h and .cpp separated
	# todo - group files into directories on the disk

	file(GLOB_RECURSE I3T_SOURCE_H
		"Source/*.h"
		"Source/*.hpp"
	)
	file(GLOB_RECURSE I3T_SOURCE_CPP
		"Source/*.cpp"
	)
	file(GLOB_RECURSE I3T_SOURCE_DEP
		"Dependencies/pgr-framework/src/*"
		"Dependencies/pgr-framework/include/*"
#		"Dependencies/imgui/img*.*"
#		"Dependencies/imgui/imconfig.h"
		"Dependencies/imgui_node_editor/external/imgui/imgui*.h"
		"Dependencies/imgui_node_editor/external/imgui/imgui*.cpp"
		"Dependencies/imgui_node_editor/imgui*.h"
		"Dependencies/imgui_node_editor/imgui*.cpp"
#		"Dependencies/imgui/examples/imgui_impl_glfw.*"
#		"Dependencies/imgui/examples/imgui_impl_opengl3.*"
	)
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "Header Files" FILES ${I3T_SOURCE_H})
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/Source PREFIX "Source Files" FILES ${I3T_SOURCE_CPP})
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${I3T_SOURCE_DEP})
endif()
# =============================================================

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
        RUNTIME_OUTPUT_DIRECTORY ${I3T_OUT_DIR})
# =============================================================
