/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_STATICMESHNODE_H
#define BERSERK_STATICMESHNODE_H

#include <String/CString.h>
#include <Math/Aabbf.h>
#include <MaterialInstance.h>
#include <RHI/RHIIndexBuffer.h>

namespace Berserk {
    namespace Render {

        /** Represent node (an geometry view) for single mesh lod level */
        struct StaticMeshNode {
            CString name;
            Aabbf aabbf;
            EIndexType indexType;
            bool isIndexed = false;
            uint32 offset = 0;
            uint32 count = 0;
            uint32 lod = 0;
            TPtrShared<RHIIndexBuffer> indexData;
            TPtrShared<MaterialInstance> material;
        };

    }
}

#endif //BERSERK_STATICMESHNODE_H
