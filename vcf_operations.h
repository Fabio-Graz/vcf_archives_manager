#pragma once

#include <string>
#include <map>
#include <vector>
#include "gui.h" // Include the MainWindow class definition


// Structure to represent a VCF contact
struct Contact {
    std::string name;
    std::vector<std::string> fields;
};


// Overload the << operator for the Contact struct
inline std::ostream& operator<<(std::ostream& os, const Contact& contact) {
    os << "Name: " << contact.name;
    return os;
}

// Function declarations
std::map<std::string, Contact> parseVCF(const std::string& filePath);
std::map<std::string, Contact> mergeVCF(const std::string& file1, const std::string& file2, MainWindow& window);
void writeMergedVCF(const std::map<std::string, Contact>& contacts, const std::string& outputPath);
void deduplicateVCF(const std::string& filePath, const std::string& outputPath, MainWindow& window);
