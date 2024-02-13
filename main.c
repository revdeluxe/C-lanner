#include <stdio.h>
#include <string.h>
#include "savetofile.h" // Personally made by yours truly

#define MAX_NOTES 100
#define MAX_TASKS 50

char noteStack[MAX_NOTES][100];
int noteCount = 0;

char taskStack[MAX_TASKS][100];
int taskCount = 0;

void displayMainMenu () {
    printf("MAIN MENU\n");
    printf("\t1.Notes\n\t2.Tasks\n\t3. Exit\n");
    printf("Enter your choice: ");
}

void displayNoteMenu () {
    printf("NOTE MENU\n");
    printf("\t1.Add/Edit\n\t2.Show notes\n\t3. Back\n");
    printf("Enter your choice: ");
}

void displayTaskMenu () {
    printf("TASK MENU\n");
    printf("\t1.Set Tasks\n\t2.Show Pending Tasks\n\t3. Back\n");
    printf("Enter your choice: ");
}


void AddEditNote() {
    printf("Enter your note: ");
    getchar();
    fgets(noteStack[noteCount], sizeof(noteStack[noteCount]), stdin);
    noteStack[noteCount][strlen(noteStack[noteCount]) - 1] = '\n';
    createFile ("note.txt", noteStack[noteCount]);
    noteCount++;
}

void displayAllnote () {
    readFile("note.txt");
}

void setTask () {
    printf("Enter your Task: ");
    getchar();
    fgets(taskStack[taskCount], sizeof(taskStack[taskCount]), stdin);
    taskStack[taskCount][strlen(taskStack[taskCount]) - 1] = '\n';
    createFile ("task.txt", taskStack[taskCount]);
    taskCount++;
}

void displayAlltask () {
    readFile("task.txt");
}

int main () {
    int mainChoice, noteChoice, taskChoice;

    do {
        displayMainMenu();
        scanf("%d", &mainChoice);
        switch (mainChoice) {
            case 1: // Note
                do {
                    displayNoteMenu();
                    scanf("%d", &noteChoice);
                    switch (noteChoice) {
                        case 1: // Add/Edit
                            AddEditNote ();
                            break;
                        case 2: // Display notes
                            displayAllnote();
                            break;
                        case 3: // Back
                            break;
                        default:
                            printf("Invalid input. Try again\n");
                    }
                } while (noteChoice != 3);
                break;
            case 2: // Task
                do {
                    displayTaskMenu();
                    scanf("%d", &taskChoice);
                    switch (taskChoice) {
                        case 1: // Set task
                            setTask();

                            break;
                        case 2: // Show pending
                            displayAlltask();
                            break;
                        case 3: // Back
                            break;
                        default:
                            printf("Invalid input. Try again\n");
                    }
                } while (taskChoice != 3);
                break;
            case 3: // Exit
                printf("Program end.\n");
                break;
            default:
                printf("Invalid input. Try again\n");
        }
    } while (mainChoice != 3);

    return 0;
}
