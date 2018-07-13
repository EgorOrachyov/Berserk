//
// Created by Egor Orachyov on 12.07.2018.
//

#ifndef BERSERKENGINE_CONFIGLOADER_H
#define BERSERKENGINE_CONFIGLOADER_H

#include "Strings/CStaticString.h"
#include "ConfigTable.h"

namespace Berserk
{

    /**
     * Loads config list properties (defines all the config properties,
     * which can be mention in engine.cfg file)
     *
     * @param fileName Name of config list file
     * @param table Config table to save params
     *
     * @return true if cfg was successfully loaded
     */
    bool loadConfigList(const CHAR *fileName, ConfigTable& table);

    /**
     * Loads config list properties (defines all the config properties,
     * which can be mention in engine.cfg file)
     *
     * @param fileName Name of config list file
     * @param table Config table to save params
     *
     * @return true if cfg was successfully loaded
     */
    bool loadConfigList(const CStaticString& fileName, ConfigTable& table);

    /**
     * Loads config file and saves new gained values in config table
     *
     * @param fileName Name of config file for application
     * @param table Config table with loaded config list
     * @return true if cfg file was successfully loaded
     */
    bool loadConfigFile(const CHAR* fileName, ConfigTable& table);

    /**
     * Loads config file and saves new gained values in config table
     *
     * @param fileName Name of config file for application
     * @param table Config table with loaded config list
     * @return true if cfg file was successfully loaded
     */
    bool loadConfigFile(const CStaticString& fileName, ConfigTable& table);

    /**
     * @brief Internal function
     *
     * Reads value from string via using type: interprets type and saves
     * data in value pointer
     *
     * @param string Source buffer
     * @param type Type of value to read
     * @param value Pointer to save read value
     *
     * @return true if cfg was successfully loaded
     */
    bool readDataViaType(CHAR* string, CHAR* type, void* value, ParamType &paramType);

} // namespace Berserk

#endif //BERSERKENGINE_CONFIGLOADER_H