/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_RHIINDEXBUFFER_H
#define BERSERK_RHIINDEXBUFFER_H

#include <BerserkCore/RHI/RHIDefinitions.hpp>
#include <BerserkCore/RHI/RHIResource.hpp>

namespace Berserk {

    class RHIIndexBuffer: public RHIResource {
    public:
        ~RHIIndexBuffer() override = default;
        uint32 GetIndexCount() const { return mIndexCount; }
        uint32 GetSizeBytes() const { return mIndexCount * RHI::GetIndexSize(mIndexType); }
        EIndexType GetIndexType() const { return mIndexType; }
        EBufferUsage GetBufferUsage() const { return mBufferUsage; }

    protected:
        uint32 mIndexCount;
        EIndexType mIndexType;
        EBufferUsage mBufferUsage;
    };
}




#endif //BERSERK_RHIINDEXBUFFER_H
