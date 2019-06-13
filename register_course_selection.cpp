#include "register_common.h"
#include "register_course_selection.h"

#include <string>
#include <cstdio>
#include <iostream>
#include <stdexcept>

using namespace std;

RegisterCourseSelection::RegisterCourseSelection():mark("N/A") {}

RegisterCourseSelection::RegisterCourseSelection(const RegisterCourseSelection& r) :id(r.id), code(r.code), mark(r.mark) {}

void RegisterCourseSelection::serialize(ostream& os) const {
    char buff[16];
    int temp;

    temp = id.length();
    PACK_NUMBER(temp, buff)
    os.write(buff, 4);
    os.write(id.c_str(), id.size());
    buff[0] = 255;
    os.write(buff, 1);

    temp = code.length();
    PACK_NUMBER(temp, buff)
    os.write(buff, 4);
    os.write(code.c_str(), code.size());
    buff[0] = 255;
    os.write(buff, 1);

    temp = mark.length();
    PACK_NUMBER(temp, buff)
    os.write(buff, 4);
    os.write(mark.c_str(), mark.size());
    buff[0] = 255;
    os.write(buff, 1);

    buff[0] = 0;
    os.write(buff, 1);
}

void RegisterCourseSelection::recover(istream& is) {
    char buff[16];
    int i;
    int temp;

    is.read(buff, 4);
    temp = UNPACK_NUMBER(buff);
    id.clear();
    for(i = 0; i < static_cast<int>(temp); i++) {
        id.push_back(is.get());
    }
    is.read(buff, 1);
    if(buff[0] != -1) {
        throw runtime_error("Corrupted");
    }

    is.read(buff, 4);
    temp = UNPACK_NUMBER(buff);
    code.clear();
    for(i = 0; i < static_cast<int>(temp); i++) {
        code.push_back(is.get());
    }
    is.read(buff, 1);
    if(buff[0] != -1) {
        throw runtime_error("Corrupted");
    }

    is.read(buff, 4);
    temp = UNPACK_NUMBER(buff);
    mark.clear();
    for(i = 0; i < static_cast<int>(temp); i++) {
        mark.push_back(is.get());
    }
    is.read(buff, 1);
    if(buff[0] != -1) {
        throw runtime_error("Corrupted");
    }

    is.read(buff, 1);
    if(buff[0] != 0) {
        throw runtime_error("Corrupted");
    }
}