/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_SYSTEM_H
#define BERSERK_SYSTEM_H

#include <Platform/Mutex.h>
#include <Platform/Atomic.h>
#include <Platform/File.h>
#include <Platform/Directory.h>
#include <Platform/Library.h>

#include <TimeValue.h>
#include <TPtrUnique.h>
#include <Math/Point2i.h>
#include <IO/Log.h>
#include <IO/OutputDevice.h>

namespace Berserk {

    /** Types of provided render devices by the system */
    enum class ERenderDeviceType {
        OpenGL,
        Undefined
    };

    /**
     * @brief System access
     *
     * Provides access to target platform specific operations
     * (window management, time, resources creation, output, logging, etc.)
     *
     * @note Thread safe (updated only on Main thread)
     */
    class System {
    public:
        System();
        virtual ~System() = default;

        /**
         * Initialize platform system with specified window properties and rendering device type
         * @param name Unique name of the window for look-up
         * @param caption Window caption
         * @param size Window size in pixels
         * @param forceVSync Set in true to force v-sync of the window update rate
         * @param deviceType Type of the rendering device
         */
        virtual void initialize(const CString& name, const CString& caption, Size2i size, bool forceVSync, ERenderDeviceType deviceType) = 0;

        /** System update: must be called for each main loop iteration */
        virtual void update() = 0;

        /** Finalize system processing: must be called before application shut down*/
        virtual void finalize() = 0;

        /** Called to process an error in error macro */
        virtual void onError(const char* message, uint64 line, const char* function, const char* file) = 0;

        /** Called to process an warning in error macro */
        virtual void onWarning(const char* message, uint64 line, const char* function, const char* file) = 0;

        /** @return Default logger */
        virtual Log& getLog() = 0;

        /** @return Default output stream */
        virtual OutputDevice& getOutputDevice() = 0;

        /** @return Path to the executable binary file (could be used to detect engine directory) */
        virtual const CString &getExecutablePath() const = 0;

        /** @return Path to the engine directory */
        virtual const CString &getEnginePath() const = 0;

        /** @return Path to the engine cache directory */
        virtual const CString &getCachePath() const = 0;

        /** @return Path to engine config directory */
        virtual const CString &getConfigPath() const = 0;

        /** System data and time info */
        struct Time {
            int32 year = 0;
            int32 month = 0;    /** Indexed from 0 */
            int32 dayWeek = 0;  /** Indexed from 0 */
            int32 dayMonth = 0; /** Indexed from 0 */
            int32 dayYear = 0;  /** Indexed from 0 */
            int32 hour = 0;     /** Indexed from 0 */
            int32 min = 0;      /** Indexed from 0 */
            int32 sec = 0;      /** Indexed from 0 */
        };

        /** @return System date/time info */
        virtual Time getTime() const { return {}; }

        /** @return Interpreted as date/time info time value */
        virtual Time getTime(const TimeValue& t) const { return {}; };

        /** @return Time value for specified system time (dayYear and dayWeek are ignored) */
        virtual TimeValue getTimeValue(const Time& t) const { return {}; }

        /** @return Opened file for path */
        virtual TPtrUnique<File> openFile(CString path, EFileMode mode) = 0;

        /** @return Opened directory for path */
        virtual TPtrUnique<Directory> openDirectory(CString path) = 0;

        /** @return Opened library for path */
        virtual TPtrUnique<Library> openLibrary(CString path) = 0;

        /** @return System globally accessible singleton */
        static System& getSingleton();

        /** @return Device type from string */
        static ERenderDeviceType getDeviceTypeFromString(const CString& deviceName);

        /** @return String from device type */
        static CString getDeviceTypeAsString(ERenderDeviceType deviceType);

    private:

        /** Singleton reference */
        static System* gSystem;
    };

}

#endif //BERSERK_SYSTEM_H