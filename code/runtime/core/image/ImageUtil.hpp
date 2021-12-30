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

#ifndef BERSERK_IMAGEUTIL_HPP
#define BERSERK_IMAGEUTIL_HPP

#include <core/Config.hpp>
#include <core/image/Image.hpp>
#include <core/math/TVecN.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class ImageUtil
 * @brief Image processing utils
 */
class ImageUtil {
public:
    BRK_API static uint32 GetMaxMipsCount(uint32 width, uint32 height, uint32 depth);
    BRK_API static Size2u GetMipSize(uint32 level, uint32 width, uint32 height);
    BRK_API static uint32 GetPixelSize(Image::Format format);
    BRK_API static uint32 GetChannelsCount(Image::Format format);
    BRK_API static bool CanAccept(Image::Format format);
    BRK_API static bool CanSaveRgba(Image::Format format);
    BRK_API static bool CanResize(Image::Format format);
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_IMAGEUTIL_HPP
