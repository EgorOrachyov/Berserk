//
// Created by Egor Orachyov on 28.03.2019.
//

#include "Helpers/ProfileHelpers.h"
#include "Helpers/MaterialManagerHelper.h"

namespace Berserk::Resources
{

    bool MaterialManagerHelper::import(Material *material, XMLNode &node, const CString &path, ITextureManager* manager)
    {
        for (auto block = node.getChild(); !block.isEmpty(); block = block.getNext())
        {
            if (CName("technique") == block.getName())
            {
                material->setMaterialType(getMaterialType(block.getAttribute("mask").getValue()));
            }
            else if (CName("colors") == block.getName())
            {
                for (auto current = block.getChild(); !current.isEmpty(); current = current.getNext())
                {
                    if (CName("default") == current.getName())
                    {
                        material->mDefaultColor = getColorRGBA(current.getAttribute("r").getValue(),
                                                               current.getAttribute("g").getValue(),
                                                               current.getAttribute("b").getValue(),
                                                               current.getAttribute("a").getValue());
#if PROFILE_MATERIAL_MANAGER_HELPER
                        PUSH("MaterialManagerHelper: default color ['%s']", material->mDefaultColor.toString().get());
#endif
                    }
                    else if (CName("emissive") == current.getName())
                    {
                        material->mEmissiveColor = getColorRGBA(current.getAttribute("r").getValue(),
                                                                current.getAttribute("g").getValue(),
                                                                current.getAttribute("b").getValue(),
                                                                current.getAttribute("a").getValue());
#if PROFILE_MATERIAL_MANAGER_HELPER
                        PUSH("MaterialManagerHelper: emissive color ['%s']", material->mEmissiveColor.toString().get());
#endif
                    }
                    else if (CName("wireframe") == current.getName())
                    {
                        material->mWireFrameColor = getColorRGBA(current.getAttribute("r").getValue(),
                                                                 current.getAttribute("g").getValue(),
                                                                 current.getAttribute("b").getValue(),
                                                                 current.getAttribute("a").getValue());
#if PROFILE_MATERIAL_MANAGER_HELPER
                        PUSH("MaterialManagerHelper: wireframe color ['%s']", material->mWireFrameColor.toString().get());
#endif
                    }
                    else
                    {
                        WARNING("Unknown node in XML node parsing for material [name: '%s'][node: '%s']",
                                material->getName(), current.getName());
                    }
                }
            }
            else if (CName("textures") == block.getName())
            {
                for (auto current = block.getChild(); !current.isEmpty(); current = current.getNext())
                {
                    if (CName("texture") == current.getName())
                    {
                        uint32 index;
                        IMaterial::MaterialLayer layer;
                        getLayerTypeIndex(current.getAttribute("type").getValue(), layer, index);

                        if (index == UNSUPPORTED_INDEX)
                        {
                            WARNING("Unknown texture type in XML node parsing for material [name: '%s'][node: '%s']",
                                    material->getName(), material->getName());

                            continue;
                        }

                        ITexture* texture = manager->loadTextureFromXML(nullptr, current);

                        if (texture)
                        {
                            material->mTextures[index] = texture;
                            material->mMaterialLayers |= layer;
                        }
                        else
                        {
                            WARNING("Failed to load texture for material [name: '%s'][node: '%s']",
                                    material->getName(), current.getValue());

                            continue;
                        }
#if PROFILE_MATERIAL_MANAGER_HELPER
                        PUSH("MaterialManagerHelper: texture ['%s']", material->mTextures[index]->getName());
#endif
                    }
                    else
                    {
                        WARNING("Unknown node in XML node parsing for material [name: '%s'][node: '%s']",
                                material->getName(), current.getName());
                    }
                }
            }
            else
            {
                WARNING("Unknown node in XML node parsing for material [name: '%s'][node: '%s']",
                        material->getName(), block.getName());
            }
        }

        return true;
    }

    uint32 MaterialManagerHelper::getMaterialType(const char *source)
    {
        uint32 result = 0;
        uint32 i = 0;

        while (source[i] != '\0')
        {
            result = result << 1;
            result += (source[i] - '0');
            i += 1;
        }

        return result;
    }

    Vec4f MaterialManagerHelper::getColorRGBA(const char *r, const char *g, const char *b, const char *a)
    {
        Vec4f result;

        if (r) result.x = (float32) atof(r);
        if (g) result.y = (float32) atof(g);
        if (b) result.z = (float32) atof(b);
        if (a) result.w = (float32) atof(a);

        return result;
    }

    void MaterialManagerHelper::getLayerTypeIndex(const char *source, IMaterial::MaterialLayer &layer, uint32 &index)
    {
        if (CName("ALBEDO") == source)
        {
            index = 0;
            layer = IMaterial::MaterialLayer::eML_ALBEDO_MAP;
        }
        else if (CName("NORMAL") == source)
        {
            index = 1;
            layer = IMaterial::MaterialLayer::eML_NORMAL_MAP;
        }
        else if (CName("METALLIC") == source)
        {
            index = 2;
            layer = IMaterial::MaterialLayer::eML_METALLIC_MAP;
        }
        else if (CName("ROUGHNESS") == source)
        {
            index = 3;
            layer = IMaterial::MaterialLayer::eML_ROUGHNESS_MAP;
        }
        else if (CName("AMBIENT") == source)
        {
            index = 4;
            layer = IMaterial::MaterialLayer::eML_AMBIENT_MAP;
        }
        else if (CName("DISPLACEMENT") == source)
        {
            index = 5;
            layer = IMaterial::MaterialLayer::eML_DISPLACEMENT_MAP;
        }
        else
        {
            index = UNSUPPORTED_INDEX;
            layer = IMaterial::MaterialLayer::eML_NOT_MATERIAL_LAYER;

            WARNING("Unknown material layer type [type: '%s']", source);
        }
    }

} // namespace Berserk::Resources