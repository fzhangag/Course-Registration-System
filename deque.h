#ifndef DEQUE_H
#define DEQUE_H

#include <iostream>

using namespace std;

template <typename T>
struct Node {
  T data;
  Node<T>* next;
  Node<T>* prev;
};

template <typename T>
class Deque {
public:
  class Iterator;
  Deque();                    // construct an empty deque
  bool isEmpty();          // is the deque empty?
  int size();                 // return the number of items on the deque
  void addFirst(const T& item);   // insert the item at the front of the queue
  void addLast(const T& item);    // insert the item at the end of the queue
  T removeFirst();         // delete and return the first item in queue
  T removeLast();          // delete and return the last item in queue
  Iterator iterator() const;  // return an iterator that examines the 
                             // items in order from front to back
  
private:
  int counter;
  Node<T>* front;
  Node<T>* rear;
};

template <typename T>
class Deque<T>::Iterator {
public:
  Iterator();
  Iterator(const Deque&);
  T operator*();
  void operator++();
  Node<T>* itrNode;
};

template <typename T>
Deque<T>::Iterator::Iterator():itrNode(NULL) {}

template<typename T>
Deque<T>::Iterator::Iterator(const Deque<T>& d):itrNode(d.iterator().itrNode) {}

template <typename T>
T Deque<T>::Iterator::operator*() {
  return itrNode->data;
}

template <typename T>
void Deque<T>::Iterator::operator++() {
  itrNode = itrNode->next;
}

template <typename T>
Deque<T>::Deque():counter(0), front(NULL), rear(NULL) {}

template <typename T>
bool Deque<T>::isEmpty() {
  if (counter) {
    return false;
  }
  else {
    return true;
  }
}

template <typename T>
int Deque<T>::size() {
  return counter;
}

template <typename T>
void Deque<T>::addFirst(const T& item) {
  Node<T>* newNode = new Node<T>;
  newNode->data = item;
  newNode->next = front;
  newNode->prev = NULL;
  front = newNode;
  if (counter == 0) {
    rear = newNode;
  }
  if (front->next != NULL) {
    front->next->prev = front;
  }
  counter++;
}

template <typename T>
void Deque<T>::addLast(const T& item) {
  Node<T>* newNode = new Node<T>;
  newNode->data = item;
  newNode->prev = rear;
  newNode->next = NULL;
  rear = newNode;
  if (counter == 0) {
    front = newNode;
  }
  if (rear->prev != NULL) {
    rear->prev->next = rear;
  }
  counter++;
}

template <typename T>
T Deque<T>::removeFirst() {
  T t;
  if (isEmpty()) {
    cout << "Error: the deque is empty." << endl;
  }
  else {
    t = front->data;
    Node<T>* nextNode = front->next;
    delete front;
    front = nextNode;
    counter--;
  }
  return t;
}

template <typename T>
T Deque<T>::removeLast() {
  T t;
  if (isEmpty()) {
    cout << "Error: the deque is empty." << endl;
  }
  else {
    t = rear->data;
    Node<T>* prevNode = rear->prev;
    delete rear;
    rear = prevNode;
    counter--;
  }
  return t;
}

template <typename T>
typename Deque<T>::Iterator Deque<T>::iterator() const {
  Deque<T>::Iterator a;
  a.itrNode = front;
  return a;
}

#endif