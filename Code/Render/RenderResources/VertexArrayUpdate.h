/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_VERTEXARRAYUPDATE_H
#define BERSERK_VERTEXARRAYUPDATE_H

#include <VertexStreams.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Vertex array update
         *
         * Allows to access vertex arrays elements in the
         * type and byte safe manner and update GPU buffers.
         */
        class VertexArrayUpdate {
        public:

            /** Create stream for specified array */
            VertexArrayUpdate(const TPtrShared<VertexArray> &array);

            /** @return Element stream for element by its name */
            ElementStream getStreamFor(const CString& elementName);

            /** @return Index stream */
            IndexStream getIndexStream();

            /** Updates required GPU buffers in the array */
            void updateDataOnGPU();

        private:
            TPtrShared<VertexArray> mArray;
            TArrayStatic<BinaryData> mBinaryData;
            BinaryData mIndexData;
        };

    }
}

#endif //BERSERK_VERTEXARRAYUPDATE_H