//
// Created by Egor Orachyov on 05.06.2019.
//

#ifndef BERSERK_CACHEDFILEWRITER_H
#define BERSERK_CACHEDFILEWRITER_H

#include <IO/IFile.h>
#include <Misc/Include.h>
#include <Memory/IAllocator.h>
#include <Memory/Allocator.h>

namespace Berserk
{

    /** Cached file wrapper for an IFile object */
    class CORE_API CachedFileWriter : public IFile
    {
    public:

        /** Default cache size for file writer */
        static const uint32 CACHE_SIZE = 8 * KiB;

    public:

        /**
         * Creates only writeable file with cached operations
         * to reduce OS system calls number. Does not holds itself an
         * internal OS file handler. Works only with specified IFile.
         *
         * @param file File, which will be used to write cached data
         * @param cacheSize Size of cache
         * @param allocator Memory allocator used to allocate buffer for the cache
         */
        explicit CachedFileWriter(IFile& file, uint32 cacheSize = CACHE_SIZE,
                   IAllocator& allocator = Allocator::getSingleton());

        ~CachedFileWriter() override;

        /** @copydoc IFile::read() */
        bool read(void *destination, uint64 bytesToRead) override;

        /** @copydoc IFile::write() */
        bool write(const void *source, uint64 bytesToWrite) override;

        /** @copydoc IFile::seek() */
        bool seek(uint64 position) override;

        /** @copydoc IFile::flush() */
        bool flush() override;

        /** @copydoc IFile::valid() */
        bool valid() override;

        /** @copydoc IFile::tell() */
        int64 tell() override;

        /** @copydoc IFile::size() */
        int64 size() override;

    protected:

        /** Allocates cache, if it was not allocated */
        void allocateCache();

        /** Flushes cache content to the file  */
        bool flushCache();

        /** Writes data to the cache */
        bool writeCache(const void *source, uint64 bytesToWrite);

    protected:

        /** Size of cache buffer */
        uint64 mCacheSize = 0;

        /** Where cache beginning in file*/
        int64 mCacheStart = 0;

        /** Position of writing in cache buffer */
        int64 mCachePosition = 0;

        /** Files size (or max achieved position of writing in file) */
        uint64 mFileSize = 0;

        /** Pointer to cache buffer */
        uint8* mCache = nullptr;

        /** File handler */
        IFile& mFile;

        /** Used allocator for the cahce */
        IAllocator& mAllocator;

    };

} // namespace Berserk

#endif //BERSERK_CACHEDFILEWRITER_H