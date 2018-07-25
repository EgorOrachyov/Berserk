//
// Created by Egor Orachyov on 25.07.2018.
//

#ifndef BERSERKENGINE_SCENENODE_H
#define BERSERKENGINE_SCENENODE_H

#include "Objects/Actor.h"
#include "Containers/ArrayList.h"
#include "Math/Matrix4x4f.h"

namespace Berserk
{

    class SceneNode
    {
    public:

        SceneNode(SceneNode* root);
        ~SceneNode();

        void attachNode(SceneNode* node);
        void attachActor(Actor* actor);

        const SceneNode* getRoot() const;
        const ArrayList<Actor*> &getActors() const;
        const ArrayList<SceneNode*> &getNodes() const;
        const Matrix4x4f& getTransformation() const;

    protected:

        void update(const Matrix4x4f& rootTransformation);

    private:

        SceneNode* mRoot;
        ArrayList<Actor*> mActors;
        ArrayList<SceneNode*> mChildren;

        Matrix4x4f mTransformation;

    };

} // namespace Berserk

#endif //BERSERKENGINE_SCENENODE_H