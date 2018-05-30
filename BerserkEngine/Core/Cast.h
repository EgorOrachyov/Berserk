//
// Created by Egor Orachyov on 30.05.2018.
//

#ifndef BERSERKENGINE_CAST_H
#define BERSERKENGINE_CAST_H

namespace Berserk
{

    /**
     * Engine cast for elements
     *
     * @tparam TypeOut Destination type of reduction
     * @tparam TypeIn  Source type of reduction
     * @param element  Element for reduction
     * @return Element casted from TypeIn to TypeOut
     */
    template <typename Type_Out, typename Type_In>
    Type_Out Cast(Type_In element)
    {
        return (Type_Out)element;
    }

} // namespace Berserk

#endif //BERSERKENGINE_CAST_H
