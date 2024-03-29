#include "html_utility.h"
#include "register_common.h"
#include "deque.h"

#include <cstring>
#include <string>

using namespace std;

static const char header_first[] = 
"<!DOCTYPE html>\n"
"<html>\n"
"<head>\n";

static const char header_middle[] = 
"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n"
"<meta http-equiv=\"keyword\" content=\"register\" />\n"
"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n"
"<style type=\"text/css\" >\n";

static const char header_final[] = 
"</style>\n"
"</head>\n"
"<body>\n"
"<div class=\"header\">";

static const char html_final[] = 
"</div>\n"
"</body>\n"
"</html>\n"
"\n";

static string escape(string str) {
    size_t i = 0;
    while(i < str.length()) {
        switch(str[i]) {
            case '&':
                str.replace(i, 1, "&amp;");
                i += 5;
                break;
            case '<':
                str.replace(i, 1, "&lt;");
                i += 4;
                break;
            case '>':
                str.replace(i, 1, "&rt;");
                i += 4;
                break;
            default:
                i++;
        }
    }
    return str;
}

HTMLFile::HTMLFile()
:fstate(ready) {
    os.exceptions(ofstream::failbit | ofstream::badbit);
}

HTMLFile::~HTMLFile() {
    if(os.is_open()) {
        os.close();
    }
}

HTMLFile::HTMLFile(string fileName, string title)
:file(fileName),
 html_title(title),
 fstate(ready) {
    os.exceptions(ofstream::failbit | ofstream::badbit);
 }

void HTMLFile::begin() {
    if(file.length() == 0) {
        throw "No file name specified";
    }
    os.open(file.c_str(), ofstream::out | ofstream::trunc);

    fstate = opened;

    os << header_first;
    os << "<title>" << html_title << "</title>\n";
    os << header_middle;
    os << header_final;
    os << "<h3>HKUST Course Registration System</h3></div>\n";
    os << "<div class=\"wrap\">\n";
}

void HTMLFile::end() {
    if(fstate != opened) {
        throw "File is not opened";
    }
    if(!tag_stack.isEmpty()) {
        throw "Tag corrupted";
    }
    os << html_final;

    os.close();

    fstate = closed;
}

void HTMLFile::openTag(const char tagName[], const char className[]) {
    if(fstate != opened) {
        throw "File not opened";
    }
    if(!tagName || strlen(tagName) == 0) {
        throw "Empty Tag";
    }
    os << "<" << escape(tagName) << " ";
    if(className && strlen(className) > 0) {
        os << "class=\"" << className << "\"";
    }
    os << ">\n";

    tag_stack.addLast(tagName);
}

void HTMLFile::closeTag() {
    if(tag_stack.isEmpty()) {
        throw "Tag corrupted";
    }
    os << "\n</" << tag_stack.removeLast() << ">\n";
}

void HTMLFile::writeString(string content) {
    os << escape(content);
}