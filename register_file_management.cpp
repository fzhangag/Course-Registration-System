#include "register_common.h"
#include "register_system.h"
#include "hashmap.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

using namespace std;

int RegisterSystem::saveData() {
    Hashmap<string, RegisterStudent, int(*)(const string&, int)>::Iterator_All itr_student;
    Hashmap<string, RegisterCourse, int(*)(const string&, int)>::Iterator_All itr_course;
    SortList<RegisterCourseSelection>::Iterator itr_selection;
    ofstream output_file;
    string fileName;
    char buff[16];
    int temp, i;
    output_file.exceptions(ofstream::failbit | ofstream::badbit);
    try {
        cout << "Enter the filename: ";
        getline(cin, fileName);
        output_file.open(fileName.c_str(), ofstream::out | ofstream::trunc);
        PACK_NUMBER(0x077CB531U, buff)
        output_file.write(buff, 4);

        temp = DataStudents.size();
        PACK_NUMBER(temp, buff)
        output_file.write(buff, 4);
        itr_student = DataStudents.dump();
        for(i = 0; i < temp; i++, ++itr_student) {
            itr_student->serialize(output_file);
        }

        temp = DataCourses.size();
        PACK_NUMBER(temp, buff)
        output_file.write(buff, 4);
        itr_course = DataCourses.dump();
        for(i = 0; i < temp; i++, ++itr_course) {
            itr_course->serialize(output_file);
        }

        temp = DataCourseSelections.size();
        PACK_NUMBER(temp, buff)
        output_file.write(buff, 4);
        itr_selection = DataCourseSelections.iterator();
        for(i = 0; i < temp; i++, ++itr_selection) {
            itr_selection->serialize(output_file);
        }

        output_file.close();
        cout << "Saving Successful" << endl;
    }
    catch(...) {
        cout << "Error: Write File Error" << endl;
    }

    terminal->pause();
    return 0;
}

int RegisterSystem::loadData() {
    Hashmap<string, RegisterStudent, int(*)(const string&, int)> temp_student(hashStuId, 29);
    Hashmap<string, RegisterCourse, int(*)(const string&, int)> temp_course(hashCode, 17);
    SortList<RegisterCourseSelection> temp_selection;
    SortList<RegisterCourseSelection>::Iterator t_itr;
    spHasp temp_ID(hashStuId, 31, cmpCSp);
    spHasp::Iterator itr;
    ifstream input_file;
    string fileName, input;
    RegisterStudent *student;
    RegisterCourse *course;
    RegisterStudent new_student;
    RegisterCourse new_course;
    RegisterCourseSelection new_selection;
    RegisterCourseSelection *refSelection;
    char buff[16];
    int i;
    unsigned int temp;
    input_file.exceptions(ofstream::failbit | ofstream::badbit);

    try {
        cout << "Enter the filename: ";
        getline(cin, fileName);
        input_file.open(fileName.c_str(), ofstream::in);

        input_file.read(buff, 4);
        if(UNPACK_NUMBER(buff) != 0x077CB531U) {
            throw 0;
        }
        input_file.read(buff, 4);
        temp = UNPACK_NUMBER(buff);
        for(i = 0; i < static_cast<int>(temp); i++) {
            new_student.recover(input_file);
            if(!(
            checker.check("ID", new_student.getStuId())
            && checker.check("StuName", new_student.getStuName())
            && checker.check("Year", new_student.getYear())
            && checker.check("Gender", new_student.getGender())
            )) {
                throw 0;
            }
            student = temp_student[new_student.getStuId()];
            if(student) {
                throw 0;
            }
            temp_student.insert(new_student.getStuId(), new_student);
        }

        input_file.read(buff, 4);
        temp = UNPACK_NUMBER(buff);
        for(i = 0; i < static_cast<int>(temp); i++) {
            new_course.recover(input_file);
            if(!(
            checker.check("Code", new_course.getCode())
            && checker.check("CourseName", new_course.getCourseName())
            && checker.check("Credit", new_course.getCredit())
            )) {
                throw 0;
            }
            course = temp_course[new_course.getCode()];
            if(course) {
                throw 0;
            }
            temp_course.insert(new_course.getCode(), new_course);
        }

        input_file.read(buff, 4);
        temp = UNPACK_NUMBER(buff);
        for(i = 0; i < static_cast<int>(temp); i++) {
            new_selection.recover(input_file);
            if(!(
            checker.check("ID", new_selection.getStuId())
            && checker.check("Code", new_selection.getCode())
            && checker.check("MarkImport", new_selection.getMark())
            )) {
                throw 0;
            }
            
            itr = temp_ID.query(new_selection.getStuId());
            try {
                while(true) {
                    if((*itr)->getCode() == new_selection.getCode()) {
                        break;
                    }
                    itr++;
                }
                throw 0;
            }
            catch(runtime_error& e) {
            }
            
            refSelection = temp_selection.insert(new_selection);
            temp_ID.insert(new_selection.getStuId(), refSelection);
        }

        input_file.close();
        DataStudents = temp_student;
        DataCourses = temp_course;
        DataCourseSelections = temp_selection;

        CourseSelectionID = spHasp(hashStuId, 31, cmpCSp);
        CourseSelectionCode = spHasp(hashCode, 31, cmpCSp);

        t_itr = DataCourseSelections.iterator();
        for(i = 0; i < DataCourseSelections.size(); i++, ++t_itr) {
            CourseSelectionID.insert(t_itr->getStuId(), &(*t_itr));
            CourseSelectionCode.insert(t_itr->getCode(), &(*t_itr));
        }

        cout << "Loading Successful" << endl;
    }
    catch(...) {
        cout << "Error: Load File Error (File not exist / File Corrupted / Incorrect Format)" << endl;
    }

    terminal->pause();
    return 0;
}