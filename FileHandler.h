#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <string>
#include "University.h"

class FileHandler {
public:
    static void saveData(const University& uni, const std::string& filename);
    static void loadData(University& uni, const std::string& filename);
};
#endif