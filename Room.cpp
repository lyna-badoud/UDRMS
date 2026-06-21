#include "Room.h"
#include <iostream>
using namespace std;

Room::Room(int number, int capacity) {
     roomNumber = number;
     this->capacity = capacity;
}
      
bool Room::addStudent(Student* S) {
    if (isFull()) return false;
    else {
        students.push_back(S);
        return true;
    }
}
      
bool Room::removeStudent(string id) {
    if (isEmpty()) return false;
    else {
        for(size_t i = 0; i < students.size(); i++) {
            if(students[i]->getId() == id) {
                students.erase(students.begin() + i);
                return true;
            }
        }
        return false;
    }
}
      
bool Room::isFull() const {
     return students.size() == (size_t)capacity;
}
      
bool Room::isEmpty() const {
     return students.empty();
}
      
int Room::getRoomNumber() const {
     return roomNumber;
}
      
int Room::getCapacity() const {
      return capacity;
}
      
const vector<Student*>& Room::getStudents() const {
      return students;
}
      
void Room::displayInfo() const {
     cout << " Room number :" << roomNumber << endl;
     cout << " Capacity:" << capacity << endl; 
     cout << "Students in the room: " << endl;
     for(size_t i = 0; i < students.size(); i++) {
         students[i]->displayInfo();
     }  
}