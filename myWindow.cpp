#include "myWindow.h"
#include <gtkmm.h>

#include <iostream>
#include <filesystem>
//#include <cmath>
//#include <cstring>
#include <sstream>

///

#include <fstream>
#include <map>
#include <vector>


////////////



// Structure to represent a VCF contact
struct Contact {
    std::string name;
    std::vector<std::string> fields;
};




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



// Overload the << operator for the Contact class
std::ostream& operator<<(std::ostream& os, const Contact& contact) {
    os << "Name: " << contact.name;
    return os;
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



// helper functions to verify existance of output fodler
// creates it if doesn't exist

bool ensureOutputFolderExists(const std::filesystem::path& folder) {
    if (!std::filesystem::is_directory(folder)) {
        return std::filesystem::create_directories(folder);
    }
    return true;
}


// helper functions to verify existance of output file, returns true if user cancels
// note: the previous helper does not need to be in the namespace of the MainWindow
// this does because this calls a dialog window, which needs a parent

bool MainWindow::checkUserCancelOnFileExists(const std::filesystem::path& outfile) {
    if (std::filesystem::exists(outfile)) {

        Gtk::MessageDialog dialog(*this, "File already exists. Overwrite?", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL);
        dialog.set_secondary_text("The file " + outfile.string() + " already exists. Do you want to overwrite it?");
        int result = dialog.run();

        if (result != Gtk::RESPONSE_OK) {
            // User chose to cancel
            return true;

        }
    }

    return false;

}
////////
// codice GUI
///////





MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& p_builder)
: Gtk::Window(cobject), m_builder{p_builder}
{
    Gtk::Button* p_button_open_1 = nullptr; // Create a raw pointer to a button
    m_builder->get_widget("button_open_1", p_button_open_1); // Retrieve the button from XML file
    m_button_open_1 = Glib::RefPtr<Gtk::Button>(p_button_open_1); // Assign to m_button

    Gtk::Button* p_button_open_2 = nullptr; // Create a raw pointer to a button
    m_builder->get_widget("button_open_2", p_button_open_2); // Retrieve the button from XML file
    m_button_open_2 = Glib::RefPtr<Gtk::Button>(p_button_open_2); // Assign to m_button





    Gtk::Button* p_button_run = nullptr; // Create a raw pointer to a button
    m_builder->get_widget("button_run", p_button_run); // Retrieve the button from XML file
    m_button_run = Glib::RefPtr<Gtk::Button>(p_button_run); // Assign to m_button


    Gtk::Button* p_button_quit = nullptr; // Create a raw pointer to a button
    m_builder->get_widget("button_quit", p_button_quit); // Retrieve the button from XML file
    m_button_quit = Glib::RefPtr<Gtk::Button>(p_button_quit); // Assign to m_button



    Gtk::Entry* p_entry_file_1 = nullptr; // Create a raw pointer to an entry field for input file
    m_builder->get_widget("entry_file_1", p_entry_file_1); // Retrieve the entry for input file
    m_entry_file_1 = Glib::RefPtr<Gtk::Entry>(p_entry_file_1); // Assign to m_entry


    Gtk::Entry* p_entry_file_2 = nullptr; // Create a raw pointer to an entry field for input file
    m_builder->get_widget("entry_file_2", p_entry_file_2); // Retrieve the entry for input file
    m_entry_file_2 = Glib::RefPtr<Gtk::Entry>(p_entry_file_2); // Assign to m_entry


    Gtk::Entry* p_entry_out_folder = nullptr; // Create a raw pointer to an entry field for out folder
    m_builder->get_widget("entry_out_folder", p_entry_out_folder); // Retrieve the entry for out folder
    m_entry_out_folder = Glib::RefPtr<Gtk::Entry>(p_entry_out_folder); // Assign to m_entry



    Gtk::Entry* p_entry_out_filename = nullptr; // Create a raw pointer to an entry field for out filename
    m_builder->get_widget("entry_out_filename", p_entry_out_filename); // Retrieve the entry for out filename
    m_entry_out_filename = Glib::RefPtr<Gtk::Entry>(p_entry_out_filename); // Assign to m_entry



    Gtk::TextView* p_text_log= nullptr; // Create a raw pointer to a TextView
    m_builder->get_widget("textview_log", p_text_log); // Retrieve the entry for textview_log
    m_text_log = Glib::RefPtr<Gtk::TextView>(p_text_log); // Assign to m_entry


    // Connect the open button click event to the relative function
    m_button_open_1->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_select_input_1_clicked));

    // Connect the open button click event to the relative function
    m_button_open_2->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_select_input_2_clicked));



    // Connect the process button click event to the relative function
    m_button_run->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_run_button_clicked));

    // Connect the quit button click event to the relative function
    m_button_quit->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_quit_button_clicked));

}



