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

#ifndef BERSERK_RESOURCE_HPP
#define BERSERK_RESOURCE_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/UUID.hpp>
#include <core/string/StringName.hpp>
#include <core/templates/RefCnt.hpp>

BRK_NS_BEGIN

/**
 * @addtogroup resource
 * @{
 */

/**
 * @class Resource
 * @brief Base class for any engine resource object
 */
class Resource : public RefCnt {
public:
    BRK_API ~Resource() override = default;

    BRK_API void SetName(StringName name);
    BRK_API void SetPath(String path);
    BRK_API void SetUUID(UUID uuid);

    BRK_API const StringName &GetName() const { return mName; };
    BRK_API const String &GetPath() const { return mPath; };
    BRK_API const UUID &GetUUID() const { return mUUID; };

private:
    StringName mName; /** Resource name */
    String mPath;     /** Load path */
    UUID mUUID;       /** Unique id */
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_RESOURCE_HPP
