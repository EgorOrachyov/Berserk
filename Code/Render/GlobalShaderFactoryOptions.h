/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_GLOBALSHADERFACTORYOPTIONS_H
#define BERSERK_GLOBALSHADERFACTORYOPTIONS_H

#include <ShaderFactory.h>

namespace Berserk {
    namespace Render {

        class GlobalShaderFactoryOptions : public ShaderFactoryOptions {
        public:
            ~GlobalShaderFactoryOptions() override = default;

            /** Set shader name to import */
            void setShaderName(CString name) { shaderName = std::move(name); }

            /** @return Shader name to import */
            const CString& getShaderName() const { return shaderName; }

        private:
            CString shaderName;
        };

    }
}


#endif //BERSERK_GLOBALSHADERFACTORYOPTIONS_H
