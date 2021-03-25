/**
 * @file ProfinetDataStructure.h
 * @brief Header file for class ProfinetDataStructure
 * @date 14/01/2021
 * @author Giuseppe Avon
 * @author Pedro Lourenco
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
 *
 * @details This header file contains the declaration of the class ProfinetDataStructure
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_PROFINET_DATASTRUCTURE_H
#define DATASOURCES_PROFINET_DATASTRUCTURE_H

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#ifndef LINT
//We need stddef just for the NULL constant, evaluate if a define can be better?
#include <stddef.h>
#endif

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Forward declarations                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/*lint -e1066 Disabled because lint gets confused with standard C headers */
/*lint -e1714 Excess messages dealing with templates and their usage */
/*lint -e1795 Excess messages dealing with templates and their usage */

namespace ProfinetDataSourceDriver {
/**
 * @brief Implementation of classes for the ProfinetDataStructure
 *
 * @details Implements a generic double-linked list with the Sentinel Node approach.
 * Provides also a convenient iterator (forward and reverse) for easy traversal.
 */

/**
 * @brief Template for class SimpleLinkedListNode declaration.
 */
template <class T>
class SimpleLinkedListNode;

/**
 * @brief Template for class SimpleLinkedListIterator declaration.
 */
template <class T>
class SimpleLinkedListIterator;

/**
 * @brief Template for class SimpleLinkedList declaration.
 */
template <class T>
class SimpleLinkedList;

/**
 * @brief Template for class SimpleLinkedListNode.
 */
template <class T>
class SimpleLinkedListNode {
public:

    /**
     * @brief Memory holding the previous node.
     */
    SimpleLinkedListNode<T>    *previousNode;

    /**
     * @brief Memory holding the next node.
     */
    SimpleLinkedListNode<T>    *nextNode;

    /**
     * @brief The data on the current node.
     */
    T                           nodeData;
};

/**
 * @brief Template for class SimpleLinkedListIterator.
 */
template <class T>
class SimpleLinkedListIterator {
public:

    /**
     * @brief Sets the current node to the first node on the current list.
     * @details Sets the current node to the next node of the sentinel node on
     * the current list.
     * @pre
     *   GetIterator
     */
    void First();

    /**
     * @brief Gets the data on the node and moves the iterator to the next node.
     * @details Gets the data on the current node and then sets the current node
     * to the next node on the linked list. Performs the operations only if the
     * current node is not the sentinel node.
     * @return The type T node data on the current node.
     * @pre
     *   GetIterator
     */
    T Next();

    /**
     * @brief Sets the current node to the last node on the current list.
     * @details Sets the current node to the previous node of the sentinel node
     * on the current list.
     * @pre
     *   GetIterator
     */
    void Last();

    /**
     * @brief Gets the data on the node and moves the iterator to the previous node.
     * @details Gets the data on the current node and then sets the current node
     * to the previous node on the linked list. Performs the operations only if the
     * current node is not the sentinel node.
     * @return The type T node data on the current node.
     * @pre
     *   GetIterator
     */
    T Previous();

private:

    /**
     * Allow private access by SimpleLinkedList class methods.
     */
    friend class SimpleLinkedList<T>;

    /**
     * @brief Set the current list for the linked list iterator.
     * @param[in] currentList The current list for the iterator.
     */
    /*lint -e{1704} Private constructor is intentional here as part of the iterator pattern. */
    SimpleLinkedListIterator(SimpleLinkedList<T>* currentListParam);

    /**
     * Memory holding the current list.
     */
    SimpleLinkedList<T>        *currentList;

    /**
     * Memory holding the current node.
     */
    SimpleLinkedListNode<T>    *currentNode;
};

/**
 * @brief Template for class SimpleLinkedList.
 */
template <class T>
class SimpleLinkedList {
public:

    /**
     * @brief Default constructor.
     * @details Creates the sentinel node and sets the next node and the
     * previous node to the created sentinel node.
     */
    SimpleLinkedList();

    /**
     * @brief Destructor.
     * @details Calls the EmptyLinkedList method and deletes the sentinel node.
     */
    ~SimpleLinkedList();

