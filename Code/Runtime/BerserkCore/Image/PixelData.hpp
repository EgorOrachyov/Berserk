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

#ifndef BERSERK_PIXELDATA_HPP
#define BERSERK_PIXELDATA_HPP

#include <BerserkCore/Typedefs.hpp>
#include <BerserkCore/Memory/Data.hpp>

namespace Berserk {

    /** Pixel Data Format */
    enum class PixelDataFormat: uint8 {
        /** One Red channel */
        R,
        /** Two Red and Green channels */
        RG,
        /** Three Red, Green and Blue channels */
        RGB,
        /** Four Red, Green, Blue and Alpha channels */
        RGBA,
        /** Depth, 16-bit or 24-bits usually */
        DEPTH_COMPONENT,
        /** Two Depth (24-bits) + Stencil (8-bits) channels */
        DEPTH_STENCIL
    };

    /** Pixel data type */
    enum class PixelDataType : uint8 {
        /** unsigned byte */
        UBYTE,
        /** signed byte */
        BYTE,
        /** unsigned short (16-bit) */
        USHORT,
        /** signed short (16-bit) */
        SHORT,
        /** unsigned int (16-bit) */
        UINT,
        /** signed int (32-bit) */
        INT,
        /** half-float (16-bit float) */
        HALF,
        /** float (32-bits float) */
        FLOAT
    };

    /** Pixel buffer used to update textures data */
    class PixelData: public RefCountedThreadSafe {
    public:
        using CallbackType = Function<void(const RcPtr<const PixelData>& self)>;

        PixelData(PixelDataFormat format, PixelDataType type, RcPtr<Data> data);
        ~PixelData() override = default;

        /** Set callback function, no be notified when pixel data is consumed */
        void SetNotificationCallback(CallbackType callback);

        /** @return Pointer to pixel data buffer */
        const void* GetData() const;

        /** @return Pixel data size in bytes */
        uint64 GetDataSize() const;

        /** Called to notify owner that buffer was consumed */
        void NotifyConsumed() const;

        /** @return Pixel data format */
        PixelDataFormat GetDataFormat() const { return mFormat; }

        /** @return Pixel data type */
        PixelDataType GetDataType() const { return mType; }

    private:
        PixelDataFormat mFormat;
        PixelDataType mType;
        RcPtr<Data> mData;
        CallbackType mCallback;
    };

}

#endif //BERSERK_PIXELDATA_HPP