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
        ShaderData* emlaced = mShadersData.addUninitialized();
        new (emlaced) ShaderData(std::move(data));
    }

    void ShaderImportData::output(const Berserk::ShaderImportData &data, Berserk::IOutputDevice &device)
    {
        device.printf("Program: %s\n", data.mShaderName.get());

        const ShaderInitializer& initializer = data.mShaderInitializer;

        device.printf("Uniform vars: \n");
        for (auto v = initializer.uniformVarNames.begin(); v != nullptr; v = initializer.uniformVarNames.next())
        {
            device.printf("Name:%s\n", v->get());
        }

        device.printf("Uniform blocks: \n");
        for (auto b = initializer.uniformBlocksInfo.begin(); b != nullptr; b = initializer.uniformBlocksInfo.next())
        {
            device.printf("Name:%s Binding:%u\n", b->name.get(), b->bindingPoint);
        }

        device.printf("Subroutines: \n");
        for (auto s = initializer.subroutinesInfo.begin(); s != nullptr; s = initializer.subroutinesInfo.next())
        {
            device.printf("Name:%s Type:%u\n", s->name.get(), s->shaderType);
        }

        device.printf("Shaders:\n");
        for (auto s = data.mShadersData.begin(); s != nullptr; s = data.mShadersData.next())
        {
            device.printf("Type:%u\n%s\n", s->getShaderType(), s->getSourceCode());
        }
    }

} // namespace Berserk