/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/Ini.h>
#include <IO/Json.h>
#include <IO/Archive.h>
#include <IO/ArchiveFile.h>
#include <TPtrShared.h>
#include <Platform/ISystem.h>
#include <String/CStringBuilder.h>

#include <TSet.h>
#include <TMap.h>
#include <TArray.h>

#include <TestMacro.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestIO)
{
    BERSERK_TEST_COND(Json, false)
    {
        JsonValue value;
        JsonDocument document = R"(
        {
          "project":"something",
          "attributes":"attr",
          "image":{
            "path":"/some/path",
            "extension":"png"
          },
          "colors":["green","red","blue"]
        })";

        printf("Value size: %lu\n", sizeof(JsonValue));
        printf("Is parsed: %i\n", document.isParsed());
        printf("Dyn alloc stat: %llu %llu\n", Memory::getAllocCalls(), Memory::getFreeCalls());

        auto& content = document.data();

        if (content.isNotNull()) {
            printf("project: %s\n", content["project"].getString().data());
            printf("attributes: %s\n", content["attributes"].getString().data());

            auto& image = content["image"];

            if (image.isNotNull()) {
                printf("path: %s\n", image["path"].getString().data());
                printf("extension: %s\n", image["extension"].getString().data());
            }

            auto& colors = content["colors"];

            if (colors.isNotNull()) {
                for (uint32 i = 0; i < colors.getArray().size(); i++) {
                    printf("%i= %s\n", i, colors[i].getString().data());
                }
            }
        }

        auto file = ISystem::getSingleton().openFile("resource.json", EFileMode::Read);

        if (file.isNotNull() && file->isOpen()) {
            JsonDocument resource = *file;

            printf("Is parsed: %i\n", resource.isParsed());
            printf("Dyn alloc stat: %llu %llu\n", Memory::getAllocCalls(), Memory::getFreeCalls());

            auto& root = resource.data();

            if (root.isNotNull()) {
                printf("%s \n", root["uuid"].getString().data());
                printf("%s \n", root["type"].getString().data());
                printf("%s \n", root["extension"].getString().data());
                printf("%s \n", root["path"].getString().data());
                printf("%i \n", root["options"].isNull());
            }

            printf("Out json in console\n");
            CStringBuilder serialized;

            root.toStringBuilder(serialized);
            serialized.append('\0');
            printf("%s\n", serialized.data());

            root.toStringBuilderCompact(serialized);
            serialized.append('\0');
            printf("%s\n", serialized.data());
        }

    };

    BERSERK_TEST_COND(FontMeta,false)
    {
        struct MyVisitor : public JsonVisitor {
        public:
            bool parseResource = false;
            bool parseFont = false;

            bool acceptString(CString &value) override {
                return true;
            }

            bool acceptArray(TArray<JsonValue> &body) override {
                return true;
            }

            bool acceptObject(TArray<TPair<CString, JsonValue>> &body) override {
                if (parseResource) {
                    for (auto& pair: body) {
                        if (pair.first() == "mUUID") {
                            printf(" mUUID: %s\n", pair.second().getString().data());
                        }
                        if (pair.first() == "mName") {
                            printf(" mName: %s\n", pair.second().getString().data());
                        }
                        if (pair.first() == "mPath") {
                            printf(" mPath: %s\n", pair.second().getString().data());
                        }
                        if (pair.first() == "mImportPath") {
                            printf(" mImportPath: %s\n", pair.second().getString().data());
                        }
                    }

                    return true;
                }

                if (parseFont) {
                    for (auto& pair: body) {
                        if (pair.first() == "mWidth") {
                            printf(" mWidth: %s\n", pair.second().getString().data());
                        }
                        if (pair.first() == "mHeight") {
                            printf(" mHeight: %s\n", pair.second().getString().data());
                        }
                    }

                    return true;
                }

                for (auto& pair: body) {
                    if (pair.first() == "Resource") {
                        parseResource = true;
                        printf("Resource:\n");
                        pair.second().accept(*this);
                        parseResource = false;
                    }
                    if (pair.first() == "Font") {
                        parseFont = true;
                        printf("Font:\n");
                        pair.second().accept(*this);
                        parseFont = false;
                    }
                }

                return true;
            }
        };

        JsonDocument document = R"(
             {
                 "Resource" : {
                     "mUUID":"12345068af8b095bcdff0127810faeba",
                     "mName":"font_arial_16",
                     "mPath":"resources/fonts/font_arial_16.png",
                     "mImportPath":"desktop/fonts/arial.ttf"
                 },
                 "Font" : {
                     "mWidth":"16",
                     "mHeight":"16"
                 }
             }
        )";

        MyVisitor visitor;
        document.accept(visitor);
    };

    BERSERK_TEST_COND(ArchiveFileWrite,false)
    {
        auto& system = ISystem::getSingleton();
        auto& log = system.getLog();
        auto file = system.openFile("TestArchiveFile.bn", EFileMode::Write);

        if (file.isNotNull() && file->isOpen()) {

            auto archivePtr = TPtrShared<ArchiveFile>::make(file, "TestArchiveFile");
            auto& archive = *archivePtr;

            archive << "Some data";
            archive << CString("Another data");
            archive << CStringStatic("And some another data");
            archive << 456u << 0.5f;

            log.logf(ELogVerbosity::Info, "Archive info: name: %s; size: %i; can read: %i; can write: %i",
                     archive.getName().data(), archive.getSize(), archive.canRead(), archive.canWrite());
        }
    };


    BERSERK_TEST_COND(ArchiveFileRead,false)
    {
        auto& system = ISystem::getSingleton();
        auto& log = system.getLog();
        auto file = system.openFile("TestArchiveFile.bn", EFileMode::Read);

        if (file.isNotNull() && file->isOpen()) {

            auto archivePtr = TPtrShared<ArchiveFile>::make(file, "TestArchiveFile");
            auto& archive = *archivePtr;

            CString d1;
            CString d2;
            CStringStatic d3;
            uint32 v1;
            float v2;

            archive >> d1;
            archive >> d2;
            archive >> d3;
            archive >> v1 >> v2;

            log.logf(ELogVerbosity::Info, "Archive is empty?: %i", archive.vempty());

            log.logf(ELogVerbosity::Info, "Archive info: name: %s; size: %i; can read: %i; can write: %i",
                     archive.getName().data(), archive.getSize(), archive.canRead(), archive.canWrite());

            log.logf(ELogVerbosity::Info, "Data: %s; %s; %s; %u; %f;",
                     d1.data(), d2.data(), d3.data(), v1, v2);
        }

    };

    BERSERK_TEST_COND(ArraySerialization,false)
    {
        auto& system = ISystem::getSingleton();
        auto file = system.openFile("TestArraySerialization.bn", EFileMode::Write);

        if (file.isNotNull() && file->isOpen()) {
            TArray<CString> items = { "apple", "banana", "lemon", "orange", "potato", "etc" };
            ArchiveFile archive(file);

            archive << items;
        }
    };

    BERSERK_TEST_COND(ArrayDeserialization,false)
    {
        auto& system = ISystem::getSingleton();
        auto& log = system.getLog();
        auto file = system.openFile("TestArraySerialization.bn", EFileMode::Read);

        if (file.isNotNull() && file->isOpen()) {
            TArray<CString> items;
            ArchiveFile archive(file);

            archive >> items;

            for (const auto& item: items) {
                log.logf(ELogVerbosity::Info, "[Item: %s]", item.data());
            }
        }
    };

    BERSERK_TEST_COND(SetSerialization,false)
    {
        auto& system = ISystem::getSingleton();
        auto& log = system.getLog();
        auto file = system.openFile("TestSetSerialization.bn", EFileMode::Write);

        if (file != nullptr && file->isOpen()) {
            TSet<CString> items = { "apple", "banana", "lemon", "orange", "potato", "etc" };
            ArchiveFile archive(file);

            archive << items;
        }
    };

    BERSERK_TEST_COND(SetDeserialization, false)
    {
        auto& system = ISystem::getSingleton();
        auto& log = system.getLog();
        auto file = system.openFile("TestSetSerialization.bn", EFileMode::Read);

        if (file != nullptr && file->isOpen()) {
            TSet<CString> items;
            ArchiveFile archive(file);

            archive >> items;

            for (auto& item: items) {
                log.logf(ELogVerbosity::Info, "[Item: %s]", item.data());
            }
        }
    };

    BERSERK_TEST_COND(Ini, false)
    {
        Ini file = R"(
            [General]
            [Application]
            [Rendering]
            Shaders.Path = "Engine/Shaders/"
            +Shaders.Path = "Engine/Shaders/OpenGL/"
            +Shaders.Path = "Engine/Shaders/DirectX/"
            +Shaders.Path = "Engine/Shaders/Vulkan/"
            Shadows.Enable = True
            Shadows.Quality = Good
            Shadows.Map.Size = 1024
            Shadows.Filtering = Soft
            DebugDraw.Enable = False
            [Scripting]
            [Profiling]
        )";

        auto &content = file.getContent();

        if (file.isParsed())
            printf("Parsed\n");

        for (auto& section: content) {
            printf("[%s]\n", section.first().data());

            for (auto& value: section.second()) {
                printf(" %s = %s\n", value.first().data(), value.second().data());
            }
        }
    };

    BERSERK_TEST_COND(IniFromFile,true)
    {
        auto file = ISystem::getSingleton().openFile("configuration.ini", EFileMode::Read);

        Ini config = *file;

        auto &content = config.getContent();

        if (!config.isParsed())
            printf("Not parsed\n");

        for (auto& section: content) {
            printf("[%s]\n", section.first().data());

            for (auto& value: section.second()) {
                printf(" %s = %s\n", value.first().data(), value.second().data());
            }
        }

        CString text = config.toString();
        printf("\n%s", text.data());

        auto section = config.getSection("Scripting");
        if (section.isNotNull()) {
            TArray<CString> values;
            config.getValues(*section, "script.path", values);


            printf("Scripting paths:\n");
            for (auto& v: values) {
                printf("%s\n", v.data());
            }
        }
    };

}
