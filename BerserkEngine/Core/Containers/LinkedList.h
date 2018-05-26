//
// Created by Egor Orachyov on 25.05.2018.
//

#ifndef BERSERKENGINE_LINKEDLIST_H
#define BERSERKENGINE_LINKEDLIST_H

#include "../Types.h"

namespace Berserk
{

    /**
     * Linked list based on block allocation in single
     * pre-allocated space for elements of equal size.
     * If list is full or nearly full, it will reallocate memory
     * and gen bigger buffer
     */
    template<typename Element> class LinkedList
    {
    private:

        /* List Node */
        typedef struct Node
        {
            Element data;
            Node* next;

        } Node;

    public:

        LinkedList()
        {

        }

        ~LinkedList()
        {

        }

        void Add(Element element)
        {

        }

        void Remove(Element element)
        {

        }

        void Empty()
        {

        }

        Element GetFirst()
        {

        }

        Element GetLast()
        {

        }

        void Iterate(bool restart)
        {

        }

        Element GetCurrent()
        {

        }

        Element GetNext(Element element)
        {

        }

        int32 GetTotalElements()
        {

        }

    private:

        int16 mNumOfElements;
        int16 mBufferSize;
        void* mBuffer;
        Node  mHead;
        Node  mFreeNodes;
        Node* mCurrent;

    };

} // namespace Berserk

#endif //BERSERKENGINE_LINKEDLIST_H
