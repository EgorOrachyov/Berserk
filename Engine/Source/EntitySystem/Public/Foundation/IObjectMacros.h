//
// Created by Egor Orachyov on 01.04.2019.
//

#ifndef BERSERK_IOBJECTMACROS_H
#define BERSERK_IOBJECTMACROS_H

#include <Foundation/Forward.h>

#ifndef GENERATE_OBJECT_BODY
    #define GENERATE_OBJECT_BODY(OBJECT_CLASS)                                                                  \
        public:                                                                                                 \
            void* operator new(size_t size) = delete;                                                           \
            void* operator new[](size_t size) = delete;                                                         \
            void* operator new(size_t size, void* memory) { return memory; }                                    \
            void  operator delete(void* object) { return; }                                                     \
            const OBJECT_CLASS& operator=(const OBJECT_CLASS& object) = delete;                                 \
            static const char* getType() { static CName typeName( #OBJECT_CLASS ); return typeName.get(); }
#endif // GENERATE_OBJECT_BODY

#ifndef GENERATE_CLASS_BODY
    #define GENERATE_CLASS_BODY(OBJECT_CLASS) GENERATE_OBJECT_BODY(OBJECT_CLASS)
#endif // GENERATE_CLASS_BODY

#ifndef GENERATE_STRUCTURE_BODY
    #define GENERATE_STRUCTURE_BODY(OBJECT_CLASS) GENERATE_OBJECT_BODY(OBJECT_CLASS)
#endif // GENERATE_STRUCTURE_BODY

#endif //BERSERK_IOBJECTMACROS_H
