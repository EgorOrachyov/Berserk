//
// Created by Egor Orachyov on 01.08.2018.
//

#ifndef BERSERKENGINE_POSTPROCESSSTRUCTURES_H
#define BERSERKENGINE_POSTPROCESSSTRUCTURES_H

#include "Essential/Types.h"

namespace Berserk
{

    class Tonemap
    {
    public:

        INT32 Exposure;
        INT32 White;
        INT32 AverageLuminance;
    };

    class GaussianBlur10
    {
    public:

        INT32 Weight[10];
    };

} // namespace Berserk

#endif //BERSERKENGINE_POSTPROCESSSTRUCTURES_H
