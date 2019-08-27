//
// Created by Egor Orachyov on 2019-08-27.
//

#include "Rendering/GpuProgram.h"

namespace Berserk
{

    GpuProgram::GpuProgram(String name, GpuProgramDataRef data, RHIShaderProgramRef program)
                           : mProgramName(std::move(name)),
                             mProgramData(std::move(data)),
                             mProgramRHI(std::move(program))

    {

    }

} // namespace Berserk