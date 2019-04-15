//
// Created by Egor Orachyov on 01.03.2019.
//

#ifndef BERSERK_GLSHADERMANAGER_H
#define BERSERK_GLSHADERMANAGER_H

#include "Platform/GLShader.h"
#include "Containers/LinkedList.h"
#include "Managers/IShaderManager.h"

namespace Berserk::Resources
{

    /**
    * OpenGL platform shader manager implementation
    */
    class ENGINE_API GLShaderManager : public IShaderManager
    {
    public:

        /** Want to manually allocate this manager */
        GEN_NEW_DELETE(GLShaderManager);

        /** Explicit initialization of manager (must be invoked) */
        GLShaderManager(const char *path);

        /** De-initialize manager */
        ~GLShaderManager() override;

        /** @copydoc IShaderManager::renameShader() */
        void renameShader(IShader* shader, const char* name) override;

        /** @copydoc IShaderManager::deleteShader() */
        void deleteShader(IShader* shader) override;

        /** @copydoc IShaderManager::createShader() */
        IShader* createShader(const char* name) override;

        /** @copydoc IShaderManager::findShader() */
        IShader* findShader(const char* name) override;

        /** @copydoc IShaderManager::getShader() */
        IShader* getShader(const char* name) override;

        /** @copydoc IShaderManager::loadShader() */
        IShader* loadShader(const char* path) override;

        /** @copydoc IShaderManager::loadShaderFromXML() */
        IShader* loadShaderFromXML(const char* name, XMLNode& node) override;

        /** @copydoc IShaderManager::getMemoryUsage() */
        uint32 getMemoryUsage() override;

        /** @copydoc IShaderManager::getMemoryUsage() */
        void getMemoryUsage(MemorySizer* sizer) override;

    private:

        /** Number of shaders to preallocate in buffer (and the expand by that value) */
        static const uint32 INITIAL_SHADERS_COUNT = 20;

        CString mPath;
        LinkedList<GLShader> mShaders;
        PoolAllocator mShadersUniformsPool;

    };

} // namespace Berserk::Resources

#endif //BERSERK_GLSHADERMANAGER_H