//
// Created by Nico Russo on 5/12/26.
//

#ifndef SANDBOX_RENDERER_H
#define SANDBOX_RENDERER_H

#include "coreOpenGL.h"

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename... Args>
std::shared_ptr<T> MakeRef(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}


/*
 * struct RenderCommand {
 *     Ref<Geometry> geometry; // WHAT TO DRAW
 *     Ref<Material> material; // HOW TO DRAW IT
 *     Transform transform;    // WHERE TO DRAW IT
 * }
 *
 * PipelineSpec pipelineSpec;
 * pipelineSpec.shader = shader;
 * pipelineSpec.layout = bufferLayout;
 *
 * // note: should eventually use a PIPELINE CACHE, so already used PipelineSpecs can just reuse the Pipeline that was created with it.
 * Ref<Pipeline> pipeline = Pipeline::create(pipelineSpec);
 *
 * // note: eventually implement shader reflection (parse the shader files so the pipeline knows parameter types and offsets)
 * //            (eventually, don't even parse shaders. compile the shaders to something like SPIR-V and reflect that. maybe there's a library that does this already?)
 * //     this way, we can do actual typechecking when setting parameters on the material and dont have to specify string names like "u_color" or "u_transform"
 * Ref<Material> material = Material::create(pipeline);
 * material->set("u_color", {1.0f, 1.0f, 1.0f});
 *
 * Transform transform = { glm::mat4(1.0f) };
 *
 * // when rendering:
 * RenderCommand cmd = { geometry, material, transform }; // geometry contains VertexBuffer and IndexBuffer
 * Renderer::submit( cmd );
 *
 * // later in the render loop:
 * // sort submissions by pipeline first, then material within that { (p1, m1), (p1, m2), (p1, m2), (p2, m1), (p2, m3), ... }
 * // eventual sort key: (pass, pipeline, material, geometry)
 * void execute() {
 *     // sort the queue and whatever else could optimize it
 *     for (const RenderCommand& cmd : s_drawQueue) {
 *         draw(cmd);
 *     }
 * }
 *
 * // draw(cmd) implementation notes:
 * if cmd.pipeline != currentPipeline
 *     use cmd.pipeline
 *     currentPipeline = cmd.pipeline
 *
 * if cmd.material != currentMaterial
 *     bind descriptor/resources
 *     currentMaterial = cmd.material
 *
 * upload transform/object data
 *
 * if cmd.geometry != currentGeometry
 *     bind vertex/index buffers
 *     currentGeometry = cmd.geometry
 *
 * issue draw call
*/

/* ================================================================================================================ */
/* BUFFERS AND TYPES ============================================================================================== */
/* ================================================================================================================ */

/* GENERIC */
enum class ShaderDataType : u8 {
    None = 0,
    Float, Float2, Float3, Float4,
    Int,   Int2,   Int3,   Int4,
    UInt,  UInt2,  UInt3,  UInt4,
                   Mat3,   Mat4,
    Bool,
};

/* GENERIC */
static u32 ShaderDataTypeSize(const ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:   return 4;
        case ShaderDataType::Float2:  return 4 * 2;
        case ShaderDataType::Float3:  return 4 * 3;
        case ShaderDataType::Float4:  return 4 * 4;
        case ShaderDataType::Int:     return 4;
        case ShaderDataType::Int2:    return 4 * 2;
        case ShaderDataType::Int3:    return 4 * 3;
        case ShaderDataType::Int4:    return 4 * 4;
        case ShaderDataType::UInt:    return 4;
        case ShaderDataType::UInt2:   return 4 * 2;
        case ShaderDataType::UInt3:   return 4 * 3;
        case ShaderDataType::UInt4:   return 4 * 4;
        case ShaderDataType::Mat3:    return 4 * 3 * 3;
        case ShaderDataType::Mat4:    return 4 * 4 * 4;
        case ShaderDataType::Bool:    return 1;
        default:
            LOG_ASSERT(false && "Unknown ShaderDataType");
    }
}

