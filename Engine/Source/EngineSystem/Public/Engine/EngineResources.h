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

    typedef ResourceHandle<Font> FortRef;
    typedef ResourceHandle<Mesh> MeshRef;
    typedef ResourceHandle<Image> ImageRef;
    typedef ResourceHandle<Material> MaterialRef;

} // namespace Berserk

#endif //BERSERK_ENGINERESOURCES_H
