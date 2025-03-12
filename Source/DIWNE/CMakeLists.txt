file(GLOB_RECURSE DIWNE_SOURCE
        "*.cpp"
        "*.h"
        "*.hpp"
)
list(APPEND DIWNE_SOURCE "${I3T_IMGUI_SOURCE}")

include_directories(/)

add_executable(DIWNE diwne_example.cpp ${DIWNE_SOURCE})

target_link_libraries(
        DIWNE
        PUBLIC
        ${OPENGL_LIBRARIES}
        glfw
        spdlog
        ImGui
)