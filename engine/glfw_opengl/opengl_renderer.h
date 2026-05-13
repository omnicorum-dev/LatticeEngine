//
// Created by Nico Russo on 5/12/26.
//

#ifndef SANDBOX_OPENGL_RENDERER_H
#define SANDBOX_OPENGL_RENDERER_H


#include "generic/renderer.h"
#include "glm/gtx/string_cast.hpp"

/* ================================================================================================================ */
/* BUFFERS AND TYPES ============================================================================================== */
/* ================================================================================================================ */

class OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer(const void* data, u32 size) {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    // dynamic — no data yet, allocate size only
    OpenGLVertexBuffer(u32 size) {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    ~OpenGLVertexBuffer() override {
        glDeleteBuffers(1, &m_id);
    }

    void setData(const void* data, u32 size) override {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    u32 getID() const { return m_id; }

private:
    u32 m_id;
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(const u32* indices, u32 count)
        : m_count(count) {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), indices, GL_STATIC_DRAW);
    }

    ~OpenGLIndexBuffer() override {
        glDeleteBuffers(1, &m_id);
    }

    u32 getCount() const override { return m_count; }
    u32 getID() const { return m_id; }

private:
    u32 m_id;
    u32 m_count;
};

static GLenum ShaderDataTypeToOpenGLBaseType(const ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:   return GL_FLOAT;

        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:     return GL_INT;

        case ShaderDataType::UInt:
        case ShaderDataType::UInt2:
        case ShaderDataType::UInt3:
        case ShaderDataType::UInt4:    return GL_UNSIGNED_INT;

        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:     return GL_FLOAT;

        case ShaderDataType::Bool:     return GL_BOOL;
        default: LOG_ASSERT(false && "Unknown shader data type");
    }
}

/* ================================================================================================================ */
/* GEOMETRY ======================================================================================================= */
/* ================================================================================================================ */

class OpenGLGeometry : public Geometry {
public:
    OpenGLGeometry(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer) {
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        m_vertexBuffers.push_back(vertexBuffer);
        m_indexBuffer = indexBuffer;

        glBindBuffer(GL_ARRAY_BUFFER, dynamic_cast<OpenGLVertexBuffer&>(*vertexBuffer).getID());

        const auto& layout = vertexBuffer->getLayout();
        u32 attributeIndex = 0;
        for (const auto& attribute : layout.getAttributes()) {
            glEnableVertexAttribArray(attributeIndex);
            glVertexAttribPointer(
                attributeIndex,
                GetShaderDataTypeComponentCount(attribute.type),
                ShaderDataTypeToOpenGLBaseType(attribute.type),
                attribute.normalized ? GL_TRUE : GL_FALSE,
                layout.getStride(),
                (const void*)(uintptr_t)attribute.offset
            );
            attributeIndex++;
        }

        // bind the index buffer so the VAO records it too
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dynamic_cast<OpenGLIndexBuffer&>(*indexBuffer).getID());

        glBindVertexArray(0);
    }

    ~OpenGLGeometry() override {
        glDeleteVertexArrays(1, &m_vao);
    }

    u32 getVAO() const { return m_vao; }
private:
    u32 m_vao;
};

/* ================================================================================================================ */
/* SHADERS ======================================================================================================== */
/* ================================================================================================================ */

class OpenGLShader : public Shader {
public:
    explicit OpenGLShader(const std::unordered_map<ShaderStage, std::string>& sources, const std::string& debugName = "OpenGLShader") {
        m_sources = sources;
        m_debugName = debugName;
        compile();
    }

    ~OpenGLShader() override {
        glDeleteProgram(m_id);
    }

    [[nodiscard]]
    const std::unordered_map<ShaderStage, std::string>&
    getSources() const override {
        return m_sources;
    }

    [[nodiscard]]
    const ShaderBinary&
    getBinary(ShaderStage stage) const override {
        const auto it = m_binaries.find(stage);

        if (it == m_binaries.end())
            throw std::runtime_error("Shader binary not found");

        return it->second;
    }

    [[nodiscard]]
    GLuint getRendererID() const {
        return m_id;
    }

    void bind() const {
        glUseProgram(m_id);
    }

    void unbind() const {
        glUseProgram(0);
    }

