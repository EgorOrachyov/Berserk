/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_GLSLUTILS_HPP
#define BERSERK_GLSLUTILS_HPP

#include <render/shader/Shader.hpp>
#include <render/shader/ShaderParams.hpp>

#include <sstream>

BRK_NS_BEGIN

/**
 * @addtogroup render
 * @{
 */

/**
 * @class UtilsGLSL
 * @brief GLSL source code processing utils
 */
class UtilsGLSL {
public:
    static void GenerateDefines(const ShaderCompileOptions &options, std::stringstream &stream) {
        stream << "//<-- inserted generated options -->//\n";
        for (const auto &option : options.Get())
            stream << "#define " << option << "\n";
        stream << "//<-- inserted generated options -->//\n";
    }

    static void GenerateStruct(const char *name, const char *layout, const ShaderParams &params, std::stringstream &stream) {
        stream << "//<-- inserted generated params code -->//\n";
        stream << "layout (" << layout << ") uniform " << name << " {\n";

        for (const auto &p : params.GetParams()) {
            if (p.type == ShaderParamType::Data) {
                auto array = p.arraySize > 1;
                auto type = p.typeData;

                stream << "  " << RHIGetShaderDataIdGLSL(type) << " " << p.name;

                if (array)
                    stream << "[" << p.arraySize << "];\n";
                else
                    stream << ";\n";
            }
        }

        stream << "};\n";
        stream << "//<-- inserted generated params code -->//\n";
    }

    static void GenerateUniformParams(const ShaderParams &params, std::stringstream &stream) {
        stream << "//<-- inserted generated uniform params -->//\n";

        for (const auto &p : params.GetParams()) {
            if (p.type == ShaderParamType::Texture) {
                auto array = p.arraySize > 1;
                auto type = p.typeParam;

                stream << "uniform " << RHIGetShaderParamIdGLSL(type) << " " << p.name;

                if (array)
                    stream << "[" << p.arraySize << "];\n";
                else
                    stream << ";\n";
            }
        }

        stream << "//<-- inserted generated uniform params -->//\n";
    }

    static void GenerateUserCode(const String &code, std::stringstream &stream) {
        stream << "//<-- inserted user code -->//\n";
        stream << code << "\n";
        stream << "//<-- inserted user code -->//\n";
    }
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_GLSLUTILS_HPP
