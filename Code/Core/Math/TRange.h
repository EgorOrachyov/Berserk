/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TRANGE_H
#define BERSERK_TRANGE_H

#include <Math/Math.h>

namespace Berserk {

    /**
     * Represents template based range init,init+step,.. with specified step.
     * Note that border is not included in the range.
     *
     * @tparam T Type of the range values
     */
    template <typename T>
    class TRange {
    public:

        TRange(T init, T border) : init(init), border(border) {}
        TRange(T init, T border, T step) : init(init), border(border), step(step) {}

        struct Iterator {
            T current;
            T border;
            T step;

            bool operator!=(const Iterator& other) const {
                return current != other.current;
            }
            T operator*() {
                return current;
            }
            void operator++() {
                if (current < border) {
                    current+=step;
                }
                else {
                    current = border;
                }
            }
        };

        Iterator begin() { return {init,border,step}; }
        Iterator end() { return {border,border,step}; }

        Iterator begin() const { return {init,border,step}; }
        Iterator end() const { return {border,border,step}; }

        T getInit() const { return init; }
        T getBorder() const { return border; }
        T getStep() const { return step; }

    public:
        T init;
        T border;
        T step = 1;
    };

    using Rangef = TRange<float>;
    using Rangei = TRange<int32>;
    using Rangeu = TRange<uint32>;

}

#endif //BERSERK_TRANGE_H