    // TEMP
    GLint getUniformLocation(const std::string& name) const {
        const auto it = m_uniformCache.find(name);
        if (it != m_uniformCache.end()) return it->second;
        const GLint loc = glGetUniformLocation(m_id, name.c_str());
        m_uniformCache[name] = loc;
        return loc;
    }

private:
    void compile() {
        const GLuint program = glCreateProgram();

        std::vector<GLuint> shaderIDs;

        for (const auto& [stage, source] : m_sources) {
            GLuint shader = glCreateShader(shaderStageToOpenGL(stage));

            const char* src = source.c_str();

            glShaderSource(shader, 1, &src, nullptr);
            glCompileShader(shader);

            GLint success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

            if (!success) {
                GLint length = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

                std::string infoLog(length, '\0');

                glGetShaderInfoLog(shader, length, &length, infoLog.data());

                glDeleteShader(shader);

                throw std::runtime_error(
                    "Shader compilation failed (" +
                    shaderStageToString(stage) +
                    "):\n" +
                    infoLog
                );
            }

            glAttachShader(program, shader);

            shaderIDs.push_back(shader);
        }

        glLinkProgram(program);

        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (!success) {
            GLint length = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

            std::string infoLog(length, '\0');

            glGetProgramInfoLog(program, length, &length, infoLog.data());

            glDeleteProgram(program);

            for (const GLuint shader : shaderIDs)
                glDeleteShader(shader);

            throw std::runtime_error(
                "Shader program link failed:\n" + infoLog
            );
        }

        for (const GLuint shader : shaderIDs) {
            glDetachShader(program, shader);
            glDeleteShader(shader);
        }

        m_id = program;
    }

    static GLenum shaderStageToOpenGL(const ShaderStage stage) {
        switch (stage) {
            case ShaderStage::Vertex:
                return GL_VERTEX_SHADER;

            case ShaderStage::Fragment:
                return GL_FRAGMENT_SHADER;

            case ShaderStage::Compute:
                return GL_COMPUTE_SHADER;
        }

        throw std::runtime_error("Unknown shader stage");
    }

    static std::string shaderStageToString(const ShaderStage stage) {
        switch (stage) {
            case ShaderStage::Vertex:
                return "Vertex";

            case ShaderStage::Fragment:
                return "Fragment";

            case ShaderStage::Compute:
                return "Compute";
        }

        return "Unknown";
    }

private:
    GLuint m_id = 0;

    // TEMP
    mutable std::unordered_map<std::string, GLint> m_uniformCache;

    // placeholder for spir-v binaries
    std::unordered_map<ShaderStage, ShaderBinary> m_binaries;
};

/* ================================================================================================================ */
/* TEXTURES ======================================================================================================= */
/* ================================================================================================================ */

static GLenum toGLFormat(const TextureFormat format) {
    switch (format) {
        case TextureFormat::RGBA8:   return GL_RGBA8;
        case TextureFormat::RGBA16F: return GL_RGBA16F;
        default:
            LOG_ERROR("Unknown texture format. defaulting to RGBA8");
            return GL_RGBA8;
    }
}

static GLenum toGLFilter(const TextureSpec::Filter filter) {
    switch (filter) {
        case TextureSpec::Filter::Linear:  return GL_LINEAR;
        case TextureSpec::Filter::Nearest: return GL_NEAREST;
        default:
            LOG_ERROR("Unknown texture filter. defaulting to linear");
            return GL_LINEAR;
    }
}

static GLenum toGLWrap(const TextureSpec::Wrap wrap) {
    switch (wrap) {
        case TextureSpec::Wrap::Repeat:         return GL_REPEAT;
        case TextureSpec::Wrap::ClampToEdge:    return GL_CLAMP_TO_EDGE;
        case TextureSpec::Wrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
        default:
            LOG_ERROR("Unknown texture wrap mode. defaulting to repeat");
            return GL_REPEAT;
    }
}

class OpenGLTexture2D : public Texture2D {
public:
    // RGBA8 - unsigned char
    OpenGLTexture2D(const TextureSpec& spec, const unsigned char* data)
        : m_width(spec.width), m_height(spec.height) {
        upload(spec, GL_UNSIGNED_BYTE, data);
    }

