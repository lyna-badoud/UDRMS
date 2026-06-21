#ifndef ROOM_H
#define ROOM_H
#include <vector>
#include <string>
#include "Student.h"

class Room {
private:
    int roomNumber;
    int capacity;
    std::vector<Student*> students;
public:
    Room(int number = 0, int capacity = 0);
    bool addStudent(Student* S);
    bool removeStudent(std::string id);
    bool isFull() const;
    bool isEmpty() const;
    int getRoomNumber() const;
    int getCapacity() const;
    const std::vector<Student*>& getStudents() const;
    void displayInfo() const;
};
#endif