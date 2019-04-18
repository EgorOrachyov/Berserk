//
// Created by Egor Orachyov on 12.04.2019.
//

#include <Foundation/RenderBase.h>

namespace Berserk::Render
{

    IRenderSystem* RenderBase::mRenderSystem = nullptr;

    IWindow* RenderBase::mMainWindow = nullptr;

    IRenderDriver* RenderBase::mRenderDriver = nullptr;

    IBufferManager* RenderBase::mBufferManager = nullptr;

    IShaderManager* RenderBase::mShaderManager = nullptr;

    ITextureManager* RenderBase::mTextureManager = nullptr;

    IMaterialManager* RenderBase::mMaterialManager = nullptr;

    DebugDrawManager* RenderBase::mDebugRenderManager = nullptr;

    IImageImporter* RenderBase::mImageImporter = nullptr;

    IFontImporter* RenderBase::mFontImporter = nullptr;

} // namespace Berserk::Render