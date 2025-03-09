
# VCF Archives Manager


This is a simple application with two functions that are useful in the management of vcf files.

VCF files are used to store databases of data from Address Book applications, both in mobile phones and in computer systems.
In particular, VCF format is a standard, and it is possible to save in this format, and read from this format, in most of the applications used in most of the systems.
So, this makes it a good option for the local backup of Address Book data.

The application can run in two "modes": Merge and Deduplicate.


## Merge mode

The application takes two files in input, and generates a single file in output. All the records (contacts) present in each input file will be copied in the output file. If a contact is present in both input files, with different fields (e.g. phone number, or address) all the different fields will be copied, and a single contact will be created in the merged output file.

## Deduplicate mode

This is used to remove *duplicate records* (i.e. contacts with the same name).
In this mode the application accepts only one file in input. It will look for records with the same name and surname, and it will merge them in a single record.


## Clean mode

This is used to remove *duplicate fields* from the same record (e.g. the same telephone number is saved twice, as "home number" and as "mobile number".
In this mode the application accepts only one file in input. It will look for fields with the same content, and it will keep only one.


## Technical details

The application is written in C++, and the graphical interface is using GTK library, in particular gtkmm-3.0.

It ispossible to build just using g++, opening a terminal in the project's directory and giving the following command:

> g++ -g -o build/vcf_archive_manager main.cpp gui.cpp vcf_operations.cpp helpers.cpp `pkg-config --cflags --libs gtkmm-3.0`


It is also possible to use cMake, opening a terminal in the 'build' subdirectory, and giving the following commands:

> cmake ..
> cmake --build .

In both cases an executable will be created in the 'build' subdirectory.



The application is in an early stage of development, future imporvements and more functionalities are planned.

Please send comments to <fabio.forall@gmail.com>


8 March 2025
