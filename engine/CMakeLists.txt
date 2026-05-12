cmake_minimum_required(VERSION 4.2)
project(LatticeEngine)

set(CMAKE_CXX_STANDARD 20)

# =============== GLM ===============
add_subdirectory(vendor/glm)

# ========== IMGUI OPENGL ==========
add_library(imgui_gl STATIC
        vendor/imgui/imgui.cpp
        vendor/imgui/imgui_demo.cpp
        vendor/imgui/imgui_draw.cpp
        vendor/imgui/imgui_tables.cpp
        vendor/imgui/imgui_widgets.cpp
        vendor/imgui/backends/imgui_impl_glfw.cpp
        vendor/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui_gl PUBLIC
        vendor/imgui
        vendor/imgui/backends
        vendor/glfw/include
)
target_compile_definitions(imgui_gl PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

# =============== GLAD ===============
add_library(glad STATIC vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

# =============== GLFW ===============
add_subdirectory(vendor/glfw)

# =============== Engine ===============
add_library(LatticeEngine
        generic/application.cpp
        glfw_opengl/glfw_opengl.cpp
)
target_link_libraries(LatticeEngine PUBLIC glm imgui_gl glad glfw)
target_include_directories(LatticeEngine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/other/)
if(APPLE)
    target_link_libraries(LatticeEngine PUBLIC
            "-framework Cocoa"
            "-framework OpenGL"
            "-framework IOKit"
            "-framework CoreVideo"
            "-framework QuartzCore"
    )
elseif(UNIX)
    target_link_libraries(LatticeEngine PUBLIC
            GL X11 Xrandr Xi pthread dl
    )
elseif(WIN32)
    target_link_libraries(LatticeEngine PUBLIC
            opengl32 gdi32 user32
    )
endif()