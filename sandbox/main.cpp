#include <iostream>

#include "engine.h"

class ImGuiEditorLayer : public Layer {
public:
    ImGuiEditorLayer() : Layer("ImGuiEditor Layer") {}
    ~ImGuiEditorLayer() override = default;

    void update() override {
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
        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar);
        ImGui::PopStyleVar(2);

        ImGui::TextColored({0.8, 0.8, 0.2, 1.0}, "Viewport will go here!!!");

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
};



class TestLayer : public Layer {
public:
    TestLayer() : Layer("Test Layer") {}
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
        Renderer::submit( {m_geometry, m_material, Transform{} });
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
};


class TexTestLayer : public Layer {
public:
    TexTestLayer() : Layer("TexTestLayer") {}
    ~TexTestLayer() override = default;

    void setup() override {
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
        m_material->setTexture("u_texture", m_texture);
    }

    void onEvent(Event& event) override {}

    void update() override {
        Renderer::submit({ m_geometry, m_material, Transform{} });
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
};


class May10EngineEditor : public Application {
public:
    May10EngineEditor(const WindowCreationProps& props = WindowCreationProps()) : Application(props) {
        //pushOverlay(new ImGuiEditorLayer());
        //pushLayer(new TestLayer());
        pushLayer(new TexTestLayer());
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