#ifndef UNIVERSITY_H
#define UNIVERSITY_H
#include <vector>
#include <string>
#include "Dormitory.h"

class University {
private:
    std::string name;
    std::vector<Dormitory> dormitories;
public:
    University(std::string name);
    void addDormitory(const Dormitory& d);
    void removeDormitory(std::string name);
    Student* findStudent(std::string id);
    std::string getName() const;
    void setName(std::string name);
    void displayAllDormitories() const;
    const std::vector<Dormitory>& getDormitories() const;
    std::vector<Dormitory>& getDormitories();
    ~University();
};
#endif