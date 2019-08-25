//
// Created by Egor Orachyov on 03.07.2019.
//

#ifndef BERSERK_CONSOLETEST_H
#define BERSERK_CONSOLETEST_H

#include <Console/ConsoleManager.h>
#include <Threading/Thread.h>

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

        OutputDevice::print("\n");

        auto gShadowsQualityCallback = [](IConsoleVariable* variable){ return; };
        auto gShadowsQuality = console->registerVariable(
                "gShadowsQuality",
                0,
                "Defines quality of the shadows in the rendering engine:\n"
                "0 - low (default)\n"
                "1 - medium\n"
                "2 - high",
                gShadowsQualityCallback,
                EConsoleObjectFlags::ThreadSafe,
                EConsolePriority::SetByCode
        );

        console->processInput("gShadowsQuality ?", OutputDevice::get());
        console->processInput("gShadowsQuality",   OutputDevice::get());
        console->processInput("gShadowsQuality 2", OutputDevice::get());

        OutputDevice::print("\n");

        auto gTextPrinterExec = [](const TArray<String> &args, IOutputDevice &device)
        {
            for (uint32 i = 1; i < args.getSize(); i++)
            {
                device.printf("arg[%i]=%s\n", i, args.get(i).get());
            }

            return true;
        };
        auto gTextPrinter = console->registerCommand(
                "gTextPrinter",
                "Prints indexed arguments of the input",
                gTextPrinterExec,
                EConsoleObjectFlags::ThreadSafe,
                EConsolePriority::SetByCode
        );

        console->processInput("gTextPrinter ?", OutputDevice::get());
        console->processInput("gTextPrinter arg1, arg2, arg3", OutputDevice::get());

        OutputDevice::print("\n");

        console->processInput("unregisteredObject", OutputDevice::get());
        console->processInput("gShadowsQuality arg arg", OutputDevice::get());
        console->processInput("", OutputDevice::get());
    }

    static void ConsoleTest3()
    {
        ConsoleManager manager(Allocator::get());
        IConsoleManager* console = &manager;

        auto var = console->registerVariable("name", "1", "help", nullptr,
                                             EConsoleObjectFlags::Default, EConsolePriority::SetByCode);

        class Task : public IRunnable
        {
        public:

            explicit Task(IConsoleManager* console) : mConsole(console) {}

            uint32 run() override
            {
                auto var = mConsole->findVariable("name");

                for (int i = 0; i < 10000; ++i)
                {
                    Thread::yield();
                    String value = var->getString();
                    String name = var->getName();
                    if (i % 1000 == 0) OutputDevice::printf("Thread: %s %s \n", name.get(), value.get());
                }
            }

        private:

            IConsoleManager* mConsole;

        };

        Task task(console);


        Thread thread(TSharedPtr<IRunnable>(&task, nullptr), 1, "test");

        for (int i = 0; i < 100; ++i)
        {
            Thread::yield();
            String value = toString(i);
            String name = var->getName();
            console->processInput((String("name ") + value).get(), OutputDevice::get());
            if (i % 1000 == 0) OutputDevice::printf("Process: %s %s", name.get(), value.get());
        }

        thread.join();
    }

    static void run()
    {
        //ConsoleTest1();
        //ConsoleTest2();
        ConsoleTest3();
    }

};

#endif //BERSERK_CONSOLETEST_H