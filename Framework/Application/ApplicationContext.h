//
// Created by Egor Orachyov on 10.06.2018.
//

#ifndef BERSERKENGINE_APPLICATIONCONTEXT_H
#define BERSERKENGINE_APPLICATIONCONTEXT_H

#include "Essential/Types.h"

#include "System/RenderSystem.h"

namespace Berserk
{

    /**
     * @brief Application
     *
     * Basic blueprint for engine application
     */
    class ApplicationContext
    {
    public:

        /**
         * @brief Standard init for application.
         *
         * Calls config manager for getting init properties, creates special setup list for
         * each system. Initialize systems via calling init().
         */
        void init();

        /**
         * User specific function to setup your application properties. Create desired objects,
         * add it to rendering queue, setup camera, window, input handling and etc.
         */
        virtual void setup();

        /**
         * @brief Standard main loop for application.
         *
         * Main application entry point. Joins endless cycle (or main loop) and
         * starts process systems' update() functions.
         */
        void run();

        /**
         * User specific function for proper deleting of created objects and specific elements.
         */
        virtual void close();

        /**
         * @brief Standard destroy for application.
         *
         * Correct closing of initialized systems and dependencies.
         *
         * @note Can be called via user or standard destructor
         */
        void destroy();

    protected:

    private:

        UINT8 mIsInitialized;
        UINT8 mIsDestroyed;

        RenderSystem* mRenderSystem;

    };

} // namespace Berserk

#endif //BERSERKENGINE_APPLICATIONCONTEXT_H
