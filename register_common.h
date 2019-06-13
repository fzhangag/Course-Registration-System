#ifndef REGISTER_COMMON_H
#define REGISTER_COMMON_H

#include <string>

using namespace std;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define WIN_TERM
#endif

template<typename T>
class ReturnTrue {
    public:
        ReturnTrue(){}
        bool operator()(const T&) const{return true;}
};

class RegisterObject {
    public:
        RegisterObject():parent(NULL), head(NULL) {}
		
        virtual ~RegisterObject() {
            _node *t = head, *p;
            while(t) {
                p = t;
                t = t->next;
                delete p->item;
                delete p;
            }
        }

        RegisterObject(RegisterObject* _parent):parent(_parent), head(NULL) {
            _node *t;
            if(parent) {
                t = new _node;
                t->item = this;
                t->next = parent->head;
                parent->head = t;
            }
        }

        virtual int getId(string name) {
			name.append("");
			return -1;
		}
		
        virtual int fire(int index_gai) {
			index_gai++;
			return 1;
		}
		
    protected:
        RegisterObject *parent;
        struct _node {
            RegisterObject *item;
            _node *next;
        } *head;
};

#define REGISTER_EVENT(class)                   \
    public:                                     \
        typedef struct {                        \
            const char *name;                   \
            int(class::*fnc)();                 \
        }RegType;                               \
        static RegType methods[];               \
        virtual int getId(string name);         \
        virtual int fire(int id);

#define REGISTER_EVENT_FNC(class)               \
int class::getId(string name) {                 \
    int i;                                      \
    for(i = 0; methods[i].name; i++) {          \
        if(name == methods[i].name) {           \
            return i;                           \
        }                                       \
    }                                           \
    return -1;                                  \
}                                               \
int class::fire(int id) {                       \
    return (this->*(methods[id].fnc))();        \
}                                       

#define PACK_NUMBER(NUM, SLOT)                  \
{                                               \
SLOT[0]=static_cast<char>((NUM) >> 24);         \
SLOT[1]=static_cast<char>(((NUM) >> 16) & 0xFF);\
SLOT[2]=static_cast<char>(((NUM) >> 8) & 0xFF); \
SLOT[3]=static_cast<char>((NUM) & 0xFF);        \
}

#define UNPACK_NUMBER(SLOT)                     \
((SLOT[3] & 0xFFU)                              \
 | ((SLOT[2] << 8) & 0xFF00U)                   \
 | ((SLOT[1] << 16) & 0xFF0000U)                \
 | ((SLOT[0] << 24) & 0xFF000000U))

int ELFHash_c(const char *str, int m);
int ELFHash(const string& str, int m);

#endif