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

#include <core/image/ImageUtil.hpp>

#include <cassert>

BRK_NS_BEGIN

uint32 ImageUtil::GetMaxMipsCount(uint32 width, uint32 height, uint32 depth) {
    uint32 maxDim = MathUtils::Max(width, MathUtils::Max(height, depth));
    assert(maxDim > 0);

    return static_cast<uint32>(MathUtils::Floor(MathUtils::Log2((float) maxDim))) + 1;
}

Size2u ImageUtil::GetMipSize(uint32 level, uint32 width, uint32 height) {
    while (level > 0) {
        if (width > 0) width /= 2;
        if (height > 0) height /= 2;
        level -= 1;
    }

    return {width, height};
}

uint32 ImageUtil::GetPixelSize(Image::Format format) {
    switch (format) {
        case Image::Format::R8:
        case Image::Format::R8_SNORM:
            return 1;
        case Image::Format::R16:
        case Image::Format::R16_SNORM:
        case Image::Format::RG8:
        case Image::Format::RG8_SNORM:
            return 2;
        case Image::Format::RG16:
        case Image::Format::RG16_SNORM:
            return 4;
        case Image::Format::RGB8:
        case Image::Format::RGB8_SNORM:
            return 3;
        case Image::Format::RGB16_SNORM:
            return 6;
        case Image::Format::RGBA8:
        case Image::Format::RGBA8_SNORM:
            return 4;
        case Image::Format::RGBA16:
            return 8;
        case Image::Format::SRGB8:
            return 3;
        case Image::Format::SRGB8_ALPHA8:
            return 4;
        case Image::Format::R16F:
        case Image::Format::RG16F:
        case Image::Format::RGB16F:
        case Image::Format::RGBA16F:
            return 0;
        case Image::Format::R32F:
            return 4;
        case Image::Format::RG32F:
            return 8;
        case Image::Format::RGB32F:
            return 12;
        case Image::Format::RGBA32F:
            return 16;
        case Image::Format::DEPTH32F:
            return 4;
        case Image::Format::DEPTH32F_STENCIL8:
        case Image::Format::DEPTH24_STENCIL8:
        default:
            return 0;
    }
}

uint32 ImageUtil::GetChannelsCount(Image::Format format) {
    switch (format) {
        case Image::Format::R8:
        case Image::Format::R8_SNORM:
        case Image::Format::R16:
        case Image::Format::R16_SNORM:
            return 1;
        case Image::Format::RG8:
        case Image::Format::RG8_SNORM:
        case Image::Format::RG16:
        case Image::Format::RG16_SNORM:
            return 2;
        case Image::Format::RGB8:
        case Image::Format::RGB8_SNORM:
        case Image::Format::RGB16_SNORM:
            return 3;
        case Image::Format::RGBA8:
        case Image::Format::RGBA8_SNORM:
        case Image::Format::RGBA16:
            return 4;
        case Image::Format::SRGB8:
            return 3;
        case Image::Format::SRGB8_ALPHA8:
            return 4;
        case Image::Format::R16F:
            return 1;
        case Image::Format::RG16F:
            return 2;
        case Image::Format::RGB16F:
            return 3;
        case Image::Format::RGBA16F:
            return 4;
        case Image::Format::R32F:
            return 1;
        case Image::Format::RG32F:
            return 2;
        case Image::Format::RGB32F:
            return 3;
        case Image::Format::RGBA32F:
            return 4;
        case Image::Format::DEPTH32F:
            return 1;
        case Image::Format::DEPTH32F_STENCIL8:
        case Image::Format::DEPTH24_STENCIL8:
            return 2;
        default:
            return 0;
    }
}

bool ImageUtil::CanAccept(Image::Format format) {
    switch (format) {
        case Image::Format::R8:
        case Image::Format::R8_SNORM:
        case Image::Format::R16:
        case Image::Format::R16_SNORM:
        case Image::Format::RG8:
        case Image::Format::RG8_SNORM:
        case Image::Format::RG16:
        case Image::Format::RG16_SNORM:
        case Image::Format::RGB8:
        case Image::Format::RGB8_SNORM:
        case Image::Format::RGB16_SNORM:
        case Image::Format::RGBA8:
        case Image::Format::RGBA8_SNORM:
        case Image::Format::RGBA16:
        case Image::Format::SRGB8:
        case Image::Format::SRGB8_ALPHA8:
            return true;
        case Image::Format::R16F:
        case Image::Format::RG16F:
        case Image::Format::RGB16F:
        case Image::Format::RGBA16F:
            return false;
        case Image::Format::R32F:
        case Image::Format::RG32F:
        case Image::Format::RGB32F:
        case Image::Format::RGBA32F:
        case Image::Format::DEPTH32F:
            return true;
        case Image::Format::DEPTH32F_STENCIL8:
        case Image::Format::DEPTH24_STENCIL8:
        default:
            return false;
    }
}

bool ImageUtil::CanSaveRgba(Image::Format format) {
    switch (format) {
        case Image::Format::R8:
        case Image::Format::R8_SNORM:
        case Image::Format::RG8:
        case Image::Format::RG8_SNORM:
        case Image::Format::RGB8:
        case Image::Format::RGB8_SNORM:
        case Image::Format::RGBA8:
        case Image::Format::RGBA8_SNORM:
        case Image::Format::SRGB8:
        case Image::Format::SRGB8_ALPHA8:
            return true;
        default:
            return false;
    }
}

bool ImageUtil::CanResize(Image::Format format) {
    switch (format) {
        case Image::Format::R8:
        case Image::Format::R8_SNORM:
        case Image::Format::RG8:
        case Image::Format::RG8_SNORM:
        case Image::Format::RGB8:
        case Image::Format::RGB8_SNORM:
        case Image::Format::RGBA8:
        case Image::Format::RGBA8_SNORM:
        case Image::Format::SRGB8:
        case Image::Format::SRGB8_ALPHA8:
            return true;
        default:
            return false;
    }
}

BRK_NS_END