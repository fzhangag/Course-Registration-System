#include "register_common.h"
#include "register_student.h"

#include <string>
#include <cstdio>
#include <iostream>
#include <stdexcept>

using namespace std;

RegisterStudent::RegisterStudent() {
}

RegisterStudent::RegisterStudent(const RegisterStudent& r)
:id(r.id), name(r.name), gender(r.gender), year(r.year) {
}

void RegisterStudent::serialize(ostream& os) const {
    char buff[16];
    int temp;

    temp = id.length();
    PACK_NUMBER(temp, buff)
    os.write(buff, 4);
    os.write(id.c_str(), id.size());
    buff[0] = 255;
    os.write(buff, 1);

    temp = name.length();
    PACK_NUMBER(temp, buff)
    os.write(buff, 4);
    os.write(name.c_str(), name.size());
    buff[0] = 255;
    os.write(buff, 1);

    temp = gender.length();
    PACK_NUMBER(temp, buff)
    os.write(buff, 4);
    os.write(gender.c_str(), gender.size());
    buff[0] = 255;
    os.write(buff, 1);

    temp = year.length();
    PACK_NUMBER(temp, buff)
    os.write(buff, 4);
    os.write(year.c_str(), year.size());
    buff[0] = 255;
    os.write(buff, 1);

    buff[0] = 0;
    os.write(buff, 1);
}

void RegisterStudent::recover(istream& is) {
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
    gender.clear();
    for(i = 0; i < static_cast<int>(temp); i++) {
        gender.push_back(is.get());
    }
    is.read(buff, 1);
    if(buff[0] != -1) {
        throw runtime_error("Corrupted");
    }

    is.read(buff, 4);
    temp = UNPACK_NUMBER(buff);
    year.clear();
    for(i = 0; i < static_cast<int>(temp); i++) {
        year.push_back(is.get());
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