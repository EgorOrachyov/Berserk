//
// Created by Egor Orachyov on 22.03.2019.
//

#include "Helpers/ProfileHelpers.h"
#include "Helpers/ShaderManagerHelper.h"
#include "Misc/FileUtility.h"

namespace Berserk
{

    namespace Resources
    {

        bool ShaderManagerHelper::import(IShader *shader, XMLNode &node, const CString &path)
        {
            bool compiled = false;
            uint32 attachments = 0;

            for (auto current = node.getChild(); !current.isEmpty(); current = current.getNext())
            {
                if (CName("shader") == current.getName())
                {
                    if (compiled)
                    {
                        return false;
                    }

                    CPath filename(current.getAttribute("path").getValue());
                    filename = filename.replace(CPath("{SHADERS}"), CPath(path.get()));

                    const uint32 size = Buffers::KiB * 20;
                    char buffer[size];
                    FileUtility::read(filename.get(), buffer, size);

                    shader->attachShader(getShaderType(current.getAttribute("type").getValue()),
                                         buffer,
                                         filename.get());

#if PROFILE_SHADBER_MANAGER_HELPER
                    PUSH("ShaderManagerHelper: Compile from path [path: '%s'] \n", filename.get());
#endif

                    attachments += 1;
                }
                else if (!compiled)
                {
                    if (attachments == 0)
                    {
                        return false;
                    }

                    shader->link();
                    compiled = true;
                }

                if (compiled)
                {
                    if (CName("uniform") == current.getName())
                    {
                        shader->addUniformVariable(current.getAttribute("name").getValue());
#if PROFILE_SHADBER_MANAGER_HELPER
                        PUSH("ShaderManagerHelper: Uniform variable [name: '%s'] \n", current.getAttribute("name").getValue());
#endif
                    }
                    else if (CName("uniformblock") == current.getName())
                    {
                        const char* name = current.getAttribute("name").getValue();
                        const char* binding = current.getAttribute("binding").getValue();
                        const auto  binding_point = (uint32) atoi(binding);

                        shader->setUniformBlockBinding(name, binding_point);
#if PROFILE_SHADBER_MANAGER_HELPER
                        PUSH("ShaderManagerHelper: Uniform block [name: '%s'][binding: %u] \n", current.getAttribute("name").getValue(), binding_point);
#endif
                    }
                    else if (CName("subroutine") == current.getName())
                    {
                        /* Not used now */
                    }
                    else
                    {
                        WARNING("Unknown node name in XML node parsing for program");
                        return false;
                    }
                }
            }

            return true;
        }

        IRenderDriver::ShaderType ShaderManagerHelper::getShaderType(const char *string)
        {
            if (CName("VERTEX") == string)
            {
                return IRenderDriver::ShaderType::VERTEX;
            }
            else if (CName("TESSELLATION_CONTROL") == string)
            {
                return IRenderDriver::ShaderType::TESSELLATION_CONTROL;
            }
            else if (CName("TESSELLATION_EVALUATION") == string)
            {
                return IRenderDriver::ShaderType::TESSELLATION_EVALUATION;
            }
            else if (CName("GEOMETRY") == string)
            {
                return IRenderDriver::ShaderType::GEOMETRY;
            }
            else if (CName("FRAGMENT") == string)
            {
                return IRenderDriver::ShaderType::FRAGMENT;
            }
            else if (CName("COMPUTE") == string)
            {
                return IRenderDriver::ShaderType::COMPUTE;
            }

            WARNING("Cannot find shader type from string [string: '%s']", string);
            return IRenderDriver::NOT_SHADER_TYPE;
        }

    } // namespace Resources

} // namespace Berserk