//
// Created by Egor Orachyov on 14.08.2018.
//

#ifndef BERSERKENGINE_GLTIMER_H
#define BERSERKENGINE_GLTIMER_H

#include "Misc/Timer.h"
#include "Essential/GLInclude.h"

namespace Berserk
{

    class GLTimer : public Timer
    {
    public:

        virtual ~GLTimer() = default;

        void init()
        {
            mStart = 0;
            mEnd = 0;
        }

        void start()
        {
            mStart = glfwGetTime();
            mEnd = mStart;
        }

        void stop()
        {
            mEnd = glfwGetTime();
        }

        void reset()
        {
            mStart = 0;
            mEnd = 0;
        }

        UINT64 getMilliseconds()
        {
            return (UINT32)(glfwGetTime() - mStart) * 1000;
        }

        FLOAT64 getSeconds()
        {
            return glfwGetTime() - mStart;
        }

        UINT64 getMillisecondsInInterval()
        {
            return (UINT32)(mEnd - mStart) * 1000;
        }

        FLOAT64 getSecondsInInterval()
        {
            return mEnd - mStart;
        }

    private:

        FLOAT64 mStart;
        FLOAT64 mEnd;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLTIMER_H