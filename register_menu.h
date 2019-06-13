#ifndef REGISTER_MENU_H
#define REGISTER_MENU_H

#include "register_common.h"
#include "term.h"

#include <vector>
#include <iostream>
#include <string>

using namespace std;

class RegisterMenu: public RegisterObject {
    REGISTER_EVENT(RegisterMenu)
    public:
        RegisterMenu();
        RegisterMenu(const RegisterMenu&);
        explicit RegisterMenu(Term* term, RegisterObject* _parent = NULL, string _title = "Untitled");
        RegisterMenu& operator=(const RegisterMenu&);
        void setTitle(string _title);
        void insertItem(string _title, RegisterMenu* item);
        void insertItem(string _title, RegisterObject* obj, string event);
        int exec();

    private:
        typedef struct {
            string title;
            RegisterObject *item;
            int index_gai;
        } Item;
		
        void displayMenu() const;
        vector<Item> subItems;
        Term* activeTerm;
        string title;
};

#endif