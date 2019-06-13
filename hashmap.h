#ifndef HASHMAP_H
#define HASHMAP_H

#include "register_common.h"
#include "sortlist.h"

#include <functional>
#include <stdexcept>

using namespace std;

template<typename Key, typename Value, class Hasher, class Compare = less<Value> >
class Hashmap {
    public:
        class Iterator;
        class Iterator_All;
        Hashmap();
        ~Hashmap();
        explicit Hashmap(Hasher _hash, int _slot = 503, Compare _comp = Compare());
        Hashmap(const Hashmap&);
        Hashmap<Key, Value, Hasher, Compare>& operator=(const Hashmap<Key, Value, Hasher, Compare>&);
        Value* operator[](const Key&);
		inline bool isEmpty() const;
        inline int size() const;
        void insert(const Key& hashKey, const Value& item);
        int remove(const Key& hashKey) {
            return remove<ReturnTrue<Value> >(hashKey);
        }
		
        template<class CompRemove>
        int remove(const Key& hashKey) {
            CompRemove t = CompRemove();
            return remove<CompRemove>(hashKey, t);
        }

        template<class CompRemove>
        int remove(const Key& hashKey, CompRemove& _comp);
        Iterator query(const Key& hashKey) const;
        template<class Callback>
        void apply(const Key& hashKey, Callback& _cbk) const;

        Iterator_All dump() const;

    private:
        struct Node {
            Key k;
            Value val;
        };

        class _Compare {
            public:
                _Compare() {}
                _Compare(const Compare &_cmp): cmp(_cmp) {}
                _Compare& operator=(const _Compare& c) {
                    cmp = c.cmp;
                    return (*this);
                }
                bool operator()(const Node &a, const Node &b) {
                    return cmp(a.val, b.val);
                }
                bool operator()(const Value &a, const Value &b) {
                    return cmp(a, b);
                }
            private:
                Compare cmp;
        };
        _Compare comp;

        template<class CompRemove>
        class _CustomRemove {
            public:
                _CustomRemove(const Key& hashKey, const CompRemove& _comp)
                :k(hashKey), cmp(_comp) {}
                bool operator()(const Node& n) {return (n.k == k) && cmp(n.val);}
            private:
                Key k;
                CompRemove  cmp;
        };

        SortList<Node, _Compare> **arr_bucket;

        int slotn;

        int length;

        Hasher hashFunctoin;
};

template<typename Key, typename Value, class Hasher, class Compare>
class Hashmap<Key, Value, Hasher, Compare>::Iterator: public SortList<Node, _Compare>::Iterator {
    public:
        Iterator();
        virtual ~Iterator();

        Iterator(const SortList<Node, _Compare>&, const Key&);
        
        Iterator(const Iterator&);
        
        Iterator& operator=(const Iterator&);
        Value& operator*();
        Value* operator->();
        Value& operator*() const;
        Value* operator->() const;

    protected:
        virtual typename SortList<Node, _Compare>::Iterator::Node_A* findValid() const;

    private:
        Key hashKey;
};

template<typename Key, typename Value, class Hasher, class Compare>
class Hashmap<Key, Value, Hasher, Compare>::Iterator_All {
    public:
        Iterator_All();
        Iterator_All(const Hashmap<Key, Value, Hasher, Compare>&);
        
        Iterator_All(const Iterator_All&);
        
        Iterator_All& operator=(const Iterator_All&);

        Value& operator*();
        Value* operator->();
        Value& operator*() const;
        Value* operator->() const;
        Iterator_All& operator++();
        Iterator_All operator++(int);

    private:
        void removeNull();
        typename SortList<Node, _Compare>::Iterator* findSmall();
        int size;
        typename SortList<Node, _Compare>::Iterator** arr_itr;
        typename SortList<Node, _Compare>::Iterator* current;
        _Compare comp;
};

template<typename Key, typename Value, class Hasher, class Compare>
Hashmap<Key, Value, Hasher, Compare>::Iterator_All::Iterator_All()
:size(0), arr_itr(NULL) {
}

