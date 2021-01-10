/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
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
    String path = BERSERK_TEXT("missing_texture.jpeg");
    Image::Channels channels = Image::Channels::R;

    Image image = Image::Load(path, channels);

    EXPECT_TRUE(!image.IsEmpty());
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Image size=({0}, {1}) format={2}"), image.GetWidth(), image.GetHeight(), image.GetPixelFormat());
}

TEST_F(ImageFixture, ImageWriteBmp) {
    String path = BERSERK_TEXT("missing_texture.jpeg");
    Image::Channels channels = Image::Channels::RGB;

    Image image = Image::Load(path, channels);

    EXPECT_TRUE(!image.IsEmpty());
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Image size=({0}, {1}) format={2}"), image.GetWidth(), image.GetHeight(), image.GetPixelFormat());

    String filename = BERSERK_TEXT("saved_texture.bmp");

    EXPECT_TRUE(image.SaveBmp(filename));
}

TEST_F(ImageFixture, ImageWritePng) {
    String path = BERSERK_TEXT("missing_texture.jpeg");
    Image::Channels channels = Image::Channels::RGB;

    Image image = Image::Load(path, channels);

    EXPECT_TRUE(!image.IsEmpty());
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Image size=({0}, {1}) format={2}"), image.GetWidth(), image.GetHeight(), image.GetPixelFormat());

    String filename = BERSERK_TEXT("saved_texture.png");

    EXPECT_TRUE(image.SavePng(filename));
}

TEST_F(ImageFixture, ImageWriteJpg) {
    String path = BERSERK_TEXT("missing_texture.jpeg");
    Image::Channels channels = Image::Channels::RGB;

    Image image = Image::Load(path, channels);

    EXPECT_TRUE(!image.IsEmpty());
    BERSERK_CORE_LOG_INFO(BERSERK_TEXT("Image size=({0}, {1}) format={2}"), image.GetWidth(), image.GetHeight(), image.GetPixelFormat());

    String filename = BERSERK_TEXT("saved_texture.jpg");

    EXPECT_TRUE(image.SaveJpg(filename, 40));
}


BERSERK_GTEST_MAIN