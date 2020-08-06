/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_RHITIMEQUERY_H
#define BERSERK_RHITIMEQUERY_H

#include <RHI/RHIResource.h>

namespace Berserk {

    /**
     * @brief Time query
     *
     * Time query for elapsed time measurement for operations on GPU.
     * Since CPU -> GPU communication has async nature, time queries, inserted
     * in draw list are required for proper execution time measurement.
     *
     * @warning Current RHI time query resource allows measure elapsed time for commands,
     *          which can not be nested or interleaved, i.e. we can’t call beginQuery within a pair of begin-end.
     */
    class RHITimeQuery : public RHIResource {
    public:
        ~RHITimeQuery() override = default;

        /** @return If available: time query elapsed time (if query is not available result undefined) */
        virtual TimeValue tryGetElapsedTime() const = 0;

        /** @return True if query result already available */
        virtual bool isResultAvailable() const = 0;
    };

}

#endif //BERSERK_RHITIMEQUERY_H
