/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2018,2019,2020 Egor Orachyov                                     */
/**********************************************************************************/

#ifndef BERSERK_SINGLETON_HPP
#define BERSERK_SINGLETON_HPP

namespace Berserk {

    /**
     * @brief Singleton class
     *
     * @tparam TInstance Type of provided singleton
     */
    template<typename TInstance>
    class Singleton {
    public:

        /** @return Public instance pointer */
        static TInstance *InstancePointer() {
            return mInstancePointer;
        }

        /** @return Public instance reference */
        static TInstance &Instance() {
            return *mInstancePointer;
        }

    protected:

        /** Must be called by instance provider */
        static void Provide(TInstance *reference) {
            if (mInstancePointer == nullptr) {
                mInstancePointer = reference;
            }
        }

        /** Must be called by instance provider */
        static void Remove(TInstance *reference) {
            if (mInstancePointer == reference) {
                mInstancePointer = nullptr;
            }
        }

    private:
        static TInstance *mInstancePointer;
    };

    template<typename TInstance>
    TInstance *Singleton<TInstance>::mInstancePointer = nullptr;
}

#endif //BERSERK_SINGLETON_HPP
