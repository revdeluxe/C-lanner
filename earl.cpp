#include <iostream>
#include <string>

using namespace std;

const int MAX_NOTES = 100;
const int MAX_TASKS = 50;

char noteStack[MAX_NOTES][100];
int noteCount = 0;

char taskStack[MAX_NOTES][100];
int taskCount = 0;

void displayMainMenu () {
    cout << "MAIN MENU\n";
    cout << "\t1.Notes\n\t2.Tasks\n\t3. Exit\n";
    cout << "Enter your choice: ";
}

void displayNoteMenu () {
    cout << "NOTE MENU\n";
     cout << "\t1.Add/Edit\n\t2.Show notes\n\t3. Back\n";
    cout << "Enter your choice: ";
}

void displayTaskMenu () {
    cout << "TASK MENU\n";
     cout << "\t1.Set Tasks\n\t2.Show Pending Tasks\n\t3. Back\n";
    cout << "Enter your choice: ";
}

void AddEditNote() {
    cout << "Enter your note: ";
    cin.ignore();
   cin.getline(noteStack[noteCount], sizeof(noteStack[noteCount]));
    cout << "Note has been added/edited successfully!" <<endl;
    noteCount++;
}

void displayAllnote () {
      if (noteCount == 0) {
        cout << "EMPTY" << endl;
    } else {
        cout << "Your noted:\n";
        for (int i = 0; i < noteCount; ++i) {
            cout << i + 1 << ". " << noteStack[i] << endl;
        }
    }
}

void setTask () {
    cout << "Enter your Task: ";
    cin.ignore();
    cin.getline(taskStack[taskCount], sizeof(taskStack[taskCount]));
    cout << "Note has been added/edited successfully!" <<endl;
    taskCount++;
}

void displayAlltask () {
    if (noteCount == 0) {
        cout << "EMPTY" << endl;
    } else {
        cout << "Your notes:\n";
        for (int i = 0; i < taskCount; ++i) {
            cout << i + 1 << ". " << taskStack[i] << endl;
        }
    }
}

int main () {
    int mainChoice, noteChoice, taskChoice;

do {
    displayMainMenu();
    cin >> mainChoice;
    switch (mainChoice) {
        case 1: //Note
        do {
            displayNoteMenu();
            cin >> noteChoice;
            switch (noteChoice) {
                case 1: //Add/Edit
                AddEditNote ();
                break;
                case 2: //display note
                displayAllnote();
                break;
                case 3: //back
                break;
                default:
                cout << "Invalid input. Try again" << endl;
            }
    }while (noteChoice != 3);
    break;
        case 2: // Task
        do {
            displayTaskMenu();
            cin >> taskChoice;
            switch (taskChoice) {
                case 1: //set task
                setTask ();
                break;
                case 2: //Show pending
                displayAlltask();
                break;
                case 3: //back
                break;
                default:
                cout << "Invalid input. Try again" << endl;
            }
    }while (taskChoice != 3);
    break;
        case 3:
        cout << "Program end." << endl;
        break;
        default:
        cout << "Invalid input. Try again" << endl;
}
}while (taskChoice != 3);
    return 0;
}
