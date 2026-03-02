#include "main.h"
/*
-----------------------------------------------------------------------------------------------------------------------------------------
Student Name: Abhijit Awasare
Project Name: MP3 Tag Reader & Editor
Description:
This file contains the functions and logic for the MP3 Tag Reader & Editor project.

The program allows the user to:
  - View MP3 file details (Title, Artist, Album, Year, Comment)
  - Edit selected MP3 tags
  - Display help and error messages

The program reads and modifies ID3 tags using binary file handling.
A temporary file is used while editing to safely update the tag
without damaging the original MP3 audio data.
-----------------------------------------------------------------------------------------------------------------------------------------
*/


/* Displays error message and correct usage format */
void display_error()
{
    printf("\033[31mERROR: Invalid Argument\n\033[0m");
    printf("\033[33mUSAGE:\n\033[0m");
    printf("\033[33mTo View Please Pass Like: ./a.out -v [mp3 filename]\n\033[0m");
    printf("\033[33mTo Edit Please Pass Like: ./a.out -e (-t/-T/-a/-A/-y/-c/-g) [changing text] [mp3 filename]\n\033[0m");
    printf("\033[33mTo Get Help Please Pass Like: ./a.out --help\n\033[0m");

}
/* Displays help menu with available options */
void display_help()
{
    printf("\033[33m--------------- [ HELP OPTION ] ---------------\n\033[0m");

    printf("\033[33m1. -v -> To View mp3 file Contents\n\033[0m");
    printf("\033[33m2. -e -> To Edit mp3 file Contents\n\033[0m");

    printf("\033[33m   2.1. -t -> Song Title\n\033[0m");
    printf("\033[33m   2.2. -a -> Artist Name\n\033[0m");
    printf("\033[33m   2.3. -A -> Album Name\n\033[0m");
    printf("\033[33m   2.4. -y -> Year\n\033[0m");
    printf("\033[33m   2.5. -m -> Content\n\033[0m");
    printf("\033[33m   2.6. -c -> Comment\n\033[0m");

}

/* Main function: Handles command line arguments */
int main (int argc, char *argv[])
{
    /* Check minimum argument count */
    if(argc < 2)
    {
        display_error();
        return FAILURE;
    }

    /* Help option */
    if(strcmp(argv[1], "--help") == 0)
    {
        display_help();
        return SUCCESS;
    }
    /* View option */
    else if(strcmp(argv[1], "-v") == 0 && argc == 3)
    {
        if(view_tags(argv[2]) != SUCCESS)
        {
            printf("\033[31mERROR: Unable to view tags\n\033[0m");
            return FAILURE;
        }
    }
    /* Edit option */
    else if(strcmp(argv[1], "-e") == 0 && argc == 5)
    {
        char *tag = argv[2];        // Tag type (-t, -a, etc.)
        char *value = argv[3];      // New value
        char *filename = argv[4];   // MP3 file

        if(edit_tags(filename, tag, value) != SUCCESS)
        {
            printf("\033[31mERROR: Failed to Edit Tag.\n\033[0m");
            return FAILURE;
        }

        printf("\033[32mSUCCESS: Tag Edited Successfully. \n\033[0m");

    }
    /* Invalid input */
    else
    {
        display_error();
        return FAILURE;
    }

    return SUCCESS;
}