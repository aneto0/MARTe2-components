#include "ProfinetDataStructure.h"

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