/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <GpuMeshDeclarationBuilder.h>

namespace Berserk {
    namespace Render {

        GpuMeshDeclarationBuilder& GpuMeshDeclarationBuilder::setMeshFormat(const MeshFormat &format) {
            BERSERK_COND_ERROR_RET_VALUE(*this, !mMeshFormatSpecified, "Format already specified");

            CString bufferName = MeshFormatUtil::getFormatAsDeclarationString(format);

            mBuilder.addBuffer(bufferName, EVertexIterating::PerVertex);

            for (uint32 i = 0; i < (uint32)EMeshAttribute::Max; i++) {
                auto attribute = (EMeshAttribute)i;

                if (format.getFlag(attribute)) {
                    auto name = MeshFormatUtil::getAttributeDeclarationNameFromEnum(attribute);
                    auto type = MeshFormatUtil::getAttributeType(attribute);
                    mBuilder.addElement(name, type);
                }
            }

            mMeshFormatSpecified = true;
            mMeshFormat = format;

            return *this;
        }

        TPtrShared<VertexDeclaration> GpuMeshDeclarationBuilder::buildShared() {
            if (mMeshFormatSpecified) {
                return mBuilder.buildShared();
            }

            return nullptr;
        }

    }
}