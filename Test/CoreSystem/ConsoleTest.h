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
        IConsoleVariable* var;

        ConsoleVariable<String> variableString("Param", "Help", String("12.04"), nullptr);
        var = variableString.asVariable();

        OutputDevice::printf("Name: %s \n", var->getName().get());
        OutputDevice::printf("Help: %s \n", var->getHelp().get());

        OutputDevice::printf("Value: %i \n", var->getInt());
        OutputDevice::printf("Value: %f \n", var->getFloat());
        OutputDevice::printf("Value: %s \n", var->getString().get());

        ConsoleVariable<int32> variableInt("Param", "Help", 132, nullptr);
        var = variableInt.asVariable();

        OutputDevice::printf("Name: %s \n", var->getName().get());
        OutputDevice::printf("Help: %s \n", var->getHelp().get());

        OutputDevice::printf("Value: %i \n", var->getInt());
        OutputDevice::printf("Value: %f \n", var->getFloat());
        OutputDevice::printf("Value: %s \n", var->getString().get());

        ConsoleVariable<float32 > variableFloat("Param", "Help", 154.07, nullptr);
        var = variableFloat.asVariable();

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