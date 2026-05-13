#include <iostream>

#include "engine.h"
#include "glfw_opengl/opengl_renderer.h"

class ImGuiEditorLayer : public Layer {
public:
    ImGuiEditorLayer(Ref<RenderTarget> source) : Layer("ImGuiEditor Layer"), m_source(std::move(source)) {}
    ~ImGuiEditorLayer() override = default;

    void update() override {

        // 1. act on the pending resize from last frame, before rendering
        if (m_pendingResize) {
            m_viewportTarget->resize(m_pendingViewportSize.x, m_pendingViewportSize.y);
            m_material->setTexture("u_buffer", m_source->getColorAttachment(0));
            m_material->set("u_targetAspect", m_pendingViewportSize.x / m_pendingViewportSize.y);
            m_pendingResize = false;
        }

        // 2. render into the correctly-sized target
        Renderer::bindRenderTarget(m_viewportTarget);
        Renderer::submit({ m_geometry, m_material, Transform{} });
        Renderer::execute();
        Renderer::unbindRenderTarget();

        beginFrame();

        ImGui::PushFont(customFont);

        // Menu Bar
        if (ImGui::BeginMainMenuBar()) {

            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New"))  { LOG_DEBUG("File/New Pressed!"); }
                if (ImGui::MenuItem("Open")) { LOG_DEBUG("File/Open Pressed!"); }
                if (ImGui::BeginMenu("Open Recent"))
                {
                    if (ImGui::MenuItem("Recent item 1")) { LOG_DEBUG("Open Recent item 1!"); }
                    if (ImGui::MenuItem("Recent item 2")) { LOG_DEBUG("Open Recent item 2!"); }
                    if (ImGui::MenuItem("Recent item 3")) { LOG_DEBUG("Open Recent item 3!"); }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Save")) { LOG_DEBUG("Save Pressed!"); }
                if (ImGui::MenuItem("Save As")) { LOG_DEBUG("Save As Pressed!"); }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit")) {
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View")) {
                if (ImGui::MenuItem("Toggle Demo Window")) { m_showDemoWindow = !m_showDemoWindow; }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        ImGui::DockSpaceOverViewport();

        // Log Console
        ImGui::Begin("Log Console", nullptr, ImGuiWindowFlags_MenuBar);

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Test Logs")) {
                if (ImGui::MenuItem("Test Trace")) { LOG_TRACE("Testing trace"); }
                if (ImGui::MenuItem("Test Debug")) { LOG_DEBUG("Testing debug"); }
                if (ImGui::MenuItem("Test Info"))  { LOG_INFO("Testing info"); }
                if (ImGui::MenuItem("Test Warn"))  { LOG_WARN("Testing warn"); }
                if (ImGui::MenuItem("Test Error")) { LOG_ERROR("Testing error"); }
                if (ImGui::MenuItem("Test Fatal")) { LOG_FATAL("Testing fatal"); }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        for (const nopp::LogEvent& logEvent : nopp::log_handler) {
            ImColor color = {1.0f, 1.0f, 1.0f , 1.0f};
            std::string prefix;
            switch (logEvent.level) {
                case nopp::LogLevel::TRACE:
                    color = ImVec4(0.75f, 0.75f, 0.75f, 1.0f);
                    prefix = "[TRACE] ";
                    break;
                case nopp::LogLevel::DEBUG:
                    color = ImVec4(0.5f, 0.6f, 1.0f, 1.0f);
                    prefix = "[DEBUG] ";
                    break;
                case nopp::LogLevel::INFO:
                    color = ImVec4(0.4f, 0.8f, 0.5f, 1.0f);
                    prefix = " [INFO] ";
                    break;
                case nopp::LogLevel::WARN:
                    color = ImVec4(0.95f, 0.6f, 0.85f, 1.0f);
                    prefix = " [WARN] ";
                    break;
                case nopp::LogLevel::ERROR:
                    color = ImVec4(0.9f, 0.4f, 0.4f, 1.0f);
                    prefix = "[ERROR] ";
                    break;
                case nopp::LogLevel::FATAL:
                    color = ImVec4(0.9f, 0.4f, 0.4f, 1.0f);
                    prefix = "[FATAL] ";
                    break;
                default:
                    LOG_ASSERT(false && "unknown log level");
            }
            ImGui::TextColored(color, "%s %s", prefix.c_str(), logEvent.message.c_str());
        }
        ImGui::End();

        // Inspector
        ImGui::Begin("Inspector");
        ImGui::Text("Inspector will go here");
        ImGui::End();

        // Scene Tree
        ImGui::Begin("Scene Tree");
        ImGui::Text("Scene Tree will go here");
        ImGui::End();

        // Viewport
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
        ImGui::PopStyleVar(2);

        // 3. read the viewport size this frame and queue a resize for next frame if it changed
        ImVec2 viewportContentSize = ImGui::GetContentRegionAvail();
        if (viewportContentSize.x != m_pendingViewportSize.x ||
            viewportContentSize.y != m_pendingViewportSize.y) {
            m_pendingViewportSize = viewportContentSize;
            m_pendingResize = true;
            }

        // 4. display whatever is in the target — always valid, sized for this frame
        const auto glTex = std::dynamic_pointer_cast<OpenGLTexture2D>(m_viewportTarget->getColorAttachment(0));
        ImGui::Image((ImTextureID)(uintptr_t)glTex->getID(), viewportContentSize, ImVec2(0, 1), ImVec2(1, 0));

        m_viewportFocused = ImGui::IsWindowFocused();
        m_viewportHovered = ImGui::IsWindowHovered();
        ImGui::End();

        // Demo window
        if (m_showDemoWindow)
            ImGui::ShowDemoWindow(&m_showDemoWindow);

        ImGui::PopFont();

        endFrame();
    }

    void setup() override {
        // SETUP VIEWPORT TARGET
        m_viewportTarget = RenderTarget::create({
            .width            = 1280,
            .height           = 720,
            .colorAttachments = { TextureFormat::RGBA8 },
            .clearColor       = true,
            .clearColorValue  = { 0.1, 0.1, 0.1, 1.f },
            .filterMin = TextureSpec::Filter::Nearest,
            .filterMag = TextureSpec::Filter::Nearest,
        });

        const std::vector<float> vertices = {
            // position            texCoord
            -1.0f, -1.0f, 0.0f,    0.0f, 0.0f,
             1.0f, -1.0f, 0.0f,    1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,    1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f,    0.0f, 1.0f,
        };
        const std::vector<u32> indices = { 0, 1, 2,  2, 3, 0 };

        const auto vb = VertexBuffer::create(vertices.data(), vertices.size() * sizeof(float));
        vb->setLayout({
            { ShaderDataType::Float3, "a_position" },
            { ShaderDataType::Float2, "a_texCoord" },
        });

        const auto ib = IndexBuffer::create(indices.data(), indices.size());
        m_geometry = Geometry::create(vb, ib);

        // shader
        const auto shader = ShaderLoader::load({
            { ShaderStage::Vertex,   cur_dir / "assets/shaders/quadAspect.vert" },
            { ShaderStage::Fragment, cur_dir / "assets/shaders/quadTexture.frag" },
        });

        // pipeline
        PipelineSpec pipelineSpec;
        pipelineSpec.shader       = shader;
        pipelineSpec.blendMode = BlendMode::Alpha;
        pipelineSpec.vertexLayout = {
                { ShaderDataType::Float3, "a_position" },
                { ShaderDataType::Float2, "a_texCoord" },
            };
        m_pipeline = RenderPipeline::create(pipelineSpec);

        // material
        const auto layout = MakeRef<MaterialLayout>(MaterialLayout{
            { MaterialParamType::Texture2D, "u_buffer" },
            { ShaderDataType::Float, "u_sourceAspect" },
            { ShaderDataType::Float, "u_targetAspect" }
        });
        m_material = MakeRef<Material>(m_pipeline, layout);

        // The texture object is stable — same ptr every frame,
        // contents are whatever TestLayer last rendered into it
        m_material->setTexture("u_buffer", m_source->getColorAttachment(0));
        m_material->set("u_sourceAspect", 1280.0f / 720.0f);
        m_material->set("u_targetAspect", 1280.0f / 720.0f);

        // setup IMGUI

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // enable keyboard controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // enable gamepad controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // enable docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // enable multi-viewport / platform windows
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        io.Fonts->AddFontDefault();
        customFont = io.Fonts->AddFontFromFileTTF("/Users/nicorusso/Development/may10engine/assets/fonts/JetBrainsMonoNL-Medium.ttf", 18.f);
        LOG_ASSERT(customFont && "failed to load custom font");
        // setup platform/renderer bindings
        ImGuiImplementation::init();
    }

    void cleanup() override {
        ImGuiImplementation::shutdown();
        ImGui::DestroyContext();
    }

    void onEvent(Event& event) override {
        if (!m_viewportFocused) {
            if (event.isInCategory(EventCategoryInput)) {
                event.handled() = true;
            }
        }
    }

private:
    static void beginFrame() {
        ImGuiImplementation::newFrame();
        ImGui::NewFrame();
    }
    static void endFrame() {
        ImGui::Render();
        ImGuiImplementation::render();

        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGuiImplementation::updateWindowsAndContext();
        }
    }

private:
    bool m_viewportFocused = false;
    bool m_viewportHovered = false;

    bool m_showDemoWindow = false;

    ImFont* customFont = nullptr;

    ImVec2 m_pendingViewportSize = { 1280.f, 720.f };
    bool   m_pendingResize = false;

    Ref<Geometry>       m_geometry;
    Ref<Material>       m_material;
    Ref<RenderPipeline> m_pipeline;
    Ref<RenderTarget> m_source;
    Ref<RenderTarget> m_viewportTarget;
};




class TestLayer : public Layer {
public:
    TestLayer(Ref<RenderTarget> target) : Layer("Test Layer"),  m_renderTarget(std::move(target)) {}
    ~TestLayer() override = default;

