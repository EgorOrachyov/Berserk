//
// Created by Egor Orachyov on 01.04.2019.
//

#ifndef BERSERK_IOBJECTMACRO_H
#define BERSERK_IOBJECTMACRO_H

#ifndef GENERATE_OBJECT_BODY
    #define GENERATE_OBJECT_BODY(OBJECT_CLASS)                                      \
        public:                                                                     \
            void* operator new(size_t size) = delete;                               \
            void* operator new[](size_t size) = delete;                             \
            void* operator new(size_t size, void* memory) { return memory; }        \
            void  operator delete(void* object) { return; }                         \
            const OBJECT_CLASS& operator=(const OBJECT_CLASS& object) = delete;
#endif // GENERATE_OBJECT_BODY

#endif //BERSERK_IOBJECTMACRO_H
