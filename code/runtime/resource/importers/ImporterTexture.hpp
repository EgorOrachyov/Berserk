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

#ifndef BERSERK_IMPORTERTEXTURE_HPP
#define BERSERK_IMPORTERTEXTURE_HPP

#include <resource/ResourceImporter.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup resource
 * @{
 */

/**
 * @class ImporterTexture
 * @brief Texture resources importer
 */
class ImporterTexture final : public ResourceImporter {
public:
    BRK_API ImporterTexture();
    BRK_API ~ImporterTexture() override = default;
    Ref<ResourceImportOptions> CreateDefaultOptions() const override;
    const std::vector<String> &GetSupportedExtensions() const override;
    void Import(const String &fullpath, const Ref<ResourceImportOptions> &options, ResourceImportResult &result) override;

private:
    std::vector<String> mExtensions;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_IMPORTERTEXTURE_HPP
