#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>
#include <string>

class InvalidYearException : public std::exception {
private:
    std::string message;
public:
    InvalidYearException(std::string msg = "Invalid year!") : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InvalidInputException : public std::exception {
private:
    std::string message;
public:
    InvalidInputException(std::string msg = "Invalid input!") : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};
#endif