//
// Created by Nico Russo on 5/12/26.
//

#ifndef LATTICEENGINE_COREOPENGL_H
#define LATTICEENGINE_COREOPENGL_H

#include "core.h"

#include "vendor/other/tiny_obj_loader.h"

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/backends/imgui_impl_glfw.h"
#include "vendor/imgui/backends/imgui_impl_opengl3.h"

#define GLCall(x) do { GLClearError(); x; ASSERT(GLLogCall()); } while(0)
static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}
static bool GLLogCall() {
    bool result = true;
    while (GLenum err = glGetError()) {
        LOG_ERROR("\nOpenGL Error: {}", err);
        result = false;
    }
    return result;
}

#endif //LATTICEENGINE_COREOPENGL_H