/* GENERIC */
static u32 GetShaderDataTypeComponentCount(const ShaderDataType& type) {
    switch (type) {
        case ShaderDataType::Float:   return 1;
        case ShaderDataType::Float2:  return 2;
        case ShaderDataType::Float3:  return 3;
        case ShaderDataType::Float4:  return 4;
        case ShaderDataType::Int:     return 1;
        case ShaderDataType::Int2:    return 2;
        case ShaderDataType::Int3:    return 3;
        case ShaderDataType::Int4:    return 4;
        case ShaderDataType::UInt:    return 1;
        case ShaderDataType::UInt2:   return 2;
        case ShaderDataType::UInt3:   return 3;
        case ShaderDataType::UInt4:   return 4;
        case ShaderDataType::Mat3:    return 3 * 3;
        case ShaderDataType::Mat4:    return 4 * 4;
        case ShaderDataType::Bool:    return 1;
        default: LOG_ASSERT(false && "Unknown ShaderDataType");
    }
}

/* GENERIC */
struct VertexAttribute {
    ShaderDataType type;
    std::string name;
    u32 size;
    bool normalized;

    u32 offset; // calculated automatically

    VertexAttribute(const ShaderDataType type, const std::string& name, const bool normalized = false)
        : type(type), name(name), normalized(normalized) {
        size   = ShaderDataTypeSize(type);
        offset = 0;
    }
};

/* GENERIC */
class VertexLayout {
public:
    VertexLayout() = default;

    VertexLayout(const std::initializer_list<VertexAttribute>& attributes)
        : m_attributes(attributes)
    {
        calculateOffsetAndStride();
    }

    [[nodiscard]] u32 getStride() const { return m_stride; }
    [[nodiscard]] const std::vector<VertexAttribute>& getAttributes() const { return m_attributes; }

private:
    void calculateOffsetAndStride() {
        u32 offset = 0;
        m_stride = 0;
        for (auto& attribute : m_attributes) {
            attribute.offset = offset;
            offset += attribute.size;
            m_stride += attribute.size;
        }
    }
private:
    std::vector<VertexAttribute> m_attributes;
    u32 m_stride = 0;
};

/* INTERFACE */
class VertexBuffer {
public:
    virtual ~VertexBuffer() = default;
    virtual void setData(const void* data, u32 size) = 0;

    static Ref<VertexBuffer> create(const void* data, u32 size);
    static Ref<VertexBuffer> create(u32 size);

    void setLayout(const VertexLayout& layout) { m_layout = layout; }
    [[nodiscard]] const VertexLayout& getLayout() const { return m_layout; }
private:
    VertexLayout m_layout;
};

/* INTERFACE */
class IndexBuffer {
public:
    virtual ~IndexBuffer() = default;
    [[nodiscard]] virtual u32 getCount() const = 0;

    static Ref<IndexBuffer> create(const u32* indices, u32 count);
};

/* ================================================================================================================ */
/* GEOMETRY ======================================================================================================= */
/* ================================================================================================================ */

/* INTERFACE */
class Geometry {
public:
    virtual ~Geometry() = default;

    [[nodiscard]] const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const { return m_vertexBuffers; }
    [[nodiscard]] const Ref<IndexBuffer>& getIndexBuffer() const { return m_indexBuffer; }

    static Ref<Geometry> create(const Ref<VertexBuffer>& vb, const Ref<IndexBuffer>& ib);
protected:
    std::vector<Ref<VertexBuffer>> m_vertexBuffers;
    Ref<IndexBuffer> m_indexBuffer;
};

/* ================================================================================================================ */
/* SHADERS ======================================================================================================== */
/* ================================================================================================================ */

/* GENERIC */
enum class ShaderStage { Vertex, Fragment, Compute };

/* GENERIC */
struct ShaderBinary {
    std::vector<u32> data; // future SPIR-V
};

/* GENERIC */
template<>
struct std::hash<ShaderStage> {
    size_t operator()(const ShaderStage stage) const noexcept {
        return hash<int>()(static_cast<int>(stage));
    }
};

/* INTERFACE */
class Shader {
public:
    virtual ~Shader() = default;

