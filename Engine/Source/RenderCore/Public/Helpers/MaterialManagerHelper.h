//
// Created by Egor Orachyov on 28.03.2019.
//

#ifndef BERSERK_MATERIALMANAGERHELPER_H
#define BERSERK_MATERIALMANAGERHELPER_H

#include "Strings/String.h"
#include "Base/XMLDocument.h"
#include "Base/IMaterial.h"

namespace Berserk::Resources
{

    class ENGINE_API MaterialManagerHelper
    {
    public:

        /**
         * Initialize 1 allocated material object from XML file
         *
         * @param material[out] Allocated engine material resource which will be
         *                      initialized by the importer and XML node
         * @param node[in]      XML material node, which describes only 1 material
         * @param path[in]      Path to the source folder with textures
         * @return              True if loading is successfully done
         */
        static bool import(IMaterial *material, XMLNode& node, const CString& path);

        /** @return Material type mask from string representation */
        static uint32 getMaterialType(const char* source);

        /** @return Vec4 RGBA component color from string representation of components */
        static Vec4f  getColorRGBA(const char *r, const char *g, const char *b, const char *a);

    };

} // namespace Berserk::Resources




#endif //BERSERK_MATERIALMANAGERHELPER_H
