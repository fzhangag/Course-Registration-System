#ifndef REGISTER_COURSE_SELECTION_H
#define REGISTER_COURSE_SELECTION_H

#include "register_common.h"

#include <string>
#include <iostream>

using namespace std;

class RegisterCourseSelection {
    public:
        RegisterCourseSelection();
        RegisterCourseSelection(const RegisterCourseSelection&);
		inline RegisterCourseSelection& operator=(const RegisterCourseSelection&);
        inline bool operator<(const RegisterCourseSelection&) const;
        inline bool operator>=(const RegisterCourseSelection&) const;
        inline bool operator>(const RegisterCourseSelection&) const;
        inline bool operator<=(const RegisterCourseSelection&) const;
        inline bool operator==(const RegisterCourseSelection&) const;
        inline bool operator!=(const RegisterCourseSelection&) const;
		inline string getStuId() const;
		inline void setStuID(string _id);
		inline string getCode() const;
		inline void setCode(string _code);
		inline string getMark() const;
		inline void setMark(string _mark);
        void serialize(ostream& os) const;
        void recover(istream& is);
		
    private:
        string id;
		string code;
		string mark;
};

inline RegisterCourseSelection& RegisterCourseSelection::operator=(const RegisterCourseSelection& r) {
    id = r.id;
    code = r.code;
    mark = r.mark;
    return (*this);
}

inline bool RegisterCourseSelection::operator<(const RegisterCourseSelection& r) const {
    return (id == r.id)?(code < r.code):(id < r.id);
}

inline bool RegisterCourseSelection::operator>=(const RegisterCourseSelection& r) const {
    return !((*this) < r);
}

inline bool RegisterCourseSelection::operator>(const RegisterCourseSelection& r) const {
    return (id == r.id)?(code > r.code):(id > r.id);
}

inline bool RegisterCourseSelection::operator<=(const RegisterCourseSelection& r) const {
    return !((*this) > r);
}

inline bool RegisterCourseSelection::operator==(const RegisterCourseSelection& r) const {
    return id == r.id && code == r.code;
}

inline bool RegisterCourseSelection::operator!=(const RegisterCourseSelection& r) const {
    return id != r.id || code != r.code;
}

inline string RegisterCourseSelection::getStuId() const {
    return id;
}

inline void RegisterCourseSelection::setStuID(string _id) {
    id = _id;
}

inline string RegisterCourseSelection::getCode() const {
    return code;
}

inline void RegisterCourseSelection::setCode(string _code) {
    code = _code;
}

inline string RegisterCourseSelection::getMark() const {
    return mark;
}

inline void RegisterCourseSelection::setMark(string _mark) {
    mark = _mark;
}

#endif