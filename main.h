#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include<ctype.h>
#include<stdlib.h>

/* Status macros */
#define SUCCESS 0
#define FAILURE -1


/* Function declarations */
void display_help();
void display_error();
int view_tags(char *filename);
int edit_tags(char *filename, char *tag, char *value);

#endif