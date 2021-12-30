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

#include <core/Engine.hpp>
#include <resource/ResourceManager.hpp>

#include <algorithm>

BRK_NS_BEGIN

Ref<Resource> ResourceManager::Import(const String &filepath, const Ref<ResourceImportOptions> &options, const UUID &uuid) {
    auto &fileSystem = Engine::Instance().GetFileSystem();

    auto path = fileSystem.GetFullFilePath(filepath);
    if (path.empty()) {
        BRK_ERROR("Failed to find full path for resource import " << filepath);
        return Ref<Resource>();
    }

    auto importer = FindImporter(fileSystem.GetFileExtension(filepath));
    if (!importer) {
        BRK_ERROR("Failed to find importer for resource path " << filepath);
        return Ref<Resource>();
    }

    ResourceImportResult importResult;
    importer->Import(path, options, importResult);

    if (importResult.failed) {
        BRK_ERROR("Failed to import resource path=" << filepath << " error=" << importResult.error);
        return Ref<Resource>();
    }
    if (importResult.resource.IsNull()) {
        BRK_ERROR("No single resource imported for path=" << filepath << " use import all instead");
        return Ref<Resource>();
    }

    importResult.resource->SetUUID(uuid);
    return importResult.resource;
}

void ResourceManager::RegisterImporter(std::shared_ptr<ResourceImporter> importer) {
    assert(importer);
    std::lock_guard<std::recursive_mutex> guard(mMutex);
    mImporters.push_back(std::move(importer));
}

bool ResourceManager::CanImport(const String &filepath) const {
    auto &fileSystem = Engine::Instance().GetFileSystem();
    auto ext = fileSystem.GetFileExtension(filepath);
    return FindImporter(ext) != nullptr;
}

ResourceImporter *ResourceManager::FindImporter(const String &ext) const {
    std::lock_guard<std::recursive_mutex> guard(mMutex);

    for (auto &importer : mImporters) {
        auto &extensions = importer->GetSupportedExtensions();
        auto query = std::find(extensions.begin(), extensions.end(), ext);
        if (query != extensions.end())
            return importer.get();
    }

    return nullptr;
}

BRK_NS_END