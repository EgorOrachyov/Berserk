//
// Created by Egor Orachyov on 2019-10-08.
//

#ifndef BERSERK_TPREDICATE_H
#define BERSERK_TPREDICATE_H

#include <functional>

namespace Berserk {

    class TPredicate {
    public:

        template <typename T>
        struct Equals {
            typedef bool (*_type)(const T& a, const T& b);
            typedef std::function<bool(const T& a, const T& b)> type;
        };

        template <typename T>
        struct Satisfy {
            typedef bool (*_type)(const T& a);
            typedef std::function<bool(const T& a)> type;
        };

        template <typename T>
        struct Compare {
            typedef bool (*_type)(const T& a, const T& b);
            typedef std::function<bool(const T& a, const T& b)> type;
        };

        template <typename T>
        struct Consume {
            typedef void (*_type)(const T& a);
            typedef std::function<void(const T& a)> type;
        };

    };

}

#endif //BERSERK_TPREDICATE_H
