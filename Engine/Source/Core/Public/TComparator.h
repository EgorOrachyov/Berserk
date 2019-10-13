//
// Created by Egor Orachyov on 2019-10-08.
//

#ifndef BERSERK_TCOMPARATOR_H
#define BERSERK_TCOMPARATOR_H

namespace Berserk {

    template <typename  T>
    class TEquals {
    public:
        static bool cmp(const T& a, const T& b) {
            return a == b;
        }
    };

    template <typename T>
    class TLess {
    public:
        static bool cmp(const T& a, const T& b) {
            return a < b;
        }
    };

}

#endif //BERSERK_TCOMPARATOR_H