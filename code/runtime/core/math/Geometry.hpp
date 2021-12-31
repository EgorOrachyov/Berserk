/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_GEOMETRY_HPP
#define BERSERK_GEOMETRY_HPP

#include <core/math/MathUtils.hpp>
#include <core/math/TVecN.hpp>

#include <array>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class Geometry
 * @brief Util class to manipulate 3d geometry
 */
class Geometry {
public:
    /**
     * @brief Generates tangent vectors from provided vertex data
     *
     * @note cross(tangent,bitangent) = normal
     * @note cross(normal,tangent) = bitangent
     *
     * @param positions Counterclockwise vertices positions
     * @param normals Counterclockwise vertices positions
     * @param texCoords Counterclockwise vertices texture coords
     * @param tangents Output tangents vector
     * @param bitangents Output bitangents vector
     */
    BRK_API static void GenTangentSpace(const std::array<Vec3f, 3> &positions, const std::array<Vec3f, 3> &normals, const std::array<Vec2f, 3> &texCoords, std::array<Vec3f, 3> &tangents, std::array<Vec3f, 3> &bitangents);
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_GEOMETRY_HPP
