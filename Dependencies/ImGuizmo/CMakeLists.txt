cmake_minimum_required(VERSION 3.13)

project(ImGuizmo VERSION 1.8.3)

set(CMAKE_C_STANDARD 11)

set(IMGUIZMO_SOURCES
        ImGuizmo/GraphEditor.h
        ImGuizmo/GraphEditor.cpp
        ImGuizmo/ImCurveEdit.h
        ImGuizmo/ImCurveEdit.cpp
        ImGuizmo/ImGradient.h
        ImGuizmo/ImGradient.cpp
        ImGuizmo/ImGuizmo.h
        ImGuizmo/ImGuizmo.cpp
        ImGuizmo/ImSequencer.h
        ImGuizmo/ImSequencer.cpp
        ImGuizmo/ImZoomSlider.h
)

add_library(${PROJECT_NAME} STATIC ${IMGUIZMO_SOURCES})