    /**
     * @brief Inserts a new node at the head of the sentinel node.
     * @details Inserts a new node with type T data between the sentinel node
     * and its next node.
     * @param[in] nodeData The type T node data for the new node.
     * @return Returns true if the new node was created, otherwise false.
     */
    bool InsertHead(T nodeData);

    /**
     * @brief Inserts a new node at the tail of the sentinelNode.
     * @details Inserts a new node with type T data between the sentinel node
     * and its previous node.
     * @param[in] nodeData The type T node data for the new node.
     * @return Returns true if the new node was created, otherwise false.
     */
    bool InsertTail(T nodeData);

    /**
     * @brief Removes the node at the head of the sentinelNode.
     * @details Removes the node corresponding to the next node of the sentinel node.
     * @return Returns type T data of the removed node.
     */
    T RemoveHead();

    /**
     * @brief Removes the node at the tail of the sentinelNode.
     * @details Removes the node corresponding to the previous node of the sentinel node.
     * @return Returns type T data of the removed node.
     */
    T RemoveTail();

    /**
     * @brief Empties the linked list of all created nodes.
     * @details Removes all existing nodes on the linked list with exception
     * of the sentinel node.
     */
    void EmptyLinkedList();

    /**
     * @brief Gets the status of the linked list.
     * @details Reports the status of the linked list regarding the presence
     * of other nodes apart from  the sentinel node.
     * @return Returns true if there the linked list has other nodes besides the
     * sentinel node, otherwise false.
     */
    bool LinkedListStatus();

    /**
     * @brief Gets a new iterator to the linked list.
     * @details The caller method is responsbible for destructing the iterator.
     * @return Returns the linked list iterator or NULL if the operation was not successfull.
     */
    SimpleLinkedListIterator<T>* GetIterator();

private:

    /**
     * Allow private access by SimpleLinkedListIterator class methods.
     */
    friend class SimpleLinkedListIterator<T>;

    /**
     * Memory holding the sentinel node.
     */
    SimpleLinkedListNode<T>    *sentinelNode;

    /**
     * @brief Appends a new node to the current node of the linked list.
     * @param[in] currentNode The current node.
     * @param[in] newNode The new node to be appended.
     */
    void BaseAppendNode(SimpleLinkedListNode<T> *currentNode, SimpleLinkedListNode<T> *newNode);

    /**
     * @brief Removes a node of the linked list.
     * @param[in] node The node to be removed.
     * @return the removed node.
     */
    SimpleLinkedListNode<T>* BaseRemoveNode(SimpleLinkedListNode<T> *node);

    /**
     * @brief Appends a new node to the head of the linked list.
     * @details Inserts a new node between the sentinel node and its next node
     * using the BaseAppendNode method.
     * @param[in] newNode The new node to be appended.
     */
    void AppendHead(SimpleLinkedListNode<T> *newNode);

