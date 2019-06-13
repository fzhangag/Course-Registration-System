#ifndef REGISTER_SYSTEM_H
#define REGISTER_SYSTEM_H

#include "register_common.h"
#include "term.h"
#include "sortlist.h"
#include "hashmap.h"
#include "checker.h"
#include "register_menu.h"
#include "register_student.h"
#include "register_course.h"
#include "register_course_selection.h"

#include <string>

using namespace std;

class RegisterSystem: public RegisterObject {
    REGISTER_EVENT(RegisterSystem)
	
    public:
        RegisterSystem();
        ~RegisterSystem();
        explicit RegisterSystem(Term* _terminal);
        int insertStudent();
        int modifyStudent();
        int deleteStudent();
        int queryStudent();
        int insertCourse();
        int modifyCourse();
        int deleteCourse();
        int queryCourse();
        int addCourse();
        int dropCourse();
        int modifyExamMark();
        int queryRegistration();
        int allStudents();
        int allCourses();
        int listCourses();
        int listStudents();
        int saveData();
        int loadData();
        void exec();

    private:
        void init_checker();
        void init_menu();
        Term* terminal;
        RegisterMenu menu;
        registerChecker checker;
        Hashmap<string, RegisterStudent, int(*)(const string&, int)> DataStudents;
        Hashmap<string, RegisterCourse, int(*)(const string&, int)> DataCourses;
        SortList<RegisterCourseSelection> DataCourseSelections;
        typedef Hashmap<string, RegisterCourseSelection*, int(*)(const string&, int), bool(*)(RegisterCourseSelection* const &, RegisterCourseSelection* const &)> spHasp;
        spHasp CourseSelectionID;
        spHasp CourseSelectionCode;
		
        class delStuId {
            public:
                delStuId(string _id):id(_id) {}
                bool operator()(const RegisterCourseSelection* r) {
                    return r->getStuId() == id;
                }
                bool operator()(const RegisterCourseSelection& r) {
                    return r.getStuId() == id;
                }
            private:
                string id;
        };
		
        class delCode {
            public:
                delCode(string _code):code(_code) {}
                bool operator()(const RegisterCourseSelection* r) {
                    return r->getCode() == code;
                }
                bool operator()(const RegisterCourseSelection& r) {
                    return r.getCode() == code;
                }
            private:
                string code;
        };
		
        class delIdCode {
            public:
                delIdCode(string _id, string _code):id(_id), code(_code) {}
                bool operator()(const RegisterCourseSelection* r) {
                    return r->getStuId() == id && r->getCode() == code;
                }
                bool operator()(const RegisterCourseSelection& r) {
                    return r.getStuId() == id && r.getCode() == code;
                }
            private:
                string id, code;
        };
		
        class delCourseSelection {
            public:
                delCourseSelection(RegisterSystem *_sys): sys(_sys) {}
                void operator()(RegisterCourseSelection* r) {
                    delStuId t(r->getStuId());
                    sys->CourseSelectionCode.remove(r->getCode(), t);
                }
            private:
                RegisterSystem *sys;
        };
};

int hashStuId(const string& id, int m);
int hashCode(const string& code, int m);
bool cmpCSp(RegisterCourseSelection* const & a, RegisterCourseSelection* const & b);

#endif