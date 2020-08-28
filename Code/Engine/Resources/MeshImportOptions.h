/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MESHIMPORTOPTIONS_H
#define BERSERK_MESHIMPORTOPTIONS_H

#include <IO/ResourceImportOptions.h>
#include <Resources/MeshFormat.h>
#include <Math/Transformf.h>

namespace Berserk {

    /** Options to tweak mesh importing */
    class MeshImportOptions : public ResourceImportOptions {
    public:
        ~MeshImportOptions() override = default;

        /** Set mesh attributes format (which attributes to import) */
        void setFormat(MeshFormat f) { format = f; }

        /** @return Mesh attributes format (which attributes to import) */
        const MeshFormat& getFormat() const { return format; }

        /** Set transform, which should be applied to the mesh */
        void setTransform(const Transformf& t) { transform = t; }

        /** @return Initial transform, which should be applied to the mesh */
        const Transformf& getTransform() const { return transform; }

        /** Set true if should flip UVs to meet RHI requirements */
        void setFlipUVs(bool flip) { flipUVs = flip; };

        /** @return True if flip UVs to meet RHI requirements */
        bool getFlipUVs() const { return flipUVs; }

    private:

        MeshFormat format = { EMeshAttribute::Position };
        Transformf transform;
        bool flipUVs = true;
    };

}

#endif //BERSERK_MESHIMPORTOPTIONS_H