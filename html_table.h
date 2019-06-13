#ifndef HTML_TABLE_H
#define HTML_TABLE_H

#include "html_utility.h"
#include "deque.h"

#include <string>

using namespace std;

class HTMLTable {
    public:
        HTMLTable();
        HTMLTable(HTMLFile *_file, string _title = "", bool index = true);
        HTMLTable(HTMLTable&);
        void insertHead(string _head);
        void begin();
        void end();
        void beginRow();
        void endRow();
        void insertCell(string content);
        enum state {
            ready = 0, opened, closed
        };
        state getState() {
			return fstate;
		}

    private:
        HTMLFile *file;
        string title;
        state fstate;
        int index_gai;
        Deque<string> heads;
};

#endif