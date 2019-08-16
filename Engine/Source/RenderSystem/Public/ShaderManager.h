//
// Created by Egor Orachyov on 2019-08-16.
//

#ifndef BERSERK_SHADERMANAGER_H
#define BERSERK_SHADERMANAGER_H

#include <RHI/RHIDriver.h>
#include <Importers/IShaderImporter.h>
#include <Containers/THashMap.h>

namespace Berserk
{

    /**
     * Manages all the rhi shaders, created by driver.
     * Pipeline stages could share the same shader logic, therefore there is
     * need of entry of the all loaded shaders.
     *
     * @note todo: support custom shader logic for user defined materials.
     *       Add pass abstraction.
     *
     * @note This manager handles only fixed pipeline RHI shaders logic.
     *       No custom user logic support.
     */
    class ShaderManager final : public Allocatable
    {
    public:

        /**
         * Creates shader manager for specified rhi driver
         * @param importer Importer to load shaders data from some source providers
         * @param driver RHI driver to create actual GPU shader
         * @param allocator Memory manager for internal usage
         */
        ShaderManager(IShaderImporter& importer, RHIDriverRef driver, IAllocator& allocator = Allocator::get());

        ~ShaderManager();

        /**
         * Load specified RHI shader from full file name and path
         *
         * @note If resource is loaded, return reference to that
         * @note If resource was not loaded, loads that in current thread and return reference
         *
         * @param shadername Shader unique name for look-up
         * @param filename Full file name and path to the shader data
         *
         * @return Resource reference
         */
        RHIShaderProgramRef load(const char* shadername, const char* filename);

    protected:

        RHIShaderProgramRef* loadShaderVF_internal(const TSharedPtr<ShaderImportData> &data);

    private:

        typedef THashMap<String, RHIShaderProgramRef> NameShaderMap;

        IAllocator& mAllocator;
        IShaderImporter& mShaderImporter;
        PoolAllocator mMapPool;
        NameShaderMap mShadersMap;
        RHIDriverRef mDriver;

    };

} // namespace Berserk

#endif //BERSERK_SHADERMANAGER_H