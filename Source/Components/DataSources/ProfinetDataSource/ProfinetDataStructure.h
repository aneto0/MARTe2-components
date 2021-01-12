/**
 * @file ProfinetDataStructure.h
 * @brief   Simple generic double lihked list with iterator.
 * @date 
 * @author
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details     Implements a generic double-linked list with the Sentinel Node approach.
 *              Provides also a convenient iterator (forward and reverse) for easy traversal.
 */

#ifndef __PROFINET_DATASOURCE_DATASTRUCTURE_H__
#define __PROFINET_DATASOURCE_DATASTRUCTURE_H__

#include <stddef.h>

namespace ProfinetDataSourceDriver {

    template <class T>
    class SimpleLinkedListNode;

    template <class T>
    class SimpleLinkedListIterator;

    template <class T>
    class SimpleLinkedList;

    template <class T>
    class SimpleLinkedListNode {
        public:
            SimpleLinkedListNode<T>    *previousNode;
            SimpleLinkedListNode<T>    *nextNode;
            T                           nodeData;
    };

    template <class T>
    class SimpleLinkedListIterator {
        private:
            friend class SimpleLinkedList<T>;
            
            SimpleLinkedList<T>        *currentList;
            SimpleLinkedListNode<T>    *currentNode;
            SimpleLinkedListIterator(SimpleLinkedList<T>* currentList);

        public:
            void First();
            T Next();
            void Last();
            T Previous();
    };

    template <class T>
    class SimpleLinkedList {
        private:
            friend class SimpleLinkedListIterator<T>;

            SimpleLinkedListNode<T>    *sentinelNode;
            void BaseAppendNode(SimpleLinkedListNode<T> *currentNode, SimpleLinkedListNode<T> *newNode);
            void AppendHead(SimpleLinkedListNode<T> *newNode);
            void AppendTail(SimpleLinkedListNode<T> *newNode);
            SimpleLinkedListNode<T>* BaseRemoveNode(SimpleLinkedListNode<T> *node);
   
        public:
            SimpleLinkedList();
            ~SimpleLinkedList();

            bool InsertHead(T nodeData);
            bool InsertTail(T nodeData);

            T RemoveHead();
            T RemoveTail();

            SimpleLinkedListIterator<T>* GetIterator();
    };

}

#endif /* __PROFINET_DATASOURCE_SIMPLELINKEDLIST_H__ */