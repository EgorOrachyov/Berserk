//
// Created by Egor Orachyov on 2019-08-07.
//

#ifndef BERSERK_ENGINERESOURCES_H
#define BERSERK_ENGINERESOURCES_H

#include <Rendering/Font.h>
#include <Rendering/Mesh.h>
#include <Rendering/Image.h>
#include <Rendering/Material.h>

#include <Resource/ResourceHandle.h>

namespace Berserk
{

    typedef ResourceHandle<Font> FortHandle;
    typedef ResourceHandle<Mesh> MeshHandle;
    typedef ResourceHandle<Image> ImageHandle;
    typedef ResourceHandle<Material> MaterialHandle;

} // namespace Berserk

#endif //BERSERK_ENGINERESOURCES_H