    static Ref<Shader> create(const std::unordered_map<ShaderStage, std::string>& sources);

    [[nodiscard]] virtual const std::unordered_map<ShaderStage, std::string>& getSources() const = 0;

    [[nodiscard]] virtual const ShaderBinary& getBinary(ShaderStage stage) const = 0;

    [[nodiscard]] const std::string& getName() const { return m_debugName; }
protected:
    std::unordered_map<ShaderStage, std::string> m_sources;

    std::string m_debugName;
};

/* GENERIC */
class ShaderLoader {
public:
    static Ref<Shader> load(const std::unordered_map<ShaderStage, std::filesystem::path>& paths) {
        std::unordered_map<ShaderStage, std::string> sources;

        for (auto& [stage, path] : paths) {
            sources[stage] = readFile(path);
        }

        return Shader::create(sources);
    }
private:
    static std::string readFile(const std::filesystem::path& path) {
        std::ifstream file(path);
        if (!file.is_open())
            throw std::runtime_error("Failed to open shader file: " + path.string());
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
};

/* ================================================================================================================ */
/* TEXTURES ======================================================================================================= */
/* ================================================================================================================ */

/* GENERIC */
enum class TextureFormat {
    RGBA8,   // unsigned char, standard images
    RGBA16F, // float, HDR
};

/* GENERIC */
struct TextureSpec {
    u32 width = 1;
    u32 height = 1;
    u32 channels = 4; // stb_image channels output
    TextureFormat format     = TextureFormat::RGBA8;

    // parameters
    enum class Filter { Linear, Nearest };
    enum class Wrap { Repeat, ClampToEdge, MirroredRepeat };

    Filter filterMin = Filter::Nearest;
    Filter filterMag = Filter::Nearest;
    Wrap   wrapS = Wrap::Repeat;
    Wrap   wrapT = Wrap::Repeat;
};

/* GENERIC */
struct CubeTextureSpec {
    u32  width    = 1;
    u32  height   = 1;   // per face
    u32  channels = 4;
    TextureFormat format     = TextureFormat::RGBA8;

    TextureSpec::Filter filterMin = TextureSpec::Filter::Linear;
    TextureSpec::Filter filterMag = TextureSpec::Filter::Linear;
};

/* GENERIC (Implementations are in Texture2D and TextureCube */
class Texture {
public:
    virtual ~Texture() = default;

    [[nodiscard]] virtual u32 getWidth() const = 0;
    [[nodiscard]] virtual u32 getHeight() const = 0;
    [[nodiscard]] virtual u32 getSlot() const = 0; // which texture unit it is bound to
};

/* INTERFACE (implement in cpp) */
class Texture2D : public Texture {
public:
    // raw pixel data from stb_image
    static Ref<Texture2D> create(const TextureSpec& spec, const unsigned char* data); // RGBA8
    static Ref<Texture2D> create(const TextureSpec& spec, const float* data);         // RGBA16F
};

/* INTERFACE (implement in cpp) */
class TextureCube : public Texture {
public:
    // 6 separate face buffers — order: right, left, top, bottom, front, back
    static Ref<TextureCube> createFromFaces(const CubeTextureSpec& spec,
                                            const std::array<const unsigned char*, 6>& faces);

    // single cross/strip image, already loaded by stb_image
    static Ref<TextureCube> createFromCross(const CubeTextureSpec& spec,
                                            const unsigned char* data);

    // HDR variant
    static Ref<TextureCube> createFromFaces(const CubeTextureSpec& spec,
                                            const std::array<const float*, 6>& faces);

    // HDR variant
    static Ref<TextureCube> createFromCross(const CubeTextureSpec& spec,
                                            const float* data);
};

/* GENERIC */
class TextureLoader {
public:
    static Ref<Texture2D> load(const std::filesystem::path& path,
                               const TextureSpec& spec = {}) {
        stbi_set_flip_vertically_on_load(true);
        int w, h, c;
        unsigned char* data = stbi_load(path.string().c_str(), &w, &h, &c, 4);
        if (!data)
            throw std::runtime_error("Failed to load texture: " + path.string());

        TextureSpec s  = spec;
        s.width        = w;
        s.height       = h;
        s.channels     = 4;  // forced 4 by stbi_load's last arg

        auto texture = Texture2D::create(s, data);
        stbi_image_free(data);
        return texture;
    }

