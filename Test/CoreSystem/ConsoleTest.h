//
// Created by Egor Orachyov on 03.07.2019.
//

#ifndef BERSERK_CONSOLETEST_H
#define BERSERK_CONSOLETEST_H

#include <Console/ConsoleManager.h>

using namespace Berserk;

class ConsoleTest
{
public:

    static void ConsoleTest1()
    {
        /*

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

        IConsoleCommand* cmd;

        auto exec = [](const TArray<String> &args, IOutputDevice &device)
        {
            device.print("Execute function 'Command'\n");
            return true;
        };

        ConsoleCommand command("Command", "Help", exec);
        command.execute(TArray<String>(), OutputDevice::get(), EConsolePriority::SetByCode);

         */
    }

    static void ConsoleTest2()
    {
        ConsoleManager manager(Allocator::get());
        IConsoleManager* console = &manager;

        auto gShadowsQualityCallback = [](IConsoleVariable* variable){ return; };
        auto gShadowsQuality = console->registerVariable(
                "gShadowQuality",
                0,
                "Defines quality of the shadows in the rendering engine:\n"
                "0 - low (default)\n"
                "1 - medium\n"
                "2 - high",
                gShadowsQualityCallback,
                EConsoleObjectFlags::ThreadSafe,
                EConsolePriority::SetByCode
        );

        OutputDevice::printf("Name: %s\nHelp: %s\nValue: %i\n",
                             gShadowsQuality->getName().get(),
                             gShadowsQuality->getHelp().get(),
                             gShadowsQuality->getInt());

        auto gShowTextExec = [](const TArray<String> &args, IOutputDevice &device){ return true; };


        console->processInput("a bsd=ae ,a", OutputDevice::get());
    }

    static void run()
    {
        //ConsoleTest1();
        ConsoleTest2();
    }

};

#endif //BERSERK_CONSOLETEST_H