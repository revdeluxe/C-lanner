#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FILES 100
#define MAX_FILENAME_LENGTH 50
#define MAX_FILE_CONTENT_LENGTH 1000

struct File {
    char name[MAX_FILENAME_LENGTH];
    char content[MAX_FILE_CONTENT_LENGTH];
};

struct File files[MAX_FILES];
int num_files = 0;

void createFile(char *name, char *content) {
    if (num_files < MAX_FILES) {
        strcpy(files[num_files].name, name);
        strcpy(files[num_files].content, content);
        num_files++;
        printf("File %s created successfully.\n", name);

        // Write to actual file
        FILE *fp = fopen(name, "a");
        if (fp != NULL) {
            fputs(content, fp);
            fclose(fp);
        }
        else {
            printf("Error writing to file %s.\n", name);
        }
    } else {
        printf("Maximum number of files reached. Unable to create file %s.\n", name);
    }
}

void readFile(char *name) {
    int i;
    for (i = 0; i < num_files; i++) {
        if (strcmp(files[i].name, name) == 0) {
            printf("Content of file %s:\n%s\n", name, files[i].content);
            return;
        }
    }

    // Read from actual file
    FILE *fp = fopen(name, "r");
    if (fp != NULL) {
        char buffer[MAX_FILE_CONTENT_LENGTH];
        while (fgets(buffer, MAX_FILE_CONTENT_LENGTH, fp) != NULL) {
            printf("%s", buffer);
        }
        fclose(fp);
    }
    else {
        printf("File %s not found.\n", name);
    }
}

void writeFile(char *name, char *content) {
    int i;
    for (i = 0; i < num_files; i++) {
        if (strcmp(files[i].name, name) == 0) {
            strcpy(files[i].content, content);
            printf("Content of file %s updated successfully.\n", name);

            // Update actual file
            FILE *fp = fopen(name, "w");
            if (fp != NULL) {
                fputs(content, fp);
                fclose(fp);
            }
            else {
                printf("Error writing to file %s.\n", name);
            }
            return;
        }
    }
    printf("File %s not found. Unable to write.\n", name);
}

bool fileExists(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp != NULL) {
        fclose(fp);
        return true;
    }
    return false;
}
