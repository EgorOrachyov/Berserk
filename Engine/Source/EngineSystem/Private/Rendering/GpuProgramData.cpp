//
// Created by Egor Orachyov on 2019-08-27.
//

#include <Rendering/GpuProgramData.h>

namespace Berserk
{

    GpuProgramData::GpuProgramData(Berserk::IAllocator &allocator)
        : variables(allocator),
          textures(allocator),
          subroutines(allocator),
          blocks(allocator)
    {

    }

} // namespace Berserk