template<typename Key, typename Value, class Hasher, class Compare>
Hashmap<Key, Value, Hasher, Compare>::Iterator_All::Iterator_All(const Hashmap<Key, Value, Hasher, Compare>& h)
:size(h.slotn), arr_itr(new typename SortList<Node, _Compare>::Iterator*[size]), comp(h.comp) {
    int i;
    for(i = 0; i < size; i++) {
        arr_itr[i] = new typename SortList<Node, _Compare>::Iterator(*(h.arr_bucket[i]));
    }
    removeNull();
    current = findSmall();
}

template<typename Key, typename Value, class Hasher, class Compare>
Hashmap<Key, Value, Hasher, Compare>::Iterator_All::Iterator_All(const Hashmap<Key, Value, Hasher, Compare>::Iterator_All& itr)
:size(itr.size), arr_itr(new typename SortList<Node, _Compare>::Iterator*[size]), comp(itr.comp) {
    for(int i = 0; i < size; i++) {
        arr_itr[i] = new typename SortList<Node, _Compare>::Iterator(*(itr.arr_itr[i]));
    }
    removeNull();
    current = findSmall();
}

template<typename Key, typename Value, class Hasher, class Compare>
typename Hashmap<Key, Value, Hasher, Compare>::Iterator_All& Hashmap<Key, Value, Hasher, Compare>::Iterator_All::operator=(const Hashmap<Key, Value, Hasher, Compare>::Iterator_All& itr) {
    if(arr_itr) {
        for(int i = 0; i < size; i++) {
            delete arr_itr[i];
        }
        delete [] arr_itr;
    }
    size = itr.size;
    arr_itr = new typename SortList<Node, _Compare>::Iterator*[size];
    for(int i = 0; i < size; i++) {
        arr_itr[i] = new typename SortList<Node, _Compare>::Iterator(*(itr.arr_itr[i]));
    }
    removeNull();
    current = findSmall();
    return (*this);
}

template<typename Key, typename Value, class Hasher, class Compare>
Value& Hashmap<Key, Value, Hasher, Compare>::Iterator_All::operator*() {
    if(!current) {
        throw runtime_error("Iteration at the end");
    }
    return current->operator*().val;
}

template<typename Key, typename Value, class Hasher, class Compare>
Value* Hashmap<Key, Value, Hasher, Compare>::Iterator_All::operator->() {
    return &(*(*this));
}

template<typename Key, typename Value, class Hasher, class Compare>
Value& Hashmap<Key, Value, Hasher, Compare>::Iterator_All::operator*() const {
    if(!current) {
        throw runtime_error("Iteration at the end");
    }
    return current->operator*().val;
}

template<typename Key, typename Value, class Hasher, class Compare>
Value* Hashmap<Key, Value, Hasher, Compare>::Iterator_All::operator->() const {
    return &(*(*this));
}

template<typename Key, typename Value, class Hasher, class Compare>
typename Hashmap<Key, Value, Hasher, Compare>::Iterator_All& Hashmap<Key, Value, Hasher, Compare>::Iterator_All::operator++() {
    if(!current) {
        throw runtime_error("Iteration at the end");
    }
    current->operator++();
    removeNull();
    current = findSmall();
    return (*this);
}

template<typename Key, typename Value, class Hasher, class Compare>
typename Hashmap<Key, Value, Hasher, Compare>::Iterator_All Hashmap<Key, Value, Hasher, Compare>::Iterator_All::operator++(int) {
    Iterator_All temp = (*this);
    operator++();
    return temp;
}

template<typename Key, typename Value, class Hasher, class Compare>
void Hashmap<Key, Value, Hasher, Compare>::Iterator_All::removeNull() {
    int i = 0;
    while(i < size) {
        try{
            arr_itr[i]->operator*();
        }
        catch(runtime_error& e) {
            delete arr_itr[i];
            arr_itr[i] = NULL;
            arr_itr[i] = arr_itr[size - 1];
            size--;
            continue;
        }
        i++;
    }
}

template<typename Key, typename Value, class Hasher, class Compare>
typename SortList<typename Hashmap<Key, Value, Hasher, Compare>::Node, typename Hashmap<Key, Value, Hasher, Compare>::_Compare>::Iterator* Hashmap<Key, Value, Hasher, Compare>::Iterator_All::findSmall() {
    int i = 0;
    typename SortList<Node, _Compare>::Iterator* res;
    if(size == 0) {
        return NULL;
    }
    res = arr_itr[0];
    for(i = 1; i < size; i++) {
        if(!comp(res->operator*(), arr_itr[i]->operator*())) {
            res = arr_itr[i];
        }
    }
    return res;
}