    static Ref<TextureCube> loadFromFaces(const std::array<std::filesystem::path, 6>& paths,
                                          const CubeTextureSpec& spec = {}) {

        stbi_set_flip_vertically_on_load(false);
        std::array<unsigned char*, 6> faceData{};
        int w, h, c;

        for (int i = 0; i < 6; i++) {
            faceData[i] = stbi_load(paths[i].string().c_str(), &w, &h, &c, 4);
            if (!faceData[i])
                throw std::runtime_error("Failed to load cubemap face: " + paths[i].string());
        }

        CubeTextureSpec s = spec;
        s.width           = w;
        s.height          = h;
        s.channels        = 4;

        auto texture = TextureCube::createFromFaces(s, { faceData[0], faceData[1],
                                                         faceData[2], faceData[3],
                                                         faceData[4], faceData[5] });
        for (auto* d : faceData) stbi_image_free(d);
        return texture;
    }

    static Ref<TextureCube> loadFromCross(const std::filesystem::path& path,
                                          const CubeTextureSpec& spec = {}) {

        stbi_set_flip_vertically_on_load(false);
        int w, h, c;
        unsigned char* data = stbi_load(path.string().c_str(), &w, &h, &c, 4);
        if (!data)
            throw std::runtime_error("Failed to load cubemap cross: " + path.string());

        CubeTextureSpec s = spec;
        s.width           = w;
        s.height          = h;
        s.channels        = 4;

        auto texture = TextureCube::createFromCross(s, data);
        stbi_image_free(data);
        return texture;
    }

    static Ref<Texture2D> loadHDR(const std::filesystem::path& path,
                               const TextureSpec& spec = {}) {

        stbi_set_flip_vertically_on_load(true);
        int w, h, c;
        float* data = stbi_loadf(path.string().c_str(), &w, &h, &c, 4);
        if (!data)
            throw std::runtime_error("Failed to load HDR texture: " + path.string());

        TextureSpec s  = spec;
        s.width        = w;
        s.height       = h;
        s.channels     = 4;
        s.format       = TextureFormat::RGBA16F;

        auto texture = Texture2D::create(s, data);
        stbi_image_free(data);
        return texture;
    }

    static Ref<TextureCube> loadHDRFromCross(const std::filesystem::path& path,
                                              const CubeTextureSpec& spec = {}) {

        stbi_set_flip_vertically_on_load(false);
        int w, h, c;
        float* data = stbi_loadf(path.string().c_str(), &w, &h, &c, 4);
        if (!data)
            throw std::runtime_error("Failed to load HDR cubemap: " + path.string());

        CubeTextureSpec s = spec;
        s.width           = w;
        s.height          = h;
        s.channels        = 4;
        s.format          = TextureFormat::RGBA16F;

        auto texture = TextureCube::createFromCross(s, data);
        stbi_image_free(data);
        return texture;
    }
};

/* ================================================================================================================ */
/* PIPELINE ======================================================================================================= */
/* ================================================================================================================ */

/* GENERIC */
enum class CullMode { None, Front, Back };

/* GENERIC */
enum class BlendMode { None, Alpha, Additive };

/* GENERIC */
enum class PrimitiveTopology { Triangles, Lines, Points };

/* GENERIC */
struct PipelineSpec {
    Ref<Shader>  shader;

    VertexLayout vertexLayout;

    CullMode          cullMode  = CullMode::Back;
    BlendMode         blendMode = BlendMode::None;
    PrimitiveTopology topology  = PrimitiveTopology::Triangles;

    bool depthTest  = true;
    bool depthWrite = true;
};

/* INTERFACE */
class RenderPipeline {
public:
    virtual ~RenderPipeline() = default;

    static Ref<RenderPipeline> create(const PipelineSpec& pipeline);

