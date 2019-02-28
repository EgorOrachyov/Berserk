//
// Created by Egor Orachyov on 07.02.2019.
//

#include "Misc/Assert.h"
#include "Platform/GLRenderDriver.h"
#include "Logging/LogMacros.h"

namespace Berserk
{

    void GLRenderDriver::initialize(const IWindow::WindowSetup &setup)
    {

        {
            // Setup glfw - Window and Input driver provider
            // In current implementation glfw will be initialized in the
            // Open GL Render device driver class

            if (!glfwInit())
            {
                FAIL(false, "Cannot initialize GLFW library");
            }

            #ifdef PLATFORM_MAC
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
            #else
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            #endif
        }


        {
            // Initialize main application window
            // Setup all common properties and pass info into gl window class

            auto handler = glfwCreateWindow(setup.width, setup.height, setup.caption.get(), nullptr, nullptr);

            glfwSetWindowPos(handler, setup.posX, setup.posY);

            if (setup.fullScreen)                      glfwMaximizeWindow(handler);
            if (!setup.resizable && !setup.fullScreen) glfwSetWindowSizeLimits(handler, setup.width, setup.height, setup.maxWidth, setup.maxHeight);

            glfwMakeContextCurrent(handler);

            // Setup GLWindow class
            mMainWindow.mHandler = handler;
            mMainWindow.initialize(setup);
        }


        {
            // Setup glew - OpenGL interface provider
            // Note: that no explicit close is needed

            if (glewInit() != GLEW_OK)
            {
                FAIL(false, "Cannot initialize GLEW library");
            }
        }

        PUSH("GLRenderDriver: initialize");
    }

    void GLRenderDriver::release()
    {
        mMainWindow.release();
        glfwTerminate();
        PUSH("GLRenderDriver: de-initialize");
    }

    void GLRenderDriver::clear(bool color, bool depth, bool stencil)
    {
        uint32 mask = 0;

        if (color)   mask |= GL_COLOR_BUFFER_BIT;
        if (depth)   mask |= GL_DEPTH_BUFFER_BIT;
        if (stencil) mask |= GL_STENCIL_BUFFER_BIT;

        glClear(mask);
    }

    void GLRenderDriver::depthTest(bool set)
    {
        mUseDepthTest = set;
        if (mUseDepthTest) glEnable(GL_DEPTH_TEST);
        else glDisable(GL_DEPTH_TEST);
    }

    void GLRenderDriver::setup(const RenderState &state)
    {
        mState = state;
    }

    void GLRenderDriver::swapBuffers()
    {
        glfwSwapBuffers(mMainWindow.mHandler);
    }

    void GLRenderDriver::setPolygonMode(PolygonMode mode)
    {
        mState.polygonMode = mode;
        mPolygoneMode = getPolygonMode(mode);
        glPolygonMode(GL_FRONT_AND_BACK, mPolygoneMode);
    }

    void GLRenderDriver::setActive(IWindow *window)
    {
        glfwMakeContextCurrent((dynamic_cast<GLWindow*>(window))->mHandler);
    }

