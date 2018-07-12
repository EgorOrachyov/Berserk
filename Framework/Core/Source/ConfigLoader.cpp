//
// Created by Egor Orachyov on 12.07.2018.
//

#include "Config/ConfigLoader.h"
#include "Config/ConfigCommon.h"
#include "Essential/Common.h"
#include "Essential/Assert.h"

namespace Berserk
{
    bool readDataViaType(CHAR* string, CHAR* type, void* value)
    {

        if (strcmp(type, PARAM_TYPE_INT16) == 0)
        {
            sscanf(string, "%s = %i;", type, (INT32*)value);
            //PUSH("default = %i", *(INT32*)value);
        }
        else if (strcmp(type,PARAM_TYPE_INT32 ) == 0)
        {
            sscanf(string, "%s = %i;", type, (INT32*)value);
            //PUSH("default = %i", *(INT32*)value);

        }
        else if (strcmp(type, PARAM_TYPE_UINT16) == 0)
        {
            sscanf(string, "%s = %u;", type, (UINT32*)value);
            //PUSH("default = %u", *(UINT32*)value);
        }
        else if (strcmp(type, PARAM_TYPE_UINT32) == 0)
        {
            sscanf(string, "%s = %u;", type, (UINT32*)value);
            //PUSH("default = %u", *(UINT32*)value);
        }
        else if (strcmp(type, PARAM_TYPE_FLOAT32) == 0)
        {
            sscanf(string, "%s = %f;", type, (FLOAT32*)value);
            //PUSH("default = %f", *(FLOAT32*)value);
        }
        else if (strcmp(type, PARAM_TYPE_FLOAT64) == 0)
        {
            sscanf(string, "%s = %lf;", type, (FLOAT64*)value);
            //PUSH("default = %lf", *(FLOAT64*)value);
        }
        else if (strcmp(type, PARAM_TYPE_BOOL) == 0)
        {
            sscanf(string, "%s = %i;", type, (INT32*)value);
            //PUSH("default = %i", *(INT32*)value);

        }
        else if (strcmp(type, PARAM_TYPE_TEXT) == 0)
        {
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
        CHAR string[BUFFER_SIZE_1024] = {'\0'};

        CHAR type[BUFFER_SIZE_128] = {'\0'};
        CHAR value[BUFFER_SIZE_128] = {'\0'};

        CStaticString name;
        void* data = NULL;

        while (!feof(file))
        {
            fgets(string, BUFFER_SIZE_1024, file);
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

                    fgets(string, BUFFER_SIZE_1024, file);
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

                        fgets(string, BUFFER_SIZE_1024, file);
                        sscanf(string, "%s = ", type);

                        if (strcmp(type, TYPE_DEFAULT) == 0)
                        {
                            if (!readDataViaType(string, value, data))
                            {
                                WARNING("Unknown cfg identifier %s in file %s\n", value, fileName);
                                return false;
                            }

                            fgets(string, BUFFER_SIZE_1024, file);
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

                                table.add(name, data);
                            }
                        }
                        else
                        {
                            WARNING("Unknown cfg identifier %s in file %s\n", type, fileName);
                            return false;
                        }
                    }
                    else
                    {
                        WARNING("Unknown cfg identifier %s in file %s\n", type, fileName);
                        return false;
                    }
                }
                else
                {
                    WARNING("Unknown cfg identifier %s in file %s\n", type, fileName);
                    return false;
                }
            }
        }

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
        CHAR string[BUFFER_SIZE_1024] = {'\0'};

        CHAR type[BUFFER_SIZE_128] = {'\0'};
        CHAR value[BUFFER_SIZE_128] = {'\0'};

        CStaticString name;
        void* data = NULL;

        while (!feof(file))
        {
            fgets(string, BUFFER_SIZE_1024, file);
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

                    fgets(string, BUFFER_SIZE_1024, file);
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

                        fgets(string, BUFFER_SIZE_1024, file);
                        sscanf(string, "%s = ", type);

                        if (strcmp(type, TYPE_DEFAULT) == 0)
                        {
                            if (!readDataViaType(string, value, data))
                            {
                                WARNING("Unknown cfg identifier %s in file %s\n", value, fileName.getChars());
                                return false;
                            }

                            fgets(string, BUFFER_SIZE_1024, file);
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

                                table.add(name, data);
                            }
                        }
                        else
                        {
                            WARNING("Unknown cfg identifier %s in file %s\n", type, fileName.getChars());
                            return false;
                        }
                    }
                    else
                    {
                        WARNING("Unknown cfg identifier %s in file %s\n", type, fileName.getChars());
                        return false;
                    }
                }
                else
                {
                    WARNING("Unknown cfg identifier %s in file %s\n", type, fileName.getChars());
                    return false;
                }
            }
        }

        return true;
    }

} // namespace Berserk