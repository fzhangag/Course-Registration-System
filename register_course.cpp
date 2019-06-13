#include "register_common.h"
#include "register_course.h"

#include <string>
#include <cstdio>
#include <iostream>
#include <stdexcept>

using namespace std;

RegisterCourse::RegisterCourse() {}

RegisterCourse::RegisterCourse(const RegisterCourse& r):code(r.code), name(r.name), credit(r.credit) {}

void RegisterCourse::serialize(ostream& os) const {
    char buff[16];
    int temp;

    temp = code.length();
    PACK_NUMBER(temp, buff)
    os.write(buff, 4);
    os.write(code.c_str(), code.size());
    buff[0] = 255;
    os.write(buff, 1);

    temp = name.length();
    PACK_NUMBER(temp, buff)
    os.write(buff, 4);
    os.write(name.c_str(), name.size());
    buff[0] = 255;
    os.write(buff, 1);

    temp = credit.length();
    PACK_NUMBER(temp, buff)
    os.write(buff, 4);
    os.write(credit.c_str(), credit.size());
    buff[0] = 255;
    os.write(buff, 1);

    buff[0] = 0;
    os.write(buff, 1);
}

void RegisterCourse::recover(istream& is) {
    char buff[16];
    int i;
    unsigned int temp;

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
    name.clear();
    for(i = 0; i < static_cast<int>(temp); i++) {
        name.push_back(is.get());
    }
    is.read(buff, 1);
    if(buff[0] != -1) {
        throw runtime_error("Corrupted");
    }

    is.read(buff, 4);
    temp = UNPACK_NUMBER(buff);
    credit.clear();
    for(i = 0; i < static_cast<int>(temp); i++) {
        credit.push_back(is.get());
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