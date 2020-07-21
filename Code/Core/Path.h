/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_PATH_H
#define BERSERK_PATH_H

#include <Containers/TArray.h>
#include <String/CString.h>
#include <String/CStringBuilder.h>

namespace Berserk {

    class Path {
    public:

        Path() = default;
        ~Path() = default;

        Path(const CString& path) {
            const char* src = path.data();

            bool hasSections = true;

            while (hasSections && *src != '\0') {
                const char* pos = CStringUtility::findFirst(src, "/");

                if (pos != nullptr) {
                    auto len = CStringUtility::offset(src, pos);
                    CString section(src, len);
                    mSections.move(section);

                    src = pos + 1;
                }
                else {
                    CString section = src;
                    mSections.move(section);

                    hasSections = false;
                }
            }
        }

        Path(const char* path) {
            const char* src = path;

            bool hasSections = true;

            while (hasSections && *src != '\0') {
                const char* pos = CStringUtility::findFirst(src, "/");

                if (pos != nullptr) {
                    auto len = CStringUtility::offset(src, pos);

                    if (len > 0) {
                        CString section(src, len);
                        mSections.move(section);
                    }

                    src = pos + 1;
                }
                else {
                    CString section = src;
                    mSections.move(section);

                    hasSections = false;
                }
            }
        }

        Path& append(const char* section) {
            mSections.emplace(section);
            return *this;
        }

        Path& append(const CString& section) {
            mSections.add(section);
            return *this;
        }

        Path& append(const Path& path) {
            mSections.add(path.mSections);
            return *this;
        }

        template <char sep = '/'>
        CString toString() const {
            CStringBuilder builder;

            if (mSections.size() > 0) {
                builder.append(mSections[0]);

                for (uint32 i = 1; i < mSections.size(); i++) {
                    builder.append(sep);
                    builder.append(mSections[i]);
                }
            }

            return builder.toString();
        }

        const CString& last() const {
            BERSERK_COND_ERROR_FAIL(entriesCount() > 0, "No entries in path");
            return mSections[entriesCount() - 1];
        }

        int32 findLastSection(const CString& section) const {
            for (int32 i = (int32)mSections.size() - 1; i >= 0; i--) {
                if (mSections[i] == section)
                    return i;
            }
            return -1;
        }

        bool containsSection(const CString& section) const {
            for (auto& s: mSections) {
                if (s == section)
                    return true;
            }
            return false;
        }

        Path firstSections(uint32 count) const {
            Path result;

            count = Math::min(count, mSections.size());
            result.mSections.ensureToAdd(count);
            result.mSections.add(mSections.data(), count);

            return result;
        }

        uint32 entriesCount() const { return mSections.size(); }
        const TArray<CString> &getSections() const { return mSections; }

        const CString* begin() const { return mSections.begin(); }
        const CString* end() const { return mSections.end(); }

        CString* begin() { return mSections.begin(); }
        CString* end() { return mSections.end(); }

    private:
        friend class PathBuilder;
        TArray<CString> mSections;
    };

    class PathBuilder {
    public:
        PathBuilder(const Path& path) : path(path
        ) {}
        PathBuilder() = default;
        ~PathBuilder() = default;

        PathBuilder& operator/(const char* section) {
            path.append(section);
            return *this;
        }

        PathBuilder& operator/(const CString& section) {
            path.append(section);
            return *this;
        }

        PathBuilder& operator/(const Path& other) {
            path.append(other);
            return *this;
        }

        Path& build() { return path; }
        const Path& build() const { return path; }

    private:
        Path path;
    };

}

#endif //BERSERK_PATH_H