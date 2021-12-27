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
#include <core/io/Config.hpp>
#include <core/io/Logger.hpp>

#include <cassert>
#include <unordered_map>

#include <tinyxml2.hpp>

BRK_NS_BEGIN

class ConfigData {
private:
    tinyxml2::XMLDocument document;
    tinyxml2::XMLElement *root{};
    std::unordered_map<StringName, tinyxml2::XMLElement *> sections;
    std::unordered_map<tinyxml2::XMLElement *, std::unordered_map<StringName, tinyxml2::XMLElement *>> keys;

public:
    inline bool Open(std::FILE *file) {
        auto error = document.LoadFile(file);
        return error == tinyxml2::XML_SUCCESS;
    }

    inline bool Parse() {
        if (!(root = document.FirstChildElement("config"))) {
            BRK_ERROR("No `config` section");
            return false;
        }

        auto nextSection = root->FirstChildElement("section");
        while (nextSection) {
            auto atrSectionName = nextSection->FindAttribute("name");
            assert(atrSectionName);

            if (atrSectionName) {
                sections[StringName(atrSectionName->Value())] = nextSection;
                auto &keysMap = keys[nextSection];

                auto nextEntry = nextSection->FirstChildElement("entry");
                while (nextEntry) {
                    auto atrEntryKey = nextEntry->FindAttribute("key");
                    assert(atrEntryKey);

                    if (atrEntryKey)
                        keysMap[StringName(atrEntryKey->Value())] = nextEntry;

                    nextEntry = nextEntry->NextSiblingElement("entry");
                }

                nextSection = nextSection->NextSiblingElement("section");
            }
        }

        return true;
    }

    inline tinyxml2::XMLElement *FindSection(const StringName &name) const {
        auto query = sections.find(name);
        return query != sections.end() ? query->second : nullptr;
    }

    inline tinyxml2::XMLElement *FindKey(tinyxml2::XMLElement *section, const StringName &name) const {
        auto mapQuery = keys.find(section);

        if (mapQuery == keys.end())
            return nullptr;

        auto query = mapQuery->second.find(name);
        return query != mapQuery->second.end() ? query->second : nullptr;
    }

    inline tinyxml2::XMLElement *Find(const StringName &section, const StringName &key) const {
        auto s = FindSection(section);
        return s ? FindKey(s, key) : nullptr;
    }
};

Config::Config() = default;
Config::~Config() = default;

bool Config::Open(const String &filepath) {
    auto &fileSystem = Engine::Instance().GetFileSystem();
    auto fullPath = fileSystem.GetFullFilePath(filepath);

    if (fullPath.empty()) {
        BRK_ERROR("Failed to get full path to config file " << filepath);
        return false;
    }

    auto file = fileSystem.OpenFile(fullPath, "rb");

    if (!file) {
        BRK_ERROR("Failed open file " << fullPath);
        return false;
    }

    mFilepath = std::move(fullPath);
    mData = std::unique_ptr<ConfigData>(new ConfigData);

    auto status = mData->Open(file);
    fileSystem.CloseFile(file);

    if (!status) {
        mData.reset();
        BRK_ERROR("Failed to parse file " << mFilepath);
        return false;
    }

    if (!mData->Parse()) {
        mData.reset();
        BRK_ERROR("Failed to parse config " << mFilepath);
        return false;
    }

    return true;
}

int32 Config::GetProperty(const StringName &section, const StringName &key, int32 defaultValue) const {
    assert(IsOpen());
    auto entry = mData->Find(section, key);

    if (entry) {
        auto atrValue = entry->FindAttribute("value");
        if (atrValue)
            return atrValue->IntValue();
    }

    return defaultValue;
}

uint32 Config::GetProperty(const StringName &section, const StringName &key, uint32 defaultValue) const {
    assert(IsOpen());
    auto entry = mData->Find(section, key);

    if (entry) {
        auto atrValue = entry->FindAttribute("value");
        if (atrValue)
            return atrValue->UnsignedValue();
    }

    return defaultValue;
}

float Config::GetProperty(const StringName &section, const StringName &key, float defaultValue) const {
    assert(IsOpen());
    auto entry = mData->Find(section, key);

    if (entry) {
        auto atrValue = entry->FindAttribute("value");
        if (atrValue)
            return atrValue->FloatValue();
    }

    return defaultValue;
}

String Config::GetProperty(const StringName &section, const StringName &key, const String &defaultValue) const {
    assert(IsOpen());
    auto entry = mData->Find(section, key);

    if (entry) {
        auto atrValue = entry->FindAttribute("value");
        if (atrValue)
            return atrValue->Value();
    }

    return defaultValue;
}

BRK_NS_END