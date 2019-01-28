//
// Created by Egor Orachyov on 08.08.2018.
//

#ifndef BERSERKENGINE_GLMATERIALMANAGER_H
#define BERSERKENGINE_GLMATERIALMANAGER_H

#include "Render/GLMaterial.h"
#include "Render/MaterialManager.h"
#include "Containers/LinkedList.h"

namespace Berserk
{

    class GLMaterialManager : public MaterialManager
    {
    public:
        
        GLMaterialManager();
        virtual ~GLMaterialManager() = default;

        void destroy();

        UINT32 getMemoryUsage() const override;

        Material* getMaterial(UINT32 id) override;
        Material* getMaterial(const CStaticString& name) override;

        Material* createMaterial(const CStaticString& name) override;
        Material* renameMaterial(const CStaticString& oldName, const CStaticString& newName) override;

        Material* loadMaterialFromMTL(const CString &file) override;
        Material* loadMaterialFromMTL(const CStaticString &file) override;

        Material* loadMaterialFromXML(const CString& file) override;
        Material* loadMaterialFromXML(const CStaticString& file) override;

        Material* getDefaultMaterial() const override;

        bool deleteMaterial(UINT32 id) override;
        bool deleteMaterial(const CStaticString& name) override;
        bool deleteMaterial(Material* toDelete) override;

    private:

        LinkedList<GLMaterial> mMaterialList;
        
    };

} // namespace Berserk

#endif //BERSERKENGINE_GLMATERIALMANAGER_H