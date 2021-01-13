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

namespace ProfinetDataSourceDriver {

    template <class T>
    SimpleLinkedListIterator<T>::SimpleLinkedListIterator(SimpleLinkedList<T>* currentList) {
        this->currentList = currentList;
    }

    template <class T>
    void SimpleLinkedListIterator<T>::First() {
         currentNode = currentList->sentinelNode->nextNode;
     }

    template <class T>
    T SimpleLinkedListIterator<T>::Next() {
        T tempNodeData = NULL;

        if(currentNode != currentList->sentinelNode) {
            tempNodeData = currentNode->nodeData;
            currentNode = currentNode->nextNode;
        }

        return tempNodeData;
    }

    template <class T>
    void SimpleLinkedListIterator<T>::Last() {
        currentNode = currentList->sentinelNode->previousNode;
    }

    template <class T>
    T SimpleLinkedListIterator<T>::Previous() {
        T tempNodeData = NULL;

        if(currentNode != currentList->sentinelNode) {
            tempNodeData = currentNode->nodeData;
            currentNode = currentNode->previousNode;
        }
        
        return tempNodeData;
    }

    template <class T>
    void SimpleLinkedList<T>::BaseAppendNode(SimpleLinkedListNode<T> *currentNode, SimpleLinkedListNode<T> *newNode) {
        newNode->nextNode = currentNode->nextNode;
        newNode->previousNode = currentNode;
        currentNode->nextNode->previousNode = newNode;
        currentNode->nextNode = newNode;
    } 

    template <class T>
    void SimpleLinkedList<T>::AppendHead(SimpleLinkedListNode<T> *newNode) {
        BaseAppendNode(sentinelNode, newNode);
    }

    template <class T>
    void SimpleLinkedList<T>::AppendTail(SimpleLinkedListNode<T> *newNode) {
        BaseAppendNode(sentinelNode->previousNode, newNode);
    }

    template <class T>
    SimpleLinkedListNode<T>* SimpleLinkedList<T>::BaseRemoveNode(SimpleLinkedListNode<T> *node) {
        if(node != sentinelNode) {
            node->previousNode->nextNode = node->nextNode;
            node->nextNode->previousNode = node->previousNode;
            node->previousNode = node->nextNode = NULL;
        }
        return node;
    }

    template <class T>
    SimpleLinkedList<T>::SimpleLinkedList() {
        sentinelNode = new SimpleLinkedListNode<T>();
        sentinelNode->nextNode = sentinelNode;
        sentinelNode->previousNode = sentinelNode;
    }

    template <class T>
    SimpleLinkedList<T>::~SimpleLinkedList() {
        //TODO: Better to check if the SimpleLinkedList is empty and eventually emptying it?
        delete sentinelNode;
    }

    template <class T>
    bool SimpleLinkedList<T>::InsertHead(T nodeData) {
        bool returnValue = false;

        SimpleLinkedListNode<T> *newNode = new SimpleLinkedListNode<T>();
        returnValue = (newNode != NULL);

        if(returnValue) {
            newNode->nodeData = nodeData;
            AppendHead(newNode);
        }

        return returnValue;
    }

    template <class T>
    bool SimpleLinkedList<T>::InsertTail(T nodeData) {
        bool returnValue = false;

        SimpleLinkedListNode<T> *newNode = new SimpleLinkedListNode<T>();
        returnValue = (newNode != NULL);

        if(returnValue) {
            newNode->nodeData = nodeData;
            AppendTail(newNode);
        }

        return returnValue;
    }

    template <class T>
    T SimpleLinkedList<T>::RemoveHead() {
        SimpleLinkedListNode<T> *tempNode = BaseRemoveNode(sentinelNode->nextNode);
        T tempNodeData = tempNode->nodeData;
        delete tempNode;

        return tempNodeData;
    }

    template <class T>
    T SimpleLinkedList<T>::RemoveTail() {
        SimpleLinkedListNode<T> *tempNode = BaseRemoveNode(sentinelNode->previousNode);
        T tempNodeData = tempNode->nodeData;
        delete tempNode;

        return tempNodeData;
    }

    template <class T>
    SimpleLinkedListIterator<T>* SimpleLinkedList<T>::GetIterator() {
        SimpleLinkedListIterator<T> *tempIterator = new SimpleLinkedListIterator<T>(this);
        return tempIterator;
    }

}

#endif /* __PROFINET_DATASOURCE_SIMPLELINKEDLIST_H__ */