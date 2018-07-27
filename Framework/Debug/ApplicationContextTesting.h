//
// Created by Egor Orachyov on 12.07.2018.
//

#ifndef BERSERKENGINE_APPLICATIONCONTEXTTESTING_H
#define BERSERKENGINE_APPLICATIONCONTEXTTESTING_H

#include "Context/ApplicationContext.h"
#include "System/GLRenderSystem.h"

void RenderSystemTest()
{
    using namespace Berserk;

    GLRenderSystem render;
    ConfigTable table;

    render.init(table);

    printf("Name: %s Render: %s Shader: %s \n",
           render.getName().getChars(), render.getRenderName().getChars(), render.getShadingLanguageName().getChars());

    render.validate();
    render.destroy();
}

void ApplicationContextStructureTest()
{
    using namespace Berserk;

    ApplicationContext application;

    application.init();
    application.setup();
    application.run();
    application.close();
    application.destroy();
}

#endif //BERSERKENGINE_APPLICATIONCONTEXTTESTING_H
