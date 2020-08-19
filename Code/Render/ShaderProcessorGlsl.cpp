/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ShaderProcessorGlsl.h>
#include <Platform/System.h>

namespace Berserk {
    namespace Render {

        ShaderProcessorGlsl::ShaderProcessorGlsl(const TArray<char> &source, const TArrayStatic<CString> &includePaths, EPathType pathsType, const ProcessorInsertionsGlsl& insertionsGlsl) {
            mSource = &source;
            mPathsType = pathsType;
            mIncludePaths = includePaths;
            mInsertions = insertionsGlsl;
        }

        void ShaderProcessorGlsl::process() {
            mPosition = 0;
            mLength = mSource->size();
            mData = mSource->data();

            while (canContinue()) {
                switch (mData[mPosition]) {
                    // Handle comment block
                    case '/': {
                        if (isNext("//")) {
                            offset("//");

                            mBuilder.append("//");
                            while (canContinue() && mData[mPosition] != '\n') {
                                mBuilder.append(mData[mPosition]);
                                mPosition += 1;
                            }

                            if (mData[mPosition] == '\n') {
                                mBuilder.append('\n');
                                mPosition += 1;
                            }
                        }
                        else {
                            mBuilder.append('/');
                            mPosition += 1;
                        }

                        break;
                    }

                    // Try to find include
                    case '#': {
                        if (isNext("#include")) {
                            offset("#include");

                            uint32 spaces = 0;

                            while (canContinue() && mData[mPosition] == ' ') {
                                mPosition += 1;
                                spaces += 1;
                            }

                            const char* include = nullptr;
                            uint32 includeLen = 0;

                            if (canContinue() && spaces > 0) {
                                if (mData[mPosition] == '"') {
                                    include = &mData[mPosition+1];
                                    mPosition += 1;

                                    while (canContinue() && mData[mPosition] != '"') {
                                        mPosition += 1;
                                        includeLen += 1;
                                    }

                                    if (!canContinue()) {
                                        error("Include macro lost right \" or > symbol");
                                        return;
                                    }

                                    if (includeLen == 0) {
                                        error("Empty include string");
                                        return;
                                    }

                                    CString includePath(include, includeLen);

                                    if (!appendInclude(includePath)) {
                                        // Failed insert include
                                        return;
                                    }

                                    mBuilder.append('\n');

                                    while (canContinue() && mData[mPosition] != '\n')
                                        mPosition += 1;

                                    break;
                                }

                                error("Expected include path");
                                return;
                            }

                            error("Invalid include macro");
                            return;
                        }
                        else if (isNext("#version")) {
                            while (canContinue() && mData[mPosition] != '\n') {
                                mBuilder.append(mData[mPosition]);
                                mPosition += 1;
                            }

                            if (mData[mPosition] == '\n') {
                                mBuilder.append(mData[mPosition]);

                                if (mInsertions.definitions != nullptr) {
                                    for (const auto& definition: *mInsertions.definitions) {
                                        // Format: '#define ${definition}\n'
                                        mBuilder.append("#define ");
                                        mBuilder.append(definition);
                                        mBuilder.append('\n');
                                    }
                                }

                                if (mInsertions.sharedCode != nullptr) {
                                    for (const auto& code: *mInsertions.sharedCode) {
                                        mBuilder.append(code);
                                        mBuilder.append('\n'); // Explicitly added, for safety
                                    }
                                }
                            }
                        }
                        else {
                            mBuilder.append('#');
                            mPosition += 1;
                        }

                        break;
                    }

                    default: {
                        mBuilder.append(mData[mPosition]);
                        mPosition += 1;
                    }
                }
            }

            // Successfully parsed if here
            mStatusOk = true;
        }

        bool ShaderProcessorGlsl::isNext(const char *string) {
            auto len = CStringUtility::length(string);
            return mLength - mPosition >= len && CStringUtility::compare(&mData[mPosition], string, len) == 0;
        }

        bool ShaderProcessorGlsl::canContinue() {
            return mPosition < mLength;
        }

        void ShaderProcessorGlsl::offset(const char *string) {
            mPosition += CStringUtility::length(string);
        }

        void ShaderProcessorGlsl::error(const char *what) {
            mErrorMessage = what;
            mStatusOk = false;
        }

        bool ShaderProcessorGlsl::appendInclude(const CString &path) {
            auto& system = System::getSingleton();

            for (auto& prefix: mIncludePaths) {
                auto fullPath = Paths::getFullPathFor(prefix + "/" + path, mPathsType);
                auto file = system.openFile(fullPath, EFileMode::Read);

                if (file.isNotNull() && file->isOpen()) {
                    uint32 size = file->getSize();
                    TArray<char> buffer;
                    buffer.resize(size);

                    auto result = file->read(buffer.data(), size);

                    if (result == EError::OK) {
                        mBuilder.append(buffer.data(), size);
                        return true;
                    }

                    error("Failed to read file content");
                    return false;
                }
            }

            error("Failed to find include file");
            return false;
        }

        const CStringBuilder& ShaderProcessorGlsl::getSourceBuilder() const {
            return mBuilder;
        }

    }
}