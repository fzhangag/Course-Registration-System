#include "register_common.h"
#include "register_system.h"
#include "checker.h"
#include "hashmap.h"

#include <iostream>
#include <string>

using namespace std;

int RegisterSystem::insertCourse() {
    string input;
    RegisterCourse course;
    do {
        cout << "Enter the course code: ";
        getline(cin, input);
        while(!checker.check("Code", input)) {
            cout << "Invalid input, re-enter again [course code]: ";
            getline(cin, input);
        }
        if(DataCourses[input]) {
            cout << "Course already exist" << endl;
            break;
        }
        course.setCode(input);

        cout << "Enter the course name: ";
        getline(cin, input);
        while(!checker.check("CourseName", input)) {
            cout << "Invalid input, re-enter again [course name]: ";
            getline(cin, input);
        }
        course.setCourseName(input);

        cout << "Enter the course credit [0-5]: ";
        getline(cin, input);
        while(!checker.check("Credit", input)) {
            cout << "Invalid input, re-enter again [course credit]: ";
            getline(cin, input);
        }
        course.setCredit(input);

        DataCourses.insert(course.getCode(), course);

        cout << "Creation of course record successful" << endl;
    } while(false);

    terminal->pause();
    return 0;
}

int RegisterSystem::modifyCourse() {
    string input;
    RegisterCourse *course;
    do {
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

        cout << "Enter the course name [" << course->getCourseName() << "]: ";
        getline(cin, input);
        while(!checker.check("CourseName", input)) {
            cout << "Invalid input, re-enter again [course name]: ";
            getline(cin, input);
        }
        course->setCourseName(input);

        cout << "Enter the course credit [" << course->getCredit() << "]: ";
        getline(cin, input);
        while(!checker.check("Credit", input)) {
            cout << "Invalid input, re-enter again [course credit]: ";
            getline(cin, input);
        }
        course->setCredit(input);

        cout << "Modification of course record successful" << endl;
    }while(false);

    terminal->pause();
    return 0;
}

int RegisterSystem::deleteCourse() {
    string input;
    RegisterCourse* course;
    RegisterCourseSelection** regInfo;
    do {
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

        regInfo = CourseSelectionCode[input];
        if(regInfo) {
            cout << "Some students already registered in this course, deletion fail" << endl;
            break;
        }

        DataCourses.remove(input);

        cout << "Deletion of course record successful" << endl;
    }while(false);
    
    terminal->pause();
    return 0;
}

int RegisterSystem::queryCourse() {
    string input;
    RegisterCourse *course;
    do {
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

        cout << endl;
        cout << "Code:     " << course->getCode() << endl;
        cout << "Name:   " << course->getCourseName() << endl;
        cout << "Credit:   " << course->getCredit() << endl;       
    } while(false);
    
    terminal->pause();
    return 0;
}