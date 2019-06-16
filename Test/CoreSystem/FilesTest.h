//
// Created by Egor Orachyov on 06.06.2019.
//

#ifndef BERSERK_FILESTEST_H
#define BERSERK_FILESTEST_H

#include <IO/IFile.h>
#include <IO/PlatformFile.h>
#include <IO/CachedFileWriter.h>
#include <IO/LoggedFile.h>
#include <Logging/Debug.h>

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

        printf("Operation done: %i\n", done);
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

        printf("Operation done: %i\n", done);
        printf("File pos: %li \n", file.tell());
        printf("File size: %li \n", file.size());

        printf("\n\n");
    }

    static void CachedFileTest1()
    {
        printf("\nCached File Test\n");

        PlatformFile platformFile("../Test/Files/test3.txt", false, true);
        CachedFileWriter cachedFileWriter(platformFile, 100);
        IFile &file = cachedFileWriter;

        printf("File pos: %li \n", file.tell());

        char buffer1[] = "Hello, world! It is Platform file writer.... No, it is new Cached File Writer, with cache less than 100 bytes!!!";
        char buffer2[] = "Oh, there is something more interesting! ";
        char buffer3[] = "This one fits into cache size!";

        bool done = file.write(buffer1, sizeof(buffer1));
        printf("Operation done: %i\n", done);
        printf("File pos: %li \n", file.tell());
        printf("File size: %li \n", file.size());

        done = file.seek(45);
        printf("Operation done: %i\n", done);
        done = file.write(buffer2, sizeof(buffer2));
        printf("Operation done: %i\n", done);
        printf("File pos: %li \n", file.tell());
        printf("File size: %li \n", file.size());

        done = file.write(buffer3, sizeof(buffer3));
        printf("Operation done: %i\n", done);
        printf("File pos: %li \n", file.tell());
        printf("File size: %li \n", file.size());

        printf("\n\n");
    }

    static void LoggedFileTest()
    {
        PlatformFile logFile("../Test/Files/fileLog.txt", false, true);
        PlatformFile writeFile("../Test/Files/loggedFile.txt", false, true);
        LogManager logManager(logFile, Display);
        LoggedFile file(writeFile, logManager);

        char string1[] = "Hello, world";
        char string2[] = "It is the test for the nex logged file writer/reader";
        char string3[] = "Which supports writing to the log info about operations";

        file.write(string1, sizeof(string1));
        file.write(string2, sizeof(string2));
        file.write(string3, sizeof(string3));

        file.flush();
    }

    static void run()
    {
        //FileReadingTest1();
        //FileReadingTest2();
        //CachedFileTest1();
        LoggedFileTest();
    }

};

#endif //BERSERK_FILESTEST_H
