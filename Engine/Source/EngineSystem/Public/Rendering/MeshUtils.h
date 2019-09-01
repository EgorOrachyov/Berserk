//
// Created by Egor Orachyov on 2019-08-15.
//

#ifndef BERSERK_MESHUTILS_H
#define BERSERK_MESHUTILS_H

#include <Rendering/Mesh.h>
#include <Engine/EngineResources.h>

namespace Berserk
{

    class ENGINE_API MeshUtils
    {
    public:

        static HMesh createMeshHandle(const TSharedPtr<Mesh> &mesh, IAllocator& allocator = Allocator::get())
        {
            auto data = allocator.engine_new_const<ResourceHandleData>((TSharedPtr<Resource>) mesh);
            TSharedPtr<ResourceHandleData> resData(data, &allocator);
            return HMesh(resData);
        }

    };

} // namespace Berserk

#endif //BERSERK_MESHUTILS_H
