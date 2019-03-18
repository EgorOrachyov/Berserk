//
// Created by Egor Orachyov on 07.02.2019.
//

#include "Misc/Assert.h"
#include "Platform/GLProfile.h"
#include "Platform/GLRenderDriver.h"
#include "Logging/LogMacros.h"

namespace Berserk
{

    namespace Resources
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
                IWindow::WindowSetup mSetup = setup;

                // Initialize main application window
                // Setup all common properties and pass info into gl window class

                mSetup.maxWidth = Math::max(mSetup.width, mSetup.maxWidth);
                mSetup.maxHeight = Math::max(mSetup.height, mSetup.maxHeight);

                auto handler = glfwCreateWindow(mSetup.width, mSetup.height, mSetup.caption.get(), nullptr, nullptr);

                glfwSetWindowPos(handler, mSetup.posX, mSetup.posY);

                if (mSetup.fullScreen)                       glfwMaximizeWindow(handler);
                if (!mSetup.resizable && !mSetup.fullScreen) glfwSetWindowSizeLimits(handler, mSetup.width, mSetup.height, mSetup.maxWidth, mSetup.maxHeight);

                glfwMakeContextCurrent(handler);

                // Setup GLWindow class
                mMainWindow.mHandler = handler;
                mMainWindow.initialize(mSetup);
                mMainWindow.getFrameBufferSize(mState.viewPort.width, mState.viewPort.height);
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

        void GLRenderDriver::clear(const Vec4f &color)
        {
            mState.clearColor = color;
            glClearColor(color.x, color.y, color.z, color.w);
        }

        void GLRenderDriver::depthTest(bool enable)
        {
            if (enable)
            {
                mState.useDepthTest = enable;
                glEnable(GL_DEPTH_TEST);
            }
            else
            {
                mState.useDepthTest = enable;
                glDisable(GL_DEPTH_TEST);
            }
        }

        void GLRenderDriver::depthTest(bool enable, bool mask, CompareFunc compare)
        {
            if (enable)
            {
                mState.useDepthTest = enable;
                mState.writeDepth = mask;
                mState.depthFunc = compare;

                glEnable(GL_DEPTH_TEST);
                glDepthFunc(getCompareFunc(compare));

                if (mask)
                {
                    glDepthMask(GL_TRUE);
                }
                else
                {
                    glDepthMask(GL_FALSE);
                }
            }
            else
            {
                mState.useDepthTest = enable;
                mState.writeDepth = mask;
                mState.depthFunc = compare;

                glDisable(GL_DEPTH_TEST);
                glDepthFunc(getCompareFunc(compare));

                if (mask)
                {
                    glDepthMask(GL_TRUE);
                }
                else
                {
                    glDepthMask(GL_FALSE);
                }
            }
        }

        void GLRenderDriver::faceCulling(bool enable)
        {
            if (enable)
            {
                mState.useFaceCulling = enable;
                glEnable(GL_CULL_FACE);
            }
            else
            {
                mState.useFaceCulling = enable;
                glDisable(GL_CULL_FACE);
            }
        }

        void GLRenderDriver::faceCulling(bool enable, FaceCulling face, WindingOrder order)
        {
            if (enable)
            {
                mState.useFaceCulling = enable;
                mState.faceCulling = face;
                mState.windingOrder = order;

                glEnable(GL_CULL_FACE);
                glCullFace(getFaceCulling(face));
                glFrontFace(getWindingOrder(order));
            }
            else
            {
                mState.useFaceCulling = enable;
                mState.faceCulling = face;
                mState.windingOrder = order;

                glDisable(GL_CULL_FACE);
                glCullFace(getFaceCulling(face));
                glFrontFace(getWindingOrder(order));
            }
        }

        void GLRenderDriver::blending(bool enable)
        {
            if (enable)
            {
                mState.useAlphaBlending = enable;
                glEnable(GL_BLEND);
            }
            else
            {
                mState.useAlphaBlending = enable;
                glDisable(GL_BLEND);
            }
        }

        void GLRenderDriver::blending(bool enable, BlendFunc source, BlendFunc destination)
        {
            if (enable)
            {
                mState.useAlphaBlending = enable;
                mState.blendFuncSource = source;
                mState.blendFuncDestination = destination;

                glEnable(GL_BLEND);
                glBlendFunc(getBlendFunc(source), getBlendFunc(destination));
            }
            else
            {
                mState.useAlphaBlending = enable;
                mState.blendFuncSource = source;
                mState.blendFuncDestination = destination;

                glDisable(GL_BLEND);
                glBlendFunc(getBlendFunc(source), getBlendFunc(destination));
            }
        }

