/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLTIMEQUERY_H
#define BERSERK_GLTIMEQUERY_H

#include <RHI/RHITimeQuery.h>
#include <GLErrors.h>

namespace Berserk {

    enum class EGLQueryState {
        Created,
        Inserted,
        Finished
    };

    // Note: it seems like MacOS 4.1 core driver implementation does not support
    // glQueryCounter function, therefore only glBeginQuery/glEndQuery with GL_TIM_ELAPSED flag available.
    // Reference: https://stackoverflow.com/questions/25633921/osx-gl-timestamp-vs-gl-time-elapsed

    class GLTimeQuery final: public RHITimeQuery {
    public:

        ~GLTimeQuery() override {
            destroy();
        }

        bool create() {
            glGenQueries(1, &mQueryHandle);
            BERSERK_CATCH_OPENGL_ERRORS();

            return true;
        }

        void destroy() {
            if (mQueryHandle) {
                glDeleteQueries(1, &mQueryHandle);
                BERSERK_CATCH_OPENGL_ERRORS();
                mQueryHandle = 0;
            }
        };

        void begin() {
            BERSERK_COND_ERROR_RET(mState == EGLQueryState::Finished || mState == EGLQueryState::Created, "Previous query call is not finished");
            glBeginQuery(GL_TIME_ELAPSED, mQueryHandle);
            BERSERK_CATCH_OPENGL_ERRORS();
            mState = EGLQueryState::Inserted;
            mHasResult = false;
        }

        void end() {
            BERSERK_COND_ERROR_RET(mState == EGLQueryState::Inserted, "Query is not inserted");
            glEndQuery(GL_TIME_ELAPSED);
            BERSERK_CATCH_OPENGL_ERRORS();
            mState = EGLQueryState::Finished;
        }

        TimeValue tryGetElapsedTimeNanoseconds() const override {
            if (mState == EGLQueryState::Created)
                return mQueryResult;

            if (mHasResult || isResultAvailable()) {
                GLuint64 nanoseconds;

                glGetQueryObjectui64v(mQueryHandle, GL_QUERY_RESULT, &nanoseconds);
                BERSERK_CATCH_OPENGL_ERRORS();

                mQueryResult = TimeValue::asNanoseconds(nanoseconds);
            }

            return mQueryResult;
        }

        bool isResultAvailable() const override {
            if (mState == EGLQueryState::Created)
                return mHasResult;

            BERSERK_COND_ERROR_RET_VALUE(false, mState == EGLQueryState::Finished, "Previous query call is not finished");

            if (!mHasResult) {
                GLint available = GL_FALSE;

                glGetQueryObjectiv(mQueryHandle, GL_QUERY_RESULT_AVAILABLE, &available);
                BERSERK_CATCH_OPENGL_ERRORS();

                mHasResult = (available == GL_TRUE);
            }

            return mHasResult;
        }

    private:

        GLuint mQueryHandle;
        mutable TimeValue mQueryResult = TimeValue::asSeconds(0.0f);
        mutable bool mHasResult = false;
        mutable EGLQueryState mState = EGLQueryState::Created;


    };

}

#endif //BERSERK_GLTIMEQUERY_H
