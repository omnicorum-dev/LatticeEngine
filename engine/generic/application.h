//
// Created by Nico Russo on 5/12/26.
//

#ifndef SANDBOX_APPLICATION_H
#define SANDBOX_APPLICATION_H

#include "coreOpenGL.h"
#include "renderer.h"

#define NATIVE_WINDOW static_cast<GLFWwindow*>(Application::get().getWindow().nativeWindow())

/* ================================================================================================================ */
/* EVENTS (Generic) =============================================================================================== */
/* ================================================================================================================ */

enum class EventType {
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory {
    None = 0,
    EventCategoryApplication   = BIT(0),
    EventCategoryInput         = BIT(1),
    EventCategoryKeyboard      = BIT(2),
    EventCategoryMouse         = BIT(3),
    EventCategoryMouseButton   = BIT(4),
};

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; }\
virtual EventType getEventType() const override { return getStaticType(); }\
virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

class Event {
    friend class EventDispatcher;
public:
    virtual ~Event() = default;

    [[nodiscard]] virtual EventType       getEventType()        const = 0;
    [[nodiscard]] virtual const char*     getName()             const = 0;
    [[nodiscard]] virtual int             getCategoryFlags()    const = 0;
    [[nodiscard]] virtual std::string     toString()            const { return getName(); }

    [[nodiscard]] bool& handled() { return m_handled; }

    [[nodiscard]] inline bool isInCategory(const EventCategory category) const {
        return getCategoryFlags() & category;
    }
private:
    bool m_handled = false;
};

inline std::ostream& operator<<(std::ostream& os, const Event& event) {
    return os << event.toString();
}

class EventDispatcher {
    template<typename T>
    using EventFn = std::function<bool(T&)>;
public:
    EventDispatcher(Event& event) : m_event(event) {}

    template<typename T>
    bool dispatch(EventFn<T> func) {
        if (m_event.getEventType() == T::getStaticType()) {
            m_event.m_handled = func(*(T*)&m_event);
            return true;
        }
        return false;
    }
private:
    Event& m_event;
};

class KeyEvent : public Event {
public:
    [[nodiscard]] int getKeyCode() const { return m_keyCode; }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
    KeyEvent(const int keyCode) : m_keyCode(keyCode) {}

    int m_keyCode;
};

class KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(const int keycode, const int repeatCount) : KeyEvent(keycode), m_repeatCount(repeatCount) {}

    [[nodiscard]] int getRepeatCount() const { return m_repeatCount; }

    [[nodiscard]] std::string toString() const override {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPressed)
private:
    int m_repeatCount;
};

class KeyTypedEvent : public KeyEvent {
public:
    KeyTypedEvent(const unsigned int keycode) : KeyEvent(keycode) {}

    [[nodiscard]] std::string toString() const override {
        std::stringstream ss;
        ss << "KeyTypedEvent: " << m_keyCode;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyTyped)
};

class KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(const int keycode) : KeyEvent(keycode) {}

    [[nodiscard]] std::string toString() const override {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << m_keyCode;
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyReleased)
};

class WindowResizeEvent : public Event {
public:
    WindowResizeEvent(const unsigned int width, const unsigned int height) : m_width(width), m_height(height) {}

    [[nodiscard]] unsigned int GetWidth() const { return m_width; }
    [[nodiscard]] unsigned int GetHeight() const { return m_height; }

    [[nodiscard]] std::string toString() const override {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << m_width << ", " << m_height;
        return ss.str();
    }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
    unsigned int m_width, m_height;
};