    // HDR - float
    OpenGLTexture2D(const TextureSpec& spec, const float* data)
        : m_width(spec.width), m_height(spec.height) {
        upload(spec, GL_FLOAT, data);
    }

    ~OpenGLTexture2D() override {
        glDeleteTextures(1, &m_id);
    }

    u32 getWidth() const override { return m_width; }
    u32 getHeight() const override { return m_height; }
    u32 getSlot() const override { return m_slot; }
    u32 getID() const { return m_id; }

    // called INTERNALLY ONLY by renderer before a draw call
    void bind(const u32 slot) const {
        m_slot = slot;
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

private:
    void upload(const TextureSpec& spec, const GLenum dataType, const void* data) {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexImage2D(GL_TEXTURE_2D, 0, toGLFormat(spec.format),
                     spec.width, spec.height, 0,
                     GL_RGBA, dataType, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLFilter(spec.filterMin));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toGLFilter(spec.filterMag));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toGLWrap(spec.wrapS));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toGLWrap(spec.wrapT));
    }
private:
    u32 m_id = 0;
    u32 m_width = 0;
    u32 m_height = 0;
    mutable u32 m_slot = 0;
};

class OpenGLTextureCube : public TextureCube {
public:
    // 6 separate faces - right, left, top, bottom, front, back
    OpenGLTextureCube(const CubeTextureSpec& spec,
        const std::array<const unsigned char*, 6>& faces)
            : m_width(spec.width), m_height(spec.height) {
        create(spec, GL_UNSIGNED_BYTE);
        for (u32 i = 0; i < 6; ++i) {
            uploadFace(i, spec, GL_UNSIGNED_BYTE, faces[i]);
        }
    }

    // single cross image
    OpenGLTextureCube(const CubeTextureSpec& spec, const unsigned char* cross)
        : m_width(spec.width), m_height(spec.height) {
        create(spec, GL_UNSIGNED_BYTE);
        uploadCross(spec, GL_UNSIGNED_BYTE, cross);
    }

    // HDR 6 faces
    OpenGLTextureCube(const CubeTextureSpec& spec, const std::array<const float*, 6>& faces)
        : m_width(spec.width), m_height(spec.height) {
        create(spec, GL_FLOAT);
        for (u32 i = 0; i < 6; ++i) {
            uploadFace(i, spec, GL_FLOAT, faces[i]);
        }
    }

    // HDR cross
    OpenGLTextureCube(const CubeTextureSpec& spec, const float* cross)
        : m_width(spec.width), m_height(spec.height) {
        create(spec, GL_FLOAT);
        uploadCross(spec, GL_FLOAT, cross);
    }

    ~OpenGLTextureCube() override {
        glDeleteTextures(1, &m_id);
    }

    u32 getWidth() const override { return m_width; }
    u32 getHeight() const override { return m_height; }
    u32 getSlot() const override { return m_slot; }
    u32 getID() const  { return m_id; }

    void bind(const u32 slot) const {
        m_slot = slot;
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
    }

private:
    void create(const CubeTextureSpec& spec, const GLenum dataType) {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, toGLFilter(spec.filterMin));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, toGLFilter(spec.filterMag));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void uploadFace(const u32 faceIndex, const CubeTextureSpec& spec, const GLenum dataType, const void* data) const {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex, 0,
                     toGLFormat(spec.format),
                     spec.width, spec.height, 0,
                     GL_RGBA, dataType, data);
    }

    void uploadCross(const CubeTextureSpec& spec, const GLenum dataType, const void* data) const {
        // a cross layout is 4 wide x 3 tall in face-sized tiles:
        //
        //      [  ]  [+Y]  [  ]  [  ]
        //      [-X]  [+Z]  [+X]  [-Z]
        //      [  ]  [-Y]  [  ]  [  ]
        //
        // faceWidth = totalWidth / 4, faceHeight = totalHeight / 3

        const u32 faceW = spec.width  / 4;
        const u32 faceH = spec.height / 3;
        const u32 bpp   = spec.channels * (dataType == GL_FLOAT ? 4 : 1);
        const u32 rowBytes = spec.width * bpp;

        // face order:  right, left, top, bottom, front, back
        // pixel offset (col, row) in face-tile units for each face
        const std::array<std::pair<u32,u32>, 6> offsets = {{
            {2, 1},  // +X right
            {0, 1},  // -X left
            {1, 0},  // +Y top
            {1, 2},  // -Y bottom
            {1, 1},  // +Z front
            {3, 1},  // -Z back
        }};

        for (u32 face = 0; face < 6; face++) {
            auto [col, row] = offsets[face];

            // extract face pixels row by row into a contiguous buffer
            std::vector<u8> faceData(faceW * faceH * bpp);
            for (u32 y = 0; y < faceH; y++) {
                const u8* src = static_cast<const u8*>(data)
                                + (row * faceH + y) * rowBytes
                                + col * faceW * bpp;
                u8* dst = faceData.data() + y * faceW * bpp;
                memcpy(dst, src, faceW * bpp);
            }

            glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0,
                         toGLFormat(spec.format),
                         faceW, faceH, 0,
                         GL_RGBA, dataType, faceData.data());
        }
    }
