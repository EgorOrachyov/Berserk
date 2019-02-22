//
// Created by Egor Orachyov on 14.02.2019.
//

#ifndef BERSERK_IRESOURCEREFERENCE_H
#define BERSERK_IRESOURCEREFERENCE_H

namespace Berserk
{

    /**
     * Smart resource reference which handles pointer to the resource
     * and pointer to the manager, which serves (create, store, delete, manage)
     * resources of that type
     *
     * @note each resource reference should be explicitly implemented
     *       by each kind of resource system
     *
     * @tparam T resource type
     */
    template <class T>
    class IResourceReference
    {
    public:

        T* operator -> ()
        {
            FATAL("Null pointer resource reference");
            return mResource;
        }

    private:

        T* mResource;

    };

} // namespace Berserk

#endif //BERSERK_IRESOURCEREFERENCE_H
