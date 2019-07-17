//
// Created by Egor Orachyov on 2019-07-17.
//

#ifndef BERSERK_IAPPLICATION_H
#define BERSERK_IAPPLICATION_H

#include <Misc/Types.h>
#include <Misc/UsageDescriptors.h>

namespace Berserk
{

    /**
     * Main application interface and engine application entry point.
     * Handles engine start-up and shut-down
     */
    class ENGINE_API IApplication
    {
    protected:

        /**
         * @brief Overrideable behavior
         *
         * Called after the application is created
         * Override this function to enable custom init logic in your application
         */
        virtual void initialize() { }

        /**
         * @brief Overrideable behavior
         *
         * Called before the application is destroyed
         * Override this function to enable custom destroy logic in your application
         */
        virtual void destroy() { }

    public:

        /**
         * Application main loop entry point
         * Should be called after application is constructed
         */
        virtual void run() { };

    };

} // namespace Berserk

#endif //BERSERK_IAPPLICATION_H