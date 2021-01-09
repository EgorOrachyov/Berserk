/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <gtest/gtest.h>
#include <PlatformSetup.hpp>

#include <BerserkCore/Image/Image.hpp>

using namespace Berserk;

BERSERK_DEFINE_FIXTURE(ImageFixture)

TEST_F(ImageFixture, BasicImage) {
    size_t w = 100, h = 50;
    Color color = Color(1.0f, 0.0f, 0.5f);

    Image image = Image::Create(w, h, color);
}

BERSERK_GTEST_MAIN