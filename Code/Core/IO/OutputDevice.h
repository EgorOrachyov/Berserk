/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_OUTPUTDEVICE_H
#define BERSERK_OUTPUTDEVICE_H

#include <Typedefs.h>
#include <String/TString.h>
#include <String/TStringStaticN.h>

namespace Berserk {

    /** Types of the messages for proper highlighting in the console/print devices */
    enum class EOutputType {
        Info,
        Warning,
        Error,
        Text,
        Input
    };

    /** Sync char-style output stream */
    class OutputDevice {
    public:
        virtual ~OutputDevice() = default;
        virtual void print(EOutputType messageType, const char *message) = 0;

        template <typename ... TArgs>
        void printf(EOutputType messageType, const char* format, TArgs&& ... args) {
            printfn<2048,TArgs...>(messageType, format, std::forward<TArgs>(args)...);
        }

        template <uint32 SIZE, typename ... TArgs>
        void printfn(EOutputType messageType, const char* format, TArgs&& ... args) {
            char buffer[SIZE];
            snprintf(buffer, SIZE, format, std::forward<TArgs>(args)...);
            print(messageType, buffer);
        }

    };

    /** Classical std output stream */
    class OutputDeviceStd : public OutputDevice {
    public:
        ~OutputDeviceStd() override = default;
        void print(EOutputType messageType, const char *message) override;
    };

}

#endif //BERSERK_OUTPUTDEVICE_H