private:
    u32 m_id = 0;
    u32 m_width = 0;
    u32 m_height = 0;
    mutable u32 m_slot = 0;
};

/* ================================================================================================================ */
/* PIPELINE ======================================================================================================= */
/* ================================================================================================================ */

class OpenGLRenderPipeline : public RenderPipeline {
public:
    explicit OpenGLRenderPipeline(PipelineSpec spec)
        : m_spec(std::move(spec))
    {}

    [[nodiscard]] const PipelineSpec& getSpec() const override {
        return m_spec;
    }

private:
    PipelineSpec m_spec;
};

/* ================================================================================================================ */
/* RENDERER ======================================================================================================= */
/* ================================================================================================================ */

class OpenGLRendererAPI : public RendererAPI {
public:
    void init() override {
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void shutdown() override {}

    void beginFrame() override {
        Renderer::setClearColor(0.8f, 0.1f, 0.8f, 1.f);
        Renderer::clear();
    }

    void endFrame() override {}

    void bindPipeline(const Ref<RenderPipeline> &pipeline) override {
        const auto& spec = pipeline->getSpec();

        const auto shader = std::dynamic_pointer_cast<OpenGLShader>(spec.shader);
        m_currentShader = shader;
        m_currentPipeline = pipeline;

        shader->bind();

        // depth test
        if (spec.depthTest)
            glEnable(GL_DEPTH_TEST);
        else
            glDisable(GL_DEPTH_TEST);

        if (spec.depthWrite)
            glDepthMask(GL_TRUE);
        else
            glDepthMask(GL_FALSE);

        // blending
        switch (spec.blendMode) {
            case BlendMode::None: {
                glDisable(GL_BLEND);
            } break;
            case BlendMode::Alpha: {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            } break;
            case BlendMode::Additive: {
                glEnable(GL_BLEND);
                glBlendFunc(GL_ONE, GL_ONE);
            } break;
        }

        // culling
        switch (spec.cullMode) {
            case CullMode::None: {
                glDisable(GL_CULL_FACE);
            } break;
            case CullMode::Back: {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
            } break;
            case CullMode::Front: {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);
            } break;
        }
    }

    void bindGeometry(const Ref<Geometry> &geometry) override {
        const auto glGeometry = std::dynamic_pointer_cast<OpenGLGeometry>(geometry);

        glBindVertexArray(glGeometry->getVAO());
    }

