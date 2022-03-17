/*
*   restoration.c
*
*   CS40 HW 1: filesofpix
*   By Gun Yang and Harnaljia Fenelon
*   Date: 01/28/22
* 
*   Summary:
*       This is the main implementation file for a restoration program that
*       converts a corrupted P2 pgm file to a normal P5 pgm file.
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <table.h>
#include <atom.h>
#include <seq.h>
#include <except.h>
#include <stdbool.h>
#include <string.h>
#include "readaline.h"
#include "dissect.h"
#include "conversion.h"

int main(int argc, char *argv[])
{
    FILE *image;

    if (argc == 1) {
        image = stdin;
    } else if (argc > 2) {
        assert(argc <=2);
    } else {
        image = fopen(argv[1], "rb");
        if (image == NULL) { 
            assert(image != NULL);
        }
    }

    size_t total_bytes;
    Table_T table = Table_new(0, NULL, NULL);
    Seq_T original_image = Seq_new(0);
    bool first_duplicate = false;
    int original_width;

    while (!feof(image)) {
        char *string;
        char *digit_bytes;
        char *non_digit_bytes;

        /* read a line from the opened file */
        total_bytes = readaline(image, &string);

        /* process the current row seperating the digit_bytes 
                                            from the non_digit_bytes. */ 
        int curr_width = dissect(string, total_bytes, &digit_bytes, 
                                                        &non_digit_bytes);

        const char *key = Atom_string(non_digit_bytes);
        const char *value = Atom_string(digit_bytes);

        /* check table for the reoccuring non_digit key */
        if (Table_get(table, key) != NULL) {
            /* The first time there's a duplicated key,
                                add the associated values to the sequence */
            if (first_duplicate == false) {
                original_width = curr_width;
                Seq_addhi(original_image, Table_get(table, key));
                Seq_addhi(original_image, (void *)value);
                first_duplicate = true;
            } else {
                /* Add every duplicated key's value */
                Seq_addhi(original_image, (void *)value);
            }
        } else {
            Table_put(table, key, (void *)value);
        }
        free(non_digit_bytes);
        free(digit_bytes);
        free(string);
    }

    int original_height = Seq_length(original_image);

    /* Convert the "plain: P2 file into a "raw" P5 file */
    raw_process(original_image, original_width, original_height);

    /* Free all remaining allocated space to prevent memory leaks */
    for(int i = 0; i < original_height; i++) {
        Seq_remhi(original_image);
    }
    Seq_free(&original_image);
    Table_free(&table);
    fclose(image);
    return EXIT_SUCCESS;
}