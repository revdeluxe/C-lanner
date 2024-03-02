#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

#define FILE_BOX_WIDTH 120
#define FILE_BOX_HEIGHT 320
#define FILE_BOX_MARGIN 10
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576
#define FONT_SIZE 24
#define FONT_PATH "include/fonts/PTMono-Regular.ttf"
#define NUM_MENU_OPTIONS 3
#define FILE_BOX_MARGIN_X 10
#define FILE_BOX_MARGIN_Y 10
#define NOTES_DIRECTORY "./notes/"
#define TASKS_DIRECTORY "./tasks/"
#define CSV_DELIMITER ","

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font* gFont = NULL;

enum MenuState {
    MENU_MAIN,
    MENU_NOTES,
    MENU_TASK,
    MENU_ABOUT
};

enum MenuOption {
    NOTES,
    SET_TASK,
    ABOUT
};

const char* menuOptions[NUM_MENU_OPTIONS] = {
    "Notes",
    "Set Task",
    "About"
};

const char* notesOptions[NUM_MENU_OPTIONS] = {
    "Add Notes",
    "Edit Notes",
    "Delete Notes"
};

const char* taskOptions[NUM_MENU_OPTIONS] = {
    "Set task",
    "Mark Done",
    "Task Status"
};

const char* aboutUsInfo = "About Us: We are a team of developers dedicated to creating useful software applications.";

int currentOption = NOTES;
int currentMenuState = MENU_MAIN;
int previousMenuState = MENU_MAIN;

typedef struct {
    SDL_Rect rect;
    char text[100];
    bool active;
} TextInput;

TextInput taskNameInput;
TextInput taskDescriptionInput;
TextInput notesInput;

void renderText(const char* text, int x, int y, SDL_Color color, TTF_Font* font) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(textTexture, NULL, NULL, &texW, &texH);
    SDL_Rect dstRect = {x, y, texW, texH};
    SDL_RenderCopy(gRenderer, textTexture, NULL, &dstRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void renderTextInput(TextInput *textInput) {

    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderFillRect(gRenderer, &textInput->rect);


    if (textInput->active) {
        SDL_Color textColor = {0, 0, 0, 255};
        renderText(textInput->text, textInput->rect.x + 5, textInput->rect.y + 5, textColor, gFont);
    }
}

void handleTextInputEvents(SDL_Event *event, TextInput *textInput) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        if (mouseX >= textInput->rect.x && mouseX <= (textInput->rect.x + textInput->rect.w) &&
            mouseY >= textInput->rect.y && mouseY <= (textInput->rect.y + textInput->rect.h)) {
            textInput->active = true;
        } else {
            textInput->active = false;
        }
    } else if (event->type == SDL_TEXTINPUT && textInput->active) {
        strcat(textInput->text, event->text.text);
    }
}

void renderSavedFiles(int scrollOffset) {

    SDL_Color textColor = {255, 255, 255, 255};

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(NOTES_DIRECTORY)) != NULL) {
        int y = FILE_BOX_MARGIN_Y;
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                char filePath[100];
                sprintf(filePath, "%s%s", NOTES_DIRECTORY, ent->d_name);
                FILE* file = fopen(filePath, "r");
                if (file != NULL) {
                    renderText(ent->d_name, FILE_BOX_MARGIN_X, y, textColor, gFont);
                    char fileContents[1000];
                    fgets(fileContents, sizeof(fileContents), file);
                    renderText(fileContents, FILE_BOX_MARGIN_X, y + FONT_SIZE + 5, textColor, gFont);
                    fclose(file);
                    y += FILE_BOX_HEIGHT + FILE_BOX_MARGIN;
                }
            }
        }
        closedir(dir);
    } else {
        perror("Error opening directory");
    }
}

void renderSetTaskInterface() {
    SDL_Color textColor = {255, 255, 255, 255};
    renderText("When", 200, 80, textColor, gFont);
    taskNameInput.rect.x = 200;
    taskNameInput.rect.y = 100;
    taskNameInput.rect.w = 300;
    taskNameInput.rect.h = 30;
    renderTextInput(&taskNameInput);
    renderText("Description", 200, 180, textColor, gFont);
    taskDescriptionInput.rect.x = 200;
    taskDescriptionInput.rect.y = 150;
    taskDescriptionInput.rect.w = 300;
    taskDescriptionInput.rect.h = 30;
    renderTextInput(&taskDescriptionInput);
    renderText("Confirm", 200, 200, textColor, gFont);
    renderText("Cancel", 200, 250, textColor, gFont);
}

void ranOutOfTime(){
  SDL_Color textColor = {255, 255, 255, 255};
  renderText("I ran out of time implement this", 200, 200, textColor, gFont);
}

void handleSetTask() {
    time_t currentTime = time(NULL);
    struct tm* localTime = localtime(&currentTime);
    char dateTime[50];
    strftime(dateTime, sizeof(dateTime), "%Y-%m-%d %H:%M:%S", localTime);
    char filePath[150];
    sprintf(filePath, "%s%s.csv", TASKS_DIRECTORY, taskNameInput.text);
    FILE* file = fopen(filePath, "w");
    if (file == NULL) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }

    fprintf(file, "Task Name%sTask Description%sDate and Time\n", CSV_DELIMITER, CSV_DELIMITER);
    fprintf(file, "%s%s%s%s%s\n", taskNameInput.text, CSV_DELIMITER, taskDescriptionInput.text, CSV_DELIMITER, dateTime);
    fclose(file);
    printf("Task set successfully.\n");
    memset(taskNameInput.text, 0, sizeof(taskNameInput.text));
    memset(taskDescriptionInput.text, 0, sizeof(taskDescriptionInput.text));
}

