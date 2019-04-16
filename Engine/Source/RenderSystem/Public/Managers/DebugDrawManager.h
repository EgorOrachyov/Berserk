//
// Created by Egor Orachyov on 13.04.2019.
//

#ifndef BERSERK_DEBUGDRAWMANAGER_H
#define BERSERK_DEBUGDRAWMANAGER_H

#include <Math/MathInclude.h>
#include <Memory/IAllocator.h>
#include <Misc/UsageDescriptors.h>
#include <Containers/ArrayList.h>
#include <mutex>

namespace Berserk::Render
{

    /**
     * Debug draw manager allows to submit debug primitive to the queue from anywhere
     * in the program code in CURRENT frame to be rendered in the NEXT frame.
     * Allows to be used in multi-threaded mode (synchronization used).
     *
     * @note Renders in the perspective projection (except text) of the active camera
     * @note Provided primitives: line, triangle, box, basis, 2D text, text, sphere.
     */
    class ENGINE_API DebugDrawManager
    {
    public:

        GENARATE_NEW_DELETE(DebugDrawManager);

        /** Local typedefs */
        typedef Vec3f Color;
        typedef Vec3f Point;

        /** Default color palette for the debug draw */
        static const Color WHITE;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;

        /**
         * Initial queue buffers size for submitting draw tasks
         * (Consider this number of primitives will have significant impact on performance)
         */
        static const uint32 INITIAL_PRIMITIVES_MAX_COUNT = 100;

        /**
         * Types of the primitives, which can be submitted
         * to the debug draw queue
         */
        enum DrawRequestType
        {
            eDRT_BASIS    = 0,
            eDRT_AABB        ,
            eDRT_LINE        ,
            eDRT_TRIANGLE    ,
            eDRT_SPHERE      ,
            eDRT_TEXT        ,
        };

        /** Stores info about one debug draw primitive (for queue) */
        struct DrawRequest
        {
            DrawRequestType mType;
            bool mDepthTest;
            Color mColor;

            union
            {
                //! Info for basis
                struct
                {
                    Point mPosition;
                };

                //! Info for oriented basis
                struct
                {
                    Mat4x4f mTransformation;
                };

                //! Info for line
                struct
                {
                    Point mLineStart;
                    Point mLineEnd;

                };

                //! Info for triangle
                struct
                {
                    Point mTriangleA;
                    Point mTriangleB;
                    Point mTriangleC;
                };

                //! Axis aligned bounding box
                AABB mAABB;

                //! Sphere info
                Sphere mSphere;

                //! 3D text (fixed size, should be valid for at least 2 frames)
                char* mStringBuffer;
            };
        };

    public:

        /* Requires engine allocator for internal usages */
        explicit DebugDrawManager(IAllocator* allocator);

        /* Free used resources */
        ~DebugDrawManager();

        /** Add AABB to the debug draw queue [for next frame] */
        void submit(const AABB& box, const Color& color, bool depthTest = true);

        /** Add Sphere to the debug draw queue [for next frame] */
        void submit(const Sphere& sphere, const Color& color, bool depthTest = true);

        /** Add aligned basis to the debug draw queue [for next frame] */
        void submit(const Point& position, const Color& color, bool depthTest = true);

        /** Add oriented basis to the debug draw queue [for next frame] */
        void submit(const Mat4x4f& transformation, const Color& color, bool depthTest = true);

        /** Add line to the debug draw queue [for next frame] */
        void submit(const Point& start, const Point& end, const Color& color, bool depthTest = true);

        /** Add triangle to the debug draw queue [for next frame] */
        void submit(const Point& A, const Point& B, const Point& C, const Color& color, bool depthTest = true);

        /** Add text to the debug draw queue [for next frame] */
        void submit(const Point& position, const char* text, const Color& color, bool depthTest = true);

        /** Swaps current render and submit buffers */
        void update();

    protected:

        /** For synchronization */
        std::mutex mMutex;

        /** Data buffer 1 [will be swapped with 2] */
        ArrayList<DrawRequest> mQueue1;

        /** Data buffer 2 [will be swapped with 1] */
        ArrayList<DrawRequest> mQueue2;

        /** List which contains data to be rendered in the current frame */
        ArrayList<DrawRequest>* mCurrentRender;

        /** List which allows to submit data to be rendered in the next frame */
        ArrayList<DrawRequest>* mCurrentSubmit;

    };

} // namespace Berserk::Render

#endif //BERSERK_DEBUGDRAWMANAGER_H