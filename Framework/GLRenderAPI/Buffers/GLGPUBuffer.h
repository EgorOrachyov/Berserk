//
// Created by Egor Orachyov on 09.07.2018.
//

#ifndef BERSERKENGINE_GLGPUBUFFER_H
#define BERSERKENGINE_GLGPUBUFFER_H

#include "Misc/Types.h"
#include "Misc/UsageDescriptors.h"

#include "Essential/GLDataType.h"
#include "Essential/GLPrimitiveMode.h"

#include "Buffers/GLDataBufferPacker.h"
#include "Render/GPUBuffer.h"

namespace Berserk
{

    /**
     * GLGPUBuffer allows to create, fill, handle and use GPU RAM buffers for
     * fast and proper pushes of data to the rendering pipeline.
     *
     * Init (VAO) via init() call. Push vertex attributes data (VBO) via attachData() call
     * with packer and create indices buffer (EBO) via attachIndices() call and
     * send draw call to GPU with drawIndices() call;
     */
    class GRAPHICS_API GLGPUBuffer : public GPUBuffer
    {
    public:

        GLGPUBuffer();
        virtual ~GLGPUBuffer();

        /**
         * Additionally used memory by this object
         * @return bytes
         */
        UINT32 getMemoryUsage() override;

        /**
         * Initialize internal vao buffer
         */
        void init() override;

        /**
         * Detach vbo and ebo, and delete all internal buffers
         */
        void destroy() override;

        /**
         * Push data to GPU as buffer
         *
         * @warning Do not recommend to use
         */
        void drawData() const override;

        /**
         * Push data to GPU as indexed buffer
         */
        void drawIndices() const override;

        /**
         * Set count of vertexes and primitive mode to draw
         * @param count Number of vertexes
         * @param mode Primitive mode @see GLPrimitiveMode
         */
        void setDrawingProperties(UINT32 count, GLPrimitiveMode mode);

        /**
         * Set count of vertexes, primitive mode and type of indices to draw
         * @param count Number of vertexes
         * @param mode Primitive mode @see GLPrimitiveMode
         * @param indexType Type of indices in ebo
         */
        void setDrawingProperties(UINT32 count, GLPrimitiveMode mode, GLDataType indexType);

        /**
         * Allows to create vbo buffer via packer and bind all the attributes
         *
         * @warning Allows to call only for one packer
         * @warning Do not work if you have already called this function
         * @warning Data should be packed
         *
         * @param packer
         */
        void attachData(GLDataBufferPacker& packer);

        void attachData(ArrayList<Vertex> &vertices) override;
        void attachData(ArrayList<VertexPN> &vertices) override;
        void attachData(ArrayList<VertexPT> &vertices) override;
        void attachData(ArrayList<VertexPNT> &vertices) override;
        void attachData(ArrayList<VertexPNTBT> &vertices) override;

        void attachData(Vertex* data, UINT32 count) override;
        void attachData(VertexPN* data, UINT32 count) override;
        void attachData(VertexPT* data, UINT32 count) override;
        void attachData(VertexPNT* data, UINT32 count) override;
        void attachData(VertexPNTBT* data, UINT32 count) override;

        void attachIndices(ArrayList<UINT16> &indices) override;
        void attachIndices(ArrayList<UINT32> &indices) override;

        void attachIndices(UINT16* indices, UINT32 count) override;
        void attachIndices(UINT32* indices, UINT32 count) override;

        /**
         * Allows to create ebo buffer via array of indexed vertexes
         *
         * @param count Number of indices in indices array
         * @param indices Pointer to indices array
         * @param mode Primitive drawing mode @see GLPrimitiveMode
         */
        void attachIndices(UINT32 count, UINT16* indices, GLPrimitiveMode mode);

        /**
         * Allows to create ebo buffer via array of indexed vertexes
         *
         * @param count Number of indices in indices array
         * @param indices Pointer to indices array
         * @param mode Primitive drawing mode @see GLPrimitiveMode
         */
        void attachIndices(UINT32 count, UINT32* indices, GLPrimitiveMode mode);

        /**
         * @return Is VAO initialized
         * @see init()
         */
        bool isInitialized() const;

        /**
         * @return Is VBO initialized
         * @see attachData()
         */
        bool isDataAttached() const;

        /**
         * @return Is EBO initialized
         * @see attachIndices()
         */
        bool isIndicesAttached() const;

        /**
         * @return Is VAO, VBO, EBO initialized and filled
         */
        bool validate() const override;

        /**
         * @return Internal VAO handle
         */
        UINT32 getVAO() const;

        /**
         * @return Internal VBO handle
         */
        UINT32 getVBO() const;

        /**
         * @return Internal EBO handle
         */
        UINT32 getEBO() const;

        /**
         * @return Count of vertexes to push gpu
         */
        UINT32 getCount() const;

        /**
         * @return Primitive drawing mode
         */
        GLPrimitiveMode getMode() const;

        /**
         * @return Type of indices in EBO
         */
        GLDataType getIndicesType() const;

    protected:

        UINT32 mVAOHandle;                  // Buffer VAO handle
        UINT32 mVBOHandle;                  // Buffer VBO handle for vertexes' attributes data
        UINT32 mEBOHandle;                  // Buffer EBO handle for indices of vertexes
        UINT32 mCount;                      // Count of indices in EBO buffer
        GLPrimitiveMode mMode;              // Mode of polygons drawing
        GLDataType mIndicesType;            // Type of indices in EBO buffer

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLGPUBUFFER_H