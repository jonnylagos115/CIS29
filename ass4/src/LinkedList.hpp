#pragma once
#include <iostream>
#include <string>
#include <cstdlib>

template <typename T>
class Node
{
private:
    T data_;
    Node* next_;
    Node(const Node&) = delete; // disabled copy constructor
    Node& operator=(const Node&) = delete; //disabled assignment operator
public:
    Node();
    Node(T d, Node*n);
    const T& data() const;
    T& data();
    Node* next() const;
    Node*& next();
};

template <typename T>
Node<T>::Node() : data_(), next_(0)
{}

template <typename T>
Node<T>::Node(T d, Node* n) : data_(d), next_(n)
{}

template <typename T>
const T& Node<T>::data() const
{
    return data_;
}

template <typename T>
T& Node<T>::data()
{
    return data_;
}

template <typename T>
Node<T>* Node<T>::next() const
{
    return next_;
}

template <typename T>
Node<T>*& Node<T>::next()
{
    return next_;
}

template <typename T>
class List
{
private:
    Node<T>* top_;
    int size;
    List(const List&) = delete;
    List& operator=(const List&) = delete;  // disable assignment operator
public:
    List();
    ~List();
    void push(T object);
    T pop();
    const Node<T>* top() const;
    bool remove(T object);
    const Node<T>* find(T object) const;
    int numOfNodes() const { return size; }
};

template<typename T> 
std::ostream& operator<<(std::ostream& out, const List<T>& L)
{
    const Node<T>* ptr = L.top();
    while (ptr)
    {
        out << (*ptr) << ' ';
        ptr = ptr -> next();
    }
    return out;
}

template<typename T> 
List<T>::List() : top_(0), size(0){}

template<typename T> 
List<T>::~List()
{
    Node<T>* ptr = top_;
    while (ptr)
    {
        top_ = top_->next();
        delete ptr;
        ptr = top_;
    }
}

template<typename T> 
void List<T>::push(T object)
{
    Node<T>* ptr = new Node<T>(object, top_);
    top_ = ptr;
    size++;
}

template<typename T> 
const Node<T>* List<T>::top() const
{
    return top_;
}

template<typename T> 
T List<T>::pop()
{
    Node<T>* ptr = top_;
    top_ = top_ -> next();
    T data = ptr->data();
    delete ptr;
    return data;
}

template<typename T> 
const Node<T>* List<T>::find(T object) const
{
    const Node<T>* ptr = top();
    while (ptr)
    {
        if (ptr->data() == object)
        {
            return ptr;
        }
        ptr = ptr->next();
    }
    return 0;
}

template<typename T> 
bool List<T>::remove(T object)
{
    if (!find(object))
    {
        std::cerr << object << " not found\n";
        return false;
    }
    Node<T>* ptr2current = top_;
    Node<T>* ptr2previous = top_;
    if (top_->data() == object)
    {
        top_ = top_ -> next();
        delete ptr2current;
        return true;
    }
    while (ptr2current)
    {
        ptr2current = ptr2current->next();
        if (ptr2current->data() == object)
        {
            ptr2previous->next() = ptr2current->next();
            delete ptr2current;
            return true;
        }
        ptr2previous = ptr2current;
    }
    return false;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Node<T>& N)
{
    out << N.data();
    return out;
}