//
// Created by Egor Orachyov on 2019-07-23.
//

#ifndef BERSERK_IPIXELDATAIMPORTER_H
#define BERSERK_IPIXELDATAIMPORTER_H

#include <Rendering/PixelData.h>
#include <Pointer/TSharedPtr.h>

namespace Berserk
{

    /** Importer which allows to load or save raw image data in runtime mode */
    class ENGINE_API IPixelDataImporter
    {
    public:

        virtual ~IPixelDataImporter() = default;

        /**
         * Import an image pixel data from fully specified name/path
         * @param filename Full file name
         * @return Loaded data or null, if there is some kind of error
         */
        virtual TSharedPtr<PixelData> importRaw(const char *filename) = 0;

        /***
         * Save an image data to the file
         * @param filename Full file name
         * @param image Image data to save
         * @return True if success
         */
        virtual bool exportRaw(const char *filename, const PixelData &image) = 0;

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

        /**
         * Allows to get all the supported extensions for the
         * reading of the image data
         * @return List with lower-case extensions
         */
        virtual const TArray<String> &getSupportedReadingExtensions() const = 0;

    };

} // namespace Berserk

#endif //BERSERK_IPIXELDATAIMPORTER_H