        void GLRenderDriver::stencilTest(bool enable)
        {
            if (enable)
            {
                mState.useStencilTest = enable;
                glEnable(GL_STENCIL_TEST);
            }
            else
            {
                mState.useStencilTest = enable;
                glDisable(GL_STENCIL_TEST);
            }
        }

        void GLRenderDriver::stencilTest(bool enable, uint32 mask, uint32 clear, CompareFunc compare, uint32 value,
                                         uint32 read, StencilOp st_fail, StencilOp dt_fail, StencilOp dt_pass)
        {
            if (enable)
            {
                mState.useStencilTest = enable;
                mState.stencilWritingMask = mask;
                mState.stencilClearValue = clear;
                mState.stencilFunc = compare;
                mState.stencilCompareValue = value;
                mState.stencilCompareMask = read;
                mState.stencilOpFail = st_fail;
                mState.stencilOpDepthFail = dt_fail;
                mState.stencilOpPass = dt_pass;

                glEnable(GL_STENCIL_TEST);
                glClearStencil(clear);
                glStencilMask(mask);
                glStencilFunc(getCompareFunc(compare), value, read);
                glStencilOp(getStencilOp(st_fail), getStencilOp(dt_fail), getStencilOp(dt_pass));
            }
            else
            {
                mState.useStencilTest = enable;
                mState.stencilWritingMask = mask;
                mState.stencilClearValue = clear;
                mState.stencilFunc = compare;
                mState.stencilCompareValue = value;
                mState.stencilCompareMask = read;
                mState.stencilOpFail = st_fail;
                mState.stencilOpDepthFail = dt_fail;
                mState.stencilOpPass = dt_pass;

                glDisable(GL_STENCIL_TEST);
                glClearStencil(clear);
                glStencilMask(mask);
                glStencilFunc(getCompareFunc(compare), value, read);
                glStencilOp(getStencilOp(st_fail), getStencilOp(dt_fail), getStencilOp(dt_pass));
            }
        }

        void GLRenderDriver::stencilTest(CompareFunc compare, uint32 value, uint32 read)
        {
            mState.stencilFunc = compare;
            mState.stencilCompareValue = value;
            mState.stencilCompareMask = read;

            glStencilFunc(getCompareFunc(compare), value, read);
        }

        void GLRenderDriver::viewPort(const ViewPort &view)
        {
            mState.viewPort = view;
            glViewport(view.x, view.y, view.width, view.height);
        }

        void GLRenderDriver::setup(const RenderState &state)
        {
            mState = state;

            clear       (state.clearColor                                                             );
            polygonMode (state.polygonMode                                                            );
            viewPort    (state.viewPort                                                               );
            depthTest   (state.useDepthTest,     state.writeDepth,          state.depthFunc           );
            faceCulling (state.useFaceCulling,   state.faceCulling,         state.windingOrder        );
            blending    (state.useAlphaBlending, state.blendFuncSource,     state.blendFuncDestination);
            stencilTest (state.useStencilTest,   state.stencilWritingMask,  state.stencilClearValue   ,
                         state.stencilFunc,      state.stencilCompareValue, state.stencilCompareMask  ,
                         state.stencilOpFail,    state.stencilOpDepthFail,  state.stencilOpPass       );
        }

        void GLRenderDriver::swapBuffers()
        {
            glfwSwapBuffers(mMainWindow.mHandler);
        }

        void GLRenderDriver::polygonMode(PolygonMode mode)
        {
            mState.polygonMode = mode;
            glPolygonMode(GL_FRONT_AND_BACK, getPolygonMode(mode));
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

        uint32 GLRenderDriver::getCompareFunc(CompareFunc value)
        {
            switch (value)
            {
                case CompareFunc::FUNC_NEVER:
                    return GLCompareFunc::DRAW_FUNC_NEVER;

                case CompareFunc::FUNC_ALWAYS:
                    return GLCompareFunc::DRAW_FUNC_ALWAYS;

                case CompareFunc::FUNC_LESS:
                    return GLCompareFunc::DRAW_FUNC_LESS;

                case CompareFunc::DRAW_FUNC_GREATER:
                    return GLCompareFunc::DRAW_FUNC_GREATER;

                case CompareFunc::DRAW_FUNC_LEQUAL:
                    return GLCompareFunc::DRAW_FUNC_LEQUAL;

                case CompareFunc::DRAW_FUNC_GEQUAL:
                    return GLCompareFunc::DRAW_FUNC_GEQUAL;

                case CompareFunc::DRAW_FUNC_EQUAL:
                    return GLCompareFunc::DRAW_FUNC_EQUAL;

                case CompareFunc::DRAW_FUNC_NOT_EQUAL:
                    return GLCompareFunc::DRAW_FUNC_NOT_EQUAL;

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

    } // namespace Resources

} // namespace Berserk