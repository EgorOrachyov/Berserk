//
// Created by Egor Orachyov on 2019-08-27.
//

#ifndef BERSERK_GPUPROGRAM_H
#define BERSERK_GPUPROGRAM_H

#include <RHI/RHIShaderProgram.h>
#include <Rendering/GpuProgramData.h>

namespace Berserk
{

    /**
     * Contains single compiled and linked shader program, structured as vs and fs and etc.
     * with it uniform data description. Contains internally RHI handle for gpu program.
     */
    class ENGINE_API GpuProgram
    {
    public:

        GpuProgram() = default;

        /** All args constructor */
        GpuProgram(String name, GpuProgramDataRef data, RHIShaderProgramRef program);

        /** Set unique name for look-up */
        void setProgramName(const String &name) { mProgramName = name; }

        /** Set program uniform data */
        void setProgramData(const GpuProgramDataRef &data) { mProgramData = data; }

        /** Set RHI resource for the program */
        void setProgramRHI(const RHIShaderProgramRef &program) { mProgramRHI = program; }

        /** @return True whether can bind this program on Gpu */
        bool isReady() const { return mProgramRHI.isPresent(); }

        /** @return Unique name for look-up */
        const String &getProgramName() const { return mProgramName; }

        /** @return Program uniform data */
        const GpuProgramDataRef &getProgramData() const { return mProgramData; }

        /** @return RHI resource handler */
        const RHIShaderProgramRef &getProgramRHI() const { return mProgramRHI; }

    private:

        String mProgramName;
        GpuProgramDataRef mProgramData;
        RHIShaderProgramRef mProgramRHI;

    };

    typedef TSharedPtr<GpuProgram> GpuProgramRef;

} // namespace Berserk

#endif //BERSERK_GPUPROGRAM_H