#ifndef RESTAURANT_H
#define RESTAURANT_H
#include <string>

class Restaurant {
private:
    std::string name;
    std::string breakfast;
    std::string lunch;
    std::string dinner;
    int mealsServedToday;
public:
    Restaurant(std::string name = "");
    void setBreakfast(std::string meal);
    void setLunch(std::string meal);
    void setDinner(std::string meal);
    std::string getBreakfast() const;
    std::string getLunch() const;
    std::string getDinner() const;
    void serveMeal();
    int getMealsServedToday() const;
    void displayMenu() const;
};
#endif