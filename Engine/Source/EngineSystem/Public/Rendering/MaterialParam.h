//
// Created by Egor Orachyov on 2019-09-01.
//

#ifndef BERSERK_MATERIALPARAM_H
#define BERSERK_MATERIALPARAM_H

#include <Misc/Allocatable.h>
#include <Strings/String.h>
#include <Math/MathInclude.h>
#include <Engine/EngineForward.h>
#include <Rendering/Texture2D.h>

namespace Berserk
{

    template <typename T>
    struct TMaterialParam : public Allocatable
    {
        TMaterialParam(String name, T param);

        String name;
        T param;
    };

    template <typename T>
    TMaterialParam<T>::TMaterialParam(Berserk::String name, T param)
         : name(std::move(name)),
           param(std::move(param))
    {

    }

    typedef TMaterialParam<Vec2f> MaterialVec2f;
    typedef TMaterialParam<Vec3f> MaterialVec3f;
    typedef TMaterialParam<Vec4f> MaterialVec4f;

    typedef TMaterialParam<int32> MaterialInt32;
    typedef TMaterialParam<uint32> MaterialUint32;

    typedef TMaterialParam<HTexture2D> MaterialTexture2D;

    typedef TMaterialParam<Mat2x2f> MaterialMat2x2f;
    typedef TMaterialParam<Mat3x3f> MaterialMat3x3f;
    typedef TMaterialParam<Mat4x4f> MaterialMat4z4f;

    /** Stores all the material named var data for rendering */
    struct MaterialData : public Allocatable
    {
        MaterialData(IAllocator& allocator = Allocator::get());

        TArray<MaterialVec2f> vec2fData;
    };

    typedef TSharedPtr<MaterialData> MaterialDataRef;

} // namespace Berserk

#endif //BERSERK_MATERIALPARAM_H
