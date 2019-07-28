//
// Created by Egor Orachyov on 2019-07-28.
//

#include <Rendering/ShaderImportData.h>

namespace Berserk
{

    ShaderData::ShaderData(Berserk::EShaderType type, Berserk::uint32 size, Berserk::IAllocator &allocator)
            : mShaderType(type),
              mSourceCode(size, allocator)
    {

    }

    ShaderImportData::ShaderImportData(Berserk::IAllocator &allocator)
            : mShaderInitializer(allocator),
              mShadersData(allocator)
    {

    }

    void ShaderImportData::setShaderName(const Berserk::String &name)
    {
        mShaderName = name;
    }

    void ShaderImportData::addUniformVar(const Berserk::String &name)
    {
        mShaderInitializer.uniformVarNames.emplace(name);
    }

    void ShaderImportData::addUniformBlock(const Berserk::String &name, Berserk::uint32 bindingPoint)
    {
        mShaderInitializer.uniformBlocksInfo.emplace(name, bindingPoint);
    }

    void ShaderImportData::addSubroutine(const Berserk::String &name, Berserk::EShaderType shaderType)
    {
        mShaderInitializer.subroutinesInfo.emplace(name, shaderType);
    }

    void ShaderImportData::addShaderData(Berserk::ShaderData &data)
    {
        mShadersData.emplace(std::move(data));
    }

} // namespace Berserk