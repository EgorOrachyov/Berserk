//
// Created by Egor Orachyov on 06.06.2019.
//

#ifndef BERSERK_FILESTEST_H
#define BERSERK_FILESTEST_H

#include <IO/IFile.h>
#include <IO/PlatformFile.h>

using namespace Berserk;

class FilesTest
{

public:

    static void FileReadingTest1()
    {
        printf("\nPlatform File Test\n");

        PlatformFile platformFile("../Test/Files/test1.txt", true, false);
        IFile &file = platformFile;

        printf("File pos: %li \n", file.tell());

        int64 size = file.size();

        char buffer[100];
        bool done = file.read(buffer, size);

        printf("Content: ");
        for (int32 i = 0; i < size; i++)
        { printf("%c", buffer[i]); }
        printf("\n");

        printf("File pos: %li \n", file.tell());

        printf("\n\n");
    }

    static void FileReadingTest2()
    {
        printf("\nPlatform File Test\n");

        PlatformFile platformFile("../Test/Files/test2.txt", false, true);
        IFile &file = platformFile;

        printf("File pos: %li \n", file.tell());

        char buffer[] = "Hello, world! It is Platform file writer";
        bool done = file.write(buffer, sizeof(buffer));

        printf("File pos: %li \n", file.tell());
        printf("File size: %li \n", file.size());

        printf("\n\n");
    }

    static void run()
    {
        FileReadingTest1();
        FileReadingTest2();
    }

};

#endif //BERSERK_FILESTEST_H
