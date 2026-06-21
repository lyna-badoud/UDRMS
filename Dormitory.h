#ifndef DORMITORY_H
#define DORMITORY_H
#include <string>
#include <vector>
#include "Room.h"
#include "Restaurant.h"
#include "Student.h"

class Dormitory {
private:
    std::string name;
    int capacity;
    std::vector<Room> rooms;
    Restaurant restaurant;
public:
    Dormitory(std::string name = "", int capacity = 0);
    void addRoom(const Room& R);
    void removeRoom(int roomNumber);
    bool addStudent(Student* S);
    bool removeStudent(std::string id);
    Student* findStudent(std::string id);
    std::vector<Room*> getAvailableRooms();
    Restaurant& getRestaurant();
    const Restaurant& getRestaurant() const;
    std::string getName() const;
    void displayInfo() const;
    std::vector<Room>& getRooms();
    const std::vector<Room>& getRooms() const;
    int getCapacity() const;
};
#endif