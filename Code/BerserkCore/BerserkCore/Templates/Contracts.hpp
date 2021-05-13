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
     * Contract type, used to print arbitrary objects to text format.
     *
     * Implement method template<typename Stream> void operator()(Stream& stream, const T& object) const
     * in order to provide functionality to print your object into arbitrary stream of text symbols.
     *
     * @tparam Stream Generic stream, used to build result text string
     * @tparam T Type of the object to print
     */
    template <typename T>
    class TextPrint;

}

#endif //BERSERK_CONTRACTS_HPP
