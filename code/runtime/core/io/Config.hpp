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

#ifndef BERSERK_IO_CONFIIG_HPP
#define BERSERK_IO_CONFIIG_HPP

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/string/String.hpp>
#include <core/templates/RefCnt.hpp>

#include <memory>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class Config
 * @brief Key-values based config file in `plist` style
 */
class Config final {
public:
    BRK_API Config();
    BRK_API ~Config();

    /**
     * @brief Open config file path with specified path
     *
     * Attempts to open config file specified in the filepath variable.
     * Path can be absolute or relative, config will use default file system
     * search paths to find config file on the disk.
     *
     * Accepts config files in `.config.xml` format.
     *
     * @param filepath Path to the config file
     *
     * @return True if successfully opened config file
     */
    BRK_API bool Open(const String &filepath);

    /**
     * @brief Read property int32 from the config
     *
     * @param section Section name where to look for value
     * @param key Key of the value
     * @param defaultValue Default value returned if failed to get value from config
     *
     * @return Value if found, otherwise `defaultValue`
     */
    BRK_API int32 GetProperty(const StringName &section, const StringName &key, int32 defaultValue) const;

    /**
     * @brief Read property uint32 from the config
     *
     * @param section Section name where to look for value
     * @param key Key of the value
     * @param defaultValue Default value returned if failed to get value from config
     *
     * @return Value if found, otherwise `defaultValue`
     */
    BRK_API uint32 GetProperty(const StringName &section, const StringName &key, uint32 defaultValue) const;

    /**
     * @brief Read property float from the config
     *
     * @param section Section name where to look for value
     * @param key Key of the value
     * @param defaultValue Default value returned if failed to get value from config
     *
     * @return Value if found, otherwise `defaultValue`
     */
    BRK_API float GetProperty(const StringName &section, const StringName &key, float defaultValue) const;

    /**
     * @brief Read property string from the config
     *
     * @param section Section name where to look for value
     * @param key Key of the value
     * @param defaultValue Default value returned if failed to get value from config
     *
     * @return Value if found, otherwise `defaultValue`
     */
    BRK_API String GetProperty(const StringName &section, const StringName &key, const String &defaultValue) const;

    /** @return Open filepath of config */
    BRK_API const String &GetFilepath() const;

    /** @return True if file is open */
    BRK_API bool IsOpen() const { return mData != nullptr; }

private:
    std::shared_ptr<class ConfigData> mData;
};

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_IO_CONFIIG_HPP
