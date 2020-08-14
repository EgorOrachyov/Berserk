/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADERINCLUDEPROCESSOR_H
#define BERSERK_SHADERINCLUDEPROCESSOR_H

#include <Paths.h>
#include <String/CStringBuilder.h>
#include <Containers/TArrayStatic.h>

namespace Berserk {
    namespace Render {

        /**
         * @brief Include processor
         *
         * Process include macro for shader source code.
         * Accepts include search paths, path types, builder
         * and source code and processes include.
         *
         * @note No recursive include substitution (i.e 1 depth of the recursion)
         * @note Currently only GLSL source code supported
         */
        class ShaderIncludeProcessor {
        public:

            /**
             * Shader include processor constructor
             * @param source Source code of the shader to process includes (capture reference)
             * @param includePaths Paths to the folders to look for include files
             * @param pathsType Type of the includes paths (i.e relation to the engine folders structure)
             */
            ShaderIncludeProcessor(const TArray<char>& source, const TArrayStatic<CString> & includePaths, EPathType pathsType);
            ~ShaderIncludeProcessor() = default;

            /** Process includes */
            void process();

            /** @return True if successfully processed include substitution */
            bool isProcessed() const { return mStatusOk; }

            /** @return Error message if something went wrong */
            const CString& getErrorMessage() const { return mErrorMessage; }

            /** @return Source builder reference */
            const CStringBuilder& getSourceBuilder() const;

        private:

            bool isNext(const char* string);
            bool canContinue();
            void offset(const char* string);
            void error(const char* what);
            bool appendInclude(const CString& path);

            /** Final source code builder */
            CStringBuilder mBuilder;
            EPathType mPathsType;
            TArrayStatic<CString> mIncludePaths;
            const TArray<char>* mSource;
            uint32 mPosition;
            uint32 mLength;
            const char* mData;
            bool mStatusOk;
            CString mErrorMessage;

        };

    }
}

#endif //BERSERK_SHADERINCLUDEPROCESSOR_H