/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Image/Image.hpp>
#include <BerserkCore/Assert.hpp>
#include <BerserkCore/Memory/MemoryBufferGeneric.hpp>

namespace Berserk {

    Image::Image(size_t width, size_t height, PixelFormat pixelFormat, Ref<MemoryBuffer> buffer) {
        BERSERK_ASSERT(width > 0);
        BERSERK_ASSERT(height > 0);
        BERSERK_ASSERT(buffer.IsNotNull());

        mWidth = width;
        mHeight = height;
        mPixelFormat = pixelFormat;
        mBuffer = std::move(buffer);
    }

    Image Image::Create(size_t width, size_t height, const Color &color) {
        PixelFormat format = PixelFormat::R8G8B8A8;
        Color::RGBA8 c = color.ToRGBA8();

        size_t pixelSize = GetPixelSize(format);
        size_t stride = width * pixelSize;
        size_t bufferSize = stride * height;

        auto memory = MemoryBufferGeneric<Platform::Allocator>::Create();
        memory->Resize(bufferSize);

        auto data = (uint8*) memory->GetData();

        for (size_t i = 0; i < height; i++) {
            auto row = (Color::RGBA8*) (data + stride * i);

            for (size_t j = 0; j < width; j++) {
                row[j] = c;
            }
        }

        return Image(width, height, format, (Ref<MemoryBuffer>) memory);
    }

    size_t Image::GetPixelSize(PixelFormat format) {
        switch (format) {
            case PixelFormat::R8:
                return 1;
            case PixelFormat::R8G8B8A8:
                return 1 * 4;
            case PixelFormat::R16G16B16A16f:
                return 2 * 4;
            default:
                return 0;
        }
    }

}