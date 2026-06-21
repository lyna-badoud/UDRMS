#ifndef STUDENT_H
#define STUDENT_H
#include "Person.h"
#include <string>

class Student : public Person {
private:
    int year;
public:
    Student(std::string name = "", std::string id = "", int year = 1);
    void setYear(int year);
    int getYear() const;
    virtual void displayInfo() const override;
    virtual ~Student() {}
};
#endif