    void setup() override {
        const std::vector<float> vertices = {
            -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,     0.0f, 0.0f,
             0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,     1.0f, 0.0f,
             0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,     0.5f, 1.0f,
        };

        const std::vector<u32> indices = { 0, 1, 2 };

        const auto vb = VertexBuffer::create(vertices.data(), vertices.size() * sizeof(float));
        vb->setLayout( {
            { ShaderDataType::Float3, "a_position" },
            { ShaderDataType::Float3, "a_normal" },
            { ShaderDataType::Float2, "a_texCoord" }
        });

        const auto ib = IndexBuffer::create(indices.data(), indices.size());
        m_geometry = Geometry::create(vb, ib);

        // shader
        const auto shader = ShaderLoader::load({
            { ShaderStage::Vertex,   cur_dir / "assets/triangle.vert" },
            { ShaderStage::Fragment, cur_dir / "assets/triangle.frag" },
        });

        // pipeline
        PipelineSpec pipelineSpec;
        pipelineSpec.shader      = shader;
        // TODO: will be automated with shader reflection
        pipelineSpec.vertexLayout = {
                { ShaderDataType::Float3, "a_position" },
                { ShaderDataType::Float3, "a_normal"   },
                { ShaderDataType::Float2, "a_texCoord" },
            };
        m_pipeline = RenderPipeline::create(pipelineSpec);

        // material
        const auto layout = MakeRef<MaterialLayout>(MaterialLayout{
            { ShaderDataType::Float3, "u_color" },
        });
        m_material = MakeRef<Material>(m_pipeline, layout);
        m_material->set("u_color", glm::vec3(1.0f, 1.0f, 1.0f));
    }

