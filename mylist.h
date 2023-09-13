#ifndef MyList_H
#define MyList_H

#include<iostream>
#include "flightticket.h"
// include additional header files if needed

using namespace std;

template <typename T>
class Node
{
    private:
        T hashnode; //data element
        Node* next; //Link (pointer) to the next Node
        Node* prev; //Link (pointer) to the previous Node
        template <typename T2> friend class MyList;
    friend class FlightHASHTABLE;
    public:
        Node(): next(NULL), prev(NULL)
        {}
        Node(T hashnode) : hashnode(hashnode),next(NULL), prev(NULL)
        {}
};

template <typename T>
class MyList
{
    public:
        class Iterator
        {
            public:
                friend class FlightHASHTABLE;
            public:
                Iterator() // constructor
                {
                    this->ptr = nullptr;
                }
                Iterator(Node<T>* v) // constructor
                {
                    this->ptr = ptr;
                }
                Node<T>& operator*() //return hashnode
                {
                    return ptr->hashnode;
                }
                bool operator==(const Iterator& p) const // comparing
                {
                    return this->ptr == p.ptr;
                }
                bool operator!=(const Iterator& p) const // comparing
                {
                    return this->ptr != p.ptr;
                }
                Iterator& operator++() //next node
                {
                    ptr = ptr->next;
                    return *this;
                }
                Iterator& operator--() //prev node
                {
                    ptr=ptr->prev;
                    return *this;
                }
            template <typename T2> friend class MyList;
            private:
                Node<T>* ptr;
        };
    public:
        MyList();
        ~MyList();
        int size() const;
        bool empty() const; // is list empty?
        Iterator begin() const;
        Iterator end() const;
        const T& front() const; // get the value (element) from front Node<T> of list
        const T& back() const;  // get the value (element) from last Node<T> of the List
        void addBefore(Node<T> *ptr, const T& hashnode);     //add a new node before ptr
        void addFront(const T& hashnode); // add a new node to the front of list
        void addBack(const T& hashnode); //add a new node to the back of the list
        void removeFront(); // remove front node from list
        void removeBack();  // remove last node from list
        void remove(const Iterator i);
        void display() const;    // display all element of the list
        friend class FlightHASHTABLE;
    private:
        Node<T>* header; //header
        Node<T>* trailer; // trailer
        int size_n = 0;
        int nn = 0;
};
template <typename T>
MyList<T>::MyList()
{
    this->header = new Node<T>();
    this->trailer = new Node<T>();
    this->header->next = trailer;
    this->trailer->prev = header;
    this->size_n = 0;
}
//============================================================
template <typename T>
MyList<T>::~MyList() // destructor to clean up all nodes
{
    while(!empty())
        removeFront();
}
//============================================================
template <typename T>
bool MyList<T>::empty() const // is list empty?
{
    return this->header->next == trailer;
}
//============================================================
template <typename T>
const T& MyList<T>::front() const // get front element
{
    if(!empty())
        return this->header->next->hashnode;
    else
        cout<<"List is Empty"<<endl;
}
//============================================================
template <typename T>
const T& MyList<T>::back() const // get front element
{
    if(!empty())
        return this->trailer->prev->hashnode;
    else
        cout<<"List is Empty"<<endl;
}

template <typename T>
void MyList<T>::remove(const Iterator i) //deleting
{
    if(!empty())
    {
        Node<T> *tmp_prev = i.ptr->prev;
        Node<T> *tmp_next = i.ptr->next;
        tmp_prev->next = tmp_next;
        tmp_next->prev = tmp_prev;
        delete i.ptr;
        size_n = size_n - 1;
    }
}
//============================================================
template <typename T>
void MyList<T>::addBefore(Node<T> *ptr, const T& hashnode) //adding before ptr
{
    Node<T> *tmp = new Node<T>(hashnode);
    tmp->next = ptr;
    tmp->prev = ptr->prev;
    ptr->prev->next = tmp;
    ptr->prev=tmp;
    size_n = size_n + 1;
}
//============================================================
template <typename T>
void MyList<T>::addFront(const T& hashnode) // add to front of list
{
    addBefore(this->header->next, hashnode);
}
//============================================================
template <typename T>
void MyList<T>::addBack(const T& hashnode) // add to Back of the list
{
    addBefore(this->trailer, hashnode);
}
//============================================================
template <typename T>
void MyList<T>::removeFront() // remove front item from list
{
    if(!empty())
    {
        Node<T> *tmp = this->header->next;
        this->header->next = tmp->next;
        tmp->next->prev = header;
        delete tmp;
        size_n = size_n - 1;
    }
}
//============================================================
template <typename T>
void MyList<T>::removeBack() // remove last item from list
{
    if(!empty())
    {
        Node<T> *tmp = this->trailer->prev;
        this->trailer->prev = tmp->prev;
        tmp->prev->next = trailer;
        delete tmp;
        size_n = size_n - 1;
    }
}
//============================================================
template <typename T>
typename MyList<T>::Iterator MyList<T>::begin() const //first node
{
    return Iterator(header->next);
}
//============================================================
template <typename T>
typename MyList<T>::Iterator MyList<T>::end() const //last node
{
    return Iterator(trailer);
}

template <typename T>
int MyList<T>::size() const //size
{
    return size_n;
}



#endif
