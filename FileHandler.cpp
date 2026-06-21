#include "FileHandler.h"
#include "Exceptions.h"
#include <fstream>
#include <sstream>
using namespace std;

void FileHandler::saveData(const University& uni, const std::string& filename) {
    const std::vector<Dormitory>& dorms = uni.getDormitories();

    std::ofstream file(filename + "/dormitories.txt");
    if(!file.is_open()) throw InvalidInputException("Could not open dormitories.txt for writing!");
    file << uni.getName() << std::endl;
    for(size_t i = 0; i < dorms.size(); i++) {
        file << dorms[i].getName() << "," << dorms[i].getCapacity() << std::endl;
    }
    file.close();

    std::ofstream file3(filename + "/rooms.txt");
    if(!file3.is_open()) throw InvalidInputException("Could not open rooms.txt for writing!");
    for(size_t i = 0; i < dorms.size(); i++) {
        const std::vector<Room>& allRooms = dorms[i].getRooms();
        for(size_t j = 0; j < allRooms.size(); j++) {
            file3 << dorms[i].getName() << ","
                  << allRooms[j].getRoomNumber() << ","
                  << allRooms[j].getCapacity() << std::endl;
        }
    }
    file3.close();

    std::ofstream file2(filename + "/students.txt");
    if(!file2.is_open()) throw InvalidInputException("Could not open students.txt for writing!");
    for(size_t i = 0; i < dorms.size(); i++) {
        const std::vector<Room>& allRooms = dorms[i].getRooms();
        for(size_t j = 0; j < allRooms.size(); j++) {
            const std::vector<Student*>& students = allRooms[j].getStudents();
            for(size_t k = 0; k < students.size(); k++) {
                file2 << students[k]->getName() << ","
                      << students[k]->getId() << ","
                      << students[k]->getYear() << ","
                      << dorms[i].getName() << ","
                      << allRooms[j].getRoomNumber() << std::endl;
            }
        }
    }
    file2.close();

    std::ofstream file4(filename + "/restaurants.txt");
    if(!file4.is_open()) throw InvalidInputException("Could not open restaurants.txt for writing!");
    for(size_t i = 0; i < dorms.size(); i++) {
        const Restaurant& r = dorms[i].getRestaurant();
        file4 << dorms[i].getName() << "|"
              << r.getBreakfast() << "|"
              << r.getLunch() << "|"
              << r.getDinner() << std::endl;
    }
    file4.close();
}

void FileHandler::loadData(University& uni, const std::string& filename) {
    std::string line;

    std::ifstream file(filename + "/dormitories.txt");
    if(!file.is_open()) throw InvalidInputException("Could not open dormitories.txt for reading!");
    getline(file, line);
    uni.setName(line);
    while(getline(file, line)) {
        if(line.empty()) continue;
        std::stringstream ss(line);
        std::string name, cap;
        getline(ss, name, ',');
        getline(ss, cap, ',');
        try {
            Dormitory d(name, std::stoi(cap));
            uni.addDormitory(d);
        } catch(const std::invalid_argument&) {
            throw InvalidInputException("Corrupted dormitories file!");
        }
    }
    file.close();

    std::ifstream file3(filename + "/rooms.txt");
    if(!file3.is_open()) throw InvalidInputException("Could not open rooms.txt for reading!");
    while(getline(file3, line)) {
        if(line.empty()) continue;
        std::stringstream ss(line);
        std::string dormName, roomNum, cap;
        getline(ss, dormName, ',');
        getline(ss, roomNum, ',');
        getline(ss, cap, ',');
        try {
            Room r(std::stoi(roomNum), std::stoi(cap));
            std::vector<Dormitory>& dorms = uni.getDormitories();
            for(size_t i = 0; i < dorms.size(); i++) {
                if(dorms[i].getName() == dormName) {
                    dorms[i].addRoom(r);
                    break;
                }
            }
        } catch(const std::invalid_argument&) {
            throw InvalidInputException("Corrupted rooms file!");
        }
    }
    file3.close();

    std::ifstream file2(filename + "/students.txt");
    if(!file2.is_open()) throw InvalidInputException("Could not open students.txt for reading!");
    while(getline(file2, line)) {
        if(line.empty()) continue;
        std::stringstream ss(line);
        std::string name, id, year, dormName, roomNum;
        getline(ss, name, ',');
        getline(ss, id, ',');
        getline(ss, year, ',');
        getline(ss, dormName, ',');
        getline(ss, roomNum, ',');

        Student* s = nullptr;
        try {
            s = new Student(name, id, std::stoi(year));
            int targetRoom = std::stoi(roomNum);

            std::vector<Dormitory>& dorms = uni.getDormitories();
            bool placed = false;
            for(size_t i = 0; i < dorms.size() && !placed; i++) {
                if(dorms[i].getName() == dormName) {
                    std::vector<Room>& rooms = dorms[i].getRooms();
                    for(size_t j = 0; j < rooms.size(); j++) {
                        if(rooms[j].getRoomNumber() == targetRoom) {
                            if(rooms[j].addStudent(s)) {
                                placed = true;
                            }
                            break;
                        }
                    }
                    break;
                }
            }

            if(!placed) {
                // Dormitory/room not found, or room was full: avoid a leak
                delete s;
                s = nullptr;
            }
        } catch(const std::invalid_argument&) {
            delete s; // safe even if s is nullptr
            throw InvalidInputException("Corrupted students file!");
        }
    }
    file2.close();

    std::ifstream file4(filename + "/restaurants.txt");
    if(file4.is_open()) {
        while(getline(file4, line)) {
            if(line.empty()) continue;
            std::stringstream ss(line);
            std::string dormName, breakfast, lunch, dinner;
            getline(ss, dormName, '|');
            getline(ss, breakfast, '|');
            getline(ss, lunch, '|');
            getline(ss, dinner, '|');

            std::vector<Dormitory>& dorms = uni.getDormitories();
            for(size_t i = 0; i < dorms.size(); i++) {
                if(dorms[i].getName() == dormName) {
                    Restaurant& r = dorms[i].getRestaurant();
                    r.setBreakfast(breakfast);
                    r.setLunch(lunch);
                    r.setDinner(dinner);
                    break;
                }
            }
        }
        file4.close();
    }
    // If restaurants.txt doesn't exist (e.g. first run before this feature existed),
    // silently skip rather than throwing, so older save data still loads fine.
}