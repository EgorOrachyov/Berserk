/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <TimeValue.h>
#include <Platform/System.h>

namespace Berserk {

    CString TimeValue::toFormatString() const  {
        return System::getSingleton().getTime(*this).toString();
    }

}