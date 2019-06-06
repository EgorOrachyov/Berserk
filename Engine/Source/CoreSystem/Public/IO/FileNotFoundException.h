//
// Created by Egor Orachyov on 06.06.2019.
//

#ifndef BERSERK_FILENOTFOUNDEXCEPTION_H
#define BERSERK_FILENOTFOUNDEXCEPTION_H

#include <exception>
#include <Strings/StaticString.h>

namespace Berserk
{

    class FileNotFoundException : public std::exception
    {
    public:

        explicit FileNotFoundException(const CPath& path)
                : mFilePath(path)
        { }

        const char* what() const noexcept override
        { return "File not found"; }

        const CPath& getFileName()
        { return mFilePath; }

    protected:

        /** File cause of the problem */
        CPath mFilePath;

    };

} // namespace Berserk

#endif //BERSERK_FILENOTFOUNDEXCEPTION_H