template<typename Key, typename Value, class Hasher, class Compare>
Hashmap<Key, Value, Hasher, Compare>::Iterator::Iterator()
:SortList<Node, _Compare>::Iterator() {
}

template<typename Key, typename Value, class Hasher, class Compare>
Hashmap<Key, Value, Hasher, Compare>::Iterator::~Iterator() {
}

template<typename Key, typename Value, class Hasher, class Compare>
Hashmap<Key, Value, Hasher, Compare>::Iterator::Iterator(const SortList<Node, _Compare>& q, const Key& _hashKey)
:SortList<Node, _Compare>::Iterator(q), hashKey(_hashKey) {
}

template<typename Key, typename Value, class Hasher, class Compare>
Hashmap<Key, Value, Hasher, Compare>::Iterator::Iterator(const Hashmap<Key, Value, Hasher, Compare>::Iterator& itr)
:SortList<Node, _Compare>::Iterator(itr), hashKey(itr.hashKey) {
}

template<typename Key, typename Value, class Hasher, class Compare>
typename Hashmap<Key, Value, Hasher, Compare>::Iterator&
Hashmap<Key, Value, Hasher, Compare>::Iterator::operator=(const Hashmap<Key, Value, Hasher, Compare>::Iterator& itr) {
    SortList<Node, _Compare>::Iterator::operator=(itr);
    hashKey = itr.hashKey;
    return (*this);
}

template<typename Key, typename Value, class Hasher, class Compare>
Value& Hashmap<Key, Value, Hasher, Compare>::Iterator::operator*() {
    return SortList<Node, _Compare>::Iterator::operator*().val;
}

template<typename Key, typename Value, class Hasher, class Compare>
Value* Hashmap<Key, Value, Hasher, Compare>::Iterator::operator->() {
    return &(*(*this));
}

template<typename Key, typename Value, class Hasher, class Compare>
Value& Hashmap<Key, Value, Hasher, Compare>::Iterator::operator*() const{
    return SortList<Node, _Compare>::Iterator::operator*().val;
}

template<typename Key, typename Value, class Hasher, class Compare>
Value* Hashmap<Key, Value, Hasher, Compare>::Iterator::operator->() const{
    return &(*(*this));
}

