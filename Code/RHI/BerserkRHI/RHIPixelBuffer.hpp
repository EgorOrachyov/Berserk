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

#ifndef BERSERK_RHIPIXELBUFFER_HPP
#define BERSERK_RHIPIXELBUFFER_HPP

#include <BerserkCore/Templates/MemoryBuffer.hpp>
#include <BerserkRHI/RHIDefs.hpp>

namespace Berserk {
    namespace RHI {

        /** Pixel buffer used to update textures data */
        class RHIPixelBuffer: public ReadOnlyMemoryBuffer {
        public:
            ~RHIPixelBuffer() override = default;

            /** Called to notify owner that buffer was consumed */
            virtual void NotifyConsumed() = 0;

            /** @return Pixel data format */
            PixelDataFormat GetDataFormat() const { return mFormat; }

            /** @return Pixel data type */
            PixelDataType GetDataType() const { return mType; }

        private:
            PixelDataFormat mFormat;
            PixelDataType mType;
        };

    }
}


#endif //BERSERK_RHIPIXELBUFFER_HPP
