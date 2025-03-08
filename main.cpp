#include <iostream>
#include <cstring>
#include <gtkmm/application.h>
#include <gtkmm/builder.h>
#include "gui.h"

int main(int argc, char** argv)
{
    auto app = Gtk::Application::create(argc, argv);

    auto builder = Gtk::Builder::create_from_file("builder.ui");

    MainWindow* window = nullptr;

    builder->get_widget_derived("mainWindow", window);

    app->run(*window);

        try
        {
            // You then show the window by using you handle (i.e window).
            app->run(*window);
        }

        catch(const std::runtime_error& re)
        {
            // speciffic handling for runtime_error
            std::cout << "Other runtime error: " << re.what() << std::endl;
        }

        catch(const std::exception& ex)
        {
            // speciffic handling for all exceptions extending std::exception, except
            // std::runtime_error which is handled explicitly
            std::cout << "Error occurred: " << ex.what() << std::endl;
        }
        catch(...)
        {
            // catch any other errors (that we have no information about)
            std::cout << "Unknown failure occurred. Possible memory corruption" << std::endl;
        }

    return 0;
}
