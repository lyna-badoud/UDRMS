#include "Person.h"
#include "Exceptions.h"
#include <iostream>
using namespace std;

Person::Person(string name, string id) {
    setName(name);
    setId(id);
}

void Person::setName(string name) {
    if(name.empty())
        throw InvalidInputException("Name cannot be empty!");
    this->name = name;
}

void Person::setId(string id) {
    if(id.empty())
        throw InvalidInputException("ID cannot be empty!");
    this->id = id;
}

string Person::getName() const {
    return name;
}

string Person::getId() const {
    return id;
}

void Person::displayInfo() const {
    cout << "Name: " << name << endl;
    cout << "ID: " << id << endl;
}

Person::~Person() {}
