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

#ifndef BERSERK_RESMATERIAL_HPP
#define BERSERK_RESMATERIAL_HPP

#include <resource/ResTexture.hpp>
#include <resource/Resource.hpp>
#include <resource/ResourceImporter.hpp>

#include <render/material/Material.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup resource
 * @{
 */

/**
 * @class ResMaterial
 * @brief 3d mesh geometry for rendering
 */
class ResMaterial final : public Resource {
public:
    BRK_API ResMaterial() = default;
    BRK_API ~ResMaterial() override = default;

    BRK_API const StringName &GetResourceType() const override;
    BRK_API static const StringName &GetResourceTypeStatic();

    BRK_API const Ref<Material> &GetMaterial() const { return mMaterial; }

private:
    Ref<Material> mMaterial; /** Internal material object for rendering */
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RESMATERIAL_HPP
