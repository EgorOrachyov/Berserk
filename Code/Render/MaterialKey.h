/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_MATERIALKEY_H
#define BERSERK_MATERIALKEY_H

#include <Crc32.h>
#include <Material.h>
#include <GpuMeshAttribute.h>

namespace Berserk {
    namespace Render {

        /** Key used to map material/mesh properties to compiled shaders */
        class MaterialKey {
        public:

            MaterialKey() = default;

            MaterialKey(const MeshFormat& format, const MaterialFeatures& features, const MaterialFlags& flags, EMaterialType type, EMaterialShading shading) {
                union {
                    struct {
                        uint32 format;
                        uint32 features;
                    };
                    uint64 key = 0;
                } k0;

                k0.format = (uint32) format.getMask();
                k0.features = (uint32) features.getMask();

                key0 = k0.key;

                union {
                    struct {
                        uint32 flags;
                        uint8 type;
                        uint8 shading;
                    };
                    uint64 key = 0;
                } k1;

                k1.flags = (uint32) flags.getMask();
                k1.type = (uint8) type;
                k1.shading = (uint8) shading;

                key1 = k1.key;
            }

            bool operator==(const MaterialKey& other) const {
                return key0 == other.key0 && key1 == other.key1;
            }

            bool operator!=(const MaterialKey& other) const {
                return key0 != other.key0 || key1 != other.key1;
            }

            uint32 hash() const {
                Crc32::Builder builder;
                return builder.hashT(key0).hashT(key1).build();
            }

        private:
            uint64 key0 = 0;
            uint64 key1 = 0;
        };

    }
}

#endif //BERSERK_MATERIALKEY_H