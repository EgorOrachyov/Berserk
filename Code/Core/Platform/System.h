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

#include <TPtrUnique.h>
#include <Math/Point2i.h>
#include <IO/Log.h>

namespace Berserk {

    class System {
    public:
        System();
        virtual ~System() = default;

        using LibID = uint64;
        using WindowID = uint64;

        static const WindowID MAIN_WINDOW = 0;

        struct VideoMode {
            uint32 width = 640; /** In pixels */
            uint32 height = 480; /** In pixels */
            bool resizeable = false;
            bool fullscreen = false;
            bool maximised = false;
        };

        /** Creates primary app window. Must be called once before engine init process */
        virtual void initialize(CString windowName, const VideoMode& videoMode) = 0;
        virtual void update() = 0;
        virtual void finalize() = 0;

        virtual void onError(const char* message, uint64 line, const char* function, const char* file) = 0;
        virtual Log& getLog() = 0;

        virtual bool    shouldClose(WindowID id) = 0;
        virtual bool    isResizeable(WindowID id) = 0;
        virtual bool    isFullscreen(WindowID id) = 0;
        virtual Size2i  getWindowPos(WindowID id) = 0;
        virtual Size2i  getWindowSize(WindowID id) = 0;
        virtual CString getWindowCaption(WindowID id) const = 0;

        /** Runtime dynamic/shared library access */
        virtual EError openLib(LibID& id, CString path)                      { return EError::UNAVAILABLE; };
        virtual EError closeLib(LibID id)                                    { return EError::UNAVAILABLE; };
        virtual EError loadSymbol(LibID id, CString symbol, void* &handle)   { return EError::UNAVAILABLE; };

        /** System data and time info */
        struct Time {
            int32 year = 0;
            int32 month = 0;   /** Indexed from 0 */
            int32 day = 0;     /** Indexed from 0 */
            int32 dayYear = 0; /** Indexed from 0 */
            int32 hour = 0;    /** Indexed from 0 */
            int32 min = 0;     /** Indexed from 0 */
            int32 sec = 0;     /** Indexed from 0 */
        };

        /** System date/time info */
        virtual Time getTime() const { return {}; }

        /** Platform specific resources access */
        virtual Mutex&              getErrorSyncMutex() = 0;
        virtual TPtrUnique<File>    openFile(CString path, EFileMode mode) = 0;
        virtual TPtrUnique<Mutex>   createMutex() = 0;
        virtual TPtrUnique<Atomic>  createAtomic() = 0;

        /**
         * Initialised prior any other engine sub-system is initialized
         * @return System globally accessible singleton
         */
        static System& getSingleton();
    };

}

#endif //BERSERK_SYSTEM_H