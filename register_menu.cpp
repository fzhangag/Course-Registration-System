#include "register_common.h"
#include "register_menu.h"
#include "term.h"

#include <cstdio>
#include <iostream>
#include <string>
#include <cctype>
#include <cstring>
#include <vector>
#include <stdlib.h>

using namespace std;

RegisterMenu::RegisterMenu() :RegisterObject(), activeTerm(NULL), title("Untitled") {}

RegisterMenu::RegisterMenu(const RegisterMenu& m) :RegisterObject(m.parent), subItems(m.subItems), activeTerm(m.activeTerm), title(m.title) {}

RegisterMenu::RegisterMenu(Term* term, RegisterObject* _parent, string _title) :RegisterObject(_parent), activeTerm(term), title(_title) {}

RegisterMenu& RegisterMenu::operator=(const RegisterMenu& m) {
    subItems = m.subItems;
    title = m.title;
    return (*this);
}

void RegisterMenu::setTitle(string _title) {
    title = _title;
}

void RegisterMenu::insertItem(string _title, RegisterMenu* item) {
    Item t = {_title, item, 0};
    subItems.push_back(t);
}

void RegisterMenu::insertItem(string _title, RegisterObject* obj, string event) {
    Item t = {_title, obj, obj->getId(event)};
    subItems.push_back(t);
}

int RegisterMenu::exec() {
    string input;
    bool valid;
    string::iterator itr;
    char buff[256];
    char *end_ptr;
    long int input_number;
    int size = static_cast<int>(subItems.size());
    activeTerm->clear();
    sprintf(buff, "%d", size + 1);
    displayMenu();

    do {
        valid = true;
        getline(cin, input);

        do {
            if(input.length() == 0 || input.length() > strlen(buff)) {
                valid = false;
                break;
            }
            for(itr = input.begin(); itr != input.end(); itr++) {
                if(!isdigit(*itr)) {
                    valid = false;
                    break;
                }
            }
            if(!valid) {
                break;
            }
            input_number = strtol(input.c_str(), &end_ptr, 10);
            if(*end_ptr || input_number == 0 || input_number > size + 1) {
                valid = false;
                break;
            }
        } while(false);

        if(valid) {
            if(input_number <= size) {
                if(subItems[input_number - 1].item->fire(subItems[input_number - 1].index_gai) || !parent) {
                    activeTerm->clear();
                    displayMenu();
                    valid = false;
                }
                else {
                    return 0;
                }
            }
        }
        else {
            cout << "Invalid input, re-enter again (1-" << (subItems.size() + 1) << "): ";
        }
    } while(!valid);

    return 1;
}

void RegisterMenu::displayMenu() const {
    vector<Item>::const_iterator itr;
    int i;
    char prev;
    cout << title << endl;
    i = cout.width(title.length());
    prev = cout.fill('-');
    cout << '-' << endl;
    cout.width(i);
    cout.fill(prev);
    cout << endl;
    for(i = 1, itr = subItems.begin(); itr != subItems.end(); itr++, i++) {
        cout << i << ". " << itr->title << endl;
    }
    if(parent) {
        cout << i << ". Go back to main menu" << endl;
    }
    else {
        cout << i << ". Exit" << endl;
    }
    cout << endl;
    cout << "Enter your choice (1-" << (subItems.size() + 1) << "): ";
}

RegisterMenu::RegType RegisterMenu::methods[] = {
    {"exec", &RegisterMenu::exec},
    {0, 0}
};

REGISTER_EVENT_FNC(RegisterMenu)