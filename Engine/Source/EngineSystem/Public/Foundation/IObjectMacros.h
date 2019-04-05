//
// Created by Egor Orachyov on 01.04.2019.
//

#ifndef BERSERK_IOBJECTMACROS_H
#define BERSERK_IOBJECTMACROS_H

#include <Foundation/Forward.h>
#include <Strings/StringUtility.h>

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

#ifndef GENERATE_FACTORY_BODY
    #define GENERATE_FACTORY_BODY(PRODUCT)                                                                      \
        public:                                                                                                 \
            const char* getTypeString() override { return PRODUCT::getType(); }                                 \
            bool checkFromString(const char* type) override                                                     \
            { return Strings<char,'\0'>::strcmp(type, getTypeString()) == 0; }
#endif // GENERATE_FACTORY_BODY

#ifndef DECLARE_PROPERTY
    #define DECLARE_PROPERTY(NAME, TYPE, INIT)                                                                  \
    protected:                                                                                                  \
        TYPE m ## NAME = INIT;                                                                                  \
    public:                                                                                                     \
        TYPE get ## NAME () { return m ## NAME; }                                                               \
        void set ## NAME (TYPE value) { m ## NAME = value; }
#endif // DECLARE_PROPERTY

#ifndef DECLARE_PROPERTY_READ_REF
#define DECLARE_PROPERTY_READ(NAME, TYPE)                                                                       \
    protected:                                                                                                  \
        TYPE m ## NAME;                                                                                         \
    public:                                                                                                     \
        const TYPE& get ## NAME () { return m ## NAME; }
#endif // DECLARE_PROPERTY_READ

#endif //BERSERK_IOBJECTMACROS_H
