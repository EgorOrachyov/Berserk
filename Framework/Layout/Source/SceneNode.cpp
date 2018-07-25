//
// Created by Egor Orachyov on 25.07.2018.
//

#include "Objects/SceneNode.h"
#include "Logging/LogMessages.h"

namespace Berserk
{

    SceneNode::SceneNode(SceneNode* root)
    {
        if (root == NULL)
        {
            WARNING("Pass NULL root pinter. Attempt to create Head Root");
        }

        mRoot = root;
        mActors.init(4);    // ensure capacity for 4 elements
        mChildren.init(4);

        mTransformation = root->getTransformation();
    }

    SceneNode::~SceneNode()
    {
        /// we should delete actors from
        /// the global memory storage and do
        /// it for each child of the node
    }

    void SceneNode::attachNode(SceneNode* node)
    {
        if (node == NULL)
        {
            WARNING("An attempt to attach NULL Scene Node");
            return;
        }

        mChildren.add(node);
    }

    void SceneNode::attachActor(Actor* actor)
    {
        if (actor == NULL)
        {
            WARNING("An attempt to attach NULL Actor");
            return;
        }

        mActors.add(actor);
    }

    const SceneNode* SceneNode::getRoot() const
    {
        return mRoot;
    }

    const ArrayList<Actor*> &SceneNode::getActors() const
    {
        return mActors;
    }

    const ArrayList<SceneNode*> &SceneNode::getNodes() const
    {
        return mChildren;
    }

    const Matrix4x4f& SceneNode::getTransformation() const
    {
        return mTransformation;
    }

    void SceneNode::update(const Matrix4x4f& rootTransformation)
    {
        Matrix4x4f pass = rootTransformation * mTransformation;

        for(UINT32 i = 0; i < mActors.getSize(); i++)
        {
            mActors.get(i)->update(pass);
        }

        for(UINT32 i = 0; i < mChildren.getSize(); i++)
        {
            mChildren.get(i)->update(pass);
        }
    }

} // namespace Berserk