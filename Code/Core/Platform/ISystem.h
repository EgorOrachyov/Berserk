/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_ISYSTEM_H
#define BERSERK_ISYSTEM_H

#include <Platform/IMutex.h>
#include <Platform/IAtomic.h>
#include <Platform/IFile.h>
#include <Platform/IDirectory.h>

#include <TimeValue.h>
#include <TPtrUnique.h>
#include <Math/Point2i.h>
#include <IO/ILog.h>
#include <IO/IOutputDevice.h>

namespace Berserk {

    /**
     * @brief System access
     * Provides access to target platform specific operations
     * (window management, time, resources creation, output, logging, etc.)
     */
    class ISystem {
    public:
        ISystem();
        virtual ~ISystem() = default;

        using LibID = uint32;
        using WindowID = uint32;
        static const WindowID MAIN_WINDOW = 0;

        /** Window video mode */
        struct VideoMode {
            uint32 width = 640; /** In pixels */
            uint32 height = 480; /** In pixels */
            bool resizeable = false;
            bool maximised = false;
        };

        /** Creates primary app window. Must be called once before engine init process */
        virtual void initialize(CString windowName, const VideoMode& videoMode) = 0;
        /** System update: must be called for each main loop iteration */
        virtual void update() = 0;
        /** Finalize system processing: must be called before application shut down*/
        virtual void finalize() = 0;

        /** Called to process an error in error macro */
        virtual void onError(const char* message, uint64 line, const char* function, const char* file) = 0;
        /** @return Default logger */
        virtual ILog& getLog() = 0;
        /** @return Default output stream */
        virtual IOutputDevice& getOutputDevice() = 0;

        /** @return True, if close operation were requested on window */
        virtual bool shouldClose(WindowID id) = 0;
        /** @return True, if window is resizable */
        virtual bool isResizeable(WindowID id) = 0;
        /** @return True, if window was currently minimized (hidden in the bar as icon) */
        virtual bool isMinimized(WindowID id) = 0;
        /** @return True, if window was currently restored from minimized state */
        virtual bool isRestored(WindowID id) = 0;
        /** @return True, if window was currently moved */
        virtual bool isMoved(WindowID id) = 0;
        /** @return True, if window was currently resized */
        virtual bool isResized(WindowID id) = 0;
        /** @return Current window position on the monitor in px */
        virtual Size2i getWindowPos(WindowID id) = 0;
        /** @return Current window size in px */
        virtual Size2i getWindowSize(WindowID id) = 0;
        /** @return Window caption */
        virtual const CString& getWindowCaption(WindowID id) const = 0;

        /** Opens shared library at runtime with specified name */
        virtual EError openLib(LibID& id, CString path) { return EError::UNAVAILABLE; };
        /** Closes dynamic library with id */
        virtual EError closeLib(LibID id) { return EError::UNAVAILABLE; };
        /** Loads symbol ptr from dynamic library id */
        virtual EError loadSymbol(LibID id, CString symbol, void* &handle) { return EError::UNAVAILABLE; };

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

        /** @return Error mutex for sync error list access (only for int usage) */
        virtual IMutex& getErrorSyncMutex() = 0;
        /** @return Opened file for path */
        virtual TPtrUnique<IFile> openFile(CString path, EFileMode mode) = 0;
        /** @return Opened directory for path */
        virtual TPtrUnique<IDirectory> openDirectory(CString path) = 0;
        /** @return Sync mutex */
        virtual TPtrUnique<IMutex> createMutex() = 0;
        /** @return Atomic counter */
        virtual TPtrUnique<IAtomic> createAtomic() = 0;
        /**
         * Initialized prior any other engine sub-system is initialized
         * @return System globally accessible singleton
         */
        static ISystem& getSingleton();
    };

}

#endif //BERSERK_ISYSTEM_H