class WindowCloseEvent : public Event {
public:
    WindowCloseEvent() = default;

    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppTickEvent : public Event {
public:
    AppTickEvent() = default;

    EVENT_CLASS_TYPE(AppTick)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent : public Event {
public:
    AppUpdateEvent() = default;

    EVENT_CLASS_TYPE(AppUpdate)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent : public Event {
public:
    AppRenderEvent() = default;

    EVENT_CLASS_TYPE(AppRender)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(const float x, const float y) : m_mouseX(x), m_mouseY(y) {}

    [[nodiscard]] float getX() const { return m_mouseX; }
    [[nodiscard]] float getY() const { return m_mouseY; }

    [[nodiscard]] std::string toString() const override {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << m_mouseX << "," << m_mouseY;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
private:
    float m_mouseX, m_mouseY;
};

class MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(const float xOffset, const float yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {}

    [[nodiscard]] float getXOffset() const { return m_xOffset; }
    [[nodiscard]] float getYOffset() const { return m_yOffset; }

    [[nodiscard]] std::string toString() const override {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << m_xOffset << "," << m_yOffset;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
private:
    float m_xOffset, m_yOffset;
};

class MouseButtonEvent : public Event {
public:
    [[nodiscard]] int GetMouseButton() const { return m_button; }

    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
protected:
    MouseButtonEvent(const int button) : m_button(button) {}
    int m_button;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
    MouseButtonPressedEvent(const int button) : MouseButtonEvent(button) {}

    [[nodiscard]] std::string toString() const override {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << m_button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
    MouseButtonReleasedEvent(const int button) : MouseButtonEvent(button) {}

    [[nodiscard]] std::string toString() const override {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << m_button;
        return ss.str();
    }

    EVENT_CLASS_TYPE(MouseButtonReleased)
};

/* ================================================================================================================ */
/* INPUT (Interface) ============================================================================================== */
/* ================================================================================================================ */

class Input {
public:
    virtual ~Input() = default;

    static bool isKeyPressed(const int keycode) { return s_instance->isKeyPressedImpl(keycode); }
    static bool isButtonPressed(const int button) { return s_instance->isButtonPressedImpl(button); }
    static std::pair<double, double> getMousePosition() { return s_instance->getMousePositionImpl(); }
    static double getMouseX() { return getMousePosition().first; }
    static double getMouseY() { return getMousePosition().second; }
protected:
    virtual bool isKeyPressedImpl(int keycode) = 0;
    virtual bool isButtonPressedImpl(int button) = 0;
    virtual std::pair<double, double> getMousePositionImpl() = 0;
private:
    static Input* s_instance;
};

/* ================================================================================================================ */
/* ImGui Implementation (Interface) =============================================================================== */
/* ================================================================================================================ */

class ImGuiImplementation {
public:
    virtual ~ImGuiImplementation() = default;

    static void init()                    { s_instance->initImpl(); }
    static void shutdown()                { s_instance->shutdownImpl(); }
    static void newFrame()                { s_instance->newFrameImpl(); }
    static void render()                  { s_instance->renderImpl(); }
    static void updateWindowsAndContext() { s_instance->updateWindowsAndContextImpl(); }

    static void setInstance(ImGuiImplementation* impl) { s_instance = impl; }
protected:

    virtual void initImpl() = 0;
    virtual void shutdownImpl() = 0;
    virtual void newFrameImpl() = 0;
    virtual void renderImpl() = 0;
    virtual void updateWindowsAndContextImpl() = 0;

private:
    static ImGuiImplementation* s_instance;
};

/* ================================================================================================================ */
/* Rendering (Generic) ============================================================================================ */
/* ================================================================================================================ */

// TODO

/* ================================================================================================================ */
/* LAYERS (Generic) =============================================================================================== */
/* ================================================================================================================ */

class Layer {
public:
    Layer(const std::string &debugName = "Layer") : m_debugName(debugName) {}
    virtual ~Layer() = default;

    virtual void setup() = 0;
    virtual void update() = 0;
    virtual void cleanup() = 0;

    virtual void onEvent(Event &event) = 0;

    [[nodiscard]] const std::string& name() const { return m_debugName; }
private:
    std::string m_debugName;
};

class LayerStack {
public:
    LayerStack() { m_layerInsertLocation = m_layers.begin(); }
    ~LayerStack() {
        for (const Layer* layer : m_layers) {
            delete layer;
        }
    }

    void pushLayer(Layer* layer) {
        m_layerInsertLocation = m_layers.emplace(m_layerInsertLocation, layer);
    }
    void pushOverlay(Layer* overlay) {
        m_layers.emplace_back(overlay);
    }
    void popLayer(Layer* layer) {
        auto it = std::ranges::find(m_layers, layer);
        if (it != m_layers.end()) {
            m_layers.erase(it);
            --m_layerInsertLocation;
        }
    }
    void popOverlay(Layer* overlay) {
        auto it = std::ranges::find(m_layers, overlay);
        if (it != m_layers.end()) {
            m_layers.erase(it);
        }
    }

    std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
    std::vector<Layer*>::iterator end() { return m_layers.end(); }
    std::vector<Layer*>::const_iterator begin() const { return m_layers.begin(); }
    std::vector<Layer*>::const_iterator end() const { return m_layers.end(); }
private:
    std::vector<Layer*> m_layers;
    std::vector<Layer*>::iterator m_layerInsertLocation;
};

/* ================================================================================================================ */
/* WINDOW  (Interface) ============================================================================================ */
/* ================================================================================================================ */

struct WindowCreationProps {
    std::string title;
    u32 width;
    u32 height;

    WindowCreationProps(std::string  title = "May 10 Engine",
        const unsigned int width = 800, const unsigned int height = 600) : title(std::move(title)), width(width), height(height) {}
};

class Window {
public:
    using EventCallbackFn = std::function<void(Event&)>;

    static Window* create(const WindowCreationProps& props = WindowCreationProps());
    virtual ~Window() = default;

    virtual void update() = 0;

    [[nodiscard]] virtual unsigned int screenWidth() const = 0;
    [[nodiscard]] virtual unsigned int screenHeight() const = 0;
    [[nodiscard]] virtual unsigned int framebufferWidth() const = 0;
    [[nodiscard]] virtual unsigned int framebufferHeight() const = 0;

    virtual void setEventCallback(const EventCallbackFn& callback) = 0;

    [[nodiscard]] virtual void* nativeWindow() const = 0;
};

/* ================================================================================================================ */
/* APPLICATION (Generic) ========================================================================================== */
/* ================================================================================================================ */

class Application {
public:
    Application(const WindowCreationProps& props = WindowCreationProps()) {
        LOG_ASSERT(!s_instance && "Application already exists");
        s_instance = this;
        m_window = std::unique_ptr<Window>(Window::create(props));

        m_window->setEventCallback([this](Event& e) { onEvent(e); });
    }
    virtual ~Application() = default;

    void run() {
        Renderer::init();
        setup(); // layers get pushed here

        while (m_running) {
            Renderer::beginFrame();

            // update layers
            for (Layer* layer : m_layerStack) {
                layer->update();
            }

            Renderer::execute();
            Renderer::endFrame();

            // update window
            m_window->update();
        }

        Renderer::shutdown();
        cleanup(); // layers are destroyed here
    }

    void onEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.dispatch<WindowCloseEvent>([this](WindowCloseEvent& e) { return onWindowClose(e); });

        for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
            (*--it)->onEvent(event);
            if (event.handled()) {
                break;
            }
        }
    }

    void pushLayer(Layer* layer) {
        m_layerStack.pushLayer(layer);
        layer->setup();
    }
    void pushOverlay(Layer* overlay) {
        m_layerStack.pushOverlay(overlay);
        overlay->setup();
    }
    void popLayer(Layer* layer) {
        m_layerStack.popLayer(layer);
        layer->cleanup();
    }
    void popOverlay(Layer* overlay) {
        m_layerStack.popOverlay(overlay);
        overlay->cleanup();
    }

    static Application& get() { return *s_instance;}
    [[nodiscard]] Window& getWindow() const { return *m_window; }
private:
    virtual void setup() {};
    virtual void cleanup() {};

    bool onWindowClose(WindowCloseEvent& event) {
        m_running = false;
        return true;
    }

    bool m_running = true;
    std::unique_ptr<Window> m_window;
    LayerStack m_layerStack;
private:
    static Application* s_instance;
};

// USER DEFINED
Application* createApplication();

#endif //SANDBOX_APPLICATION_H
