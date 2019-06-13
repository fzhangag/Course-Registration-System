#ifndef REGISTER_STUDENT_H
#define REGISTER_STUDENT_H

#include "register_common.h"

#include <string>
#include <iostream>

using std::string;
using std::ostream;
using std::istream;

class RegisterStudent {
    public:
        RegisterStudent();
        RegisterStudent(const RegisterStudent&);
        inline RegisterStudent& operator=(const RegisterStudent&);
        inline bool operator<(const RegisterStudent&) const;
        inline bool operator>=(const RegisterStudent&) const;
        inline bool operator>(const RegisterStudent&) const;
        inline bool operator<=(const RegisterStudent&) const;
        inline bool operator==(const RegisterStudent&) const;
        inline bool operator!=(const RegisterStudent&) const;
        inline string getStuId() const;
        inline void setStuID(string _id);
		inline string getStuName() const;
		inline void setStuName(string _name);
		inline string getYear() const;
		inline void setYear(string _year);
		inline string getGender() const;
		inline void setGender(string _gender);
        void serialize(ostream& os) const;
        void recover(istream& is);
		
    private:
        string id;
		string name;
		string gender;
		string year;
};

inline RegisterStudent& RegisterStudent::operator=(const RegisterStudent& r) {
    id = r.id;
    name = r.name;
    gender = r.gender;
    year = r.year;
    return (*this);
}

inline bool RegisterStudent::operator<(const RegisterStudent& r) const {
    return id < r.id;
}

inline bool RegisterStudent::operator>=(const RegisterStudent& r) const {
    return id >= r.id;
}

inline bool RegisterStudent::operator>(const RegisterStudent& r) const {
    return id > r.id;
}

inline bool RegisterStudent::operator<=(const RegisterStudent& r) const {
    return id <= r.id;
}

inline bool RegisterStudent::operator==(const RegisterStudent& r) const {
    return id == r.id;
}

inline bool RegisterStudent::operator!=(const RegisterStudent& r) const {
    return id != r.id;
}

inline string RegisterStudent::getStuId() const {
    return id;
}

inline void RegisterStudent::setStuID(string _id) {
    id = _id;
}

inline string RegisterStudent::getStuName() const {
    return name;
}

inline void RegisterStudent::setStuName(string _name) {
    name = _name;
}

inline string RegisterStudent::getYear() const {
    return year;
}

inline void RegisterStudent::setYear(string _year) {
    year = _year;
}

inline string RegisterStudent::getGender() const {
    return gender;
}

inline void RegisterStudent::setGender(string _gender) {
    gender = _gender;
}

#endif