/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_CONSOLEMANAGER_H
#define BERSERK_CONSOLEMANAGER_H

#include <IO/OutputDevice.h>
#include <Platform/Mutex.h>
#include <String/CString.h>
#include <Containers/TArray.h>
#include <Containers/TMap.h>
#include <TEnumMask.h>
#include <AllocPool.h>
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
        /** When variable is registered */
        ByConstructor = 0,
        /** When value set from engine config */
        ByConfig = 1,
        /** Value set from code */
        ByCode = 2,
        /** Value set by user */
        ByUser = 3,
    };

    /** Generic console object */
    class ConsoleObject {
    public:
        virtual ~ConsoleObject() = default;
        virtual const TEnumMask<EConsoleFlag> &getFlags() const  = 0;
        virtual const CString& getName() const  = 0;
        virtual const CString& getHelpText() const  = 0;
        virtual bool isVariable() const  = 0;
        virtual bool isCommand() const  = 0;
    };

    /** Console variable which wraps some basic types */
    class ConsoleVariable : public ConsoleObject {
    public:
        ~ConsoleVariable() override = default;
        virtual void set(const CString& value, EConsoleMod mod) = 0;
        virtual bool isInt() const  = 0;
        virtual bool isFloat() const  = 0;
        virtual bool isString() const  = 0;
        virtual int32 getInt() const  = 0;
        virtual float getFloat() const  = 0;
        virtual CString getString() const  = 0;
        virtual EConsoleMod getModificationType() const  = 0;
    };

    /** Console command which allows run some custom logic in the console */
    class ConsoleCommand : public ConsoleObject {
    public:
        ~ConsoleCommand() override = default;
        virtual void execute(const TArray<CString> &args, OutputDevice& device) = 0;
        using Signature = Function<void(const TArray<CString> &args, OutputDevice& device)>;
    };

    /** Manages all the console objects for the game and job threads of the engine */
    class ConsoleManager {
    public:

        ConsoleManager();
        ~ConsoleManager();

        /**
         * Register console variable of type int32
         * (If object with the same name already exists, return null)
         * @param name Variable name (not null)
         * @param defaultValue initial value to assign
         * @param help Help info text to show in console (not null)
         * @param flags Access flags
         * @return Not null reference to variable if it is successfully registered
         */
        virtual TRef<ConsoleVariable> registerVariable(const char* name, int32 defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags);

        /**
         * Register console variable of type float
         * (If object with the same name already exists, return null)
         * @param name Variable name (not null)
         * @param defaultValue initial value to assign
         * @param help Help info text to show in console (not null)
         * @param flags Access flags
         * @return Not null reference to variable if it is successfully registered
         */
        virtual TRef<ConsoleVariable> registerVariable(const char* name, float defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags);

        /**
         * Register console variable of type CString
         * (If object with the same name already exists, return null)
         * @param name Variable name (not null)
         * @param defaultValue initial value to assign
         * @param help Help info text to show in console (not null)
         * @param flags Access flags
         * @return Not null reference to variable if it is successfully registered
         */
        virtual TRef<ConsoleVariable> registerVariable(const char* name, const char* defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags);

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
        virtual TRef<ConsoleVariable> registerVariable(const char* name, int32 defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags, Mutex& access);

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
        virtual TRef<ConsoleVariable> registerVariable(const char* name, float defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags, Mutex& access);

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
        virtual TRef<ConsoleVariable> registerVariable(const char* name, const char* defaultValue, const char* help, const TEnumMask<EConsoleFlag> &flags, Mutex& access);

        /**
         * Register console command with body 'function'
         * (If object with the same name already exists, return null)
         * @param name Name of the command (not null)
         * @param function Function to execute as command
         * @param help Help info text to show in console (not null)
         * @param flags Access flags
         * @return Not null reference to command if it is successfully registered
         */
        virtual TRef<ConsoleCommand> registerCommand(const char* name, ConsoleCommand::Signature function, const char* help, const TEnumMask<EConsoleFlag> &flags);

        /**
         * Finds console variable with specified name (case sensitive)
         * @param name Name of the variable to find
         * @return Not null reference to variable if it is successfully found
         */
        virtual TRef<ConsoleVariable> findVariable(const CString &name) const;

        /**
         * Process user input to the console
         * @param input Input line string
         * @param outputDevice Output device for printing
         */
        virtual void processUserInput(const CString& input, OutputDevice& outputDevice);

        /**
         * Run some logic for each console object which matches the prefix
         * @param prefix Prefix of the objects to match
         * @param visitor Logic to run
         */
        virtual void forEachConsoleObjectWithPrefix(const char* prefix, const Function<void(const ConsoleObject&)> &visitor) const;

        /** @return Global console manager instance (initialized by the engine) */
        static ConsoleManager& getSingleton();

        /** @return Convert console mode to string */
        static const char* getConsoleModAsString(EConsoleMod mod);

    private:

        TMap<CString, ConsoleObject*> mConsoleObjects;
        AllocPool mVariablesAllocator;
        AllocPool mCommandsAllocator;
        mutable Mutex mAccessMutex;

        /** Singleton reference */
        static  ConsoleManager* gConsoleManager;
    };

}

#endif //BERSERK_CONSOLEMANAGER_H