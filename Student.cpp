#include "Student.h"
#include "Exceptions.h"
#include <iostream>
using namespace std;

Student::Student(string name, string id, int year) : Person(name, id) {
    setYear(year);
}

void Student::setYear(int year) {
    if(year < 1 || year > 5)
        throw InvalidYearException("Year must be between 1 and 5!");
    this->year = year;
}

int Student::getYear() const {
    return year;
}

void Student::displayInfo() const {
    Person::displayInfo();
    cout << "Year: " << year << endl;
}