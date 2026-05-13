//
// Created by Nico Russo on 5/10/26.
//
#include "glfw_opengl.h"
#include "opengl_renderer.h"

Ref<RenderTarget> RenderTarget::create(const RenderTargetSpec& spec) {
    return MakeRef<OpenGLRenderTarget>(spec);
}

Ref<RenderPipeline> RenderPipeline::create(const PipelineSpec& pipeline) {
    return MakeRef<OpenGLRenderPipeline>(pipeline);
}

Ref<Shader> Shader::create(const std::unordered_map<ShaderStage, std::string>& sources) {
    return MakeRef<OpenGLShader>(sources);
}

Ref<Texture2D> Texture2D::create(const TextureSpec& spec, const unsigned char* data) {
    return MakeRef<OpenGLTexture2D>(spec, data);
}

Ref<Texture2D> Texture2D::create(const TextureSpec& spec, const float* data) {
    return MakeRef<OpenGLTexture2D>(spec, data);
}

Ref<TextureCube> TextureCube::createFromFaces(const CubeTextureSpec& spec,
                                               const std::array<const unsigned char*, 6>& faces) {
    return MakeRef<OpenGLTextureCube>(spec, faces);
}

Ref<TextureCube> TextureCube::createFromCross(const CubeTextureSpec& spec,
                                               const unsigned char* data) {
    return MakeRef<OpenGLTextureCube>(spec, data);
}

Ref<TextureCube> TextureCube::createFromFaces(const CubeTextureSpec& spec,
                                               const std::array<const float*, 6>& faces) {
    return MakeRef<OpenGLTextureCube>(spec, faces);
}

Ref<TextureCube> TextureCube::createFromCross(const CubeTextureSpec& spec,
                                               const float* data) {
    return MakeRef<OpenGLTextureCube>(spec, data);
}

Ref<Geometry> Geometry::create(const Ref<VertexBuffer>& vb, const Ref<IndexBuffer>& ib) {
    return MakeRef<OpenGLGeometry>(vb, ib);
}

Ref<VertexBuffer> VertexBuffer::create(const void* data, u32 size) {
    return MakeRef<OpenGLVertexBuffer>(data, size);
}

Ref<VertexBuffer> VertexBuffer::create(u32 size) {
    return MakeRef<OpenGLVertexBuffer>(size);
}

Ref<IndexBuffer> IndexBuffer::create(const u32* indices, u32 count) {
    return MakeRef<OpenGLIndexBuffer>(indices, count);
}

RendererAPI* Renderer::s_instance = new OpenGLRendererAPI();
std::vector<RenderCommand> Renderer::s_queue;

/* ================================================================================================================ */
/* IMGUI IMPLEMENTATION =========================================================================================== */
/* ================================================================================================================ */

ImGuiImplementation* ImGuiImplementation::s_instance = new ImGuiImplGlfwOpenGL();

/* ================================================================================================================ */
/* INPUT ========================================================================================================== */
/* ================================================================================================================ */

Input* Input::s_instance = new GlfwInput();

bool GlfwWindow::s_glfwInitialized = false;
bool GlfwWindow::s_gladInitialized = false;

/* ================================================================================================================ */
/* WINDOW ========================================================================================================= */
/* ================================================================================================================ */

Window *Window::create(const WindowCreationProps &props) {
    return new GlfwWindow(props);
}