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
#include <Resources/MeshAttribute.h>

namespace Berserk {

    /** Options to tweak mesh importing */
    class MeshImportOptions : public ResourceImportOptions {
    public:
        ~MeshImportOptions() override = default;

        /** Set mesh attributes format (which attributes to import) */
        void setFormat(MeshFormat f) { format = f; }

        /** @return Mesh attributes format (which attributes to import) */
        MeshFormat getFormat() const { return format; }

    private:
        MeshFormat format;
    };

}

#endif //BERSERK_MESHIMPORTOPTIONS_H