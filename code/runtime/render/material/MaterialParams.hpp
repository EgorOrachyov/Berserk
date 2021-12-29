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

#ifndef BERSERK_MATERIALPARAMS_HPP
#define BERSERK_MATERIALPARAMS_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <rhi/RHIBuffer.hpp>
#include <rhi/RHIResourceSet.hpp>
#include <rhi/RHITexture.hpp>

#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup render
 * @{
 */

/**
 * @class MaterialParams
 * @brief Packed material params ready for rendering usage
 */
class MaterialParams final : public RefCnt {
public:
    static constexpr const char *const SHADER_PARAMS_BLOCK = BRK_TEXT("ShaderParams");

    /** Creates material params for material (uses material technique) */
    BRK_API explicit MaterialParams(class Material &material);
    BRK_API ~MaterialParams() override = default;

    /** Updates material params set */
    BRK_API void Update(class Material &material);

    /** @return Resources sets of material for each pass */
    BRK_API const std::vector<Ref<RHIResourceSet>> &GetResourceSets() const { return mResourceSets; }

private:
    std::vector<Ref<RHIUniformBuffer>> mUniformBuffers; /** GPU-uniform buffers for each pass */
    std::vector<Ref<RHIResourceSet>> mResourceSets;     /** Sets for each pass */
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_MATERIALPARAMS_HPP
