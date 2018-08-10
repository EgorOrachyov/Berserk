//
// Created by Egor Orachyov on 10.08.2018.
//

#ifndef BERSERKENGINE_GLRENDERMESHMANAGER_H
#define BERSERKENGINE_GLRENDERMESHMANAGER_H

#include "Render/GLRenderMesh.h"
#include "Containers/LinkedList.h"
#include "Render/RenderMeshManager.h"

namespace Berserk
{

    class GLRenderMeshManager : public RenderMeshManager
    {
    public:

        GLRenderMeshManager();
        virtual ~GLRenderMeshManager() = default;

        void destroy();

        UINT32 getMemoryUsage() override;

        RenderMesh* getRenderMesh(UINT32 id) override;
        RenderMesh* getRenderMesh(const CStaticString& name) override;

        RenderMesh* createRenderMesh(const CStaticString& name) override;
        RenderMesh* renameRenderMesh(const CStaticString& oldName, const CStaticString& newName) override;

        bool deleteRenderMesh(UINT32 id) override;
        bool deleteRenderMesh(const CStaticString& name) override;
        bool deleteRenderMesh(RenderMesh* toDelete) override;

    private:

        LinkedList<GLRenderMesh> mMeshList;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLRENDERMESHMANAGER_H