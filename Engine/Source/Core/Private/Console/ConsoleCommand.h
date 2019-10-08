//
// Created by Egor Orachyov on 03.07.2019.
//

#ifndef BERSERK_CONSOLECOMMAND_H
#define BERSERK_CONSOLECOMMAND_H

#include <Console/IConsoleCommand.h>

namespace Berserk
{


    /**
     * Default command implementation for console manager internal usage
     */
    class ConsoleCommand : public IConsoleCommand
    {
    public:

        GENERATE_NEW_DELETE(ConsoleCommand);

        ConsoleCommand(const char* name, const char* help,
                       ExecuteFunction function,
                       EConsoleObjectFlags flags = EConsoleObjectFlags::Default,
                       EConsolePriority priority = EConsolePriority::SetByCode)
                : mName(name),
                  mHelpText(help),
                  mFunction(function),
                  mObjectFlags(flags),
                  mPriority(priority)
        {

        }

        ~ConsoleCommand() override = default;

        bool execute(const TArray<String> &args, IOutputDevice &device, EConsolePriority priority) override
        {
            if (canExecute(priority))
            {
                return mFunction(args, device);
            }
            else
            {
                return false;
            }
        }

        const String &getName() const override { return mName; }

        const String &getHelp() const override { return mHelpText; }

        EConsoleObjectFlags getFlags() const override { return mObjectFlags; }

        EConsolePriority getPriority() const override { return mPriority; }

        IConsoleCommand *asCommand() const override { return (IConsoleCommand*) this; }

        IConsoleVariable *asVariable() const override { return nullptr; }

        bool canExecute(EConsolePriority priority) const { return mPriority <= priority; }

    private:

        ExecuteFunction mFunction;

        EConsolePriority mPriority;
        EConsoleObjectFlags mObjectFlags;

        String mName;
        String mHelpText;

    };

} // namespace Berserk

#endif //BERSERK_CONSOLECOMMAND_H