// Make me a program that allows me to write boilerplate code from a saved txt document and put it in a new file which is the first argument in the command line

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

void createDir(const char *dirName) {
#ifdef _WIN32
    if (!(CreateDirectory(dirName, NULL) || GetLastError() == ERROR_ALREADY_EXISTS)) {
        exit(EXIT_FAILURE);
    }
#else
    struct stat st = {0};
    if (stat(dirName, &st) == -1) {
        if (mkdir(dirName, 0700) != 0) {
            exit(EXIT_FAILURE);
        }
    }
#endif
}


void appendBoilerplateToFile(const char *boilerplateFile, const char *filename) {
    FILE *boilerplate;
    FILE *file;
    char line[1024];

    boilerplate = fopen(boilerplateFile, "r");
    if (boilerplate == NULL) {
        perror("Failed to open boilerplate file");
        exit(EXIT_FAILURE);
    }

    file = fopen(filename, "a");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), boilerplate) != NULL) {
        fputs(line, file);
    }

    fclose(boilerplate);
    fclose(file);
}

void createFile(const char *filename) {
    FILE *file;
    char dirName[512];
    char *lastSlash;

    lastSlash = strrchr(filename, '/');
#ifdef _WIN32
    if (lastSlash == NULL) {
        lastSlash = strrchr(filename, '\\');
    }
#endif

        if (lastSlash != NULL) {
            size_t length = lastSlash - filename;
            strncpy(dirName, filename, length);
            dirName[length] = '\0';

            createDir(dirName);
        }

        file = fopen(filename, "w");
        if (file == NULL) {
            perror("Failed to create file");
            exit(EXIT_FAILURE);
        }
        appendBoilerplateToFile("boilerplate.txt", filename);
    }



int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }


    createFile(argv[1]);

    return 0;
}