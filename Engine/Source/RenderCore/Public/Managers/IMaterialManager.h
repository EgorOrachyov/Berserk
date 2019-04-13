//
// Created by Egor Orachyov on 26.02.2019.
//

#ifndef BERSERK_IMATERIALMANAGER_H
#define BERSERK_IMATERIALMANAGER_H

#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"
#include "Foundation/IMaterial.h"
#include "Managers/ITextureManager.h"
#include "XMLNode.h"
#include "MemorySizer.h"

namespace Berserk::Resources
{

    /**
     * An interface which provides access to the material manager implemented in 3D Rendering System.
     * Responsible for creating, loading, saving materials in OS / Engine system.
     * Handles reference counting and memory allocation for resources.
     */
    class ENGINE_API IMaterialManager
    {
    public:

        /** De-initialize manager */
        virtual ~IMaterialManager() = default;

        /** Rename chosen material with new name */
        virtual void renameMaterial(IMaterial* material, const char* name) = 0;

        /** Save material data in chosen XML node */
        virtual void saveMaterial(IMaterial* material, XMLNode& node) = 0;

        /** Delete specified resource */
        virtual void deleteMaterial(IMaterial* material) = 0;

        /** @return New empty (if it does not exist) material with specified name */
        virtual IMaterial* createMaterial(const char* name) = 0;

        /** @return Material with specified name */
        virtual IMaterial* findMaterial(const char* name) = 0;

        /** @return Pointer to resource with incrementing reference count */
        virtual IMaterial* getMaterial(const char* name) = 0;

        /** @return Material from .mtl file with specified name (in path) */
        virtual IMaterial* loadMaterial(const char* path) = 0;

        /** @return Loaded material with specified name from XML node */
        virtual IMaterial* loadMaterialFromXML(const char* name, XMLNode& node) = 0;

        /** @return Copy (+1 reference) of specified material */
        virtual IMaterial* copyMaterial(IMaterial* material) = 0;

        /** @return Default material for models */
        virtual IMaterial* getDefaultMaterial() = 0;

        /** @return Default material, if fail to load specified material */
        virtual IMaterial* getDefaultHelperMaterial() = 0;

        /** @return Default material for landscape */
        virtual IMaterial* getDefaultTerrainMaterial() = 0;

        /** @return Memory usage on CPU (RAM) side */
        virtual uint32 getMemoryUsage() = 0;

        /** Profile memroy usage by this manager */
        virtual void getMemoryUsage(MemorySizer* sizer) = 0;

    };

} // namespace Berserk::Resources

#endif //BERSERK_IMATERIALMANAGER_H