    [[nodiscard]] virtual const PipelineSpec& getSpec() const = 0;
};

/* ================================================================================================================ */
/* MATERIALS ====================================================================================================== */
/* ================================================================================================================ */

/* GENERIC */
enum class MaterialParamType {
    Data,       // scalar/vector/matrix — stored packed in m_data
    Texture2D,
    TextureCube,
};

/* GENERIC */
struct MaterialParameter {
    MaterialParamType paramType = MaterialParamType::Data;
    ShaderDataType    type      = ShaderDataType::None;   // only meaningful for Data
    std::string       name;
    u32               size   = 0;
    u32               offset = 0;   // only meaningful for Data
    u32               alignment = 0;

    // constructor for scalar/vector/matrix params
    MaterialParameter(const ShaderDataType type, const std::string& name)
        : paramType(MaterialParamType::Data), type(type), name(name) {
        size   = ShaderDataTypeSize(type);
        offset = 0;
    }

    // constructors for texture params
    MaterialParameter(MaterialParamType texType, const std::string& name)
        : paramType(texType), type(ShaderDataType::None), name(name), size(0) {
        // textures live in m_textures, not m_data, so no size/offset needed
    }
};

/* GENERIC */
class MaterialLayout {
public:
    MaterialLayout() = default;

    MaterialLayout(const std::initializer_list<MaterialParameter>& params) : m_params(params) {
        calculateOffsets();
        buildLookup();
    }

    [[nodiscard]] const MaterialParameter* find (const std::string& name) const {
        auto it = m_lookup.find(name);

        if (it == m_lookup.end())
            return nullptr;

        return &m_params[it->second];
    }

    [[nodiscard]] u32 getStride() const { return m_stride; }

    [[nodiscard]] const std::vector<MaterialParameter>& getParameters() const { return m_params; }

private:
    void calculateOffsets() {
        u32 offset = 0;
        m_stride = 0;

        for (auto& param : m_params) {
            param.offset = offset;
            offset += param.size;
            m_stride += param.size;
        }
    }

    void buildLookup() {
        for (u32 i = 0; i < m_params.size(); i++) {
            m_lookup[m_params[i].name] = i;
        }
    }

private:
    std::vector<MaterialParameter> m_params;
    std::unordered_map<std::string, u32> m_lookup;
    u32 m_stride = 0;
};

/* GENERIC */
class Material {
public:
    Material(const Ref<RenderPipeline> &pipeline, const Ref<MaterialLayout>& layout)
        : m_pipeline(pipeline), m_layout(layout)
    {
        m_data.resize(m_layout->getStride(), 0);
    }

    template<typename T>
    void set(const std::string& name, const T& value) {
        m_dirty = true;

        const MaterialParameter* param = m_layout->find(name);

        if (!param)
            throw std::runtime_error("Material parameter not found: " + name);

        if (sizeof(T) != param->size)
            throw std::runtime_error("Material parameter size mismatch");

        memcpy(m_data.data() + param->offset, &value, sizeof(T));
    }

    void setTexture(const std::string& name, const Ref<::Texture2D>& texture) {
        const MaterialParameter* param = m_layout->find(name);
        if (!param)
            throw std::runtime_error("Material parameter not found: " + name);
        if (param->paramType != MaterialParamType::Texture2D)
            throw std::runtime_error("Parameter '" + name + "' is not a Texture2D");
        m_textures2D[name] = texture;
    }

    void setTexture(const std::string& name, const Ref<TextureCube>& texture) {
        const MaterialParameter* param = m_layout->find(name);
        if (!param)
            throw std::runtime_error("Material parameter not found: " + name);
        if (param->paramType != MaterialParamType::TextureCube)
            throw std::runtime_error("Parameter '" + name + "' is not a TextureCube");
        m_texturesCube[name] = texture;
    }

    template<typename T>
    T get(const std::string& name) const {
        const MaterialParameter* param = m_layout->find(name);

        if (!param)
            throw std::runtime_error("Material parameter not found: " + name);

        T value;
        memcpy(&value, m_data.data() + param->offset, sizeof(T));

        return value;
    }

