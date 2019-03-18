//
// Created by Egor Orachyov on 01.03.2019.
//

#include "Manager/GLShaderManager.h"
#include "XML/XMLDocument.h"

namespace Berserk
{

    void GLShaderManager::initialize()
    {
        new(&mShaders) LinkedList<GLShader>(INITIAL_SHADERS_COUNT);
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

        delete(&mShaders);
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

        //GLShader shader;
        //shader.initialize(program);
        //shader.addReference();
        //shader.createProgram();

        printf("Shader Manager: program name: %s\n", program);

        for (auto platform = node.getChild(); !platform.isEmpty(); platform = platform.getNext())
        {
            if (CName("OpenGL") == CName(platform.getAttribute("name").getValue()))
            {
                printf("Shader Manager: driver name: %s\n", platform.getAttribute("name").getValue());

                for (auto current = platform.getChild(); !current.isEmpty(); current = current.getNext())
                {
                    if (CName(current.getName()) == CName("shader"))
                    {
                        printf("Shader Manager: shader type: %s path: %s\n", current.getAttribute("type").getValue(), current.getAttribute("path").getValue());
                    }
                    else if (CName(current.getName()) == CName("uniform"))
                    {
                        printf("Shader Manager: uniform name: %s\n", current.getAttribute("name").getValue());
                    }
                    else if (CName(current.getName()) == CName("uniformblock"))
                    {
                        printf("Shader Manager: uniformblock name: %s\n", current.getAttribute("name").getValue());
                    }
                    else if (CName(current.getName()) == CName("subroutine"))
                    {
                        printf("Shader Manager: subroutine name: %s\n", current.getAttribute("name").getValue());
                    }
                    else
                    {
                        WARNING("Unknown node name in XML node parsing for program [name: '%s']", program);
                        return nullptr;
                    }
                }
            }
        }

        return nullptr;
        //mShaders += shader;
        //return mShaders.getLast();
    }

    uint32 GLShaderManager::getMemoryUsage()
    {
        return sizeof(GLShaderManager)  +
               mShaders.getMemoryUsage();
    }

} // namespace Berserk