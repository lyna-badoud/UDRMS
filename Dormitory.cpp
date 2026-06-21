#include "Dormitory.h"
#include "Exceptions.h"
#include <iostream>

Dormitory::Dormitory(std::string name, int capacity) {
    this->name = name;
    this->capacity = capacity;
}

void Dormitory::addRoom(const Room& R) {
    rooms.push_back(R);
}

void Dormitory::removeRoom(int roomNumber) {
    for(size_t i = 0; i < rooms.size(); i++) {
        if(rooms[i].getRoomNumber() == roomNumber) {
            rooms.erase(rooms.begin() + i);
            return;
        }
    }
    throw InvalidInputException("Room not found!");
}

bool Dormitory::addStudent(Student* S) {
    int total = 0;
    for(size_t i = 0; i < rooms.size(); i++)
        total += rooms[i].getStudents().size();
    if(total >= capacity) return false;
    for(size_t i = 0; i < rooms.size(); i++) {
        if(!rooms[i].isFull()) {
            rooms[i].addStudent(S);
            return true;
        }
    }
    return false;
}

bool Dormitory::removeStudent(std::string id) {
    for(size_t i = 0; i < rooms.size(); i++) {
        if(rooms[i].removeStudent(id)) return true;
    }
    return false;
}

Student* Dormitory::findStudent(std::string id) {
    for(size_t i = 0; i < rooms.size(); i++) {
        const std::vector<Student*>& roomStudents = rooms[i].getStudents();
        for(size_t j = 0; j < roomStudents.size(); j++) {
            if(roomStudents[j]->getId() == id) {
                return roomStudents[j];
            }
        }
    }
    return nullptr;
}

std::vector<Room*> Dormitory::getAvailableRooms() {
    std::vector<Room*> available;
    for(size_t i = 0; i < rooms.size(); i++) {
        if(!rooms[i].isFull()) {
            available.push_back(&rooms[i]);
        }
    }
    return available;
}

Restaurant& Dormitory::getRestaurant() {
    return restaurant;
}

const Restaurant& Dormitory::getRestaurant() const {
    return restaurant;
}

std::string Dormitory::getName() const {
    return name;
}

void Dormitory::displayInfo() const {
    std::cout << "Dormitory name: " << name << std::endl;
    std::cout << "Capacity: " << capacity << std::endl;
    restaurant.displayMenu();
    for(size_t i = 0; i < rooms.size(); i++) {
        rooms[i].displayInfo();
    }
}

std::vector<Room>& Dormitory::getRooms() {
    return rooms;
}

const std::vector<Room>& Dormitory::getRooms() const {
    return rooms;
}

int Dormitory::getCapacity() const {
    return capacity;
}