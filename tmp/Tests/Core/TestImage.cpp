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

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>
#include <BerserkCore/Debug/Debug.hpp>
#include <BerserkCore/Image/Image.hpp>

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(ImageFixture)

TEST_F(ImageFixture, BasicImage) {
    size_t w = 100, h = 50;
    Color color = Color(1.0f, 0.0f, 0.5f);

    Image image = Image::Create(w, h, color);
}

TEST_F(ImageFixture, ImageLoad) {
    String path = BERSERK_TEXT("../../Engine/Resources/Textures/missing-texture.jpeg");
    Image::Channels channels = Image::Channels::R;

    Image image = Image::Load(path, channels);

    EXPECT_TRUE(!image.IsEmpty());
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Image size=({0}, {1}) format={2}"), image.GetWidth(), image.GetHeight(), image.GetPixelFormat());
}

TEST_F(ImageFixture, ImageWriteBmp) {
    String path = BERSERK_TEXT("../../Engine/Resources/Textures/missing-texture.jpeg");
    Image::Channels channels = Image::Channels::RGB;

    Image image = Image::Load(path, channels);

    EXPECT_TRUE(!image.IsEmpty());
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Image size=({0}, {1}) format={2}"), image.GetWidth(), image.GetHeight(), image.GetPixelFormat());

    String filename = BERSERK_TEXT("saved_texture.bmp");

    EXPECT_TRUE(image.SaveBmp(filename));
}

TEST_F(ImageFixture, ImageWritePng) {
    String path = BERSERK_TEXT("../../Engine/Resources/Textures/missing-texture.jpeg");
    Image::Channels channels = Image::Channels::RGB;

    Image image = Image::Load(path, channels);

    EXPECT_TRUE(!image.IsEmpty());
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Image size=({0}, {1}) format={2}"), image.GetWidth(), image.GetHeight(), image.GetPixelFormat());

    String filename = BERSERK_TEXT("saved_texture.png");

    EXPECT_TRUE(image.SavePng(filename));
}

TEST_F(ImageFixture, ImageWriteJpg) {
    String path = BERSERK_TEXT("../../Engine/Resources/Textures/missing-texture.jpeg");
    Image::Channels channels = Image::Channels::RGB;

    Image image = Image::Load(path, channels);

    EXPECT_TRUE(!image.IsEmpty());
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Image size=({0}, {1}) format={2}"), image.GetWidth(), image.GetHeight(), image.GetPixelFormat());

    String filename = BERSERK_TEXT("saved_texture.jpg");

    EXPECT_TRUE(image.SaveJpg(filename, 40));
}

BERSERK_GTEST_MAIN