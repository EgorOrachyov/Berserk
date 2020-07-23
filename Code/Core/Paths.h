/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_PATHS_H
#define BERSERK_PATHS_H

#include <String/CString.h>

namespace Berserk {

    /** Types of path for relative look */
    enum class EPathType {
        /** Path relative to the root directory */
        Root,
        /** Path relative to the engine directory */
        Engine,
        /** Path relative to the editor directory */
        Editor,
        /** Not specified relation */
        Unknown
    };

    /**
     * @brief Engine paths
     *
     * General navigator to the engine folders and local data.
     * This path are defined by executable location, which depends on
     * your target machine and platform. But relative structure always the same.
     *
     * If you want to specify single path among all platforms, store your data
     * in the engine folder and navigate it via this class and relative path.
     */
    class Paths {
    public:

        /** @return The root directory path of the all engine relative data */
        static CString getRootDirectory();

        /** @return Path to the engine data only */
        static CString getEngineDirectory();

        /** @return Path to the editor data only */
        static CString getEditorDirectory();

        /**
         * Returns full path for relative path of specified type
         * @param relativePath Relative path to some file
         * @param pathType Type of the file (shows relation to)
         * @return Full path (relativePath for Unknown relation)
         */
        static CString getFullPathFor(const CString& relativePath, EPathType pathType);


    };

}

#endif //BERSERK_PATHS_H