    void GLRenderDriver::makeScreenShot(PixelFormat format, uint8 *data)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glReadPixels(0, 0,
                     mMainWindow.mFboWidth,
                     mMainWindow.mFboWidth,
                     getPixelFormat(format),
                     GLDataType::UNSIGNED_BYTE,
                     data);
    }

    void GLRenderDriver::update()
    {
        glfwPollEvents();
        mMainWindow.update();
    }

    uint32 GLRenderDriver::getMemoryUsage()
    {
        return sizeof(GLRenderDriver);
    }

    IWindow * GLRenderDriver::getMainWindow()
    {
        return &mMainWindow;
    }

    const IRenderDriver::RenderState & GLRenderDriver::getRenderState()
    {
        return mState;
    }

    const char* GLRenderDriver::getName()
    {

    }

    const char* GLRenderDriver::getInfo()
    {

    }

    const char* GLRenderDriver::getShaderInfo()
    {

    }

    uint32 GLRenderDriver::getGPUMemoryUsage()
    {
        return 10 * MiB; // todo: remove
    }

    uint32 GLRenderDriver::getShaderType(ShaderType value)
    {
        switch (value)
        {
            case ShaderType::VERTEX:
                return GLShaderType::VERTEX;

            case ShaderType::GEOMETRY:
                return GLShaderType::GEOMETRY;

            case ShaderType::TESSELLATION_CONTROL:
                return GLShaderType::TESSELLATION_CONTROL;

            case ShaderType::TESSELLATION_EVALUATION:
                return GLShaderType::TESSELLATION_EVALUATION;

            case ShaderType::FRAGMENT:
                return GLShaderType::FRAGMENT;

            case ShaderType::COMPUTE:
                return GLShaderType::COMPUTE;

            default:
                FAIL(false, "Unsupported format");
        }

        return 0;
    }

    uint32 GLRenderDriver::getBufferUsage(BufferUsage value)
    {
        switch (value)
        {
            case BufferUsage::USAGE_STATIC_DRAW:
                return GLBufferUsage::USAGE_STATIC_DRAW;

            case BufferUsage::USAGE_STREAM_DRAW:
                return GLBufferUsage::USAGE_STREAM_DRAW;

            case BufferUsage::USAGE_DYNAMIC_DRAW:
                return GLBufferUsage::USAGE_DYNAMIC_DRAW;

            case BufferUsage::USAGE_STATIC_COPY:
                return GLBufferUsage::USAGE_STATIC_COPY;

            case BufferUsage::USAGE_STREAM_COPY:
                return GLBufferUsage::USAGE_STREAM_COPY;

            case BufferUsage::USAGE_DYNAMIC_COPY:
                return GLBufferUsage::USAGE_DYNAMIC_COPY;

            case BufferUsage::USAGE_STATIC_READ:
                return GLBufferUsage::USAGE_STATIC_READ;

            case BufferUsage::USAGE_STREAM_READ:
                return GLBufferUsage::USAGE_STREAM_READ;

            case BufferUsage::USAGE_DYNAMIC_READ:
                return GLBufferUsage::USAGE_DYNAMIC_READ;

            default:
                FAIL(false, "Unsupported format");
        }

        return 0;
    }

    uint32 GLRenderDriver::getSamplerFilter(SamplerFilter value)
    {
        switch (value)
        {
            case SamplerFilter::FILTER_NEAREST:
                return GLSamplerFilter::FILTER_NEAREST;

            case SamplerFilter::FILTER_LINEAR:
                return GLSamplerFilter::FILTER_LINEAR;

            case SamplerFilter::FILTER_NEAREST_MIPMAP_NEAREST:
                return GLSamplerFilter::FILTER_NEAREST_MIPMAP_NEAREST;

            case SamplerFilter::FILTER_LINEAR_MIPMAP_NEAREST:
                return GLSamplerFilter::FILTER_LINEAR_MIPMAP_NEAREST;

            case SamplerFilter::FILTER_NEAREST_MIPMAP_LINEAR:
                return GLSamplerFilter::FILTER_NEAREST_MIPMAP_LINEAR;

            case SamplerFilter::FILTER_LINEAR_MIPMAP_LINEAR:
                return GLSamplerFilter::FILTER_LINEAR_MIPMAP_LINEAR;

            default:
                FAIL(false, "Unsupported format");
        }

        return 0;
    }

    uint32 GLRenderDriver::getSamplerWrapMode(SamplerWrapMode value)
    {
        switch (value)
        {
            case SamplerWrapMode::WRAP_CLAMP_TO_EDGE:
                return GLSamplerWrapMode::WRAP_CLAMP_TO_EDGE;

            case SamplerWrapMode::WRAP_CLAMP_TO_BORDER:
                return GLSamplerWrapMode::WRAP_CLAMP_TO_BORDER;

            case SamplerWrapMode::WRAP_REPEAT:
                return GLSamplerWrapMode::WRAP_REPEAT;

            case SamplerWrapMode::WRAP_CLAMP_MIRROR:
                return GLSamplerWrapMode::WRAP_CLAMP_MIRROR;

            case SamplerWrapMode::WRAP_REPEAT_MIRROR:
                return GLSamplerWrapMode::WRAP_REPEAT_MIRROR;

            default:
                FAIL(false, "Unsupported format");
        }

        return 0;
    }

    uint32 GLRenderDriver::getPrimitiveType(PrimitiveType value)
    {
        switch (value)
        {
            case PrimitiveType::TRIANGLES:
                return GLPrimitiveType::TRIANGLES;

            case PrimitiveType::POINTS:
                return GLPrimitiveType::POINTS;

            case PrimitiveType::LINE_STRIP:
                return GLPrimitiveType::LINE_STRIP;

            case PrimitiveType::LINE_LOOP:
                return GLPrimitiveType::LINE_LOOP;

            case PrimitiveType::LINES:
                return GLPrimitiveType::LINES;

            case PrimitiveType::LINE_STRIP_ADJACENCY:
                return GLPrimitiveType::LINE_STRIP_ADJACENCY;

            case PrimitiveType::LINES_ADJACENCY:
                return GLPrimitiveType::LINES_ADJACENCY;

            case PrimitiveType::TRIANGLE_STRIP:
                return GLPrimitiveType::TRIANGLE_STRIP;

            case PrimitiveType::TRIANGLE_FAN:
                return GLPrimitiveType::TRIANGLE_FAN;

            case PrimitiveType::TRIANGLE_STRIP_ADJACENCY:
                return GLPrimitiveType::TRIANGLE_STRIP_ADJACENCY;

            case PrimitiveType::TRIANGLES_ADJACENCY:
                return GLPrimitiveType::TRIANGLES_ADJACENCY;

            case PrimitiveType::PATCHES:
                return GLPrimitiveType::PATCHES;

            default:
                FAIL(false, "Unsupported format");
        }

        return 0;
    }

    uint32 GLRenderDriver::getTextureType(TextureType value)
    {
        switch (value)
        {
            case TextureType::TEXTURE_1D:
                return GLTextureType::TEXTURE_1D;

            case TextureType::TEXTURE_2D:
                return GLTextureType::TEXTURE_2D;

            case TextureType::TEXTURE_3D:
                return GLTextureType::TEXTURE_3D;

            default:
                FAIL(false, "Unsupported format");
        }

        return 0;
    }

    uint32 GLRenderDriver::getPixelFormat(PixelFormat value)
    {
        switch (value)
        {
            case PixelFormat::R:
                return GLPixelFormat::R;

            case PixelFormat::RG:
                return GLPixelFormat::RG;

            case PixelFormat::RGB:
                return GLPixelFormat::RGB;

            case PixelFormat::BGR:
                return GLPixelFormat::BGR;

            case PixelFormat::RGBA:
                return GLPixelFormat::RGBA;

            case PixelFormat::ABGR:
                return GLPixelFormat::ABGR;

            case PixelFormat::BGRA:
                return GLPixelFormat::BGRA;

            case PixelFormat::DEPTH:
                return GLPixelFormat::DEPTH;

            case PixelFormat::DEPTH_AND_STENCIL:
                return GLPixelFormat::DEPTH_AND_STENCIL;

            default:
                FAIL(false, "Unsupported format");
        }

        return 0;
    }

    uint32 GLRenderDriver::getDataType(DataType value)
    {
        switch (value)
        {
            case DataType::INT:
                return GLDataType::INT;

            case DataType::BYTE:
                return GLDataType::BYTE;

            case DataType::SHORT:
                return GLDataType::SHORT;

            case DataType::FLOAT:
                return GLDataType::FLOAT;

            case DataType::HALF_FLOAT:
                return GLDataType::HALF_FLOAT;

            case DataType::UNSIGNED_INT:
                return GLDataType::UNSIGNED_INT;

            case DataType::UNSIGNED_BYTE:
                return GLDataType::UNSIGNED_BYTE;

            case DataType::UNSIGNED_SHORT:
                return GLDataType::UNSIGNED_SHORT;

            default:
                FAIL(false, "Unsupported format");
        }

        return 0;
    }

    uint32 GLRenderDriver::getStorageFormat(StorageFormat value)
    {
        switch (value)
        {
            case StorageFormat::RGB8:
                return GLStorageFormat::RGB8;

            case StorageFormat::RGBA8:
                return GLStorageFormat::RGBA8;

            case StorageFormat::RGB16F:
                return GLStorageFormat::RGB16F;

            case StorageFormat::RGBA16F:
                return GLStorageFormat::RGBA16F;

            case StorageFormat::RGB32F:
                return GLStorageFormat::RGB32F;

            case StorageFormat::RGBA32F:
                return GLStorageFormat::RGBA32F;

            case StorageFormat::DEPTH24:
                return GLStorageFormat::DEPTH24;

            case StorageFormat::DEPTH24_STENCIL8:
                return GLStorageFormat::DEPTH24_STENCIL8;

            default:
                FAIL(false, "Unsupported format");
        }

        return 0;
    }

    uint32 GLRenderDriver::getFaceCulling(FaceCulling value)
    {
        switch (value)
        {
            case FaceCulling::FACE_CULLING_NONE:
                return GLFaceCulling::FACE_CULLING_NONE;

            case FaceCulling::FACE_CULLING_BACK:
                return GLFaceCulling::FACE_CULLING_BACK;

            case FaceCulling::FACE_CULLING_FRONT:
                return GLFaceCulling::FACE_CULLING_FRONT;

            case FaceCulling::FACE_CULLING_FRONT_AND_BACK:
                return GLFaceCulling::FACE_CULLING_FRONT_AND_BACK;

            default:
                FAIL(false, "Unsupported format");
        }

        return 0;
    }

    uint32 GLRenderDriver::getDrawFunc(DrawFunc value)
    {
        switch (value)
        {
            case DrawFunc::DRAW_FUNC_NEVER:
                return GLDrawFunc::DRAW_FUNC_NEVER;

            case DrawFunc::DRAW_FUNC_ALWAYS:
                return GLDrawFunc::DRAW_FUNC_ALWAYS;

            case DrawFunc::DRAW_FUNC_LESS:
                return GLDrawFunc::DRAW_FUNC_LESS;

            case DrawFunc::DRAW_FUNC_GREATER:
                return GLDrawFunc::DRAW_FUNC_GREATER;

            case DrawFunc::DRAW_FUNC_LEQUAL:
                return GLDrawFunc::DRAW_FUNC_LEQUAL;

            case DrawFunc::DRAW_FUNC_GEQUAL:
                return GLDrawFunc::DRAW_FUNC_GEQUAL;

            case DrawFunc::DRAW_FUNC_EQUAL:
                return GLDrawFunc::DRAW_FUNC_EQUAL;

            case DrawFunc::DRAW_FUNC_NOT_EQUAL:
                return GLDrawFunc::DRAW_FUNC_NOT_EQUAL;

            default:
                FAIL(false, "Unsupported format");
        }

        return 0;
    }


    uint32 GLRenderDriver::getFrameBuffer(FrameBuffer value)
    {
        switch (value)
        {
            case FrameBuffer::ATTACHMENT_COLOR:
                return GLFrameBuffer::ATTACHMENT_COLOR;

            case FrameBuffer::ATTACHMENT_DEPTH:
                return GLFrameBuffer::ATTACHMENT_DEPTH;

            case FrameBuffer::ATTACHMENT_STENCIL:
                return GLFrameBuffer::ATTACHMENT_STENCIL;

            default:
                FAIL(false, "Unsupported format");
        }

        return 0;
    }

    uint32 GLRenderDriver::getBlendFunc(BlendFunc value)
    {
        switch (value)
        {
            case BlendFunc::BLEND_FUNC_NONE:
                return GLBlendFunc::BLEND_FUNC_NONE;

            case BlendFunc::BLEND_FUNC_ONE:
                return GLBlendFunc::BLEND_FUNC_ONE;

            case BlendFunc::BLEND_FUNC_SRC_ALPHA:
                return GLBlendFunc::BLEND_FUNC_SRC_ALPHA;

            case BlendFunc::BLEND_FUNC_ONE_MINUS_SRC_ALPHA:
                return GLBlendFunc::BLEND_FUNC_ONE_MINUS_SRC_ALPHA;

            case BlendFunc::BLEND_FUNC_ONE_MINUS_DST_ALPHA:
                return GLBlendFunc::BLEND_FUNC_ONE_MINUS_DST_ALPHA;

            case BlendFunc::BLEND_FUNC_DST_ALPHA:
                return GLBlendFunc::BLEND_FUNC_DST_ALPHA;

            default:
                FAIL(false, "Unsupported format");
        }

        return 0;
    }

    uint32 GLRenderDriver::getStencilOp(StencilOp value)
    {
        switch (value)
        {
            case StencilOp::STENCIL_KEEP:
                return GLStencilOp::STENCIL_KEEP;

            case StencilOp::STENCIL_ZERO:
                return GLStencilOp::STENCIL_ZERO;

            case StencilOp::STENCIL_REPLACE:
                return GLStencilOp::STENCIL_REPLACE;

            case StencilOp::STENICL_INCR:
                return GLStencilOp::STENICL_INCR;

            case StencilOp::STENCIL_INCR_WRAP:
                return GLStencilOp::STENCIL_INCR_WRAP;

            case StencilOp::STENCIL_DECR_WRAP:
                return GLStencilOp::STENCIL_DECR_WRAP;

            case StencilOp::STENCIL_DECR:
                return GLStencilOp::STENCIL_DECR;

            case StencilOp::STENCIL_INVERT:
                return GLStencilOp::STENCIL_INVERT;

            default:
                FAIL(false, "Unsupported format");
        }

        return 0;
    }

    uint32 GLRenderDriver::getWindingOrder(WindingOrder value)
    {
        switch (value)
        {
            case WindingOrder::CLOCKWISE:
                return GLWindingOrder::CLOCKWISE;

            case WindingOrder::COUNTER_CLOCKWISE:
                return GLWindingOrder::COUNTER_CLOCKWISE;

            default:
                FAIL(false, "Unsupported format");
        }

        return 0;
    }

    uint32 GLRenderDriver::getPolygonMode(PolygonMode value)
    {
        switch (value)
        {
            case PolygonMode::FILL:
                return GLPolygonMode::FILL;

            case PolygonMode::LINE:
                return GLPolygonMode::LINE;

            case PolygonMode::POINT:
                return GLPolygonMode::POINT;

            default:
                FAIL(false, "Unsupported format");
        }

        return 0;
    }


} // namespace Berserk