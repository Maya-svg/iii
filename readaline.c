/*
*   readaline.c 
*
*   CS40 HW 1: filesofpix
*   By Gun Yang and Harnaljia Fenelon
*   Date: 01/28/22
* 
*   Summary: This file reads and stores a line from a provided file. 
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "readaline.h"

/*
*
*   Purpose: Read a line from the given input file. 
*
*   input: Requires an opened file stream, and a double pointer that cannot
*       be NULL. 
*   output: returns the number of bytes in that particular row.
*
*/
size_t readaline(FILE *inputfd, char **datapp)
{
    assert(inputfd != NULL);
    assert(datapp != NULL);

    int index = 0;
    size_t num_bytes = 0;
    int max_line_size = 1000;
    char current_char = fgetc(inputfd);
    char *current_line = malloc(max_line_size);
    assert(current_line != NULL);

    /* each line ends with a new line character and NULL character */ 
    while (current_char != '\n' && current_char != EOF) {
        /* if the index is greater than max_line_size - 2, then the row has
                    more than 1000 characters and the char array is resized. */
        if (index >= max_line_size - 2) {
            max_line_size = (max_line_size * 2) + 2;
            current_line = realloc(current_line, max_line_size);
            assert(current_line != NULL);
        }
        current_line[index++] = current_char;
        num_bytes++;
        current_char = fgetc(inputfd);
    }

    if (current_char == '\n') {
        current_line[index++] = '\n';
        num_bytes++;
    }

    /* Add a NULL character in the last slot of the array. */
    current_line[index] = '\0';

    *datapp = current_line;
    return num_bytes;
}
