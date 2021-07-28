/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#include <BerserkVulkan/VulkanProgram.hpp>
#include <BerserkVulkan/VulkanDevice.hpp>
#include <BerserkVulkan/VulkanDebug.hpp>

namespace Berserk {
    namespace RHI {

        VulkanProgram::VulkanProgram(class VulkanDevice &device, const Desc &desc) : mDevice(device) {
            mName = desc.name;
            mLanguage = desc.language;
            mStages = desc.stages;
        }

        VulkanProgram::~VulkanProgram() {
            if (GetCompilationStatus() == Status::Compiled) {
                for (auto& module: mModules) {
                    vkDestroyShaderModule(mDevice.GetDevice(), module, nullptr);
                }
            }
        }

        void VulkanProgram::InitializedFromBinary(const ArrayFixed <RefCounted<ReadOnlyMemoryBuffer>, Limits::MAX_SHADER_STAGES> &modules, RefCounted <ProgramMeta> meta) {
            assert(modules.GetSize() == mStages.GetSize());

            mModules.Resize(mStages.GetSize());

            for (uint64 i = 0; i < mStages.GetSize(); i++) {
                VkShaderModuleCreateInfo createInfo{};
                createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                createInfo.codeSize = modules[i]->GetSize();
                createInfo.pCode = (const uint32*) modules[i]->GetData();

                BERSERK_VK_CHECK(vkCreateShaderModule(mDevice.GetDevice(), &createInfo, nullptr, &mModules[i]));
                BERSERK_VK_NAME(mDevice.GetDevice(), mModules[i], VK_OBJECT_TYPE_SHADER_MODULE, "Shader " + GetShaderName());
            }

            mMeta = std::move(meta);
            mStatus.store((uint32) Status::Compiled);
        }

        void VulkanProgram::NotifyFailedCompile(String message) {
            mCompilerMessage = std::move(message);
            mStatus.store((uint32) Status::FailedCompile);
        }

        Program::Status VulkanProgram::GetCompilationStatus() const {
            return (Program::Status) mStatus.load();
        }

        String VulkanProgram::GetCompilerMessage() const {
            return GetCompilationStatus() != Status::PendingCompilation? mCompilerMessage: String();
        }

        RefCounted<ProgramMeta> VulkanProgram::GetProgramMeta() const {
            return GetCompilationStatus() != Status::PendingCompilation? mMeta: RefCounted<ProgramMeta>();
        }

        void VulkanProgram::OnReleased() const {
            Memory::Release(this);
        }
    }
}