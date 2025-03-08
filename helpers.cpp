#include "helpers.h"
#include <gtkmm.h>

#include <iostream>
#include <sstream>



// Helper function to split a string into tokens based on whitespace
std::vector<std::string> splitIntoTokens(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}





// Helper function to check if two sets of tokens have at least one common token
bool hasCommonToken(const std::vector<std::string>& tokens1, const std::vector<std::string>& tokens2) {
    for (const auto& token1 : tokens1) {
        for (const auto& token2 : tokens2) {
            if (token1 == token2) {
                return true;
            }
        }
    }
    return false;
}





// helper functions to verify existance of output fodler
// creates it if doesn't exist

bool ensureOutputFolderExists(const std::filesystem::path& folder) {
    if (!std::filesystem::is_directory(folder)) {
        return std::filesystem::create_directories(folder);
    }
    return true;
}

