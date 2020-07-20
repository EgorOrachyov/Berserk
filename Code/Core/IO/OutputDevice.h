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

    /** Sync char-style output stream */
    class OutputDevice {
    public:
        virtual ~OutputDevice() = default;
        virtual void print(const char* message) = 0;
    };

    /** Classical std output stream */
    class OutputDeviceStd : public OutputDevice {
    public:
        ~OutputDeviceStd() override = default;
        void print(const char *message) override;
    };

}

#endif //BERSERK_OUTPUTDEVICE_H