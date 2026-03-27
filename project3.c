#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read.h"
#include "main.h"

WAV wav;

/* Helper: check if file ends with .wav */
int is_wav_file(const char *filename)
{
    const char *ext = strrchr(filename, '.');
    return (ext && strcmp(ext, ".wav") == 0);
}

/* Helper: print usage */
void print_usage()
{
    puts("Usage:");
    puts("-list <filename>");
    puts("-reverse <input> <output>");
    puts("-mono <input> <output>");
    puts("-crop <input> <output> <start> <end>");
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        puts("Too few arguments");
        print_usage();
        return 1;
    }

    if (argc > 6)
    {
        puts("Too many arguments");
        print_usage();
        return 1;
    }

    /* ===================== -list ===================== */
    if (!strcmp(argv[1], "-list"))
    {
        if (argc != 3)
        {
            puts("Invalid number of arguments");
            puts("Usage: ./yawn -list <filename>");
            return 1;
        }

        if (!is_wav_file(argv[2]))
        {
            puts("Invalid file type. Please provide a .wav file");
            return 1;
        }

        printf("\nReading the header of \"%s\"...\n", argv[2]);
        read_header(argv[2]);
        printf("\nReading completed successfully!\n");

        printf("\nListing contents...\n\n");
        list();
        printf("\nListing completed successfully!\n");
    }

    /* ===================== -reverse ===================== */
    else if (!strcmp(argv[1], "-reverse"))
    {
        if (argc != 4)
        {
            puts("Invalid number of arguments");
            puts("Usage: ./yawn -reverse <input> <output>");
            return 1;
        }

        if (!is_wav_file(argv[2]) || !is_wav_file(argv[3]))
        {
            puts("Both input and output must be .wav files");
            return 1;
        }

        printf("\nReading \"%s\"...\n", argv[2]);
        read_header(argv[2]);

        printf("\nReversing audio...\n");
        reverse(argv[3]);

        printf("\nDone! Output saved to \"%s\"\n", argv[3]);
    }

    /* ===================== -mono ===================== */
    else if (!strcmp(argv[1], "-mono"))
    {
        if (argc != 4)
        {
            puts("Invalid number of arguments");
            puts("Usage: ./yawn -mono <input> <output>");
            return 1;
        }

        if (!is_wav_file(argv[2]) || !is_wav_file(argv[3]))
        {
            puts("Both input and output must be .wav files");
            return 1;
        }

        printf("\nReading \"%s\"...\n", argv[2]);
        read_header(argv[2]);

        if (wav.fmt.numChannels == 1)
        {
            printf("\nAlready mono. No conversion needed.\n");
            return 0;
        }

        printf("\nConverting to mono...\n");
        mono(argv[3]);

        printf("\nDone! Output saved to \"%s\"\n", argv[3]);
    }

    /* ===================== -crop ===================== */
    else if (!strcmp(argv[1], "-crop"))
    {
        if (argc != 6)
        {
            puts("Invalid number of arguments");
            puts("Usage: ./yawn -crop <input> <output> <start> <end>");
            return 1;
        }

        if (!is_wav_file(argv[2]) || !is_wav_file(argv[3]))
        {
            puts("Input and output must be .wav files");
            return 1;
        }

        int start = atoi(argv[4]);
        int end = atoi(argv[5]);

        if (start < 0 || end <= 0)
        {
            puts("Start and end must be positive integers");
            return 1;
        }

        if (start >= end)
        {
            puts("Start time must be smaller than end time");
            return 1;
        }

        printf("\nReading \"%s\"...\n", argv[2]);
        read_header(argv[2]);

        printf("\nCropping audio...\n");
        crop(argv[3], start, end);

        printf("\nDone! Output saved to \"%s\"\n", argv[3]);
    }

    /* ===================== INVALID ===================== */
    else
    {
        puts("Invalid command");
        print_usage();
        return 1;
    }

    return 0;
}