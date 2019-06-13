#ifndef SORTLIST_H
#define SORTLIST_H

#include "register_common.h"

#include <functional>
#include <stdexcept>

using namespace std;

template<typename T, class Compare = less<T> >
class SortList {
    public:
        class Iterator;
        SortList();
        ~SortList();
        explicit SortList(const Compare& _comp);
        SortList(const SortList<T, Compare>& q);
        SortList<T, Compare>& operator=(const SortList<T, Compare>& q);
		inline bool isEmpty() const;
        inline int size() const;
        inline T* insert(T item);
        int remove();
        template<class CompRemove>
        int remove(CompRemove& _comp = CompRemove());
        Iterator iterator() const;
        
    protected:
        class Node;

    private:
        Node *head;
        Node *end;
        int length;
        Compare comparator;
};

template<typename T, class Compare>
class SortList<T, Compare>::Iterator {
    public:
        Iterator();
        virtual ~Iterator();
        Iterator(const SortList&);
        Iterator(Node&);
        Iterator(const Iterator&);
        Iterator& operator=(const Iterator&);
        T& operator*();
        T* operator->();
        T& operator*() const;
        T* operator->() const;
        bool operator==(const Iterator&) const;
        bool operator!=(const Iterator&) const;
        bool operator==(Iterator&) const;
        bool operator!=(Iterator&) const;
        bool operator==(const Iterator&);
        bool operator!=(const Iterator&);
        bool operator==(Iterator&);
        bool operator!=(Iterator&);
        virtual Iterator& operator++();
        virtual Iterator operator++(int);
        virtual Iterator& operator--();
        virtual Iterator operator--(int);
        
    protected:
        typedef Node Node_A;
        virtual Node* findValid() const;
        inline void shiftNode(Node* node);
        Node *elem;
};

template<typename T, class Compare>
class SortList<T, Compare>::Node {
    public:
        Node();
        ~Node();
        Node(const T&);
        Node(const Node&);
        Node& operator=(const Node&);
        static void decCnt(Node *node);
        T *data;
        Node *prev;
        Node *next;
        int cnt;
};

template<typename T, class Compare>
SortList<T, Compare>::Node::Node()
:data(NULL), prev(NULL), next(NULL),
 cnt(0) {
}

template<typename T, class Compare>
SortList<T, Compare>::Node::~Node() {
    if(data) {
        delete data;
    }
}

template<typename T, class Compare>
SortList<T, Compare>::Node::Node(const T& t)
:data(new T(t)), prev(NULL), next(NULL),
 cnt(0) {
}

template<typename T, class Compare>
SortList<T, Compare>::Node::Node(const SortList<T, Compare>::Node& t)
:data(new T(*(t.data))), prev(NULL), next(NULL),
 cnt(0) {
}

template<typename T, class Compare>
typename SortList<T, Compare>::Node& SortList<T, Compare>::Node::operator=(const SortList<T, Compare>::Node& t) {
    data = new T(t.data);
    return *this;
}

template<typename T, class Compare>
void SortList<T, Compare>::Node::decCnt(SortList<T, Compare>::Node *node) {
    node->cnt--;
    if(node->cnt == 0) {
        if(node->data) {
            delete node->data;
            node->data = NULL;
        }
        if(node->prev) {
            decCnt(node->prev);
        }
        if(node->next) {
            decCnt(node->next);
        }
        delete node;
    }
}

template<typename T, class Compare>
SortList<T, Compare>::Iterator::Iterator()
:elem(NULL) {
}

template<typename T, class Compare>
SortList<T, Compare>::Iterator::~Iterator() {
    if(elem) {
        Node::decCnt(elem);
    }
}

template<typename T, class Compare>
SortList<T, Compare>::Iterator::Iterator(const SortList<T, Compare>& q)
:elem(q.iterator().elem) {
    elem->cnt++;
}

template<typename T, class Compare>
SortList<T, Compare>::Iterator::Iterator(SortList<T, Compare>::Node& p)
:elem(&p) {
    elem->cnt++;
}

template<typename T, class Compare>
SortList<T, Compare>::Iterator::Iterator(const SortList<T, Compare>::Iterator& itr)
:elem(itr.findValid()) {
    if(elem) {
        elem->cnt++;
    }
}

template<typename T, class Compare>
typename SortList<T, Compare>::Iterator& SortList<T, Compare>::Iterator::operator=(const SortList<T, Compare>::Iterator& itr) {
    if(elem) {
        Node::decCnt(elem);
    }
    elem = itr.findValid();
    if(elem) {
        elem->cnt++;
    }
    return (*this);
}

