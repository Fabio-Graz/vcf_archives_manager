#include "gui.h"
#include "vcf_operations.h"
#include "helpers.h"

#include <iostream>
#include <sstream>




MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& p_builder)
: Gtk::Window(cobject), m_builder{p_builder}
{
    Gtk::Button* p_button_open_1 = nullptr; // Create a raw pointer to a button
    m_builder->get_widget("button_open_1", p_button_open_1); // Retrieve the button from XML file
    m_button_open_1 = Glib::RefPtr<Gtk::Button>(p_button_open_1); // Assign to m_button

    Gtk::Button* p_button_open_2 = nullptr; // Create a raw pointer to a button
    m_builder->get_widget("button_open_2", p_button_open_2); // Retrieve the button from XML file
    m_button_open_2 = Glib::RefPtr<Gtk::Button>(p_button_open_2); // Assign to m_button

    Gtk::Button* p_button_open_3 = nullptr; // Create a raw pointer to a button
    m_builder->get_widget("button_open_3", p_button_open_3); // Retrieve the button from XML file
    m_button_open_3 = Glib::RefPtr<Gtk::Button>(p_button_open_3); // Assign to m_button





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



    // Retrieve radio buttons
    Gtk::RadioButton* p_radio_merge = nullptr;
    m_builder->get_widget("radio_merge", p_radio_merge);
    m_radio_merge = Glib::RefPtr<Gtk::RadioButton>(p_radio_merge);

    Gtk::RadioButton* p_radio_deduplicate = nullptr;
    m_builder->get_widget("radio_deduplicate", p_radio_deduplicate);
    m_radio_deduplicate = Glib::RefPtr<Gtk::RadioButton>(p_radio_deduplicate);

    Gtk::RadioButton* p_radio_clean = nullptr;
    m_builder->get_widget("radio_clean", p_radio_clean);
    m_radio_clean = Glib::RefPtr<Gtk::RadioButton>(p_radio_clean);


    // process and quit buttons

    Gtk::Button* p_button_run = nullptr; // Create a raw pointer to a button
    m_builder->get_widget("button_run", p_button_run); // Retrieve the button from XML file
    m_button_run = Glib::RefPtr<Gtk::Button>(p_button_run); // Assign to m_button


    Gtk::Button* p_button_quit = nullptr; // Create a raw pointer to a button
    m_builder->get_widget("button_quit", p_button_quit); // Retrieve the button from XML file
    m_button_quit = Glib::RefPtr<Gtk::Button>(p_button_quit); // Assign to m_button



    // log window


    Gtk::TextView* p_text_log= nullptr; // Create a raw pointer to a TextView
    m_builder->get_widget("textview_log", p_text_log); // Retrieve the entry for textview_log
    m_text_log = Glib::RefPtr<Gtk::TextView>(p_text_log); // Assign to m_entry



    // connecting events with relative functions


    // Connect the open button click event to the relative function
    m_button_open_1->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_select_input_1_clicked));

    // Connect the open button click event to the relative function
    m_button_open_2->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_select_input_2_clicked));


    m_button_open_3->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_select_folder_clicked));


    // Connect radio button signals
    m_radio_merge->signal_toggled().connect(sigc::mem_fun(*this, &MainWindow::on_radio_merge_toggled));
    m_radio_deduplicate->signal_toggled().connect(sigc::mem_fun(*this, &MainWindow::on_radio_deduplicate_toggled));
    m_radio_clean->signal_toggled().connect(sigc::mem_fun(*this, &MainWindow::on_radio_clean_toggled));


    // Connect the process button click event to the relative function
    m_button_run->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_run_button_clicked));

    // Connect the quit button click event to the relative function
    m_button_quit->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_quit_button_clicked));

}




// helper functions to verify existance of output file, returns true if user cancels
// note: this need to be in the namespace of the MainWindow because it calls
// a dialog window, which needs a parent

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



void MainWindow::on_select_folder_clicked()
{
    // Create a FileChooserNative dialog
    auto dialog = Gtk::FileChooserNative::create("Select output folder", *this, Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

    // Show the dialog and wait for a response
    int result = dialog->run();

    // Handle the response
    if (result == Gtk::ResponseType::RESPONSE_ACCEPT)
    {
        auto folderpath = dialog->get_filename();
        m_entry_out_folder->set_text(folderpath); // Set the entry text to the selected folder path

    }
}




void MainWindow::on_run_button_clicked() {
    auto inputfile_path_1 = m_entry_file_1->get_text();
    auto inputfile_path_2 = m_entry_file_2->get_text();
    Glib::ustring outputFolder = m_entry_out_folder->get_text();
    Glib::ustring outputFilename = m_entry_out_filename->get_text();



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
        switch (m_current_mode) {
            case Mode::MERGE: {

                if ( inputfile_path_1.empty() || inputfile_path_2.empty()) {
                    showErrorDialog("Please provide paths to both VCF input files.");
                    return;
                }

                auto mergedContacts = mergeVCF(inputfile_path_1, inputfile_path_2, *this);
                writeMergedVCF(mergedContacts, outputFile_fs);
                showInfoDialog("Merged VCF saved to " + outputFile_fs.string());
                break;
            }

            case Mode::DEDUPLICATE: {

                if ( inputfile_path_1.empty() ) {
                    showErrorDialog("Please provide path to VCF input file.");
                    return;
                }

                deduplicateVCF(inputfile_path_1, outputFile_fs, *this);
                showInfoDialog("Deduplicated VCF saved to " + outputFile_fs.string());
                break;
            }


            case Mode::CLEAN: {

                if ( inputfile_path_1.empty() ) {
                    showErrorDialog("Please provide path to VCF input file.");
                    return;
                }

                cleanVCF(inputfile_path_1, outputFile_fs, *this);
                showInfoDialog("Cleaned VCF saved to " + outputFile_fs.string());
                break;
            }

        }



    } catch (const std::exception& e) {
        showErrorDialog("An error occurred: " + std::string(e.what()));
    }
}



// implement the radio button handlers

void MainWindow::on_radio_merge_toggled() {
    if (m_radio_merge->get_active()) {
        m_current_mode = Mode::MERGE;
        m_entry_file_2->set_sensitive(true); // Enable the second input field
    }
}

void MainWindow::on_radio_deduplicate_toggled() {
    if (m_radio_deduplicate->get_active()) {
        m_current_mode = Mode::DEDUPLICATE;
        m_entry_file_2->set_sensitive(false); // Disable the second input field
        m_entry_file_2->set_text(""); // clear the second input field
    }
}


void MainWindow::on_radio_clean_toggled() {
    if (m_radio_clean->get_active()) {
        m_current_mode = Mode::CLEAN;
        m_entry_file_2->set_sensitive(false); // Disable the second input field
        m_entry_file_2->set_text(""); // clear the second input field
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
