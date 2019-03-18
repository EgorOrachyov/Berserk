//
// Created by Egor Orachyov on 01.03.2019.
//

#ifndef BERSERK_GLSHADERMANAGER_H
#define BERSERK_GLSHADERMANAGER_H

#include "Platform/GLShader.h"
#include "Containers/LinkedList.h"
#include "Manager/IShaderManager.h"

namespace Berserk
{

    namespace Resources
    {
        /**
        * OpenGL platform shader manager implementation
        */
        class ENGINE_API GLShaderManager : public IShaderManager
        {
        public:

            /** @copydoc IShaderManager::initialize() */
            void initialize(const char *path) override;

            /** @copydoc IShaderManager::release() */
            void release() override;

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

        private:

            /** Number of shaders to preallocate in buffer (and the expand by that value) */
            static const uint32 INITIAL_SHADERS_COUNT = 20;

            CString mPath;
            LinkedList<GLShader> mShaders;

        };

    } // namespace Resources

} // namespace Berserk

#endif //BERSERK_GLSHADERMANAGER_H