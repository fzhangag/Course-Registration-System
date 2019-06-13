#include "register_common.h"
#include "register_system.h"
#include "checker.h"
#include "hashmap.h"

#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

int RegisterSystem::addCourse() {
    string input;
    RegisterStudent *student;
    RegisterCourse *course;
    RegisterCourseSelection regCourse;
    RegisterCourseSelection *refSelection;
    spHasp::Iterator itr;
    bool flag;
    do {
        cout << "Enter the student ID: ";
        getline(cin, input);
        while(!checker.check("ID", input)) {
            cout << "Invalid input, re-enter again [student ID]: ";
            getline(cin, input);
        }
		
        student = DataStudents[input];
        if(!student) {
            cout << "Student not exist" << endl;
            break;
        }
        regCourse.setStuID(input);

        cout << "Enter the course code: ";
        getline(cin, input);
        while(!checker.check("Code", input)) {
            cout << "Invalid input, re-enter again [course code]: ";
            getline(cin, input);
        }

        course = DataCourses[input];
        if(!course) {
            cout << "Course not exist" << endl;
            break;
        }
        regCourse.setCode(input);

        itr = CourseSelectionID.query(regCourse.getStuId());
        flag = false;
        try {
            while(true) {
                if((*itr)->getCode() == input) {
                    flag = true;
                    break;
                }
                itr++;
            }
        }
        catch(runtime_error& e) {}
        if(flag) {
            cout << "The student already registered the course" << endl;
            break;
        }

        refSelection = DataCourseSelections.insert(regCourse);

        CourseSelectionID.insert(regCourse.getStuId(), refSelection);
        CourseSelectionCode.insert(input, refSelection);
        
        cout << "Add course successful" << endl;
    }while(false);

    terminal->pause();
    return 0;
}

int RegisterSystem::dropCourse() {
    string input;
    RegisterStudent *student;
    RegisterCourse *course;
    spHasp::Iterator itr;
    bool flag;
    do {
        cout << "Enter the student ID: ";
        getline(cin, input);
        while(!checker.check("ID", input)) {
            cout << "Invalid input, re-enter again [student ID]: ";
            getline(cin, input);
        }
		
        student = DataStudents[input];
        if(!student) {
            cout << "Student not exist" << endl;
            break;
        }

        cout << "Enter the course code: ";
        getline(cin, input);
        while(!checker.check("Code", input)) {
            cout << "Invalid input, re-enter again [course code]: ";
            getline(cin, input);
        }

        course = DataCourses[input];
        if(!course) {
            cout << "Course not exist" << endl;
            break;
        }

        itr = CourseSelectionID.query(student->getStuId());
        flag = true;
        try {
            while(true) {
                if((*itr)->getCode() == input) {
                    flag = false;
                    break;
                }
                itr++;
            }
        }
        catch(runtime_error& e) {}
        if(flag) {
            cout << "The registration record not exist" << endl;
            break;
        }
        
        delStuId tempDelSI(student->getStuId());
        CourseSelectionCode.remove(input, tempDelSI);

        delCode tempDelCC(input);
        CourseSelectionID.remove(student->getStuId(), tempDelCC);

        delIdCode tempDelIC(student->getStuId(), input);
        DataCourseSelections.remove(tempDelIC);

        
        cout << "Drop course successful" << endl;
    }while(false);

    terminal->pause();
    return 0;
}

int RegisterSystem::modifyExamMark() {
    string input;
    RegisterStudent *student;
    RegisterCourse *course;
    spHasp::Iterator itr;
    bool flag;
    do {
        cout << "Enter the student ID: ";
        getline(cin, input);
        while(!checker.check("ID", input)) {
            cout << "Invalid input, re-enter again [student ID]: ";
            getline(cin, input);
        }
        student = DataStudents[input];
        if(!student) {
            cout << "Student not exist" << endl;
            break;
        }

        cout << "Enter the course code: ";
        getline(cin, input);
        while(!checker.check("Code", input)) {
            cout << "Invalid input, re-enter again [course code]: ";
            getline(cin, input);
        }
        course = DataCourses[input];
        if(!course) {
            cout << "Course not exist" << endl;
            break;
        }

        itr = CourseSelectionID.query(student->getStuId());
        flag = true;
        try {
            while(true) {
                if((*itr)->getCode() == input) {
                    flag = false;
                    break;
                }
                itr++;
            }
        }
        catch(runtime_error& e) {}
        if(flag) {
            cout << "The registration record not exist" << endl;
            break;
        }
        cout << "Enter the exam mark [" << (*itr)->getMark() << "]: ";
        getline(cin, input);
        while(!checker.check("Mark", input)) {
            cout << "Invalid input, re-enter again [exam mark]: ";
            getline(cin, input);
        }
        (*itr)->setMark(input);
        cout << "Modification of exam mark successful" << endl;
    }while(false);
    
    terminal->pause();
    return 0;
}

int RegisterSystem::queryRegistration() {
    string input;
    RegisterStudent *student;
    RegisterCourse *course;
    spHasp::Iterator itr;
    bool flag;
    do {
        cout << "Enter the student ID: ";
        getline(cin, input);
        while(!checker.check("ID", input)) {
            cout << "Invalid input, re-enter again [student ID]: ";
            getline(cin, input);
        }

        student = DataStudents[input];
        if(!student) {
            cout << "Student not exist" << endl;
            break;
        }

        cout << "Enter the course code: ";
        getline(cin, input);
        while(!checker.check("Code", input)) {
            cout << "Invalid input, re-enter again [course code]: ";
            getline(cin, input);
        }
		
        course = DataCourses[input];
        if(!course) {
            cout << "Course not exist" << endl;
            break;
        }

        itr = CourseSelectionID.query(student->getStuId());
        flag = true;
        try {
            while(true) {
                if((*itr)->getCode() == input) {
                    flag = false;
                    break;
                }
                itr++;
            }
        }
        catch(runtime_error& e) {}
        if(flag) {
            cout << "The registration record not exist" << endl;
            break;
        }

        cout << endl;
        cout << "Student ID:  " << (*itr)->getStuId() << endl;
        cout << "Course Code: " << (*itr)->getCode() << endl;
        cout << "Exam Mark:   " << (*itr)->getMark() << endl;
        
    } while(false);
    
    terminal->pause();
    return 0;
}