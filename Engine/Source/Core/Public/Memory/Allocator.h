//
// Created by Egor Orachyov on 2019-10-09.
//

#ifndef BERSERK_ALLOCATOR_H
#define BERSERK_ALLOCATOR_H

#include <Memory/IAllocator.h>

namespace Berserk {

    /** General purpose allocator - Platform memory wrapper */
    class Allocator : public IAllocator {
    public:

        /** @copydoc IAllocator::malloc() */
        void *malloc(uint32 size) override;

        /** @copydoc IAllocator::malloc() */
        void *malloc(uint32 size, uint32 alignment) override;

        /** @copydoc IAllocator::free() */
        void free(void *pointer) override;

        /** @return Engine global allocator instance */
        static Allocator& getSingleton();

    private:

        Allocator() = default;
        ~Allocator() override;

        //todo: add atomic types
        int32 mCallsAllocate = 0;
        int32 mCallsFree = 0;

    };

}

#endif //BERSERK_ALLOCATOR_H