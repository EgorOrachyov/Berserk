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

#ifndef BERSERK_RESOURCEMANAGER_HPP
#define BERSERK_RESOURCEMANAGER_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <resource/Resource.hpp>
#include <resource/ResourceImporter.hpp>

#include <mutex>
#include <unordered_map>

BRK_NS_BEGIN

/**
 * @addtogroup resource
 * @{
 */

/**
 * @class ResourceManager
 * @brief Main engine resource management class
 *
 * Resource manager is responsible for loading, importing and
 * saving resources on the disc. Provides mechanism for async
 * resources loading. Caches loaded resource for fast access.
 *
 * Common use cases:
 *  - Import some resource using import options
 *  - Load resource with specified id
 *  - Save resource on disc
 */
class ResourceManager {
public:
    BRK_API ResourceManager() = default;
    BRK_API ~ResourceManager() = default;

    /**
     * @brief Import resource at specified file location
     *
     * Import resource using provided path to the file. Will use engine file system
     * and search paths setup to find real path of the file.
     *
     * @param filepath Relative or absolute path to the resource
     * @param options Options to import this specific resource
     * @param uuid Optional id to assign to imported resource
     *
     * @return Imported resource; may be null
     */
    BRK_API Ref<Resource> Import(const String &filepath, const Ref<ResourceImportOptions> &options, const UUID &uuid = UUID::Empty());

    /**
     * @brief Register new resource importer
     *
     * @param importer Importer to register
     */
    BRK_API void RegisterImporter(std::shared_ptr<ResourceImporter> importer);

    /**
     * @brief Check if can import resource
     *
     * @param filepath Relative or absolute path to the resource
     *
     * @return True if can import file with specified path
     */
    BRK_API bool CanImport(const String &filepath) const;

private:
    ResourceImporter *FindImporter(const String &ext) const;

private:
    std::vector<std::shared_ptr<ResourceImporter>> mImporters;

    mutable std::recursive_mutex mMutex;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RESOURCEMANAGER_HPP
