/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RESOURCEIMPORTOPTIONS_H
#define BERSERK_RESOURCEIMPORTOPTIONS_H

namespace Berserk {

    /**
     * @brief Import options
     * Extend this class to pass some specific options to import chosen resource.
     */
    class ResourceImportOptions {
    public:
        virtual ~ResourceImportOptions() = default;
    };

}

#endif //BERSERK_RESOURCEIMPORTOPTIONS_H