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

#ifndef BERSERK_RHIPROGRAMMETA_HPP
#define BERSERK_RHIPROGRAMMETA_HPP

#include <BerserkRHI/RHIDefs.hpp>
#include <BerserkRHI/RHIResource.hpp>
#include <BerserkCore/Templates/OpenMap.hpp>
#include <BerserkCore/Templates/ArrayFixed.hpp>

namespace Berserk {
    namespace RHI {

        /** Reflection info about compiled RHI program */
        class ProgramMeta: public RefCountedThreadSafe {
        public:

            /** Vertex shader inputs */
            struct InputAttribute {
                StringName name;
                uint16 location;
                VertexElementType type;
            };

            /** Object params like samplers, textures */
            struct ObjectParam {
                StringName name;
                uint16 location;
                uint16 arraySize;
                ShaderParamType type;
            };

            /** Data param within uniform block */
            struct DataParam {
                StringName name;
                uint16 elementSize;
                uint16 arraySize;
                uint16 arrayStride;
                uint16 blockSlot;
                uint32 blockOffset;
                ShaderDataType type;
            };

            /** Uniform block info */
            struct DataParamBlock {
                StringName name;
                uint32 slot;
                uint32 size;
            };

            StringName name;
            OpenMap<StringName, InputAttribute> inputs;
            OpenMap<StringName, DataParam> params;
            OpenMap<StringName, DataParamBlock> paramBlocks;
            OpenMap<StringName, ObjectParam> samplers;
        };

    }
}

#endif //BERSERK_RHIPROGRAMMETA_HPP