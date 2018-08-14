//
// Created by Egor Orachyov on 12.07.2018.
//

#include "Context/ApplicationContext.h"
#include "Render/GLRenderSystem.h"
#include "Render/GLRenderDriver.h"
#include "Managers/SceneManager.h"
#include "Config/ConfigLoader.h"
#include "Strings/CStringBuffer.h"
#include "Strings/WStringBuffer.h"
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
            gRenderSystem = new GLRenderSystem();
            gRenderDriver = new GLRenderDriver();
        }
        else
        {
            ERROR("Cannot initialize rendering system %s", table.getChar("RenderSystem"));
            exit(EXIT_FAILURE);
        }

        /// Init created sub systems

        gRenderSystem->init(table);
        gRenderDriver->init(table);

        /// Setup application flags

        mIsInitialized = true;
        mIsDestroyed = false;
        mShouldClose = false;

        /// Setup Scene

        gSceneManager = new SceneManager();
        setup(); // user specific setup
        gSceneManager->init();

        /// Validate systems

        gRenderSystem->validate();
    }

    void ApplicationContext::setup()
    {

    }

    void ApplicationContext::run()
    {
        /// Pre main loop entry point systems' call

        gRenderSystem->preMainLoop();

        int i = 0;
        while (!mShouldClose)
        {
            /// Pre update block

            gRenderSystem->preUpdate();

            gSceneManager->update();

            /// Post update block

            gRenderSystem->postUpdate();

            i++;
            if (i > 500) mShouldClose = true;
        }

        /// Post main loop entry point systems' call

        gRenderSystem->postMainLoop();
    }

    void ApplicationContext::close()
    {

    }

    void ApplicationContext::destroy()
    {
        gSceneManager->destroy();
        SAFE_DELETE(gSceneManager);

        close(); // ?

        gRenderDriver->destroy();
        SAFE_DELETE(gRenderDriver);

        gRenderSystem->destroy();
        SAFE_DELETE(gRenderSystem);
    }

    SceneManager* ApplicationContext::getSceneManager() const
    {
        return gSceneManager;
    }

} // namespace Berserk