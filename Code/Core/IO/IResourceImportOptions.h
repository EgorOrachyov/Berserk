/**********************************************************************************/
/* This file is part of Berserk Device project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_IRESOURCEIMPORTOPTIONS_H
#define BERSERK_IRESOURCEIMPORTOPTIONS_H

namespace Berserk {

    /**
     * @brief Import options
     * Extend this class to pass some specific options to import chosen resource.
     */
    class IResourceImportOptions {
    public:
        virtual ~IResourceImportOptions() = default;
    };

}

#endif //BERSERK_IRESOURCEIMPORTOPTIONS_H