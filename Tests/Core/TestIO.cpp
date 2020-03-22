/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <IO/Json.h>
#include <IO/Archive.h>
#include <IO/ArchiveFile.h>
#include <TPtrShared.h>
#include <Platform/ISystem.h>

#include <TestMacro.h>

using namespace Berserk;

BERSERK_TEST_SECTION(TestIO) {

    BERSERK_TEST(Json) {
        Json::Value value;
        Json document = R"(
        {
          "project":"something",
          "attributes":"atd1",
          "image":{
            "path":"/some/path",
            "extension":"png"
          },
          "colors":["green","red","blue"]
        })";

        printf("Value size: %lu\n", sizeof(Json::Value));
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

        if (file.isNotNull()) {
            Json resource = *file;

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
            resource.data().debug();
            printf("\n");
        }

    };

    BERSERK_TEST(ArchiveFileWrite) {
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


    BERSERK_TEST(ArchiveFileRead) {
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
            float32 v2;

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
}
