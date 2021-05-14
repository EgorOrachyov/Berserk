/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#include <BerserkRHI/RHICmdList.hpp>

namespace Berserk {
    namespace RHI {

        CmdList::CmdList() {
            auto& manager = Driver::Impl::Instance().GetCmdListManager();
            manager.AllocateCmdBuffer(mCommandBuffer);
        }

        CmdList::~CmdList() {
            auto& manager = Driver::Impl::Instance().GetCmdListManager();
            manager.ReleaseCmdBuffer(mCommandBuffer);
        }

        void CmdList::Commit() {
            auto& manager = Driver::Impl::Instance().GetCmdListManager();
            manager.SubmitAndAllocateCmdBuffer(mCommandBuffer, mCommandBuffer);
        }

    }
}