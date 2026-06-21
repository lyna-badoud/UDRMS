# UDRMS — University Dormitory & Restaurant Management System

A C++/Qt application for managing university dormitories, rooms, students, and restaurant services. Built as an Object-Oriented Programming project applying core OOP principles (encapsulation, inheritance, composition, polymorphism) to a real-world system.

## Overview

UDRMS models a university's dormitory infrastructure, where each dormitory contains multiple rooms and exactly one restaurant. Students are assigned to rooms within dormitories, and the system supports two types of users: **administrators**, who manage dormitories, students, and restaurant menus, and **students**, who can view their own accommodation details and their dormitory's daily menu.

## Features

- **Student Management** — add, remove, and search students; assign them to dormitories and rooms
- **Dormitory Management** — create and manage multiple dormitories with defined capacities
- **Room Management** — track room occupancy, prevent over-occupancy, view available/occupied rooms
- **Restaurant Management** — each dormitory has one restaurant; admins set daily breakfast/lunch/dinner menus, students can view them
- **Authentication** — separate login flows for admin and student users
- **Data Persistence** — all data (dormitories, rooms, students, restaurant menus) is saved to and loaded from text files, so the system retains its state across runs
- **Error Handling** — custom exceptions (`InvalidYearException`, `InvalidInputException`) handle invalid input gracefully throughout the system

## Classes

| Class | Responsibility |
|---|---|
| `University` | Top-level container; manages a collection of dormitories |
| `Dormitory` | Manages its rooms and one restaurant; handles student placement |
| `Room` | Tracks room number, capacity, and assigned students |
| `Restaurant` | Manages daily meals (breakfast/lunch/dinner) for one dormitory |
| `Person` | Abstract base class with shared name/ID logic |
| `Student` | Inherits from `Person`; adds academic year |

See `UDRMS.drawio.png` in this repository for the full UML class diagram, including all attributes, methods, and relationships (composition, aggregation, inheritance).

## OOP Concepts Applied

- **Inheritance** — `Student` inherits from abstract base class `Person`
- **Polymorphism** — `Person::displayInfo()` is a pure virtual function, overridden by `Student`
- **Composition** — `Dormitory` owns its `Room`s and `Restaurant`; `University` owns its `Dormitory`s
- **Aggregation** — `Room` holds references to `Student` objects without owning their lifetime (ownership managed at the `University` level)
- **Encapsulation** — all class attributes are private/protected, accessed via validated getters/setters
- **Exception Handling** — custom exception classes for invalid input and invalid academic years
- **Dynamic Memory Allocation** — students are dynamically allocated and tracked via pointers across `Room`/`Dormitory`

## Tech Stack

- **Language:** C++
- **GUI Framework:** Qt
- **Build System:** CMake
- **Data Storage:** Plain text files (`.txt`)

## Project Structure

```
UDRMS/
├── Person.h / Person.cpp
├── Student.h / Student.cpp
├── Room.h / Room.cpp
├── Dormitory.h / Dormitory.cpp
├── Restaurant.h / Restaurant.cpp
├── University.h / University.cpp
├── FileHandler.h / FileHandler.cpp
├── Exceptions.h
├── mainwindow.h / mainwindow.cpp / mainwindow.ui
├── main.cpp
├── resources.qrc
├── CMakeLists.txt
└── UDRMS.drawio.png   (UML class diagram)
```

## How to Build

This project uses CMake and Qt. To build:

1. Make sure you have Qt and CMake installed
2. Clone this repository
3. Open the project folder in Qt Creator, or build via command line:
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```
4. Run the generated executable

## Usage

- **Admin login:** username `admin`, password `1234`
- **Student login:** use a student's registered name and ID
- On first run, the system starts empty; use the admin dashboard to add dormitories and students

## Author

Lyna Badoud — AI Engineering student, ENSIA, 2025-2026 
