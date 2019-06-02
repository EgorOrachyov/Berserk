//
// Created by Egor Orachyov on 24.02.2019.
//

#ifndef BERSERK_CONSTRUCT_H
#define BERSERK_CONSTRUCT_H

#ifndef GENERATE_NEW_DELETE
    #define GENERATE_NEW_DELETE(name)                                   \
        public:                                                         \
            void* operator new  (size_t size) = delete;                 \
            void* operator new[](size_t size) = delete;                 \
            void* operator new  (size_t size, void* memory)             \
            {                                                           \
                return memory;                                          \
            }                                                           \
                                                                        \
            void operator delete (void* memory)                         \
            {                                                           \
                                                                        \
            }                                                           \
                                                                        \
            void operator delete[] (void* memory) = delete;             \

#endif // GEN_NEW_DELETE

#endif //BERSERK_CONSTRUCT_H
