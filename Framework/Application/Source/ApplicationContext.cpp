//
// Created by Egor Orachyov on 12.07.2018.
//

#include "Context/ApplicationContext.h"
#include "System/GLRenderSystem.h"
#include "Managers/SceneManager.h"
#include "Config/ConfigLoader.h"
#include "Logging/LogMessages.h"

namespace Berserk
{

    void ApplicationContext::init()
    {
        CHAR configList[] = "../Core/Config/ConfigList.cfg";
        CHAR configFile[] = "../Core/Config/engine.cfg";

        init(configList, configFile);
    }

    void ApplicationContext::init(const CHAR *configList, const CHAR *configFile)
    {
        /// Load Configuration File

        ConfigTable table;
        table.init(BUFFER_SIZE_64);

        bool isListLoaded = loadConfigList(configList, table);
        if (!isListLoaded)
        {
            ERROR("Cannot load config list %s", configList);
            exit(EXIT_FAILURE);
        }

        bool isFileLoaded = loadConfigFile(configFile, table);
        if (!isFileLoaded)
        {
            ERROR("Cannot load config file %s", configFile);
            exit(EXIT_FAILURE);
        }

        /// Create all the sub-systems

        if (strcmp(table.getChar("RenderSystem"), "GLRenderAPI") == 0)
        {
            mRenderSystem = new GLRenderSystem();
        }
        else
        {
            ERROR("Cannot initialize rendering system %s", table.getChar("RenderSystem"));
            exit(EXIT_FAILURE);
        }

        /// Init created sub systems

        mRenderSystem->init(table);


        /// Setup application flags

        mIsInitialized = true;
        mIsDestroyed = false;
        mShouldClose = false;

        /// Setup Scene Manager
    }

    void ApplicationContext::setup()
    {

    }

    void ApplicationContext::run()
    {
        /// Pre main loop entry point systems' call

        mRenderSystem->preMainLoop();

        int i = 0;
        while (i++ < 150)
        //while (!mShouldClose)
        {
            /// Pre update block

            mRenderSystem->preUpdate();

            /// Post update block

            mRenderSystem->postUpdate();
        }

        /// Post main loop entry point systems' call

        mRenderSystem->postMainLoop();
    }

    void ApplicationContext::close()
    {

    }

    void ApplicationContext::destroy()
    {
        mRenderSystem->destroy();

        SAFE_DELETE(mRenderSystem);
    }

} // namespace Berserk