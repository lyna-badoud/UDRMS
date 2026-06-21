#include "Restaurant.h"
#include <iostream>
using namespace std;

Restaurant::Restaurant(string name) {
     this->name = name;
     this->breakfast = "";
     this->lunch = "";
     this->dinner = "";
     this->mealsServedToday = 0;
}
       
void Restaurant::setBreakfast(string meal) {
      breakfast = meal;
}
       
void Restaurant::setLunch(string meal) {
     lunch = meal;
}

void Restaurant::setDinner(string meal) {
      dinner = meal;
}
       
string Restaurant::getBreakfast() const {
      return breakfast;
}
       
string Restaurant::getLunch() const {
      return lunch;
}
       
string Restaurant::getDinner() const {
       return dinner;
}
       
void Restaurant::serveMeal() {
       mealsServedToday++;
}
       
int Restaurant::getMealsServedToday() const {
       return mealsServedToday;
}
       
void Restaurant::displayMenu() const {
      cout << " MENU " << endl;
      cout << "Breakfast: " << breakfast << endl;
      cout << "Lunch: " << lunch << endl;
      cout << "Dinner: " << dinner << endl;
      cout << "Meal served today:" << mealsServedToday << endl;
}