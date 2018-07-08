//
// Created by Egor Orachyov on 06.07.2018.
//

#ifndef BERSERKENGINE_GLELEMENTBUFFEROBJECT_H
#define BERSERKENGINE_GLELEMENTBUFFEROBJECT_H

#include "Essential/GLInclude.h"
#include "Essential/GLDataType.h"
#include "Essential/GLPrimitiveMode.h"

#include "../../Core/Essential/Types.h"

namespace Berserk
{

    class GLElementBufferObject
    {
    public:

        GLElementBufferObject();
        ~GLElementBufferObject();

        void create(uint32 count, uint16* indices, GLPrimitiveMode mode);
        void create(uint32 count, uint32* indices, GLPrimitiveMode mode);

        void destroy();

        uint32 getHandle() const;

        bool isCreated() const;

    protected:

        friend class GLVertexArrayObject;

        uint32 mHandle;
        uint32 mCount;
        GLParamType mType;
        GLPrimitiveMode mMode;

    };

} // namespace Berserk

#endif //BERSERKENGINE_GLELEMENTBUFFEROBJECT_H