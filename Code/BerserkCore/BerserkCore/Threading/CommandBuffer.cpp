/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkCore/Threading/CommandBuffer.hpp>

namespace Berserk {

    CommandBuffer::CommandBuffer(size_t bufferSize) : mCommandsAlloc(bufferSize) {

    }

    CommandBuffer::~CommandBuffer() noexcept {
        // Release properly commands
        Clear();
    }

    void CommandBuffer::Clear() {
        // Properly destruct buffer commands
        for (auto cmd: mCommands) {
            cmd->~Command();
        }

        // Release cmd elements
        mCommands.Clear();
        // Reset linear alloc to reuse its memory
        mCommandsAlloc.Reset();
    }

    void CommandBuffer::OnReleased() const {
        this->~CommandBuffer();
        Platform::Memory::Deallocate((void*) this);
    }

    Ref<CommandBuffer> CommandBuffer::Create(size_t bufferSize) {
        auto memory = Platform::Memory::Allocate(sizeof(CommandBuffer));
        auto commandBuffer = new (memory) CommandBuffer(bufferSize);
        return Ref<CommandBuffer>(commandBuffer, false);
    }

}
