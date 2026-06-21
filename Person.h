#ifndef PERSON_H
#define PERSON_H
#include <string>

class Person {
protected:
    std::string name;
    std::string id;
public:
    Person(std::string name = "", std::string id = "");
    void setName(std::string name);
    void setId(std::string id);
    std::string getName() const;
    std::string getId() const;
    virtual void displayInfo() const = 0;
    virtual ~Person();
};
#endif