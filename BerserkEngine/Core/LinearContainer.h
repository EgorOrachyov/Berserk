//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_LINEARCONTAINER_H
#define BERSERKENGINE_LINEARCONTAINER_H

#include "Types.h"

namespace Berserk
{
    template<typename Element> class LinearContainer
    {
    public:

        virtual void Add(Element element) = 0;

        virtual void Remove(Element element) = 0;

        virtual void Empty() = 0;

        virtual Element GetFirst() = 0;

        virtual Element GetLast() = 0;

        virtual void Iterate(bool restart) = 0;

        virtual Element GetCurrent() = 0;

        virtual Element GetNext(Element element) = 0;

        virtual int32 GetTotalElements() = 0;
    };

} // namespace Berserk

#endif //BERSERKENGINE_LINEARCONTAINER_H