    /**
     * @brief Appends a new node to the tail of the linked list.
     * @details Inserts a new node between the sentinel node and its previous node
     * using the BaseAppendNode method.
     * @param[in] newNode The new node to be appended.
     */
    void AppendTail(SimpleLinkedListNode<T> *newNode);
};
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace ProfinetDataSourceDriver {

    template <class T>
    void SimpleLinkedListIterator<T>::First() {
        currentNode = currentList->sentinelNode->nextNode;
    }

    template <class T>
    T SimpleLinkedListIterator<T>::Next() {
        T tempNodeData = T();
        if (currentNode != currentList->sentinelNode) {
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
        T tempNodeData = T();
        if (currentNode != currentList->sentinelNode) {
            tempNodeData = currentNode->nodeData;
            currentNode = currentNode->previousNode;
        }
        return tempNodeData;
    }

    template <class T>
    SimpleLinkedListIterator<T>::SimpleLinkedListIterator(SimpleLinkedList<T>* currentListParam) {
        this->currentList = currentListParam;
    }

    template <class T>
    SimpleLinkedList<T>::SimpleLinkedList() {
	//lint -e{1713,1732,1733} POCO object does not need copy ctor and assignment operator overload
        sentinelNode = new SimpleLinkedListNode<T>();
        sentinelNode->nextNode = sentinelNode;
        sentinelNode->previousNode = sentinelNode;
    }

    template <class T>
    SimpleLinkedList<T>::~SimpleLinkedList() {
        EmptyLinkedList();
        if (sentinelNode != NULL) {
            delete sentinelNode;
            sentinelNode = NULL;
        }
    }

    template <class T>
    bool SimpleLinkedList<T>::InsertHead(T nodeData) {
        bool returnValue = false;
        SimpleLinkedListNode<T> *newNode = new SimpleLinkedListNode<T>();
        returnValue = (newNode != NULL);
        if (returnValue) {
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
        if (returnValue) {
            newNode->nodeData = nodeData;
            AppendTail(newNode);
        }
        return returnValue;
    }

    template <class T>
    T SimpleLinkedList<T>::RemoveHead() {
        SimpleLinkedListNode<T> *tempNode = NULL;
        T tempNodeData = T();
        if (sentinelNode->nextNode != sentinelNode) {
            tempNode = BaseRemoveNode(sentinelNode->nextNode);
            tempNodeData = tempNode->nodeData;
            delete tempNode;
        }
        return tempNodeData;
    }

    template <class T>
    T SimpleLinkedList<T>::RemoveTail() {
        SimpleLinkedListNode<T> *tempNode = NULL;
        T tempNodeData = T();
        if (sentinelNode->previousNode != sentinelNode) {
            tempNode = BaseRemoveNode(sentinelNode->previousNode);
            tempNodeData = tempNode->nodeData;
            delete tempNode;
        }
        return tempNodeData;
    }

    template <class T>
    void SimpleLinkedList<T>::EmptyLinkedList() {
        SimpleLinkedListNode<T> *tempNode = NULL;
        bool ok = true;
        while (ok) {
            ok = (sentinelNode->nextNode != sentinelNode);
            if (ok) {
                tempNode = BaseRemoveNode(sentinelNode->nextNode);
                if (tempNode != NULL) {
                    delete tempNode;        
                }
            }
        }
    }

    template <class T>
    bool SimpleLinkedList<T>::LinkedListStatus() {
        bool ok = true;
        if (ok) {
            ok = (sentinelNode->previousNode != sentinelNode);
        }
        if (ok) {
            ok = (sentinelNode->nextNode != sentinelNode);
        }
        return ok;
    }

    template <class T>
    SimpleLinkedListIterator<T>* SimpleLinkedList<T>::GetIterator() {
        SimpleLinkedListIterator<T> *tempIterator = NULL;
        tempIterator = new SimpleLinkedListIterator<T>(this);
        return tempIterator;
    }

    template <class T>
    void SimpleLinkedList<T>::BaseAppendNode(SimpleLinkedListNode<T> *currentNode, SimpleLinkedListNode<T> *newNode) {
        newNode->nextNode = currentNode->nextNode;
        newNode->previousNode = currentNode;
        currentNode->nextNode->previousNode = newNode;
        currentNode->nextNode = newNode;
    }

    template <class T>
    SimpleLinkedListNode<T>* SimpleLinkedList<T>::BaseRemoveNode(SimpleLinkedListNode<T> *node) {
        if (node != sentinelNode) {
            node->previousNode->nextNode = node->nextNode;
            node->nextNode->previousNode = node->previousNode;
            node->previousNode = node->nextNode = NULL;
        }
        return node;
    }

    template <class T>
    void SimpleLinkedList<T>::AppendHead(SimpleLinkedListNode<T> *newNode) {
        BaseAppendNode(sentinelNode, newNode);
    }

    template <class T>
    void SimpleLinkedList<T>::AppendTail(SimpleLinkedListNode<T> *newNode) {
        BaseAppendNode(sentinelNode->previousNode, newNode);
    }
}
/*lint +e1066 Enabled again after exception has been useful */
/*lint +e1714 Excess messages dealing with templates and their usage */
/*lint +e1795 Excess messages dealing with templates and their usage */

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PROFINET_DATASOURCE_DATASTRUCTURE_H_ */
