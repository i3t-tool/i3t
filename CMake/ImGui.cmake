file(GLOB_RECURSE IMGUI_TEST_ENGINE_SOURCE
        "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_test_engine/imgui_test_engine/*.cpp")

set(I3T_IMGUI_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/backends/imgui_impl_glfw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/backends/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/misc/cpp/imgui_stdlib.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/ImGuiColorTextEdit/TextEditor.cpp)
list(APPEND I3T_IMGUI_SOURCE "${IMGUI_TEST_ENGINE_SOURCE}")

add_library(ImGui STATIC ${I3T_IMGUI_SOURCE})

target_include_directories(ImGui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui/backends
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/ImGuiColorTextEdit
        ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/imgui_test_engine/imgui_test_engine)

target_compile_definitions(ImGui
        PUBLIC
        IMGUI_TEST_ENGINE_ENABLE_COROUTINE_STDTHREAD_IMPL=1
        IMGUI_ENABLE_TEST_ENGINE)