    void onEvent(Event& event) override {}

    void update() override {
        Renderer::bindRenderTarget(m_renderTarget);
        Renderer::submit({ m_geometry, m_material, Transform{} });
        Renderer::execute();
        Renderer::unbindRenderTarget();
    }

    void cleanup() override {
        m_geometry.reset();
        m_material.reset();
        m_pipeline.reset();
    }

private:
    Ref<Geometry>       m_geometry;
    Ref<Material>       m_material;
    Ref<RenderPipeline> m_pipeline;

    Ref<RenderTarget> m_renderTarget;
};


class TexTestLayer : public Layer {
public:
    TexTestLayer(Ref<RenderTarget> source) : Layer("TexTestLayer"), m_source(std::move(source)) {}
    ~TexTestLayer() override = default;

    void setup() override {
        Renderer::setClearColor(0.1, 0.1, 0.1, 1.0);

        const std::vector<float> vertices = {
            // position            normal               texCoord
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
             0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
             0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
        };
        const std::vector<u32> indices = { 0, 1, 2,  2, 3, 0 };

        const auto vb = VertexBuffer::create(vertices.data(), vertices.size() * sizeof(float));
        vb->setLayout({
            { ShaderDataType::Float3, "a_position" },
            { ShaderDataType::Float3, "a_normal"   },
            { ShaderDataType::Float2, "a_texCoord" },
        });

        const auto ib = IndexBuffer::create(indices.data(), indices.size());
        m_geometry = Geometry::create(vb, ib);

        // texture
        m_texture = TextureLoader::load(cur_dir / "assets/textures/car_red.png");

        // shader
        const auto shader = ShaderLoader::load({
            { ShaderStage::Vertex,   cur_dir / "assets/texture.vert" },
            { ShaderStage::Fragment, cur_dir / "assets/texture.frag" },
        });

        // pipeline
        PipelineSpec pipelineSpec;
        pipelineSpec.shader       = shader;
        pipelineSpec.blendMode = BlendMode::Alpha;
        pipelineSpec.vertexLayout = {
                { ShaderDataType::Float3, "a_position" },
                { ShaderDataType::Float3, "a_normal"   },
                { ShaderDataType::Float2, "a_texCoord" },
            };
        m_pipeline = RenderPipeline::create(pipelineSpec);

        // material
        const auto layout = MakeRef<MaterialLayout>(MaterialLayout{
            { MaterialParamType::Texture2D, "u_texture" },
        });
        m_material = MakeRef<Material>(m_pipeline, layout);

        // The texture object is stable — same ptr every frame,
        // contents are whatever TestLayer last rendered into it
        m_material->setTexture("u_texture", m_source->getColorAttachment(0));
    }

