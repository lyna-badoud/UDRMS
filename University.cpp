#include "University.h"
#include <iostream>
using namespace std;

University::University(string name) {
     setName(name);
}
       
void University::addDormitory(const Dormitory& d) {
     dormitories.push_back(d);
}

void University::removeDormitory(string name) {
    for (size_t i = 0; i < dormitories.size(); i++) {
        if (dormitories[i].getName() == name) {
            std::vector<Room>& rooms = dormitories[i].getRooms();
            for(size_t j = 0; j < rooms.size(); j++) {
                const std::vector<Student*>& students = rooms[j].getStudents();
                for(size_t k = 0; k < students.size(); k++) {
                    delete students[k];
                }
            }
            dormitories.erase(dormitories.begin() + i);
            break;
        }
    }
}
       
Student* University::findStudent(string id) {
    for (size_t i = 0; i < dormitories.size(); i++) {
        Student* s = dormitories[i].findStudent(id);
        if (s != nullptr) return s; 
    }
    return nullptr;
}
       
string University::getName() const {
       return name;
}
       
void University::setName(string name) {
     this->name = name;
}
       
void University::displayAllDormitories() const {
     cout << "Uni name:" << name << endl;
     for (size_t i = 0; i < dormitories.size(); i++) {
        dormitories[i].displayInfo();
     }
}

const vector<Dormitory>& University::getDormitories() const {
    return dormitories;
}

vector<Dormitory>& University::getDormitories() {
    return dormitories;
}

University::~University() {
    std::vector<Dormitory>& dorms = getDormitories();
    for(size_t i = 0; i < dorms.size(); i++) {
        std::vector<Room>& rooms = dorms[i].getRooms();
        for(size_t j = 0; j < rooms.size(); j++) {
            const std::vector<Student*>& students = rooms[j].getStudents();
            for(size_t k = 0; k < students.size(); k++) {
                delete students[k];
            }
        }
    }
}