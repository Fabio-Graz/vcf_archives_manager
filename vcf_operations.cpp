#include "vcf_operations.h"
#include "helpers.h"

#include <fstream>
#include <sstream>
//#include <algorithm> // for std::find




// Function to parse a VCF file
std::map<std::string, Contact> parseVCF(const std::string& filePath) {
    std::map<std::string, Contact> contacts;
    std::ifstream file(filePath);
    std::string line;
    Contact currentContact;

    while (std::getline(file, line)) {
        if (line.find("BEGIN:VCARD") != std::string::npos) {
            currentContact = Contact(); // Reset for a new contact
        } else if (line.find("FN:") != std::string::npos) {
            currentContact.name = line.substr(3); // Extract the name
        } else if (line.find("END:VCARD") != std::string::npos) {
            if (!currentContact.name.empty()) {
                contacts[currentContact.name] = currentContact; // Save the contact
            }
        } else {
            currentContact.fields.push_back(line); // Save other fields
        }
    }

    return contacts;
}



// Function to merge two VCF files
std::map<std::string, Contact> mergeVCF(const std::string& file1, const std::string& file2, MainWindow& window) {
    auto contacts1 = parseVCF(file1);
    auto contacts2 = parseVCF(file2);

    // Merge contacts2 into contacts1
    for (const auto& [name, contact] : contacts2) {
        if (contacts1.find(name) != contacts1.end()) {
            // Merge fields if the contact already exists
            for (const auto& field : contact.fields) {
                contacts1[name].fields.push_back(field);

            }
            // Log merged contact
            std::ostringstream osstream;
            osstream <<"\n*** merged contact: " << contacts1[name];
            window.append_log(osstream);

        } else {
            // Add new contact
            contacts1[name] = contact;

            // Log added contact
            std::ostringstream osstream;
            osstream <<"\nadded contact: " << contacts1[name];
            window.append_log(osstream);

        }
    }

    return contacts1;
}

// Function to write merged contacts to a new VCF file
void writeMergedVCF(const std::map<std::string, Contact>& contacts, const std::string& outputPath) {
    std::ofstream outputFile(outputPath);
    for (const auto& [name, contact] : contacts) {
        outputFile << "BEGIN:VCARD\n";
        outputFile << "FN:" << name << "\n";
        for (const auto& field : contact.fields) {
            outputFile << field << "\n";
        }
        outputFile << "END:VCARD\n";
    }
}





// function for deduplication

void deduplicateVCF(const std::string& filePath, const std::string& outputPath, MainWindow& window) {
    auto contacts = parseVCF(filePath);
    std::map<std::string, Contact> uniqueContacts;

    for (const auto& [name, contact] : contacts) {
        // Extract name and surname (assuming the format is "Name Surname")
        std::istringstream iss(name);
        std::string namePart, surnamePart;
        iss >> namePart >> surnamePart;


        // Split name and surname into tokens
        std::vector<std::string> nameTokens = splitIntoTokens(namePart);
        std::vector<std::string> surnameTokens = splitIntoTokens(surnamePart);


        // Create a unique key by combining all name and surname tokens
        std::string key;
        for (const auto& token : nameTokens) {
            key += token + " ";
        }
        for (const auto& token : surnameTokens) {
            key += token + " ";
        }

        // Check if this contact is a duplicate of any existing contact
        bool isDuplicate = false;
        for (const auto& [existingKey, existingContact] : uniqueContacts) {
            // Split existing contact's name and surname into tokens
            std::istringstream existingIss(existingContact.name);
            std::string existingNamePart, existingSurnamePart;
            existingIss >> existingNamePart >> existingSurnamePart;

            std::vector<std::string> existingNameTokens = splitIntoTokens(existingNamePart);
            std::vector<std::string> existingSurnameTokens = splitIntoTokens(existingSurnamePart);

            // Check if there is at least one common token in both name and surname
            bool hasCommonNameToken = hasCommonToken(nameTokens, existingNameTokens);
            bool hasCommonSurnameToken = hasCommonToken(surnameTokens, existingSurnameTokens);

            if (hasCommonNameToken && hasCommonSurnameToken) {
                isDuplicate = true;
                break;
            }
        }

        if (!isDuplicate) {
            // Add the contact to the unique contacts map
            uniqueContacts[key] = contact;
        } else {
            // Log duplicate contact
            std::ostringstream osstream;
            osstream << "\n\n*** Duplicate contact: " << name;
            window.append_log(osstream);
        }
    }

    // Write the deduplicated contacts to the output file
    writeMergedVCF(uniqueContacts, outputPath);
}






