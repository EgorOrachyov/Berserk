//
// Created by Egor Orachyov on 12.07.2018.
//

#include "Config/ConfigLoader.h"

namespace Berserk
{
    bool readDataViaType(CHAR* string, CHAR* type, void* value, ParamType &paramType)
    {

        if (strcmp(type, PARAM_TYPE_INT16) == 0)
        {
            paramType = PT_INT16;
            sscanf(string, "%s = %i;", type, (INT32*)value);
            //PUSH("default = %i", *(INT32*)value);
        }
        else if (strcmp(type,PARAM_TYPE_INT32 ) == 0)
        {
            paramType = PT_INT32;
            sscanf(string, "%s = %i;", type, (INT32*)value);
            //PUSH("default = %i", *(INT32*)value);

        }
        else if (strcmp(type, PARAM_TYPE_UINT16) == 0)
        {
            paramType = PT_UINT16;
            sscanf(string, "%s = %u;", type, (UINT32*)value);
            //PUSH("default = %u", *(UINT32*)value);
        }
        else if (strcmp(type, PARAM_TYPE_UINT32) == 0)
        {
            paramType = PT_UINT32;
            sscanf(string, "%s = %u;", type, (UINT32*)value);
            //PUSH("default = %u", *(UINT32*)value);
        }
        else if (strcmp(type, PARAM_TYPE_FLOAT32) == 0)
        {
            paramType = PT_FLOAT32;
            sscanf(string, "%s = %f;", type, (FLOAT32*)value);
            //PUSH("default = %f", *(FLOAT32*)value);
        }
        else if (strcmp(type, PARAM_TYPE_FLOAT64) == 0)
        {
            paramType = PT_FLOAT64;
            sscanf(string, "%s = %lf;", type, (FLOAT64*)value);
            //PUSH("default = %lf", *(FLOAT64*)value);
        }
        else if (strcmp(type, PARAM_TYPE_BOOL) == 0)
        {
            paramType = PT_BOOL;
            sscanf(string, "%s = %i;", type, (INT32*)value);
            //PUSH("default = %i", *(INT32*)value);

        }
        else if (strcmp(type, PARAM_TYPE_TEXT) == 0)
        {
            paramType = PT_TEXT;
            CHAR* val = (CHAR*)value;

            UINT32 i = 0;
            while (string[i] != '=') i++;
            i += 3;

            UINT32 k = 0;
            while (string[i] != '"')
            {
                val[k++] = string[i++];
            }
            val[k] = '\0';

            //PUSH("default = %s", val);

        }
        else
        {
            paramType = PT_UNDEFINED;
            WARNING("Unknown cfg identifier %s in cfg file\n", type);
            return false;
        }

        return true;
    }

    bool loadConfigList(const CHAR *fileName, ConfigTable& table)
    {
        if (fileName == NULL)
        {
            WARNING("NULL file name buffer");
            return false;
        }

        FILE* file = fopen(fileName, "r");
        if (file == NULL)
        {
            WARNING("Cannot open cfg file %s", fileName);
            return false;
        }

        CHAR c;
        CHAR string[Buffers::SIZE_1024] = {'\0'};

        CHAR type[Buffers::SIZE_128] = {'\0'};
        CHAR value[Buffers::SIZE_128] = {'\0'};

        CStaticString name;
        void* data = NULL;

        while (!feof(file))
        {
            fgets(string, Buffers::SIZE_1024, file);
            c = string[0];

            if (c == '#' || c == ' ' || c == '\0' || c == '\n')
            {
                //
            }
            else if (c == '[')
            {
                //
            }
            else
            {
                sscanf(string, "%s = ", type);

                if (strcmp(type, TYPE_NAME) == 0)
                {
                    UINT32 i = 0;
                    while (string[i] != '=') i++;
                    i += 2;

                    UINT32 k = 0;
                    while (string[i] != ';')
                    {
                        value[k++] = string[i++];
                    }
                    value[k] = '\0';

                    name.copy(value);
                    data = table.alloc();

                    fgets(string, Buffers::SIZE_1024, file);
                    sscanf(string, "%s = ", type);

                    if (strcmp(type, TYPE_TYPE) == 0)
                    {
                        i = 0;
                        while (string[i] != '=') i++;
                        i += 2;

                        k = 0;
                        while (string[i] != ';') {
                            value[k++] = string[i++];
                        }
                        value[k] = '\0';

                        fgets(string, Buffers::SIZE_1024, file);
                        sscanf(string, "%s = ", type);

                        if (strcmp(type, TYPE_DEFAULT) == 0)
                        {
                            ParamType paramType = PT_UNDEFINED;
                            readDataViaType(string, value, data, paramType);

                            if (paramType == PT_UNDEFINED)
                            {
                                WARNING("Unknown cfg identifier %s in file %s\n", value, fileName);
                                fclose(file);
                                return false;
                            }

                            fgets(string, Buffers::SIZE_1024, file);
                            sscanf(string, "%s = ", type);

                            if (strcmp(type, TYPE_DESCRIPTION) == 0)
                            {
                                i = 0;
                                while (string[i] != '=') i++;
                                i += 2;

                                k = 0;
                                while (string[i] != ';') {
                                    value[k++] = string[i++];
                                }
                                value[k] = '\0';

                                table.add(name, data, paramType);
                            }
                        }
                        else
                        {
                            WARNING("Unknown cfg identifier %s in file %s\n", type, fileName);
                            fclose(file);
                            return false;
                        }
                    }
                    else
                    {
                        WARNING("Unknown cfg identifier %s in file %s\n", type, fileName);
                        fclose(file);
                        return false;
                    }
                }
                else
                {
                    WARNING("Unknown cfg identifier %s in file %s\n", type, fileName);
                    fclose(file);
                    return false;
                }
            }
        }

        fclose(file);
        return true;
    }