    [[nodiscard]] bool has(const std::string& name) const {
        return m_layout->find(name) != nullptr;
    }

    [[nodiscard]] Ref<RenderPipeline> getPipeline() const {
        return m_pipeline;
    }
    [[nodiscard]] Ref<MaterialLayout> getLayout() const {
        return m_layout;
    }
    [[nodiscard]] const std::vector<u8>& getData() const {
        return m_data;
    }

    [[nodiscard]] const std::unordered_map<std::string, Ref<::Texture2D>>& getTextures2D() const { return m_textures2D; }

    [[nodiscard]] const std::unordered_map<std::string, Ref<TextureCube>>& getTexturesCube() const { return m_texturesCube; }

private:
    Ref<RenderPipeline> m_pipeline;
    Ref<MaterialLayout> m_layout;
    std::vector<u8> m_data;

    std::unordered_map<std::string, Ref<::Texture2D>>   m_textures2D;
    std::unordered_map<std::string, Ref<TextureCube>>   m_texturesCube;

    bool m_dirty = true;
};

/* ================================================================================================================ */
/* DRAW CALLS ===================================================================================================== */
/* ================================================================================================================ */

/* GENERIC */
class Transform {
public:
    glm::mat4 transform = glm::mat4(1.0f);
};

/* GENERIC */
struct RenderCommand {
    Ref<Geometry> geometry; // WHAT TO DRAW
    Ref<Material> material; // HOW TO DRAW IT
    Transform transform;    // WHERE TO DRAW IT
};

/* ================================================================================================================ */
/* RENDERER ======================================================================================================= */
/* ================================================================================================================ */

/* INTERFACE */
class RendererAPI {
public:
    virtual ~RendererAPI() = default;

    virtual void init() = 0;
    virtual void shutdown() = 0;

    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;

    virtual void setViewport(u32 x, u32 y, u32 width, u32 height) = 0;

    virtual void setClearColor(float r, float g, float b, float a) = 0;
    virtual void clear() = 0;

    virtual void bindPipeline(const Ref<RenderPipeline>& pipeline) = 0;

    virtual void bindGeometry(const Ref<Geometry>& geometry) = 0;

    virtual void bindMaterial(const Ref<Material>& material) = 0;

    virtual void uploadTransform(const glm::mat4& transform) = 0;

    virtual void drawIndexed(const Ref<Geometry>& geometry) = 0;
};

/* GENERIC */
class Renderer {
public:
    static void init() { s_instance->init(); }
    static void shutdown() { s_instance->shutdown(); }

    static void beginFrame() { s_instance->beginFrame(); }
    static void endFrame() { s_instance->endFrame(); }

    static void execute() {
        // sort by shader to minimize state switches eventually

        Ref<RenderPipeline> currentPipeline = nullptr;
        Ref<Material> currentMaterial = nullptr;
        Ref<Geometry> currentGeometry = nullptr;

        // go through all the draw calls
        for (const auto&[geometry, material, transform] : s_queue)
        {
            const auto pipeline = material->getPipeline();

            if (pipeline != currentPipeline)
            {
                s_instance->bindPipeline(pipeline);
                currentPipeline = pipeline;
            }

            if (material != currentMaterial)
            {
                s_instance->bindMaterial(material);
                currentMaterial = material;
            }

            if (geometry != currentGeometry)
            {
                s_instance->bindGeometry(geometry);
                currentGeometry = geometry;
            }

            s_instance->uploadTransform(transform.transform);

            s_instance->drawIndexed(geometry);
        }

        s_queue.clear();
    }

    static void submit(const RenderCommand& cmd) { s_queue.push_back(cmd); }

    static void setViewport(u32 x, u32 y, u32 width, u32 height) { s_instance->setViewport(x, y, width, height); }
    static void setClearColor(float r, float g, float b, float a = 1.0f) {  s_instance->setClearColor(r, g, b, a); }
    static void clear() { s_instance->clear(); }
private:
    static std::vector<RenderCommand> s_queue;
private:
    static RendererAPI* s_instance;
};


#endif //SANDBOX_RENDERER_H
