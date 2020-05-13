/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_ICONSOLEMANAGER_H
#define BERSERK_ICONSOLEMANAGER_H

#include <IO/IOutputDevice.h>
#include <Platform/Mutex.h>
#include <String/CString.h>
#include <TEnumMask.h>
#include <Containers/TArray.h>
#include <TRef.h>

namespace Berserk {

    /** Control access mode of the console objects */
    enum class EConsoleFlag {
        ReadOnly = 0,
        MainThread = 1,
        IgnorePriority = 2,
        ThreadSafe = 3
    };

    /** Modification type */
    enum class EConsoleMod {
        ByCode = 0,
        ByUser = 1,
    };

    /** Generic console object */
    class IConsoleObject {
    public:
        virtual ~IConsoleObject() = default;
        virtual const TEnumMask<EConsoleFlag> &getFlags() const = 0;
        virtual const CString& getName() const = 0;
        virtual const CString& getHelpText() const = 0;
        virtual bool isVariable() const = 0;
        virtual bool isCommand() const = 0;
    };

    /** Console variable which wraps some basic types */
    class IConsoleVariable : public IConsoleObject {
    public:
        ~IConsoleVariable() override = default;
        virtual bool isInt() const = 0;
        virtual bool isFloat() const = 0;
        virtual bool isString() const = 0;
        virtual int32 getInt() const = 0;
        virtual float getFloat() const = 0;
        virtual CString getString() const = 0;
        virtual bool canChange(EConsoleMod mod) const = 0;
        virtual EConsoleMod getModificationType() const = 0;

    protected:
        friend class ConsoleManager;
        virtual void set(const CString& value, EConsoleMod mod) = 0;
    };

    /** Console command which allows run some custom logic in the console */
    class IConsoleCommand : public IConsoleObject {
    public:
        ~IConsoleCommand() override = default;
        virtual void execute(const TArray<CString> &args, IOutputDevice& device) = 0;
        using Signature = Function<void(const TArray<CString> &args, IOutputDevice& device)>;
    };

    /** Manages all the console objects for the game and job threads of the engine */
    class IConsoleManager {
    public:

        IConsoleManager();

        /**
         * Register console variable of type int32
         * (If object with the same name already exists, return null)
         * @param name Variable name (not null)
         * @param defaultValue initial value to assign
         * @param help Help info text to show in console (not null)
         * @param flags Access flags
         * @return Not null reference to variable if it is successfully registered
         */
        virtual TRef<IConsoleVariable> registerVariable(const char* name, int32 defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags) = 0;

        /**
         * Register console variable of type float
         * (If object with the same name already exists, return null)
         * @param name Variable name (not null)
         * @param defaultValue initial value to assign
         * @param help Help info text to show in console (not null)
         * @param flags Access flags
         * @return Not null reference to variable if it is successfully registered
         */
        virtual TRef<IConsoleVariable> registerVariable(const char* name, float defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags) = 0;

        /**
         * Register console variable of type CString
         * (If object with the same name already exists, return null)
         * @param name Variable name (not null)
         * @param defaultValue initial value to assign
         * @param help Help info text to show in console (not null)
         * @param flags Access flags
         * @return Not null reference to variable if it is successfully registered
         */
        virtual TRef<IConsoleVariable> registerVariable(const char* name, const char* defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags) = 0;

        /**
         * Register console variable of type int32 with Mutex for safe threaded access
         * (If object with the same name already exists, return null)
         * @param name Variable name (not null)
         * @param defaultValue initial value to assign
         * @param help Help info text to show in console (not null)
         * @param flags Access flags
         * @param access Mutex for synchronized access
         * @return Not null reference to variable if it is successfully registered
         */
        virtual TRef<IConsoleVariable> registerVariable(const char* name, int32 defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags, Mutex& access) = 0;

        /**
         * Register console variable of type float with Mutex for safe threaded access
         * (If object with the same name already exists, return null)
         * @param name Variable name (not null)
         * @param defaultValue initial value to assign
         * @param help Help info text to show in console (not null)
         * @param flags Access flags
         * @param access Mutex for synchronized access
         * @return Not null reference to variable if it is successfully registered
         */
        virtual TRef<IConsoleVariable> registerVariable(const char* name, float defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags, Mutex& access) = 0;

        /**
         * Register console variable of type CString with Mutex for safe threaded access
         * (If object with the same name already exists, return null)
         * @param name Variable name (not null)
         * @param defaultValue initial value to assign
         * @param help Help info text to show in console (not null)
         * @param flags Access flags
         * @param access Mutex for synchronized access
         * @return Not null reference to variable if it is successfully registered
         */
        virtual TRef<IConsoleVariable> registerVariable(const char* name, const char* defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags, Mutex& access) = 0;

        /**
         * Register console command with body 'function'
         * (If object with the same name already exists, return null)
         * @param name Name of the command (not null)
         * @param function Function to execute as command
         * @param help Help info text to show in console (not null)
         * @param flags Access flags
         * @return Not null reference to command if it is successfully registered
         */
        virtual TRef<IConsoleCommand> registerCommand(const char* name, IConsoleCommand::Signature function, const char* help, const TEnumMask<EConsoleFlag> &flags) = 0;

        /**
         * Finds console variable with specified name (case sensitive)
         * @param name Name of the variable to find
         * @return Not null reference to variable if it is successfully found
         */
        virtual TRef<IConsoleVariable> findVariable(const CString &name) const = 0;

        /**
         * Process user input to the console
         * @param input Input line string
         * @param outputDevice Output device for printing
         */
        virtual void processUserInput(const CString& input, IOutputDevice& outputDevice) = 0;

        /**
         * Run some logic for each console object which matches the prefix
         * @param prefix Prefix of the objects to match
         * @param visitor Logic to run
         */
        virtual void forEachConsoleObjectWithPrefix(const char* prefix, const Function<void(const IConsoleObject&)> &visitor) const = 0;

        /** @return Global console manager instance (initialized by the engine) */
        static IConsoleManager& getSingleton();

        /** @return Convert console mode to string */
        static const char* getConsoleModificationModeString(EConsoleMod mod);

    private:

        /** Singleton reference */
        static  IConsoleManager* gConsoleManager;
    };

}

#endif //BERSERK_ICONSOLEMANAGER_H