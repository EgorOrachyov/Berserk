/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_ARCHIVE_HPP
#define BERSERK_ARCHIVE_HPP

#include <BerserkCore/Platform/Platform.hpp>

namespace Berserk {

    enum class EArchiveCaps {
        Input,
        Output,
        Binary,
        Text
    };

    class Archive {
    public:
        virtual ~Archive() = default;


    };

}



#endif //BERSERK_ARCHIVE_HPP
