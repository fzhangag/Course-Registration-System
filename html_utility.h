#ifndef HTML_UTILITY_H
#define HTML_UTILITY_H

#include "deque.h"

#include <string>
#include <fstream>

using namespace std;

class HTMLFile {
    public:
        HTMLFile();
        ~HTMLFile();
        HTMLFile(string fileName, string title);
        HTMLFile(HTMLFile&);
        void begin();
        void end();
        void openTag(const char tagName[], const char className[]="");
        void closeTag();
        void writeString(string content);
        enum state {
            ready = 0,
            opened,
            closed
        };
        state getState() {return fstate;}

    private:
        string file, html_title;
        ofstream os;
        state fstate;
        Deque<string> tag_stack;
};

#endif