    bool loadConfigList(const CStaticString& fileName, ConfigTable& table)
    {
        if (fileName.getChars() == NULL)
        {
            WARNING("NULL file name buffer");
            return false;
        }

        FILE* file = fopen(fileName.getChars(), "r");
        if (file == NULL)
        {
            WARNING("Cannot open cfg file %s", fileName.getChars());
            return false;
        }

        CHAR c;
        CHAR string[Buffers::SIZE_1024] = {'\0'};

        CHAR type[Buffers::SIZE_128] = {'\0'};
        CHAR value[Buffers::SIZE_128] = {'\0'};

        CStaticString name;
        void* data = NULL;

        while (!feof(file))
        {
            fgets(string, Buffers::SIZE_1024, file);
            c = string[0];

            if (c == '#' || c == ' ' || c == '\0' || c == '\n')
            {
                //
            }
            else if (c == '[')
            {
                //
            }
            else
            {
                sscanf(string, "%s = ", type);

                if (strcmp(type, TYPE_NAME) == 0)
                {
                    UINT32 i = 0;
                    while (string[i] != '=') i++;
                    i += 2;

                    UINT32 k = 0;
                    while (string[i] != ';')
                    {
                        value[k++] = string[i++];
                    }
                    value[k] = '\0';

                    name.copy(value);
                    data = table.alloc();

                    fgets(string, Buffers::SIZE_1024, file);
                    sscanf(string, "%s = ", type);

                    if (strcmp(type, TYPE_TYPE) == 0)
                    {
                        i = 0;
                        while (string[i] != '=') i++;
                        i += 2;

                        k = 0;
                        while (string[i] != ';') {
                            value[k++] = string[i++];
                        }
                        value[k] = '\0';

                        fgets(string, Buffers::SIZE_1024, file);
                        sscanf(string, "%s = ", type);

                        if (strcmp(type, TYPE_DEFAULT) == 0)
                        {
                            ParamType paramType = PT_UNDEFINED;
                            readDataViaType(string, value, data, paramType);

                            if (paramType == PT_UNDEFINED)
                            {
                                WARNING("Unknown cfg identifier %s in file %s\n", value, fileName);
                                fclose(file);
                                return false;
                            }

                            fgets(string, Buffers::SIZE_1024, file);
                            sscanf(string, "%s = ", type);

                            if (strcmp(type, TYPE_DESCRIPTION) == 0)
                            {
                                i = 0;
                                while (string[i] != '=') i++;
                                i += 2;

                                k = 0;
                                while (string[i] != ';') {
                                    value[k++] = string[i++];
                                }
                                value[k] = '\0';

                                table.add(name, data, paramType);
                            }
                        }
                        else
                        {
                            WARNING("Unknown cfg identifier %s in file %s\n", type, fileName.getChars());
                            fclose(file);
                            return false;
                        }
                    }
                    else
                    {
                        WARNING("Unknown cfg identifier %s in file %s\n", type, fileName.getChars());
                        fclose(file);
                        return false;
                    }
                }
                else
                {
                    WARNING("Unknown cfg identifier %s in file %s\n", type, fileName.getChars());
                    fclose(file);
                    return false;
                }
            }
        }

        fclose(file);
        return true;
    }

