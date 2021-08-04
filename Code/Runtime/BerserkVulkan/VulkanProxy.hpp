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

#ifndef BERSERK_VULKANPROXY_HPP
#define BERSERK_VULKANPROXY_HPP

#include <BerserkVulkan/VulkanDriver.hpp>
#include <BerserkVulkan/VulkanDeferredResources.hpp>

namespace Berserk {
    namespace RHI {

        /**
         * Proxy resource to wrap vulkan resources for deferred initialization/release on RHI thread.
         *
         * Resource is created immediately on thread of the caller, but the actual initialization
         * (of internal vulkan related state) is deferred, and its done only on RHI thread.
         * When there is no reference to the resource, it is queued to be released on RHI thread.
         *
         * @tparam Base Resource type to wrap
         */
        template <typename Base>
        struct ResourceProxy final: public Base {
        public:
            template<typename ... TArgs>
            explicit ResourceProxy(TArgs&& ... args) : Base(std::forward<TArgs>(args)...) { }

            ~ResourceProxy() override = default;

            void DeferredInit() {
                VulkanDriver::GetDeferredResourceContext().SubmitInit([this](){
                    this->Initialize();
                });
            }

        protected:
            void OnReleased() const override {
                VulkanDriver::GetDeferredResourceContext().SubmitRelease([this](){
                    Memory::Release(this);
                });
            }
        };

    }
}

#endif //BERSERK_VULKANPROXY_HPP
