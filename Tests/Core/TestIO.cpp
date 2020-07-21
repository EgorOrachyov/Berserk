/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/Ini.h>
#include <IO/JsonDocument.h>
#include <IO/Archive.h>
#include <IO/ArchiveFile.h>
#include <TPtrShared.h>
#include <Platform/System.h>
#include <String/CStringBuilder.h>

#include <Containers/TSet.h>
#include <Containers/TMap.h>
#include <Containers/TArray.h>

#include <TestMacro.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestIO)
{
    BERSERK_TEST_COND(Json, true)
    {
        JsonDocument document = R"(
            {
                "name1": "aabb",
                "array": [
                    111,
                    -111,
                    2321.12,
                    -23.002
                ],
                "object": {
                    "name2": "ccdd",
                    "name3": 2412
                }
            }
        )";

        if (document.isParsed()) {
            auto& data = document.getContent().getObject();

            printf("%s\n", data["name1"].getString().data());

            auto& array = data["array"].getArray();
            for (auto& v: array) {
                if (v.isInt()) printf("%i\n", v.getInt());
                if (v.isFloat()) printf("%f\n", v.getFloat());
            }
        }
    };

    BERSERK_TEST_COND(JsonFromFile, true)
    {
        auto& sys = System::getSingleton();
        auto file = sys.openFile("./shader.json", EFileMode::Read);

        if (file.isNotNull() && file->isOpen()) {
            JsonDocument document = *file;
            JsonValue& value = document.getContent();

            printf("%s\n", value.toString().data());
        }
    };

    BERSERK_TEST_COND(JsonBuild, true)
    {
        JsonValue value;
        auto& obj = value.getObject();

        obj["name"] = "some useful name";
        obj["version"] = "1.0.0";
        obj["params"] = { "1", 2, -3.0f };

        printf("%s\n", value.toString().data());
    };

    BERSERK_TEST_COND(ArchiveFileWrite,false)
    {
        auto& system = System::getSingleton();
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
        auto& system = System::getSingleton();
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
        auto& system = System::getSingleton();
        auto file = system.openFile("TestArraySerialization.bn", EFileMode::Write);

        if (file.isNotNull() && file->isOpen()) {
            TArray<CString> items = { "apple", "banana", "lemon", "orange", "potato", "etc" };
            ArchiveFile archive(file);

            archive << items;
        }
    };

    BERSERK_TEST_COND(ArrayDeserialization,false)
    {
        auto& system = System::getSingleton();
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
        auto& system = System::getSingleton();
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
        auto& system = System::getSingleton();
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

    BERSERK_TEST_COND(IniFromFile,false)
    {
        auto file = System::getSingleton().openFile("configuration.ini", EFileMode::Read);

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
