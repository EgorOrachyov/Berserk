/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_TIMER_HPP
#define BERSERK_TIMER_HPP

#include <BerserkCore/Typedefs.hpp>
#include <chrono>

namespace Berserk {

    class Timer {
    public:
        using sClock = std::chrono::steady_clock;
        using tPoint = sClock::time_point;

        using Nanosecons = std::chrono::nanoseconds;
        using Microseconds = std::chrono::microseconds;
        using Milliseconds = std::chrono::milliseconds ;

        tPoint Start() {
            return mEnd = mStart = sClock::now();
        }

        tPoint Stop() {
            return mEnd = sClock::now();
        }

        double GetElapsedMs() const {
            return GetElapsed<Milliseconds>();
        }

        double GetElapsedNs() const {
            return GetElapsed<Nanosecons>();
        }

        template<typename Units>
        double GetElapsed() const {
            return (double) std::chrono::duration_cast<Nanosecons>(mEnd - mStart).count()
                    / (double) Nanosecons::period::den
                    * (double) Units::period::den;
        }

    private:
        tPoint mStart{};
        tPoint mEnd{};
    };

    class ScopedTimer {
    public:
        ScopedTimer() {
            mTimer.Start();
        }

        double GetElapsedMs() {
            mTimer.Stop();
            return mTimer.GetElapsed<Timer::Milliseconds>();
        }

    private:
        Timer mTimer;
    };

}

#endif //BERSERK_TIMER_HPP