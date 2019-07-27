//
// Created by Egor Orachyov on 2019-07-28.
//

#ifndef BERSERK_SHADERIMPORTDATA_H
#define BERSERK_SHADERIMPORTDATA_H

#include <Object/Allocatable.h>
#include <Rendering/Definitions.h>
#include <Rendering/ShaderInitializer.h>

namespace Berserk
{

    /** Single shader source code and type */
    class ENGINE_API ShaderData : public Allocatable
    {
    public:

        ShaderData(EShaderType type, uint32 size, IAllocator& allocator = Allocator::get());

        ShaderData(const ShaderData& source) = default;

        ShaderData(ShaderData&& source) = default;

        /** @return Target shader type */
        EShaderType getShaderType() const { return mShaderType; }

        /** @return Target shader source code */
        char* getSourceCode() const { return mSourceCode.getRawBuffer(); }

    protected:

        EShaderType mShaderType;
        TArray<char> mSourceCode;

    };


    /** Data, loaded by shader importer and needed to create single shader program */
    class ENGINE_API ShaderImportData : public Allocatable
    {
    public:

        explicit ShaderImportData(IAllocator& allocator = Allocator::get());

        /** Add info about uniform variable in the shader program */
        void addUniformVar(const String &name);

        /** Add info about uniform block in the shader program */
        void addUniformBlock(const String& name, uint32 bindingPoint);

        /** Add info about subroutine function in the shader program */
        void addSubroutine(const String& name, EShaderType shaderType);

        /** Add info about single shader */
        void addShaderData(ShaderData& data);

        /** @return Shader initializer for RHI shader program  */
        const ShaderInitializer& getShaderInitializer() const { return mShaderInitializer; }

        /** @return Shaders sources' code */
        const TArray<ShaderData> &getShadersData() const { return mShadersData; }

    private:

        ShaderInitializer mShaderInitializer;
        TArray<ShaderData> mShadersData;

    };

} // namespace Berserk

#endif //BERSERK_SHADERIMPORTDATA_H