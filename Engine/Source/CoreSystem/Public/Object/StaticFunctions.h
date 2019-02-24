//
// Created by Egor Orachyov on 24.02.2019.
//

#ifndef BERSERK_STATICFUNCTIONS_H
#define BERSERK_STATICFUNCTIONS_H

#ifndef STATIC_CONVERT
    #define STATIC_CONVERT(name, type, result)   \
        static result get ## name (type value);
#endif // STATIC_CONVERT

#endif //BERSERK_STATICFUNCTIONS_H
