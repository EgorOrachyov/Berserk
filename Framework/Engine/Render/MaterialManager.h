//
// Created by Egor Orachyov on 08.08.2018.
//

#ifndef BERSERKENGINE_MATERIALMANAGER_H
#define BERSERKENGINE_MATERIALMANAGER_H

#include "Material.h"
#include "Strings/CString.h"
#include "Strings/CStaticString.h"

namespace Berserk
{
    class MaterialManager
    {
    public:

        virtual ~MaterialManager() = default;

        virtual UINT32 getMemoryUsage() const = 0;

        virtual Material* getMaterial(UINT32 id) = 0;
        virtual Material* getMaterial(const CStaticString& name) = 0;

        virtual Material* createMaterial(const CStaticString& name) = 0;
        virtual Material* renameMaterial(const CStaticString& oldName, const CStaticString& newName) = 0;

        virtual Material* loadMaterialFromMTL(const CString &file) = 0;
        virtual Material* loadMaterialFromMTL(const CStaticString &file) = 0;

        virtual Material* loadMaterialFromXML(const CString& file) = 0;
        virtual Material* loadMaterialFromXML(const CStaticString& file) = 0;

        virtual Material* getDefaultMaterial() const = 0;

        virtual bool deleteMaterial(UINT32 id) = 0;
        virtual bool deleteMaterial(const CStaticString& name) = 0;
        virtual bool deleteMaterial(Material* toDelete) = 0;
    };
}

#endif //BERSERKENGINE_MATERIALMANAGER_H