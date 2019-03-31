//
// Created by Egor Orachyov on 01.03.2019.
//

#include "Managers/GLShaderManager.h"
#include "Helpers/ShaderManagerHelper.h"
#include "Platform/GLProfile.h"

namespace Berserk::Resources
{

    GLShaderManager::GLShaderManager(const char *path) : mPath(path),
                                                         mShaders(INITIAL_SHADERS_COUNT),
                                                         mShadersUniformsPool(HashMap<CName,uint32>::getNodeSize(), PoolAllocator::INITIAL_CHUNK_COUNT)
    {
        PUSH("GLShaderManager: initialize");
    }

    GLShaderManager::~GLShaderManager()
    {
        {
            // Explicit deletions for shaders in the manager,
            // Which references were not decreased to the 0
            // (Avoids memory leaks)

            for (auto current = mShaders.iterate(); current != nullptr; current = mShaders.next())
            {
#if PROFILE_GL_SHADER_MANAGER
                PUSH("GLShaderManager: release shader [name: '%s']", current->getName());
#endif
                current->mReferenceCount = 0;
                current->release();
            }
        }

        PUSH("GLShaderManager: de-initialize");
    }

    void GLShaderManager::renameShader(IShader *shader, const char *name)
    {
#if PROFILE_GL_SHADER_MANAGER
        PUSH("GLShaderManager: rename shader [old name: '%s'][new name: '%s']", shader->getName(), name);
#endif

        auto renamed = dynamic_cast<GLShader*>(shader);
        renamed->mResourceName = name;
    }

    void GLShaderManager::deleteShader(IShader *shader)
    {
        CText name(shader->getName());
        shader->release();

        if (shader->getReferenceCount() == 0)
        {
#if PROFILE_GL_SHADER_MANAGER
            PUSH("GLShaderManager: delete shader [name: '%s']", name.get());
#endif
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

        auto shader = new(mShaders.preallocate()) GLShader(name);

        {
            shader->addReference();
#if PROFILE_GL_SHADER_MANAGER
            PUSH("GLShaderManager: create shader [name: '%s'][ref: %u]", shader->getName(), shader->getReferenceCount());
#endif
        }

        return shader;
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
#if PROFILE_GL_SHADER_MANAGER
            PUSH("GLShaderManager: find shader [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
#endif
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
            last = loadShaderFromXML(programs.getAttribute("name").getValue(), programs);
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
#if PROFILE_GL_SHADER_MANAGER
                PUSH("GLShaderManager: find shader [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
#endif
                return found;
            }
        }

        const char* program = node.getAttribute("name").getValue();

        {
            auto found = findShader(program);

            if (found)
            {
                found->addReference();
#if PROFILE_GL_SHADER_MANAGER
                PUSH("GLShaderManager: find shader [name: '%s'][ref: %u]", found->getName(), found->getReferenceCount());
#endif
                return found;
            }
        }

        auto shader = new(mShaders.preallocate()) GLShader(program);
        shader->createProgram(&mShadersUniformsPool);

        bool loaded = false;

        for (auto platform = node.getChild(); !platform.isEmpty(); platform = platform.getNext())
        {
            if (CName("OpenGL") == CName(platform.getAttribute("name").getValue()))
            {
                auto success = ShaderManagerHelper::import(shader, platform, mPath);

                if (!success)
                {
                    WARNING("Cannot load shader program from xml node [name: '%s']", program);

                    shader->release();
                    mShaders.remove(shader);
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

        shader->addReference();

        return shader;
    }

    uint32 GLShaderManager::getMemoryUsage()
    {
        return sizeof(GLShaderManager)                             +
               mShaders.getMemoryUsage()                           +
               (uint32) mShadersUniformsPool.getTotalMemoryUsage() ;
    }

} // namespace Berserk::Resources