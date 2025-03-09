#pragma once

#include <string>
#include <vector>
#include <filesystem>

// Helper function declarations
bool ensureOutputFolderExists(const std::filesystem::path& folder);
std::vector<std::string> splitIntoTokens(const std::string& str);
bool hasCommonToken(const std::vector<std::string>& tokens1, const std::vector<std::string>& tokens2);
