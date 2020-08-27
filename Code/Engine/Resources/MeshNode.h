/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MESHNODE_H
#define BERSERK_MESHNODE_H

#include <String/CString.h>
#include <Math/Aabbf.h>

namespace Berserk {

    /** Node represents sub-mesh in the mesh resource */
    class MeshNode {
    public:

        /** Human readable name of this node */
        CString nodeName;

        /** Bounds of this node */
        Aabbf mAabb;

        /** Offset number of indices/vertices in the parent mesh buffer */
        uint32 offset = 0;

        /** Count of the indices/vertices in this node */
        uint32 count = 0;

        /** Index of this node in the mesh */
        uint32 index = 0;

        /** LOD level of this node */
        uint32 lod = 0;

    };

}

#endif //BERSERK_MESHNODE_H
