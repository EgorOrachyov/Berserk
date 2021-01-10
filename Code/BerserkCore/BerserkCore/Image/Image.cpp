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
#include <BerserkCore/Debug/Debug.hpp>
#include <BerserkCore/Platform/FileSystem.hpp>

#include <stbimage/stb_image.hpp>
#include <stbimage/stb_image_write.hpp>

namespace Berserk {

    Image::Image(uint32 width, uint32 height, PixelFormat pixelFormat, Ref<MemoryBuffer> buffer) {
        BERSERK_ASSERT(width > 0);
        BERSERK_ASSERT(height > 0);
        BERSERK_ASSERT(buffer.IsNotNull());

        mWidth = width;
        mHeight = height;
        mPixelFormat = pixelFormat;
        mBuffer = std::move(buffer);
    }

    bool Image::SaveBmp(const String &filepath) const {
        if (!IsSupportedFormatForSaving(GetPixelFormat())) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Cannot save image \"{0}\" of this format {1}"), filepath, GetPixelFormat());
            return false;
        }

        auto filename = Platform::FileSystem::GetFileNameFromPath(filepath);
        auto extension = Platform::FileSystem::GetFileExtension(filename);

        if (extension != BERSERK_TEXT("bmp")) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Image filename \"{0}\" does not matches required extension"), filepath);
            return false;
        }

        Channels channels = GetChannelsFromPixelFormat(GetPixelFormat());
        uint32 channelsCount = GetChannelsCount(channels);

        BERSERK_ASSERT(channelsCount > 0 && channelsCount <= 4);

        return stbi_write_bmp(filepath.GetStr_C(), GetWidth(), GetHeight(), channelsCount, GetBufferRef()->GetData());
    }

    bool Image::SavePng(const String &filepath) const {
        if (!IsSupportedFormatForSaving(GetPixelFormat())) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Cannot save image \"{0}\" of this format {1}"), filepath, GetPixelFormat());
            return false;
        }

        auto filename = Platform::FileSystem::GetFileNameFromPath(filepath);
        auto extension = Platform::FileSystem::GetFileExtension(filename);

        if (extension != BERSERK_TEXT("png")) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Image filename \"{0}\" does not matches required extension"), filepath);
            return false;
        }

        Channels channels = GetChannelsFromPixelFormat(GetPixelFormat());
        uint32 channelsCount = GetChannelsCount(channels);
        uint32 stride = channelsCount * GetWidth();

        BERSERK_ASSERT(channelsCount > 0 && channelsCount <= 4);

        return stbi_write_png(filepath.GetStr_C(), GetWidth(), GetHeight(), channelsCount, GetBufferRef()->GetData(), stride);
    }

    bool Image::SaveJpg(const String &filepath, uint32 quality) const {
        if (!IsSupportedFormatForSaving(GetPixelFormat())) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Cannot save image \"{0}\" of this format {1}"), filepath, GetPixelFormat());
            return false;
        }

        quality = quality < 1 || quality > 100 ? 20: quality;

        auto filename = Platform::FileSystem::GetFileNameFromPath(filepath);
        auto extension = Platform::FileSystem::GetFileExtension(filename);

        if (extension != BERSERK_TEXT("jpg") && extension != BERSERK_TEXT("jpeg")) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Image filename \"{0}\" does not matches required extension"), filepath);
            return false;
        }

        Channels channels = GetChannelsFromPixelFormat(GetPixelFormat());
        uint32 channelsCount = GetChannelsCount(channels);

        BERSERK_ASSERT(channelsCount > 0 && channelsCount <= 4);

        return stbi_write_jpg(filepath.GetStr_C(), GetWidth(), GetHeight(), channelsCount, GetBufferRef()->GetData(), quality);
    }

    Image Image::Load(const String &filepath, Channels channels) {
        struct AutoRelease {
        public:
            explicit AutoRelease(stbi_uc* data) {
                mData = data;
            }

            ~AutoRelease() {
                if (mData) {
                    stbi_image_free(mData);
                }
            }
        private:
            stbi_uc* mData = nullptr;
        };

        auto desiredChannelsCount = GetChannelsCount(channels);
        BERSERK_ASSERT(desiredChannelsCount > 0 && desiredChannelsCount <= 4);

        int width = 0;
        int height = 0;
        int channelsInFile = 0;

        stbi_uc* data = stbi_load(filepath.GetStr_C(), &width, &height, &channelsInFile, desiredChannelsCount);

        AutoRelease autoRelease(data);

        if (!data) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Failed to import image \"{0}\""), filepath);
            return {};
        }

        BERSERK_ASSERT(width > 0);
        BERSERK_ASSERT(height > 0);
        BERSERK_ASSERT(channelsInFile > 0 && channelsInFile <= 4);

        PixelFormat format;

        switch (desiredChannelsCount) {
            case 1:
                format = PixelFormat::R8;
                break;
            case 2:
                format = PixelFormat::R8G8;
                break;
            case 3:
                format = PixelFormat::R8G8B8;
                break;
            case 4:
                format = PixelFormat::R8G8B8A8;
                break;
            default:
                BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Unsupported image format \"{0}\""), filepath);
                return {};
        }

        auto memorySize = sizeof(stbi_uc) * desiredChannelsCount * width * height;
        auto memory = MemoryBufferGeneric<Platform::Allocator>::Create();

        memory->Resize(memorySize);
        Platform::Memory::Copy(memory->GetData(), data, memorySize);

        return Image((uint32) width, (uint32) height, format, (Ref<MemoryBuffer>) memory);
    }

    Image Image::Create(uint32 width, uint32 height, const Color &color) {
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

    uint32 Image::GetPixelSize(PixelFormat format) {
        switch (format) {
            case PixelFormat::R8:
                return 1;
            case PixelFormat::R8G8:
                return 1 * 2;
            case PixelFormat::R8G8B8:
                return 1 * 3;
            case PixelFormat::R8G8B8A8:
                return 1 * 4;
            default:
                return 0;
        }
    }

    uint32 Image::GetChannelsCount(Channels channels) {
        switch (channels) {
            case Channels::R:
                return 1;
            case Channels::RG:
                return 2;
            case Channels::RGB:
                return 3;
            case Channels::RGBA:
                return 4;
            default:
                return 0;
        }
    }

    Image::Channels Image::GetChannelsFromCount(uint32 count) {
        switch (count) {
            case 1:
                return Channels::R;
            case 2:
                return Channels::RG;
            case 3:
                return Channels::RGB;
            case 4:
                return Channels::RGBA;
            default:
                return Channels::Unknown;
        }
    }

    Image::Channels Image::GetChannelsFromPixelFormat(PixelFormat format) {
        switch (format) {
            case PixelFormat::R8:
                return Channels::R;
            case PixelFormat::R8G8:
                return Channels::RG;
            case PixelFormat::R8G8B8:
                return Channels::RGB;
            case PixelFormat::R8G8B8A8:
                return Channels::RGBA;
            default:
                return Channels::Unknown;
        }
    }

    bool Image::IsSupportedFormatForSaving(PixelFormat format) {
        switch (format) {
            case PixelFormat::R8:
            case PixelFormat::R8G8:
            case PixelFormat::R8G8B8:
            case PixelFormat::R8G8B8A8:
                return true;
            default:
                return false;
        }
    }

}