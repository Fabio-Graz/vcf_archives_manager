
# VCF Archives Manager


This is a simple application with two functions that are useful in the management of vcf files.

VCF files are used to store databases of data from Address Book applications, both in mobile phones and in computer systems.
In particular, VCF format is a standard, and it is possible to save in this format, and read from this format, in most of the applications used in most of the systems.
So, this makes it a good option for the local backup of Address Book data.

The application can run in two "modes": Merge and Deduplicate.


## Merge mode

The application takes two files in input, and generates a single file in output. All the records (contacts) present in each input file will be copied in the output file. If a contact is present in both input files, with different fields (e.g. phone number, or address) all the different fields will be copied, and a single contact will be created in the merged output file.

## Deduplicate mode

The application accepts only one file in input. It will look for records with the same name and surname, and it will merge them in asingle record.



## Technical details

The application is written in C++, and the graphical interface is using GTK library.



The application is in an early stage of development, future imporvements and more functionalities are planned.

Please send comments to <fabio.forall@gmail.com>


March 2025
