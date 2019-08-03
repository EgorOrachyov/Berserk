//
// Created by Egor Orachyov on 2019-08-03.
//

#ifndef BERSERK_RESOURCEHANDLER_H
#define BERSERK_RESOURCEHANDLER_H

#include <Resource/TSharedPtr.h>

namespace Berserk
{

    /**
     * Handles various game engine resources. Acts like a smart pointer with additional
     * help and misc features. Provides:
     * 1) access to the loaded resource
     * 2) also allows hot reload of the resource in run-time mode, without updating handler
     * 3) immediately returns handler to async loaded resource, which become valid as soon as resource is loaded
     *
     * @note Each handler must be used only by one thread
     * @tparam T Type of the handler resource
     */
    template <class T>
    class ResourceHandler
    {
    public:

        bool isLoaded() const;



    };

} // namespace Berserk

#endif //BERSERK_RESOURCEHANDLER_H