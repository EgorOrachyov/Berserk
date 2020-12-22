/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_CONTRACTS_HPP
#define BERSERK_CONTRACTS_HPP

namespace Berserk {

    template <typename T>
    class Equals;

    template <typename T>
    class Hash;

    template <typename T>
    class CompareLess;

    /**
     * Contract type, used to print arbitrary objects within formatter
     *
     * @tparam Stream Generic stream, used to build result formatted string
     * @tparam T Type of the object to print
     */
    template <typename T>
    class FormatPrint; // { template<typename Stream> operator()(Stream& stream, const T& object); }

}

#endif //BERSERK_CONTRACTS_HPP
