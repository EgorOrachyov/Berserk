//
// Created by Egor Orachyov on 24.02.2019.
//

#ifndef BERSERK_PROPERTY_H
#define BERSERK_PROPERTY_H

#ifndef PROPERTY_READ
    #define PROPERTY_READ(name, type, readtype)                     \
    private: type m ## name;                                        \
    public:  readtype get ## name ()                                \
             {                                                      \
                return m ## name;                                   \
             }
#endif // PROPERTY_READ

#ifndef PROPERTY_READ_WRITE
    #define PROPERTY_READ_WRITE(name, type, readtype, writetype)    \
    private: type m ## name;                                        \
    public:  readtype get ## name ()                                \
             {                                                      \
                return m ## name;                                   \
             }                                                      \
             readtype set ## name (writetype name)                  \
             {                                                      \
                m ## name = name;                                   \
             }
#endif // PROPERTY_READ_WRITE

#endif //BERSERK_PROPERTY_H
