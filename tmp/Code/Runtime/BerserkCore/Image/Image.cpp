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

#include <BerserkCore/Image/Image.hpp>
#include <BerserkCore/Debug/Debug.hpp>
#include <BerserkCore/Platform/FileSystem.hpp>

#include <stbimage/stb_image.hpp>
#include <stbimage/stb_image_write.hpp>

namespace Berserk {

    Image::Image(uint32 width, uint32 height, PixelFormat pixelFormat, RcPtr<Data> buffer) {
        assert(width > 0);
        assert(height > 0);
        assert(buffer.IsNotNull());

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

        auto filename = FileSystem::GetFileNameFromPath(filepath);
        auto extension = FileSystem::GetFileExtension(filename);

        if (extension != BERSERK_TEXT("bmp")) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Image filename \"{0}\" does not matches required extension"), filepath);
            return false;
        }

        Channels channels = GetChannelsFromPixelFormat(GetPixelFormat());
        uint32 channelsCount = GetChannelsCount(channels);

        assert(channelsCount > 0 && channelsCount <= 4);

        return stbi_write_bmp(filepath.GetStr_C(), GetWidth(), GetHeight(), channelsCount, GetBufferRef()->GetData());
    }

    bool Image::SavePng(const String &filepath) const {
        if (!IsSupportedFormatForSaving(GetPixelFormat())) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Cannot save image \"{0}\" of this format {1}"), filepath, GetPixelFormat());
            return false;
        }

        auto filename = FileSystem::GetFileNameFromPath(filepath);
        auto extension = FileSystem::GetFileExtension(filename);

        if (extension != BERSERK_TEXT("png")) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Image filename \"{0}\" does not matches required extension"), filepath);
            return false;
        }

        Channels channels = GetChannelsFromPixelFormat(GetPixelFormat());
        uint32 channelsCount = GetChannelsCount(channels);
        uint32 stride = channelsCount * GetWidth();

        assert(channelsCount > 0 && channelsCount <= 4);

        return stbi_write_png(filepath.GetStr_C(), GetWidth(), GetHeight(), channelsCount, GetBufferRef()->GetData(), stride);
    }

    bool Image::SaveJpg(const String &filepath, uint32 quality) const {
        if (!IsSupportedFormatForSaving(GetPixelFormat())) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Cannot save image \"{0}\" of this format {1}"), filepath, GetPixelFormat());
            return false;
        }

        quality = quality < 1 || quality > 100 ? 20: quality;

        auto filename = FileSystem::GetFileNameFromPath(filepath);
        auto extension = FileSystem::GetFileExtension(filename);

        if (extension != BERSERK_TEXT("jpg") && extension != BERSERK_TEXT("jpeg")) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Image filename \"{0}\" does not matches required extension"), filepath);
            return false;
        }

        Channels channels = GetChannelsFromPixelFormat(GetPixelFormat());
        uint32 channelsCount = GetChannelsCount(channels);

        assert(channelsCount > 0 && channelsCount <= 4);

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
        assert(desiredChannelsCount > 0 && desiredChannelsCount <= 4);

        int width = 0;
        int height = 0;
        int channelsInFile = 0;

        stbi_uc* data = stbi_load(filepath.GetStr_C(), &width, &height, &channelsInFile, desiredChannelsCount);

        AutoRelease autoRelease(data);

        if (!data) {
            BERSERK_CORE_LOG_ERROR(BERSERK_TEXT("Failed to import image \"{0}\""), filepath);
            return {};
        }

        assert(width > 0);
        assert(height > 0);
        assert(channelsInFile > 0 && channelsInFile <= 4);

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
        auto memory = Data::Make(data, memorySize);

        return Image((uint32) width, (uint32) height, format, memory);
    }

    Image Image::Create(uint32 width, uint32 height, const Color &color) {
        PixelFormat format = PixelFormat::R8G8B8A8;
        Color::RGBA8 c = color.ToRGBA8();

        uint64 pixelSize = GetPixelSize(format);
        uint64 stride = width * pixelSize;
        uint64 bufferSize = stride * height;

        auto memory = Data::Make(bufferSize);
        auto data = (uint8*) memory->GetData();

        for (uint64 i = 0; i < height; i++) {
            auto row = (Color::RGBA8*) (data + stride * i);

            for (uint64 j = 0; j < width; j++) {
                row[j] = c;
            }
        }

        return Image(width, height, format, memory);
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