    void onEvent(Event& event) override {}

    void update() override {
        Renderer::submit({ m_geometry, m_material, Transform{} });
        Renderer::execute();
    }

    void cleanup() override {
        m_geometry.reset();
        m_material.reset();
        m_pipeline.reset();
        m_texture.reset();
    }

private:
    Ref<Geometry>       m_geometry;
    Ref<Material>       m_material;
    Ref<RenderPipeline> m_pipeline;
    Ref<Texture2D>      m_texture;

    Ref<RenderTarget> m_source;
};

class AspectLayer : public Layer {
public:
    AspectLayer(Ref<RenderTarget> source) : Layer("Aspect Layer"), m_source(std::move(source)) {}
    ~AspectLayer() override = default;

    void onEvent(Event& event) override {
        if (event.getEventType() == EventType::WindowResize) {
            const float targetAspect = static_cast<float>(Application::get().getWindow().framebufferWidth()) / static_cast<float>(Application::get().getWindow().framebufferHeight());
            m_material->set("u_targetAspect", targetAspect);
        }
    }

    void update() override {
        Renderer::submit({ m_geometry, m_material, Transform{} });
        Renderer::execute();
    }

    void cleanup() override {
        m_geometry.reset();
        m_material.reset();
        m_pipeline.reset();
    }

    void setup() override {
        Renderer::setClearColor(1.0, 1.0, 1.0, 1.0);

        const std::vector<float> vertices = {
            // position            texCoord
            -1.0f, -1.0f, 0.0f,    0.0f, 0.0f,
             1.0f, -1.0f, 0.0f,    1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,    1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f,    0.0f, 1.0f,
        };
        const std::vector<u32> indices = { 0, 1, 2,  2, 3, 0 };

        const auto vb = VertexBuffer::create(vertices.data(), vertices.size() * sizeof(float));
        vb->setLayout({
            { ShaderDataType::Float3, "a_position" },
            { ShaderDataType::Float2, "a_texCoord" },
        });

        const auto ib = IndexBuffer::create(indices.data(), indices.size());
        m_geometry = Geometry::create(vb, ib);

        // shader
        const auto shader = ShaderLoader::load({
            { ShaderStage::Vertex,   cur_dir / "assets/shaders/quadAspect.vert" },
            { ShaderStage::Fragment, cur_dir / "assets/shaders/quadTexture.frag" },
        });

        // pipeline
        PipelineSpec pipelineSpec;
        pipelineSpec.shader       = shader;
        pipelineSpec.blendMode = BlendMode::Alpha;
        pipelineSpec.vertexLayout = {
                { ShaderDataType::Float3, "a_position" },
                { ShaderDataType::Float2, "a_texCoord" },
            };
        m_pipeline = RenderPipeline::create(pipelineSpec);

        // material
        const auto layout = MakeRef<MaterialLayout>(MaterialLayout{
            { MaterialParamType::Texture2D, "u_buffer" },
            { ShaderDataType::Float, "u_sourceAspect" },
            { ShaderDataType::Float, "u_targetAspect" }
        });
        m_material = MakeRef<Material>(m_pipeline, layout);

        // The texture object is stable — same ptr every frame,
        // contents are whatever TestLayer last rendered into it
        m_material->setTexture("u_buffer", m_source->getColorAttachment(0));
        m_material->set("u_sourceAspect", 1280.0f / 720.0f);
        m_material->set("u_targetAspect", 1280.0f / 720.0f);
    }

private:
    Ref<Geometry>       m_geometry;
    Ref<Material>       m_material;
    Ref<RenderPipeline> m_pipeline;

    Ref<RenderTarget> m_source;
};


class May10EngineEditor : public Application {
public:
    May10EngineEditor(const WindowCreationProps& props = WindowCreationProps()) : Application(props) {
        const auto sharedTarget = RenderTarget::create({
            .width            = 128,
            .height           = 72,
            .colorAttachments = { TextureFormat::RGBA8 },
            .depthAttachment  = TextureFormat::Depth24Stencil8,
            .clearColor       = true,
            .clearColorValue  = { 0.9f, 0.3f, 0.6f, 1.f },
            .filterMin = TextureSpec::Filter::Nearest,
            .filterMag = TextureSpec::Filter::Nearest,
        });

        pushLayer(new TestLayer(sharedTarget));
        //pushLayer(new TexTestLayer(sharedTarget));
        //pushLayer(new AspectLayer(sharedTarget));
        pushOverlay(new ImGuiEditorLayer(sharedTarget));
    }
};

WindowCreationProps windowProps(
    "SILICON EDITOR",
    static_cast<u32>(1280),
    static_cast<u32>(720)
);

Application* createApplication() {
    return new May10EngineEditor(windowProps);
}