void renderNotesInterface() {
    notesInput.rect.x = 200;
    notesInput.rect.y = 100;
    notesInput.rect.w = 500;
    notesInput.rect.h = 400;
    renderTextInput(&notesInput);
    SDL_Color textColor = {255, 255, 255, 255};
    renderText("Save", 10, 520, textColor, gFont);
}

void handleSaveNotes() {
    time_t currentTime = time(NULL);
    struct tm* localTime = localtime(&currentTime);
    char fileName[50];
    strftime(fileName, sizeof(fileName), "%Y%m%d%H%M%S", localTime);
    strcat(fileName, ".txt");
    char filePath[100];
    sprintf(filePath, "%s%s", NOTES_DIRECTORY, fileName);
    FILE* file = fopen(filePath, "w");
    if (file == NULL) {
        printf("Error: Unable to open file for writing.\n");
        return;
    }
    fprintf(file, "%s\n", notesInput.text);
    fclose(file);
    printf("Notes saved successfully.\n");
    memset(notesInput.text, 0, sizeof(notesInput.text));
}

void renderMenu() {
    SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(gRenderer);
    SDL_Color titleColor = {255, 255, 255, 255};
    TTF_Font* titleFont = TTF_OpenFont(FONT_PATH, FONT_SIZE * 2);
    if (titleFont == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }
    renderText("C planner", SCREEN_WIDTH / 2 - 120, 10, titleColor, titleFont);
    TTF_CloseFont(titleFont);
    const char** currentMenuOptions;
    int numOptions;
    switch (currentMenuState) {
        case MENU_MAIN:
            currentMenuOptions = menuOptions;
            numOptions = NUM_MENU_OPTIONS;
            break;
        case MENU_ABOUT:
            currentMenuOptions = &aboutUsInfo;
            numOptions = 1;
            break;
        case MENU_NOTES:
            currentMenuOptions = notesOptions;
            numOptions = NUM_MENU_OPTIONS;
            break;
        case MENU_TASK:
            currentMenuOptions = taskOptions;
            numOptions = NUM_MENU_OPTIONS;
            break;
    }

    SDL_Color defaultColor = {255, 255, 255, 255};
    SDL_Color highlightColor = {255, 0, 0, 255};

    for (int i = 0; i < numOptions; ++i) {
        SDL_Color textColor = (i == currentOption) ? highlightColor : defaultColor;
        renderText(currentMenuOptions[i], 10, 100 + i * FONT_SIZE, textColor, gFont);
    }
    if (currentMenuState == MENU_TASK && currentOption == 0) {
        renderSetTaskInterface();
    }
    if (currentMenuState == MENU_TASK && currentOption == 2) {
        ranOutOfTime();
    }
    if (currentMenuState == MENU_NOTES && currentOption == 1) {
        renderSavedFiles(0);
    }
    if (currentMenuState == MENU_NOTES && currentOption == 2) {
        ranOutOfTime();
    }
    if (currentMenuState == MENU_NOTES && currentOption == 0) {
        renderNotesInterface();
    }
    SDL_RenderPresent(gRenderer);
}

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    gWindow = SDL_CreateWindow("C - Planner", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }
    gFont = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (gFont == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        currentOption = (currentOption - 1 + NUM_MENU_OPTIONS) % NUM_MENU_OPTIONS;
                        break;
                    case SDLK_DOWN:
                        currentOption = (currentOption + 1) % NUM_MENU_OPTIONS;
                        break;
                    case SDLK_RETURN:
                        if (currentMenuState == MENU_MAIN) {
                            switch (currentOption) {
                                case NOTES:
                                    previousMenuState = currentMenuState;
                                    currentMenuState = MENU_NOTES;
                                    break;
                                case SET_TASK:
                                    previousMenuState = currentMenuState;
                                    currentMenuState = MENU_TASK;
                                    break;
                                case ABOUT:
                                    previousMenuState = currentMenuState;
                                    currentMenuState = MENU_ABOUT;
                                    break;
                                default:
                                    printf("Selected Option: %s\n", menuOptions[currentOption]);
                                    break;
                            }
                        } else if (currentMenuState == MENU_TASK && currentOption == 0) {
                            handleSetTask();
                        } else if (currentMenuState == MENU_NOTES && currentOption == 0) {
                            handleSaveNotes();
                        } else {
                            currentMenuState = previousMenuState;
                        }
                        break;
                    case SDLK_ESCAPE:
                        currentMenuState = previousMenuState;
                        break;
                    default:
                        break;
                }
            }
            if (currentMenuState == MENU_TASK && currentOption == 0) {
                handleTextInputEvents(&e, &taskNameInput);
            } else if (currentMenuState == MENU_TASK && currentOption == 1) {
                handleTextInputEvents(&e, &taskDescriptionInput);
            } else if (currentMenuState == MENU_NOTES && currentOption == 0) {
                handleTextInputEvents(&e, &notesInput);
            }
        }
        renderMenu();
    }
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    TTF_CloseFont(gFont);
    gRenderer = NULL;
    gWindow = NULL;
    gFont = NULL;
    TTF_Quit();
    SDL_Quit();
    return 0;
}

