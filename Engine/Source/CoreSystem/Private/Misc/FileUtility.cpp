//
// Created by Egor Orachyov on 26.02.2019.
//

#include "Misc/Assert.h"
#include "Misc/FileUtility.h"
#include <fstream>

namespace Berserk
{

    bool FileUtility::exist(const char *filename)
    {
        std::ifstream infile(filename);
        return infile.good();
    }

    void FileUtility::read(const char *filename, void *buffer)
    {
        if (!filename)
        {
            FAIL(false, "NULL file name string");
        }

        FILE* file = fopen(filename, "r");
        if (!file)
        {
            FAIL(false, "Cannot open file [name: %s]", filename);
        }


        int readChars = 0;
        char* data = (char*)buffer;

        while (!feof(file))
        {
            char c = fgetc(file);
            data[readChars] = c;
            readChars += 1;
        }
        readChars -= 1;
        data[readChars] = '\0';

        fclose(file);
    }

    void FileUtility::read(const char *filename, void *buffer, uint32 size)
    {
        if (size == 1)
        {
            //WARNING("Inappropriate argument value (size == 1) [name: %s]", filename);
            return;
        }

        if (!filename)
        {
            FAIL(false, "NULL file name string");
        }

        FILE* file = fopen(filename, "r");
        if (!file)
        {
            FAIL(false, "Cannot open file [name: %s]", filename);
        }


        int readChars = 0;
        char* data = (char*)buffer;

        while (!feof(file) && readChars < size)
        {
            char c = fgetc(file);
            data[readChars] = c;
            readChars += 1;
        }
        readChars -= 1;
        data[readChars] = '\0';

        fclose(file);
    }

} // namespace Berserk