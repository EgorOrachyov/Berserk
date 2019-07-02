//
// Created by Egor Orachyov on 03.07.2019.
//

#ifndef BERSERK_CONSOLETEST_H
#define BERSERK_CONSOLETEST_H

#include <Console/ConsoleManager.h>
#include <Console/ConsoleVariable.h>

using namespace Berserk;

class ConsoleTest
{
public:

    static void ConsoleTest1()
    {
        ConsoleVariable<String> variableString("Param", "Help text about param", String("12.04"), nullptr);
        IConsoleVariable* var = variableString.asVariable();

        OutputDevice::printf("Name: %s \n", var->getName().get());
        OutputDevice::printf("Help: %s \n", var->getHelp().get());

        OutputDevice::printf("Value: %i \n", var->getInt());
        OutputDevice::printf("Value: %f \n", var->getFloat());
        OutputDevice::printf("Value: %s \n", var->getString().get());
    }

    static void run()
    {
        ConsoleTest1();
    }

};

#endif //BERSERK_CONSOLETEST_H