//
// Created by Egor Orachyov on 12.04.2019.
//

#include <Foundation/RenderBase.h>

namespace Berserk::Render
{

    IWindow* RenderBase::mMainWindow = nullptr;

    IRenderDriver* RenderBase::mRenderDriver = nullptr;

    IBufferManager* RenderBase::mBufferManager = nullptr;

    IShaderManager* RenderBase::mShaderManager = nullptr;

    ITextureManager* RenderBase::mTextureManager = nullptr;

    IMaterialManager* RenderBase::mMaterialManager = nullptr;

    IDebugDrawManager* RenderBase::mDebugRenderManager = nullptr;

    IImageImporter* RenderBase::mImageImporter = nullptr;

} // namespace Berserk::Render