template<typename T, class Compare>
T& SortList<T, Compare>::Iterator::operator*() {
    if(!elem) {
        throw runtime_error("Uninitialized iterator!");
    }
    Node* t = findValid();
    if(!t) {
        Node::decCnt(elem);
        elem = NULL;
        throw runtime_error("Iterator no longer valid!");
    }
    shiftNode(t);
    if(!elem->prev && elem->next) {
        throw runtime_error("Empty sortlist!");
    }
    if(!elem->next && elem->prev) {
        throw runtime_error("End of sortlist!");
    }
    return *(elem->data);
}

template<typename T, class Compare>
T* SortList<T, Compare>::Iterator::operator->() {
    return &(*(*this));
}

template<typename T, class Compare>
T& SortList<T, Compare>::Iterator::operator*() const{
    if(!elem) {
        throw runtime_error("Uninitialized iterator!");
    }
    Node* t = findValid();
    if(!t || elem != t) {
        throw runtime_error("Iterator no longer valid!");
    }
    if(!elem->prev && elem->next) {
        throw runtime_error("Empty sortlist!");
    }
    if(!elem->next && elem->prev) {
        throw runtime_error("End of sortlist!");
    }
    return *(elem->data);
}

template<typename T, class Compare>
T* SortList<T, Compare>::Iterator::operator->() const{
    return &(*(*this));
}

template<typename T, class Compare>
bool SortList<T, Compare>::Iterator::operator==(const SortList<T, Compare>::Iterator& itr) const {
    if(!elem || !itr.elem) {
        return false;
    }
    Node* t1 = findValid();
    Node* t2 = itr.findValid();
    if(!t1 || !t2 || t1 != elem || t2 != itr.elem) {
        return false;
    }
    return t1 == t2;
}

template<typename T, class Compare>
bool SortList<T, Compare>::Iterator::operator!=(const SortList<T, Compare>::Iterator& itr) const {
    return !((*this) == itr);
}

template<typename T, class Compare>
bool SortList<T, Compare>::Iterator::operator==(SortList<T, Compare>::Iterator& itr) const {
    if(!elem || !itr.elem) {
        return false;
    }
    Node* t1 = findValid();
    Node* t2 = itr.findValid();
    itr.shiftNode(t2);
    if(!t1 || !t2 || t1 != elem) {
        return false;
    }
    return t1 == t2;
}

template<typename T, class Compare>
bool SortList<T, Compare>::Iterator::operator!=(SortList<T, Compare>::Iterator& itr) const {
    return !((*this) == itr);
}

template<typename T, class Compare>
bool SortList<T, Compare>::Iterator::operator==(const SortList<T, Compare>::Iterator& itr) {
    if(!elem || !itr.elem) {
        return false;
    }
    Node* t1 = findValid();
    Node* t2 = itr.findValid();
    shiftNode(t1);
    if(!t1 || !t2 || t2 != itr.elem) {
        return false;
    }
    return t1 == t2;
}

template<typename T, class Compare>
bool SortList<T, Compare>::Iterator::operator!=(const SortList<T, Compare>::Iterator& itr) {
    return !((*this) == itr);
}

template<typename T, class Compare>
bool SortList<T, Compare>::Iterator::operator==(SortList<T, Compare>::Iterator& itr) {
    if(!elem || !itr.elem) {
        return false;
    }
    Node* t1 = findValid();
    Node* t2 = itr.findValid();
    shiftNode(t1);
    itr.shiftNode(t2);
    if(!t1 || !t2) {
        return false;
    }
    return t1 == t2;
}

template<typename T, class Compare>
bool SortList<T, Compare>::Iterator::operator!=(SortList<T, Compare>::Iterator& itr) {
    return !((*this) == itr);
}

template<typename T, class Compare>
typename SortList<T, Compare>::Iterator& SortList<T, Compare>::Iterator::operator++() {
    if(!elem) {
        throw runtime_error("Uninitialized iterator!");
    }
    if(!elem->prev && elem->next) {
        if(elem->next->next) {
            shiftNode(elem->next->next);
        }
    }
    Node* t = findValid();
    shiftNode(t);
    if(!elem) {
        throw runtime_error("Iterator no longer valid!");
    }
    if(!elem->next && elem->prev) {
        throw runtime_error("At the end of the sortlist!");
    }
    if(!elem->data) {
        throw runtime_error("Empty sortlist!");
    }
    shiftNode(elem->next);
    return (*this);
}

template<typename T, class Compare>
typename SortList<T, Compare>::Iterator SortList<T, Compare>::Iterator::operator++(int) {
    Iterator temp = *this;
    ++(*this);
    return temp;
}

