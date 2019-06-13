#include "register_common.h"
#include "register_system.h"
#include "checker.h"
#include "hashmap.h"

#include <iostream>
#include <string>

using namespace std;

int RegisterSystem::insertStudent() {
    string input;
    RegisterStudent student;
    do {
        cout << "Enter the student ID: ";
        getline(cin, input);
        while(!checker.check("ID", input)) {
            cout << "Invalid input, re-enter again [student ID]: ";
            getline(cin, input);
        }
		
        if(DataStudents[input]) {
            cout << "Student already exist" << endl;
            break;
        }
        student.setStuID(input);

        cout << "Enter the student name: ";
        getline(cin, input);
        while(!checker.check("StuName", input)) {
            cout << "Invalid input, re-enter again [student name]: ";
            getline(cin, input);
        }
        student.setStuName(input);

        cout << "Enter the student year [1-3]: ";
        getline(cin, input);
        while(!checker.check("Year", input)) {
            cout << "Invalid input, re-enter again [student year]: ";
            getline(cin, input);
        }
        student.setYear(input);

        cout << "Enter the student gender [M,F]: ";
        getline(cin, input);
        while(!checker.check("Gender", input)) {
            cout << "Invalid input, re-enter again [M,F]: ";
            getline(cin, input);
        }
        student.setGender(input);
        DataStudents.insert(student.getStuId(), student);
        cout << "Creation of student record successful" << endl;
    } while(false);

    terminal->pause();
    return 0;
}

int RegisterSystem::modifyStudent() {
    string input;
    RegisterStudent *student;
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

        cout << "Enter the student name [" << student->getStuName() << "]: ";
        getline(cin, input);
        while(!checker.check("StuName", input)) {
            cout << "Invalid input, re-enter again [student name]: ";
            getline(cin, input);
        }
        student->setStuName(input);

        cout << "Enter the student year [" << student->getYear() << "]: ";
        getline(cin, input);
        while(!checker.check("Year", input)) {
            cout << "Invalid input, re-enter again [student year]: ";
            getline(cin, input);
        }
        student->setYear(input);

        cout << "Enter the student gender [" << student->getGender() << "]: ";
        getline(cin, input);
        while(!checker.check("Gender", input)) {
            cout << "Invalid input, re-enter again [M,F]: ";
            getline(cin, input);
        }
        student->setGender(input);
		cout << "Modification of student record successful" << endl;
    } while(false);

    terminal->pause();
    return 0;
}

int RegisterSystem::deleteStudent() {
    string input;
    RegisterStudent* student;
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
        
        delCourseSelection tempDelCS(this);
        CourseSelectionID.apply(input, tempDelCS);
        CourseSelectionID.remove(input);
        delStuId tempDelSI(input);
        DataCourseSelections.remove(tempDelSI);
        DataStudents.remove(input);
        cout << "Deletion of student record successful" << endl;
    }while(false);
    
    terminal->pause();
    return 0;
}

int RegisterSystem::queryStudent() {
    string input;
    RegisterStudent *student;
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

        cout << endl;
        cout << "ID:     " << student->getStuId() << endl;
        cout << "Name:   " << student->getStuName() << endl;
        cout << "Year:   " << student->getYear() << endl;
        cout << "Gender: " << ((student->getGender() == "M")?("Male"):("Female")) << endl;
    } while(false);
    
    terminal->pause();
    return 0;
}