    void bindMaterial(const Ref<Material> &material) override {
        const auto pipeline = material->getPipeline();
        const auto shader = std::dynamic_pointer_cast<OpenGLShader>(pipeline->getSpec().shader);
        shader->bind();

        const auto& layout      = material->getLayout()->getParameters();
        const auto& data        = material->getData();
        const auto& textures2D  = material->getTextures2D();
        const auto& texturesCube = material->getTexturesCube();

        u32 textureSlot = 0;

        for (const auto& param : layout) {
            const GLint loc = shader->getUniformLocation(param.name);
            if (loc == -1) { continue; }

            if (param.paramType == MaterialParamType::Texture2D) {
                const auto it = textures2D.find(param.name);
                if (it != textures2D.end()) {
                    const auto glTex = std::dynamic_pointer_cast<OpenGLTexture2D>(it->second);
                    glTex->bind(textureSlot);
                    glUniform1i(loc, textureSlot);
                    textureSlot++;
                }
                continue;
            }

            if (param.paramType == MaterialParamType::TextureCube) {
                const auto it = texturesCube.find(param.name);
                if (it != texturesCube.end()) {
                    const auto glTex = std::dynamic_pointer_cast<OpenGLTextureCube>(it->second);
                    glTex->bind(textureSlot);
                    glUniform1i(loc, textureSlot);
                    textureSlot++;
                }
                continue;
            }

            const void* ptr = data.data() + param.offset;
            switch (param.type) {
               case ShaderDataType::Float:
                    glUniform1fv(loc, 1, static_cast<const GLfloat*>(ptr)); break;
                case ShaderDataType::Float2:
                    glUniform2fv(loc, 1, glm::value_ptr(*static_cast<const glm::vec2*>(ptr))); break;
                case ShaderDataType::Float3:
                    glUniform3fv(loc, 1, glm::value_ptr(*static_cast<const glm::vec3*>(ptr))); break;
                case ShaderDataType::Float4:
                    glUniform4fv(loc, 1, glm::value_ptr(*static_cast<const glm::vec4*>(ptr))); break;

                case ShaderDataType::Int:
                    glUniform1iv(loc, 1, static_cast<const GLint*>(ptr)); break;
                case ShaderDataType::Int2:
                    glUniform2iv(loc, 1, glm::value_ptr(*static_cast<const glm::ivec2*>(ptr))); break;
                case ShaderDataType::Int3:
                    glUniform3iv(loc, 1, glm::value_ptr(*static_cast<const glm::ivec3*>(ptr))); break;
                case ShaderDataType::Int4:
                    glUniform4iv(loc, 1, glm::value_ptr(*static_cast<const glm::ivec4*>(ptr))); break;

                case ShaderDataType::UInt:
                    glUniform1uiv(loc, 1, static_cast<const GLuint*>(ptr)); break;
                case ShaderDataType::UInt2:
                    glUniform2uiv(loc, 1, glm::value_ptr(*static_cast<const glm::uvec2*>(ptr))); break;
                case ShaderDataType::UInt3:
                    glUniform3uiv(loc, 1, glm::value_ptr(*static_cast<const glm::uvec3*>(ptr))); break;
                case ShaderDataType::UInt4:
                    glUniform4uiv(loc, 1, glm::value_ptr(*static_cast<const glm::uvec4*>(ptr))); break;

                case ShaderDataType::Mat3:
                    glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(*static_cast<const glm::mat3*>(ptr))); break;
                case ShaderDataType::Mat4:
                    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(*static_cast<const glm::mat4*>(ptr))); break;

                case ShaderDataType::Bool:
                    glUniform1i(loc, *static_cast<const bool*>(ptr) ? 1 : 0); break;

                case ShaderDataType::None:
                    LOG_WARN("Material parameter '{}' has ShaderDataType::None", param.name);
                    break;
            }
        }
    }

    void uploadTransform(const glm::mat4 &transform) override {
        const auto shader = m_currentShader;

        const GLint loc = shader->getUniformLocation("u_transform");

        glUniformMatrix4fv(
            loc,
            1,
            GL_FALSE,
            glm::value_ptr(transform)
        );
    }

    void drawIndexed(const Ref<Geometry> &geometry) override {
        const auto glGeometry = std::dynamic_pointer_cast<OpenGLGeometry>(geometry);

        GLenum topology = 0;
        switch (m_currentPipeline->getSpec().topology) {
            case PrimitiveTopology::Triangles:
                topology = GL_TRIANGLES; break;
            case PrimitiveTopology::Lines:
                topology = GL_LINES; break;
            case PrimitiveTopology::Points:
                topology = GL_POINTS; break;
        }
        glDrawElements(topology, geometry->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
    }

    void setViewport(const u32 x, const u32 y, const u32 width, const u32 height) override {
        glViewport(x, y, width, height);
    }

    void setClearColor(float r, float g, float b, float a) override {
        glClearColor(r, g, b, a);
    }

    void clear() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

private:

    Ref<OpenGLShader> m_currentShader;
    Ref<RenderPipeline> m_currentPipeline;

};


#endif //SANDBOX_OPENGL_RENDERER_H
