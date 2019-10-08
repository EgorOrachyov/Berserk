//
// Created by Egor Orachyov on 01.07.2019.
//

#ifndef BERSERK_ICONSOLEMANAGER_H
#define BERSERK_ICONSOLEMANAGER_H

#include <Console/IConsoleCommand.h>
#include <Console/IConsoleVariable.h>

namespace Berserk
{

    /**
     * Manages console variables and commands. Allows to modify and execute them.
     * Console manager could be accessed via code or engine console.
     *
     * Work flow for console manager variables:
     *      <variable name>         - shows the current variable value
     *      <variable name> ?       - shows help text for the value
     *      <variable name> <value> - set new variable value
     *
     * Work flow for console manager commands:
     *      <command name> ?                        - shows help text for the command
     *      <command name> <value1> .. <valueN>     - execute command
     *
     * @note Thread-Safe, accessible from any thread
     */
    class IConsoleManager
    {
    public:

        virtual ~IConsoleManager() = default;

        /**
         * Creates new console variable [int32] and registers it in the console manager.
         * @param name          Variable name
         * @param initialValue  Initial variable value
         * @param callback      Called after the value of the variable changed
         * @param help          Help text about this variable
         * @param flags         Initial flags
         * @param priority      Modification priority
         * @return Not-null value if successfully create new variable
         */
        virtual const IConsoleVariable* registerVariable(const char* name, int32 initialValue, const char* help,
                                                   IConsoleVariable::OnChangeCallback callback,
                                                   EConsoleObjectFlags flags, EConsolePriority priority) = 0;

        /**
         * Creates new console variable [float32] and registers it in the console manager.
         * @param name          Variable name
         * @param initialValue  Initial variable value
         * @param callback      Called after the value of the variable changed
         * @param help          Help text about this variable
         * @param flags         Initial flags
         * @param priority      Modification priority
         * @return Not-null value if successfully create new variable
         */
        virtual const IConsoleVariable* registerVariable(const char* name, float32 initialValue, const char* help,
                                                   IConsoleVariable::OnChangeCallback callback,
                                                   EConsoleObjectFlags flags, EConsolePriority priority) = 0;

        /**
         * Creates new console variable [String] and registers it in the console manager.
         * @param name          Variable name
         * @param initialValue  Initial variable value
         * @param callback      Called after the value of the variable changed
         * @param help          Help text about this variable
         * @param flags         Initial flags
         * @param priority      Modification priority
         * @return Not-null value if successfully create new variable
         */
        virtual const IConsoleVariable* registerVariable(const char* name, const char* initialValue, const char* help,
                                                   IConsoleVariable::OnChangeCallback callback,
                                                   EConsoleObjectFlags flags, EConsolePriority priority) = 0;

        /**
         * Creates new console command and registers it in the console manager.
         * @param name      Command name
         * @param help      Help text about this command
         * @param function  Pointer to the function to call
         * @param flags     Initial flags
         * @param priority  Modification priority
         * @return Not-null value if successfully create new command
         */
        virtual const IConsoleCommand* registerCommand(const char* name, const char* help,
                                                 IConsoleCommand::ExecuteFunction function,
                                                 EConsoleObjectFlags flags, EConsolePriority priority) = 0;

        /**
        * Finds a console object via its name
        * @param name Name of the object
        * @return Not-null value if object was successfully found
        */
        virtual const IConsoleObject* findObject(const char* name) const = 0;

        /**
         * Finds variable via its name
         * @param name Name of the variable
         * @return Not-null value if variable was successfully found
         */
        virtual const IConsoleVariable* findVariable(const char* name) const = 0;

        /**
         * Finds command via its name
         * @param name Name of the command
         * @return Not-null value if command was successfully found
         */
        virtual const IConsoleCommand* findCommand(const char* name) const = 0;

        /**
         * Process input string for manager
         * @param input String with input text to process
         * @param device Used output printing device
         * @return True if successfully processed
         */
        virtual bool processInput(const char* input, IOutputDevice& device) = 0;

        /**
         * Returns all input commands successfully processed by the manager
         * @param out Array to store commands
         */
        virtual void getConsoleHistory(TArray<String> &out) const = 0;

        /**
         * Clears the history of console input command execution
         */
        virtual void clearConsoleHistory() = 0;

    };

} // namespace Berserk

#endif //BERSERK_ICONSOLEMANAGER_H