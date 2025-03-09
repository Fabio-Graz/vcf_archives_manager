#pragma once

#include <gtkmm.h>
#include <string>
#include <filesystem>


class MainWindow : public Gtk::Window
{
public:

    // Define an enumeration for the modes
    enum class Mode {
        MERGE,
        DEDUPLICATE,
        CLEAN
    };


    MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& p_builder);

    bool checkUserCancelOnFileExists(const std::filesystem::path& outfile);

    void append_log(std::ostringstream& osstream);

protected:
    Glib::RefPtr<Gtk::Builder> m_builder;

    Glib::RefPtr<Gtk::Button> m_button_open_1;
    Glib::RefPtr<Gtk::Button> m_button_open_2;
    Glib::RefPtr<Gtk::Button> m_button_open_3;



    Glib::RefPtr<Gtk::Entry> m_entry_file_1;
    Glib::RefPtr<Gtk::Entry> m_entry_file_2;


    Glib::RefPtr<Gtk::Entry> m_entry_out_folder;

    Glib::RefPtr<Gtk::Entry> m_entry_out_filename;


    Glib::RefPtr<Gtk::Button> m_button_run;

    Glib::RefPtr<Gtk::TextView> m_text_log;

    Glib::RefPtr<Gtk::Button> m_button_quit;


    Glib::RefPtr<Gtk::RadioButton> m_radio_merge;
    Glib::RefPtr<Gtk::RadioButton> m_radio_deduplicate;
    Glib::RefPtr<Gtk::RadioButton> m_radio_clean;


    // Current mode
    Mode m_current_mode = Mode::MERGE;


    //
    // private methods
    //

    // helper functions

    void showErrorDialog(const std::string& message);

    void showInfoDialog(const std::string& message);


    // Signal handler:
    void on_select_input_1_clicked();

    void on_select_input_2_clicked();

    void on_select_folder_clicked();

    void on_run_button_clicked();

    void on_quit_button_clicked();

    void on_radio_merge_toggled();
    void on_radio_deduplicate_toggled();
    void on_radio_clean_toggled();

};
