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

#ifndef BERSERK_RESOURCEIMPORTER_HPP
#define BERSERK_RESOURCEIMPORTER_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <resource/Resource.hpp>

#include <vector>

BRK_NS_BEGIN

/**
 * @addtogroup resource
 * @{
 */

/**
 * @class ResourceImportOptions
 * @brief Base class for import options to import specific resource
 */
class ResourceImportOptions : public RefCnt {
public:
    BRK_API ~ResourceImportOptions() override = default;
};

/**
 * @class ResourceImportResult
 * @brief Data returned after resource
 */
struct ResourceImportResult {
    bool failed = false;
    String error;
    Ref<Resource> resource;
    std::vector<Ref<Resource>> resources;
};

/**
 * @class ResourceImporter
 * @brief Base class responsible for importing a specific resource type
 */
class ResourceImporter {
public:
    BRK_API virtual ~ResourceImporter() = default;

    /**
     * @brief Creates default importer options for import
     *
     * @return Options
     */
    BRK_API virtual Ref<ResourceImportOptions> CreateDefaultOptions() const = 0;

    /**
     * @brief Get list of supported file extensions for import
     *
     * @return List of extensions
     */
    BRK_API virtual const std::vector<String> &GetSupportedExtensions() const = 0;

    /**
     * @brief Imports resource or set of resources
     *
     * @param[in] fullpath Full path to the resource on the disc
     * @param[in] options Options used to import resource
     * @param[out] result Result of the import operation
     */
    BRK_API virtual void Import(const String &fullpath, const Ref<ResourceImportOptions> &options, ResourceImportResult &result) = 0;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RESOURCEIMPORTER_HPP
