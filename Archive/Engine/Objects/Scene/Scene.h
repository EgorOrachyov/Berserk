//
// Created by Egor Orachyov on 28.07.2018.
//

#ifndef BERSERKENGINE_SCENE_H
#define BERSERKENGINE_SCENE_H

#include "Objects/Object.h"
#include "Objects/Actor.h"

namespace Berserk
{
    enum SceneInfo
    {
        SI_MAX_NUM_OF_LAYOUTS = 2,

        SI_LAYOUT_0 = 0,
        SI_LAYOUT_1 = 1
    };

    class Scene : public Object
    {
    public:

        Scene(const CStaticString &name);
        virtual ~Scene();

        virtual void onInit();
        virtual void onUpdate(FLOAT64 delta);
        virtual void onReset();
        virtual void onDestroy();

        Actor &getRoot(UINT32 layout = 0);

    private:

        friend class SceneManager;

        void init();
        void process(UINT32 layout, FLOAT64 delta);
        void reset();
        void destroy();

    private:

        Actor *mRoots[SceneInfo::SI_MAX_NUM_OF_LAYOUTS];

    };

} // namespace Berserk

#endif //BERSERKENGINE_SCENE_H