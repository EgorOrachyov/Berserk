//
// Created by Egor Orachyov on 2019-07-23.
//

#ifndef BERSERK_IIMAGEIMPORTER_H
#define BERSERK_IIMAGEIMPORTER_H

#include <Rendering/ImageData.h>
#include <Resource/TSharedPtr.h>

namespace Berserk
{

    /** Importer which allows to load or save image data in runtime mode */
    class ENGINE_API IImageImporter
    {
    public:

        virtual ~IImageImporter() = default;

        /**
         * Import an image from fully specified name/path
         * @param filename Full file name
         * @return Loaded data or null, if there is some kind of error
         */
        virtual TSharedPtr<ImageData> load(const char* filename) = 0;

        /***
         * Save an image data to the file
         * @param filename Full file name
         * @param image Image data to save
         * @return True if success
         */
        virtual bool save(const char* filename, const ImageData& image) = 0;

        /**
         * Checks whether can read from file with extension
         * @param filename Name of file to check
         * @return True if can read
         */
        virtual bool isReadingSupported(const char* filename) = 0;

        /**
         * Checks whether can write to file with chosen extension
         * @param filename Name of file to check
         * @return True if can write
         */
        virtual bool isWritingSupported(const char* filename) = 0;

    };

} // namespace Berserk

#endif //BERSERK_IIMAGEIMPORTER_H