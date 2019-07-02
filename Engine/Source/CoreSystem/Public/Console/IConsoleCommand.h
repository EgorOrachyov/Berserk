//
// Created by Egor Orachyov on 01.07.2019.
//

#ifndef BERSERK_ICONSOLECOMMAND_H
#define BERSERK_ICONSOLECOMMAND_H

#include <Console/IConsoleObject.h>
#include <Containers/TArray.h>
#include <IO/IOutputDevice.h>

namespace Berserk
{

    /**
     * Allows to process some command called in the console
     */
    class CORE_API IConsoleCommand : public IConsoleObject
    {
    public:

        ~IConsoleCommand() override = default;

        /** Type of execute function to register console commands */
        typedef bool (*ExecuteFunction)(const TArray<String> &args, IOutputDevice& device, EConsolePriority priority);

        /**
         * Executes console command (with arguments or without)
         * @note Could be called from any thread
         *
         * @param args     Arguments of the command call
         * @param device   Output device to out some execution info
         * @param priority Priority of this command caller
         * @return True if command successfully executed
         */
        virtual bool execute(const TArray<String> &args, IOutputDevice& device, EConsolePriority priority) = 0;

    };

} // namespace Berserk

#endif //BERSERK_ICONSOLECOMMAND_H