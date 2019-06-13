#ifndef REGISTER_COURSE_H
#define REGISTER_COURSE_H

#include "register_common.h"

#include <string>
#include <iostream>

using namespace std;

class RegisterCourse {
    public:
        RegisterCourse();
		RegisterCourse(const RegisterCourse&);
        inline RegisterCourse& operator=(const RegisterCourse&);
        inline bool operator<(const RegisterCourse&) const;
        inline bool operator>=(const RegisterCourse&) const;
        inline bool operator>(const RegisterCourse&) const;
        inline bool operator<=(const RegisterCourse&) const;
        inline bool operator==(const RegisterCourse&) const;
        inline bool operator!=(const RegisterCourse&) const;
		inline string getCode() const;
		inline void setCode(string _code);
		inline string getCourseName() const;
		inline void setCourseName(string _name);
		inline string getCredit() const;
		inline void setCredit(string _credit);
        void serialize(ostream& os) const;
        void recover(istream& is);
		
    private:
        string credit;
		string code;
		string name;
};

inline RegisterCourse& RegisterCourse::operator=(const RegisterCourse& r) {
    code = r.code;
    name = r.name;
    credit = r.credit;
    return (*this);
}

inline bool RegisterCourse::operator<(const RegisterCourse& r) const {
    return code < r.code;
}

inline bool RegisterCourse::operator>=(const RegisterCourse& r) const {
    return code >= r.code;
}

inline bool RegisterCourse::operator>(const RegisterCourse& r) const {
    return code > r.code;
}

inline bool RegisterCourse::operator<=(const RegisterCourse& r) const {
    return code <= r.code;
}

inline bool RegisterCourse::operator==(const RegisterCourse& r) const {
    return code == r.code;
}

inline bool RegisterCourse::operator!=(const RegisterCourse& r) const {
    return code != r.code;
}

inline string RegisterCourse::getCode() const {
    return code;
}

inline void RegisterCourse::setCode(string _code) {
    code = _code;
}

inline string RegisterCourse::getCourseName() const {
    return name;
}

inline void RegisterCourse::setCourseName(string _name) {
    name = _name;
}

inline string RegisterCourse::getCredit() const {
    return credit;
}

inline void RegisterCourse::setCredit(string _credit) {
    credit = _credit;
}

#endif