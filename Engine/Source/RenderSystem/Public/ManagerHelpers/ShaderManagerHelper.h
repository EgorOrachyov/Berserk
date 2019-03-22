//
// Created by Egor Orachyov on 22.03.2019.
//

#ifndef BERSERK_SHADERMANAGERHELPER_H
#define BERSERK_SHADERMANAGERHELPER_H

#include "Strings/String.h"
#include "XML/XMLDocument.h"
#include "Platform/IShader.h"
#include "Platform/IRenderDriver.h"

namespace Berserk
{

    namespace Resources
    {

        class ENGINE_API ShaderManagerHelper
        {
        public:

            /**
             * Reads data from xml node, which specifies concrete shader properties
             * (uniforms, subroutines, blocks, paths to the source code, etc.) and initializes
             * these shader's params
             *
             * @param[inout] shader Allocated shader resource which will be initialized by
             *                      importer and input XML node
             * @param[in]    node   XML meta-info document node with shader infor
             * @param[in]    path   Path to the folder with shader's source code
             */
            static bool import(IShader *shader, XMLNode &node, const CString &path);

            /** @return Shader type from string */
            static IRenderDriver::ShaderType getShaderType(const char* string);

        };

    } // namespace Resources

} // namespace Berserk

#endif //BERSERK_SHADERMANAGERHELPER_H