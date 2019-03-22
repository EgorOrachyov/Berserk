//
// Created by Egor Orachyov on 01.03.2019.
//

#include "Manager/GLShaderManager.h"
#include "ShaderImporter/ShaderImporter.h"

namespace Berserk
{

    namespace Resources
    {

        void GLShaderManager::initialize(const char *path)
        {
            new(&mPath) CString(path);
            new(&mShaders) LinkedList<GLShader>(INITIAL_SHADERS_COUNT);
            new(&mShadersUniformsPool) PoolAllocator(HashMap<CName,uint32>::getNodeSize(), PoolAllocator::INITIAL_CHUNK_COUNT);
        }

        void GLShaderManager::release()
        {
            {
                // Explicit deletions for shaders in the manager,
                // Which references were not decreased to the 0
                // (Avoids memory leaks)

                for (auto current = mShaders.iterate(); current != nullptr; current = mShaders.next())
                {
                    PUSH("GLShaderManager: release shader [name: '%s']", current->getName());

                    current->mReferenceCount = 0;
                    current->release();
                }
            }

            delete(&mPath);
            delete(&mShaders);
            delete(&mShadersUniformsPool);
        }

        void GLShaderManager::renameShader(IShader *shader, const char *name)
        {
            PUSH("GLShaderManager: rename shader [old name: '%s'][new name: '%s']", shader->getName(), name);

            auto renamed = dynamic_cast<GLShader*>(shader);
            renamed->mResourceName = name;
        }

        void GLShaderManager::deleteShader(IShader *shader)
        {
            CText name(shader->getName());
            shader->release();

            if (shader->getReferenceCount() == 0)
            {
                PUSH("GLShaderManager: delete shader [name: '%s']", name.get());
                mShaders.remove((GLShader*)shader);
            }
        }

        IShader* GLShaderManager::createShader(const char *name)
        {
            IShader* found = findShader(name);

            if (found)
            {
                WARNING("GLShaderManager: shader already exist [name: '%s']", name);
                return nullptr;
            }

            {
                GLShader shader;
                shader.initialize(name);
                shader.addReference();

                mShaders += shader;

                PUSH("GLShaderManager: create shader [name: '%s'][ref: %u]", shader.getName(), shader.getReferenceCount());
            }

            return mShaders.getLast();
        }

        IShader* GLShaderManager::findShader(const char *name)
        {
            for (auto current = mShaders.iterate(); current != nullptr; current = mShaders.next())
            {
                if (current->mResourceName == name)
                {
                    return current;
                }
            }

            return nullptr;
        }

        IShader* GLShaderManager::getShader(const char *name)
        {
            IShader* found = findShader(name);

            if (found)
            {
                found->addReference();
                PUSH("GLShaderManager: find shader [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
                return found;
            }

            return nullptr;
        }

        IShader* GLShaderManager::loadShader(const char *path)
        {
            CPath filename(path);
            filename = filename.replace(CPath("{SHADERS}"), CPath(mPath.get()));

            IShader* last = nullptr;
            XMLDocument meta_info(filename.get(), ".xml");

            for (auto programs = meta_info.getFirst(); !programs.isEmpty(); programs = programs.getNext())
            {
                last = loadShaderFromXML(nullptr, programs);
            }

            return last;
        }

        IShader* GLShaderManager::loadShaderFromXML(const char *name, XMLNode &node)
        {
            if (name != nullptr)
            {
                auto found = findShader(name);

                if (found)
                {
                    found->addReference();
                    PUSH("GLShaderManager: find shader [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
                    return found;
                }
            }

            const char* program = node.getAttribute("name").getValue();

            GLShader shader;
            shader.initialize(program);
            shader.createProgram(&mShadersUniformsPool);

            bool loaded = false;

            for (auto platform = node.getChild(); !platform.isEmpty(); platform = platform.getNext())
            {
                if (CName("OpenGL") == CName(platform.getAttribute("name").getValue()))
                {
                    auto success = Importers::ShaderImporter::import(&shader, platform, mPath);

                    if (!success)
                    {
                        WARNING("Cannot load shader program from xml node [name: '%s']", program);

                        shader.release();
                        return nullptr;
                    }

                    loaded = true;
                    break;
                }
            }

            if (!loaded)
            {
                WARNING("Non-exhaustive meta-inf.xml for OpenGL platform [name: '%s']", program);
                return nullptr;
            }

            shader.addReference();
            mShaders += shader;

            return mShaders.getLast();
        }

        uint32 GLShaderManager::getMemoryUsage()
        {
            return sizeof(GLShaderManager)  +
                   mShaders.getMemoryUsage();
        }

    } // namespace Resources

} // namespace Berserk