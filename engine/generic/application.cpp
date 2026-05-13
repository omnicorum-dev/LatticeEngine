//
// Created by Nico Russo on 5/10/26.
//

#define STB_IMAGE_IMPLEMENTATION
#include "application.h"

u32 Renderer::s_viewportX, Renderer::s_viewportY, Renderer::s_viewportWidth, Renderer::s_viewportHeight;

glm::vec4 Renderer::m_clearColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

Application* Application::s_instance = nullptr;