//
// Created by Egor Orachyov on 10.06.2018.
//

#ifndef BERSERKENGINE_APPLICATIONCONTEXT_H
#define BERSERKENGINE_APPLICATIONCONTEXT_H

#include <Managers/SceneManager.h>
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
         * @brief Standard init for application.
         *
         * Calls config manager for getting init properties, creates special setup list for
         * each system. Initialize systems via calling init().
         *
         * @param configList Path of configuration list
         * @param configFile Path of configuration file
         */
        void init(const CHAR* configList, const CHAR* configFile);

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

        SceneManager* getSceneManager() const;

    private:

        INT8 mIsInitialized;
        INT8 mIsDestroyed;
        INT8 mShouldClose;

    };

} // namespace Berserk

#endif //BERSERKENGINE_APPLICATIONCONTEXT_H
