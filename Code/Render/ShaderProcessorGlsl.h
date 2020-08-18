/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SHADERPROCESSORGLSL_H
#define BERSERK_SHADERPROCESSORGLSL_H

#include <Paths.h>
#include <String/CStringBuilder.h>
#include <Containers/TArrayStatic.h>

namespace Berserk {
    namespace Render {

        /** This data is inserted into shader source code (after version macro) */
        struct ShaderInsertionsGlsl {
            const TArray<CString>* definitions = nullptr;
            const TArray<CString>* sharedCode = nullptr;
        };

        /**
         * @brief Glsl processor
         *
         * Processes GLSL source code, allows to auto include
         * source for include macro, appends definitions, and common code
         * blocks to the source code.
         * Accepts include search paths, path types, builder
         * and source code and processes include.
         *
         * @note No recursive include substitution (i.e 1 depth of the recursion)
         */
        class ShaderProcessorGlsl {
        public:

            /**
             * Shader processor constructor
             * @param source Source code of the shader to process includes (capture reference)
             * @param includePaths Paths to the folders to look for include files
             * @param pathsType Type of the includes paths (i.e relation to the engine folders structure)
             * @param insertionsGlsl Data, appended into shader source code explicitly
             */
            ShaderProcessorGlsl(const TArray<char>& source, const TArrayStatic<CString> & includePaths, EPathType pathsType,  const ShaderInsertionsGlsl& insertionsGlsl = ShaderInsertionsGlsl());
            ~ShaderProcessorGlsl() = default;

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
            ShaderInsertionsGlsl mInsertions;
            EPathType mPathsType;
            TArrayStatic<CString> mIncludePaths;
            const TArray<char>* mSource;
            uint32 mPosition = 0;
            uint32 mLength = 0;
            const char* mData = nullptr;
            bool mStatusOk = false;
            CString mErrorMessage;

        };

    }
}

#endif //BERSERK_SHADERPROCESSORGLSL_H