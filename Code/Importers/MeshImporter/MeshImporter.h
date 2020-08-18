/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MESHIMPORTER_H
#define BERSERK_MESHIMPORTER_H

#include <IO/ResourceImporter.h>
#include <Math/Vec3f.h>
#include <tiny_obj_loader.h>

namespace Berserk {

    /** Tiny obj based mesh importer (for OBJ format only) */
    class MeshImporter : public ResourceImporter {
    public:
        MeshImporter();
        ~MeshImporter();

        /** For triangulated mesh */
        static const uint32 VERTICES_PER_FACE = 3;
        static const uint32 DIMENSION_3D = 3;
        static const uint32 DIMENSION_2D = 2;

        /** @copydoc ResourceImporter::import() */
        EError import(TPtrShared<Resource> &resource, const CString &importPath, TRef<const ResourceImportOptions> options) override;

        /** @copydoc ResourceImporter::getRecognizedExtensions() */
        const TArray<CString> &getRecognizedExtensions() const override;

        /** @copydoc ResourceImporter::getImporterName() */
        const char *getImporterName() const override;

        /** @copydoc ResourceImporter::getImporterProjectName() */
        const char *getImporterProjectName() override;

        /** @copydoc ResourceImporter::isThreadSafe() */
        bool isThreadSafe() const override;

    private:

        /** Imported resource files extension */
        TArray<CString> mRecognizedExtensions;

        /** Cached tangent vectors buffer */
        TArray<Vec3f> mTangents;

        /** For thread-safe access */
        mutable Mutex mAccessMutex;
    };

}




#endif //BERSERK_MESHIMPORTER_H
