/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <UUID.h>

using namespace Berserk;

int main(int argc, char** argv) {

    CString optionH = "-h";
    CString optionN = "-n";

    if (argc == 1) {
        auto id = UUID::generate();
        auto asString = id.toStringStatic();
        printf("Generated id: %s\n", asString.data());
        return 0;
    }

    if (argc == 2 && optionN == argv[1]) {
        auto id = UUID::generateNull();
        auto asString = id.toStringStatic();
        printf("Generated id: %s\n", asString.data());
        return 0;
    }

    printf("Universal unique id generator tool. Pass No arguments to generate id\n"
           "-h print help text\n"
           "-n generate null id\n");

    return 0;
}