template<typename T, class Compare>
typename SortList<T, Compare>::Iterator& SortList<T, Compare>::Iterator::operator--() {
    if(!elem) {
        throw runtime_error("Uninitialized iterator!");
    }
    Node* t = findValid();
    shiftNode(t);
    if(!elem) {
        throw runtime_error("Iterator no longer valid!");
    }
    if(!elem->prev && elem->next) {
        throw runtime_error("Empty sortlist!");
    }
    if(!elem->prev->prev) {
        throw runtime_error("At the beginning of the sortlist!");
    }
    do {
        shiftNode(elem->prev);
        t = findValid();
    }while(t != elem && t->prev->prev);
    return (*this);
}

template<typename T, class Compare>
typename SortList<T, Compare>::Iterator SortList<T, Compare>::Iterator::operator--(int) {
    Iterator temp = *this;
    --(*this);
    return temp;
}

template<typename T, class Compare>
typename SortList<T, Compare>::Node* SortList<T, Compare>::Iterator::findValid() const {
    if(elem->data) {
        return elem;
    }
    if(!elem->next && elem->prev) {
        return elem;
    }
    Node* t = elem;
    while(!t->data && t->next) {
        t = t->next;
    }
    if(t->data) {
        return t;
    }
    if(!t->prev) {
        return NULL;
    }
    return t->prev;
}

template<typename T, class Compare>
inline void SortList<T, Compare>::Iterator::shiftNode(SortList<T, Compare>::Node* node) {
    if(node) {
        node->cnt++;
        Node::decCnt(elem);
        elem = node;
    }
    else {
        Node::decCnt(elem);
        elem = NULL;
    }
}

template<typename T, class Compare>
SortList<T, Compare>::SortList()
:head(new Node()), end(new Node()),
 length(0), comparator(Compare()) {
    head->next = end;
    end->prev = head;
    head->cnt++;
    end->cnt++;
}

template<typename T, class Compare>
SortList<T, Compare>::~SortList() {
    Node *t = head;
    Node *p;
    while(t) {
        p = t;
        t = t->next;
        p->prev = NULL;
        p->next = NULL;
        if(p->data) {
            delete p->data;
            p->data = NULL;
        }
        Node::decCnt(p);
    }
}

template<typename T, class Compare>
SortList<T, Compare>::SortList(const SortList<T, Compare>& q)
:head(new Node()),
 end(new Node()),
 length(0), comparator(q.comparator) {
    Node *t;
    head->next = end;
    end->prev = head;
    head->cnt++;
    end->cnt++;
    for(t = q.head->next; t->next; t = t->next) {
        insert(*(t->data));
    }
}

template<typename T, class Compare>
SortList<T, Compare>::SortList(const Compare& _comp)
:head(new Node()), end(new Node()),
 length(0), comparator(_comp) {
    head->next = end;
    end->prev = head;
    head->cnt++;
    end->cnt++;
}

template<typename T, class Compare>
SortList<T, Compare>& SortList<T, Compare>::operator=(const SortList<T, Compare>& q) {
    Node *t;
    remove();
    comparator = q.comparator;
    for(t = q.head->next; t->next; t = t->next) {
        insert(*(t->data));
    }
    return (*this);
}

template<typename T, class Compare>
inline bool SortList<T, Compare>::isEmpty() const {
    return head->next == end;
}

template<typename T, class Compare>
inline int SortList<T, Compare>::size() const {
    return length;
}

template<typename T, class Compare>
inline T* SortList<T, Compare>::insert(T item) {
    Node *t = new Node(item);
    Node *p = head->next;
    t->cnt++;
    while(p != end && comparator(*(p->data), item)) {
        p = p->next;
    }
    t->next = p;
    t->prev = p->prev;
    t->prev->next = t;
    p->prev = t;
    length++;
    return t->data;
}

template<typename T, class Compare>
int SortList<T, Compare>::remove() {
    ReturnTrue<T> t = ReturnTrue<T>();
    return remove<ReturnTrue<T> >(t);
}

template<typename T, class Compare>
template<class CompRemove>
int SortList<T, Compare>::remove(CompRemove& _comp) {
    Node *t, *p;
    int cnt = 0;
    if(isEmpty()) {
        return 0;
    }
    t = head->next;
    while(t != end) {
        p = t;
        t = t->next;
        if(_comp(*(p->data))) {
            p->prev->next = t;
            t->prev = p->prev;
            p->prev->cnt++;
            t->cnt++;
            delete p->data;
            p->data = NULL;
            Node::decCnt(p);
            length--;
            cnt++;
        }
    }
    return cnt;
}

template<typename T, class Compare>
typename SortList<T, Compare>::Iterator SortList<T, Compare>::iterator() const {
    if(isEmpty()) {
        return Iterator(*head);
    }
    return Iterator(*(head->next));
}

#endif