    bool loadConfigFile(const CHAR* fileName, ConfigTable& table)
    {
        if (fileName == NULL)
        {
            WARNING("NULL file name buffer");
            return false;
        }

        FILE* file = fopen(fileName, "r");
        if (file == NULL)
        {
            WARNING("Cannot open cfg file %s", fileName);
            return false;
        }

        CHAR c;
        CHAR string[Buffers::SIZE_1024] = {'\0'};

        CHAR paramName[Buffers::SIZE_128] = {'\0'};

        void* data = NULL;

        while (!feof(file))
        {
            fgets(string, Buffers::SIZE_1024, file);
            c = string[0];

            if (c == '#' || c == ' ' || c == '\0' || c == '\n')
            {
                //
            }
            else if (c == '[')
            {
                //
            }
            else
            {
                sscanf(string, "%s = ", paramName);

                data = table.get(paramName);
                if (data == NULL)
                {
                    WARNING("Wrong param name %s in cfg file %s\n", paramName, fileName);
                    return false;
                }

                ParamType paramType = table.getType(paramName);

                if (paramType == PT_INT16 || paramType == PT_INT32 || paramType == PT_BOOL)
                {
                    sscanf(string, "%s = %i;", paramName, (INT32*)data);
                    //PUSH("default = %i", *(INT32*)value);
                }
                else if (paramType == PT_UINT16 || paramType == PT_UINT32)
                {
                    sscanf(string, "%s = %u;", paramName, (UINT32*)data);
                    //PUSH("default = %i", *(INT32*)value);
                }
                else if (paramType == PT_FLOAT32)
                {
                    sscanf(string, "%s = %f;", paramName, (FLOAT32*)data);
                    //PUSH("default = %i", *(INT32*)value);
                }
                else if (paramType == PT_FLOAT64)
                {
                    sscanf(string, "%s = %lf;", paramName, (FLOAT64*)data);
                    //PUSH("default = %i", *(INT32*)value);
                }
                else if (paramType == PT_TEXT)
                {
                    CHAR* val = (CHAR*)data;

                    UINT32 i = 0;
                    while (string[i] != '=') i++;
                    i += 3;

                    UINT32 k = 0;
                    while (string[i] != '"')
                    {
                        val[k++] = string[i++];
                    }
                    val[k] = '\0';

                    //PUSH("default = %s", val);
                }
                else
                {
                    WARNING("Unknown param type %s %i file %s", paramName, paramType, fileName);
                    fclose(file);
                    return false;
                }
            }
        }

        fclose(file);
        return true;
    }

    bool loadConfigFile(const CStaticString& fileName, ConfigTable& table)
    {
        if (fileName == NULL)
        {
            WARNING("NULL file name buffer");
            return false;
        }

        FILE* file = fopen(fileName.getChars(), "r");
        if (file == NULL)
        {
            WARNING("Cannot open cfg file %s", fileName.getChars());
            return false;
        }

        CHAR c;
        CHAR string[Buffers::SIZE_1024] = {'\0'};

        CHAR paramName[Buffers::SIZE_128] = {'\0'};

        void* data = NULL;

        while (!feof(file))
        {
            fgets(string, Buffers::SIZE_1024, file);
            c = string[0];

            if (c == '#' || c == ' ' || c == '\0' || c == '\n')
            {
                //
            }
            else if (c == '[')
            {
                //
            }
            else
            {
                sscanf(string, "%s = ", paramName);

                data = table.get(paramName);
                if (data == NULL)
                {
                    WARNING("Wrong param name %s in cfg file %s\n", paramName, fileName.getChars());
                    return false;
                }

                ParamType paramType = table.getType(paramName);

                if (paramType == PT_INT16 || paramType == PT_INT32 || paramType == PT_BOOL)
                {
                    sscanf(string, "%s = %i;", paramName, (INT32*)data);
                    //PUSH("default = %i", *(INT32*)value);
                }
                else if (paramType == PT_UINT16 || paramType == PT_UINT32)
                {
                    sscanf(string, "%s = %u;", paramName, (UINT32*)data);
                    //PUSH("default = %i", *(INT32*)value);
                }
                else if (paramType == PT_FLOAT32)
                {
                    sscanf(string, "%s = %f;", paramName, (FLOAT32*)data);
                    //PUSH("default = %i", *(INT32*)value);
                }
                else if (paramType == PT_FLOAT64)
                {
                    sscanf(string, "%s = %lf;", paramName, (FLOAT64*)data);
                    //PUSH("default = %i", *(INT32*)value);
                }
                else if (paramType == PT_TEXT)
                {
                    CHAR* val = (CHAR*)data;

                    UINT32 i = 0;
                    while (string[i] != '=') i++;
                    i += 3;

                    UINT32 k = 0;
                    while (string[i] != '"')
                    {
                        val[k++] = string[i++];
                    }
                    val[k] = '\0';

                    //PUSH("default = %s", val);
                }
                else
                {
                    WARNING("Unknown param type %s %i file %s", paramName, paramType, fileName.getChars());
                    fclose(file);
                    return false;
                }
            }
        }

        fclose(file);
        return true;
    }

} // namespace Berserk