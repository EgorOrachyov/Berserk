//
// Created by Egor Orachyov on 2019-10-08.
//

#ifndef BERSERK_TPREDICATE_H
#define BERSERK_TPREDICATE_H

namespace Berserk {

    class TPredicate {
    public:

        template <typename T>
        struct Equals {
            typedef bool (*type)(const T& a, const T& b);
        };

        template <typename T>
        struct Satisfy {
            typedef bool (*type)(const T& a);
        };

        template <typename T>
        struct Compare {
            typedef bool (*type)(const T& a, const T& b);
        };


    };

}

#endif //BERSERK_TPREDICATE_H