// Show an error dialog
void MainWindow::showErrorDialog(const std::string& message) {
    Gtk::MessageDialog dialog(*this, message, false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
    dialog.run();
}

// Show an info dialog
void MainWindow::showInfoDialog(const std::string& message) {
    Gtk::MessageDialog dialog(*this, message, false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);
    dialog.run();

}

void MainWindow::on_select_input_1_clicked()
{
    // Create a FileChooserNative dialog
    auto dialog = Gtk::FileChooserNative::create("Select File 1", *this, Gtk::FILE_CHOOSER_ACTION_OPEN);

    // Show the dialog and wait for a response
    int result = dialog->run();

    // Handle the response
    if (result == Gtk::ResponseType::RESPONSE_ACCEPT)
    {
        auto filepath = dialog->get_filename();
        m_entry_file_1->set_text(filepath); // Set the entry text to the selected filepath


    // inizio inciarmo
    // contestualmente alla acquisizione dell'input file
    // tramite finestra di dialogo, viene anche scritto
    // un path di default della cartella di output

    std::filesystem::path filepath_fs = filepath;

    std::filesystem::path filename = filepath_fs.stem();

    std::filesystem::path parentpath_fs = filepath_fs.parent_path();

    std::filesystem::path outputFolder_fs = parentpath_fs;

    m_entry_out_folder->set_text(outputFolder_fs.u8string());

    // fine inciarmo


    }
}



void MainWindow::on_select_input_2_clicked()
{
    // Create a FileChooserNative dialog
    auto dialog = Gtk::FileChooserNative::create("Select File 2", *this, Gtk::FILE_CHOOSER_ACTION_OPEN);

    // Show the dialog and wait for a response
    int result = dialog->run();

    // Handle the response
    if (result == Gtk::ResponseType::RESPONSE_ACCEPT)
    {
        auto filepath = dialog->get_filename();
        m_entry_file_2->set_text(filepath); // Set the entry text to the selected filepath



    }
}






void MainWindow::on_run_button_clicked() {
    auto inputfile_path_1 = m_entry_file_1->get_text();
    auto inputfile_path_2 = m_entry_file_2->get_text();
    Glib::ustring outputFolder = m_entry_out_folder->get_text();
    Glib::ustring outputFilename = m_entry_out_filename->get_text();

    if (inputfile_path_1.empty() || inputfile_path_2.empty()) {
        showErrorDialog("Please provide paths to both VCF files.");
        return;
    }

    if (outputFilename.empty()) {
        showErrorDialog("Please specify an output filename.");
        return;
    }

    std::filesystem::path outputFolder_fs = std::string(outputFolder);
    std::filesystem::path outputFile_fs = outputFolder_fs / outputFilename.c_str();

    // Ensure the output filename has a .vcf extension
    if (outputFile_fs.extension() != ".vcf") {
        outputFile_fs.replace_extension(".vcf");
    }

    // Ensure the output folder exists
    if (!ensureOutputFolderExists(outputFolder_fs)) {
        showErrorDialog("Failed to create output folder.");
        return;
    }

    // Ensure the use did not cancel because output file exists
    if (checkUserCancelOnFileExists(outputFile_fs)) {
        // User chose to cancel
        return;
    }




    // Log the output file path
    std::ostringstream osstream;
    osstream << "\nOutput file: " << outputFile_fs.string();
    append_log(osstream);

    try {
        auto mergedContacts = mergeVCF(inputfile_path_1, inputfile_path_2, *this);
        writeMergedVCF(mergedContacts, outputFile_fs);
        showInfoDialog("Merged VCF saved to " + outputFile_fs.string());
    } catch (const std::exception& e) {
        showErrorDialog("An error occurred: " + std::string(e.what()));
    }
}



void MainWindow::append_log(std::ostringstream& osstream)
{


    Glib::ustring ready_ustrm = osstream.str();


    Glib::RefPtr<Gtk::TextBuffer> buff = m_text_log->get_buffer();


    buff->insert(buff->end(),  ready_ustrm);

    // Scroll to the end of the TextView to show the latest message

    Gtk::TextBuffer::iterator end_iter = buff->end();

    m_text_log->scroll_to(end_iter);
}



void MainWindow::on_quit_button_clicked()
{
this->close();
}
