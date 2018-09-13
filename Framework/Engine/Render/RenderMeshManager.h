//
// Created by Egor Orachyov on 08.08.2018.
//

#ifndef BERSERKENGINE_RENDERMESHMANAGER_H
#define BERSERKENGINE_RENDERMESHMANAGER_H

#include "Essential/Types.h"
#include "RenderMesh.h"

namespace Berserk
{

    class RenderMeshManager
    {
    public:

        virtual ~RenderMeshManager() = default;

        virtual UINT32 getMemoryUsage() = 0;

        virtual RenderMesh* getRenderMesh(UINT32 id) = 0;
        virtual RenderMesh* getRenderMesh(const CStaticString& name) = 0;

        virtual RenderMesh* createRenderMesh(const CStaticString& name) = 0;
        virtual RenderMesh* renameRenderMesh(const CStaticString& oldName, const CStaticString& newName) = 0;

        virtual bool deleteRenderMesh(UINT32 id) = 0;
        virtual bool deleteRenderMesh(const CStaticString& name) = 0;
        virtual bool deleteRenderMesh(RenderMesh* toDelete) = 0;
    };

} // namespace Berserk

#endif //BERSERKENGINE_RENDERMESHMANAGER_H