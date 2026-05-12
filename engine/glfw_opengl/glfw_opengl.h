//
// Created by Nico Russo on 5/12/26.
//

#ifndef SANDBOX_GLFW_OPENGL_H
#define SANDBOX_GLFW_OPENGL_H

#include "generic/application.h"

/* ================================================================================================================ */
/* IMGUI IMPLEMENTATION ========================================================================================== */
/* ================================================================================================================ */

class ImGuiImplGlfwOpenGL : public ImGuiImplementation {
protected:
    void initImpl() override {
        ImGui_ImplGlfw_InitForOpenGL(NATIVE_WINDOW, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }
    void shutdownImpl() override {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    }
    void newFrameImpl() override {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
    }
    void renderImpl() override {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    void updateWindowsAndContextImpl() override {
        GLFWwindow* backup = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup);
    }
};

/* ================================================================================================================ */
/* INPUT ========================================================================================================== */
/* ================================================================================================================ */

class GlfwInput : public Input {
protected:
    bool isKeyPressedImpl(int keycode) override {
        const auto state = glfwGetKey(NATIVE_WINDOW, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool isButtonPressedImpl(int button) override {
        const auto state = glfwGetMouseButton(NATIVE_WINDOW, button);
        return state == GLFW_PRESS;
    }

    std::pair<double, double> getMousePositionImpl() override {
        double x, y;
        glfwGetCursorPos(NATIVE_WINDOW, &x, &y);
        return {x, y};
    }
};

/* ================================================================================================================ */
/* WINDOW ========================================================================================================= */
/* ================================================================================================================ */

class GlfwWindow : public Window {
public:
    GlfwWindow(const WindowCreationProps& props = WindowCreationProps()) { setup(props); }
    ~GlfwWindow() override { cleanup(); }

    void update() override { glfwPollEvents(); glfwSwapBuffers(m_window); }

    [[nodiscard]] inline unsigned int screenWidth() const override { return m_data.screenWidth; }
    [[nodiscard]] inline unsigned int screenHeight() const override { return m_data.screenHeight; }
    [[nodiscard]] inline unsigned int framebufferWidth() const override { int x; int y; glfwGetFramebufferSize(m_window, &x, &y); return x; }
    [[nodiscard]] inline unsigned int framebufferHeight() const override { int x; int y; glfwGetFramebufferSize(m_window, &x, &y); return y; }

    [[nodiscard]] void* nativeWindow() const override { return m_window; }
private:
    static void glfwErrorCallback(const int error, const char* description) {
        LOG_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    void setup(const WindowCreationProps& props) {
        // setup data struct
        m_data.screenWidth = props.width;
        m_data.screenHeight = props.height;
        m_data.title = props.title;

        LOG_INFO("Creating GLFW window {}, ({}, {})", props.title, props.width, props.height);

        // initialize GLFW
        if (!s_glfwInitialized) {
            LOG_ASSERT(glfwInit() && "failed to initialize glfw");
            glfwSetErrorCallback(glfwErrorCallback);
            s_glfwInitialized = true;
        }

        // create the window
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);
        if (!m_window) { glfwTerminate(); LOG_ASSERT(false && "glfwCreateWindow failed!"); }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);
        glfwSetWindowUserPointer(m_window, &m_data);

        // initialize glad
        if (!s_gladInitialized) {
            LOG_ASSERT(gladLoadGL() != 0 && "glad load failed!");
            s_gladInitialized = true;
        }

        // SET GLFW CALLBACKS
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, const int w, const int h) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.screenWidth = w;
            data.screenHeight = h;

            WindowResizeEvent event(w,h);
            data.eventCallback(event);
        });

        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, const int w, const int h) {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.framebufferWidth = w;
            data.framebufferHeight = h;
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
            const WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.eventCallback(event);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
            const WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);
                    data.eventCallback(event);
                } break;
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    data.eventCallback(event);
                } break;
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, 1);
                    data.eventCallback(event);
                } break;
                default:
                    UNREACHABLE();
            }
        });

        glfwSetCharCallback(m_window, [](GLFWwindow* window, const unsigned int character) {
            const WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            KeyTypedEvent event(character);
            data.eventCallback(event);
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, const int button, const int action, const int mods) {
            const WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    data.eventCallback(event);
                } break;
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    data.eventCallback(event);
                } break;
                default:
                    UNREACHABLE();
            }
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, const double xoffset, const double yoffset) {
            const WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event(xoffset, yoffset);
            data.eventCallback(event);
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, const double xpos, const double ypos) {
            const WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event(xpos, ypos);
            data.eventCallback(event);
        });
    }

    void cleanup() const {
        glfwDestroyWindow(m_window);
    }

    void setEventCallback(const EventCallbackFn &callback) override { m_data.eventCallback = callback; }

private:
    GLFWwindow* m_window = nullptr;

    struct WindowData {
        std::string title;
        u32 screenWidth, screenHeight;
        u32 framebufferWidth, framebufferHeight;

        EventCallbackFn eventCallback;
    };

    WindowData m_data;
private:
    static bool s_glfwInitialized;
    static bool s_gladInitialized;
};

#endif //SANDBOX_GLFW_OPENGL_H