template<typename Key, typename Value, class Hasher, class Compare>
typename SortList<typename Hashmap<Key, Value, Hasher, Compare>::Node, typename Hashmap<Key, Value, Hasher, Compare>::_Compare>::Iterator::Node_A* Hashmap<Key, Value, Hasher, Compare>::Iterator::findValid() const {
    typename SortList<Node, _Compare>::Iterator::Node_A* elem = SortList<Node, _Compare>::Iterator::elem;
    typename SortList<Node, _Compare>::Iterator::Node_A* t = elem;
    if(elem->data) {
        while(t->data && t->data->k != hashKey) {
            t = t->next;
        }
        if(t->data) {
            return t;
        }
    }
    if(!t->next && t->prev) {
        return t;
    }
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

template<typename Key, typename Value, class Hasher, class Compare>
Hashmap<Key, Value, Hasher, Compare>::Hashmap()
:comp(Compare()), arr_bucket(new SortList<Node, _Compare>*[503]),
 slotn(503), length(0), hashFunctoin(Hasher()) {
    int i;
    for(i = 0; i < slotn; i++) {
        arr_bucket[i] = new SortList<Node, _Compare>(comp);
    }
}

template<typename Key, typename Value, class Hasher, class Compare>
Hashmap<Key, Value, Hasher, Compare>::~Hashmap() {
    int i;
    if(arr_bucket) {
        for(i = 0; i < slotn; i++) {
            delete arr_bucket[i];
        }
        delete [] arr_bucket;
    }
}

template<typename Key, typename Value, class Hasher, class Compare>
Hashmap<Key, Value, Hasher, Compare>::Hashmap(Hasher _hash, int _slot, Compare _comp)
:comp(_comp), arr_bucket(new SortList<Node, _Compare>*[(_slot > 0) ? (_slot) : (503)]),
 slotn((_slot > 0) ? (_slot) : (503)), length(0), hashFunctoin(_hash) {
    int i;
    for(i = 0; i < slotn; i++) {
        arr_bucket[i] = new SortList<Node, _Compare>(comp);
    }
}

template<typename Key, typename Value, class Hasher, class Compare>
Hashmap<Key, Value, Hasher, Compare>::Hashmap(const Hashmap<Key, Value, Hasher, Compare>& m)
:comp(m.comp), arr_bucket(new SortList<Node, _Compare>*[m.slotn]),
 slotn(m.slotn), length(m.length), hashFunctoin(m.hashFunctoin) {
    int i;
    for(i = 0; i < slotn; i++) {
        arr_bucket[i] = new SortList<Node, _Compare>(*(m.arr_bucket[i]));
    }
}

template<typename Key, typename Value, class Hasher, class Compare>
Hashmap<Key, Value, Hasher, Compare>& Hashmap<Key, Value, Hasher, Compare>::operator=(const Hashmap<Key, Value, Hasher, Compare>& m) {
    int i;
    if(arr_bucket) {
        for(i = 0; i < slotn; i++) {
            delete arr_bucket[i];
        }
        delete [] arr_bucket;
    }

    slotn = m.slotn;
    length = m.length;
    comp = m.comp;
    hashFunctoin = m.hashFunctoin;
    arr_bucket = NULL;

    if(m.arr_bucket) {
        arr_bucket = new SortList<Node, _Compare>*[slotn];
        for(i = 0; i < slotn; i++) {
            arr_bucket[i] = new SortList<Node, _Compare>(*(m.arr_bucket[i]));
        }
    }

    return (*this);
}

template<typename Key, typename Value, class Hasher, class Compare>
Value* Hashmap<Key, Value, Hasher, Compare>::operator[](const Key& hashKey) {
    Iterator itr = Iterator(*arr_bucket[hashFunctoin(hashKey, slotn)], hashKey);
    try {
        return &(*itr);
    }
    catch(runtime_error& e) {
        return NULL;
    }
    return NULL;
}

template<typename Key, typename Value, class Hasher, class Compare>
inline bool Hashmap<Key, Value, Hasher, Compare>::isEmpty() const {
    return length == 0;
}

template<typename Key, typename Value, class Hasher, class Compare>
inline int Hashmap<Key, Value, Hasher, Compare>::size() const {
    return length;
}

template<typename Key, typename Value, class Hasher, class Compare>
void Hashmap<Key, Value, Hasher, Compare>::insert(const Key& hashKey, const Value& item) {
    Node temp;
    temp.k = hashKey;
    temp.val = item;
    arr_bucket[hashFunctoin(hashKey, slotn)]->insert(temp);
    length++;
}

template<typename Key, typename Value, class Hasher, class Compare>
template<class CompRemove>
int Hashmap<Key, Value, Hasher, Compare>::remove(const Key& hashKey, CompRemove& _comp) {
    _CustomRemove<CompRemove> t = _CustomRemove<CompRemove>(hashKey, _comp);
    int cnt = arr_bucket[hashFunctoin(hashKey, slotn)]->remove<_CustomRemove<CompRemove> >(t);
    length -= cnt;
    return cnt;
}

template<typename Key, typename Value, class Hasher, class Compare>
typename Hashmap<Key, Value, Hasher, Compare>::Iterator Hashmap<Key, Value, Hasher, Compare>::query(const Key& hashKey) const {
    return Iterator(*arr_bucket[hashFunctoin(hashKey, slotn)], hashKey);
}

template<typename Key, typename Value, class Hasher, class Compare>
template<class Callback>
void Hashmap<Key, Value, Hasher, Compare>::apply(const Key& hashKey, Callback& _cbk) const {
    Iterator itr = Iterator(*arr_bucket[hashFunctoin(hashKey, slotn)], hashKey);
    try {
        while(true) {
            _cbk(*itr);
            itr++;
        }
    }
    catch(runtime_error& e) {
    }
}

template<typename Key, typename Value, class Hasher, class Compare>
typename Hashmap<Key, Value, Hasher, Compare>::Iterator_All Hashmap<Key, Value, Hasher, Compare>::dump() const {
    return Iterator_All(*this);
}

#endif