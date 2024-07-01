#define _CRT_SECURE_NO_WARNINGS
#include "fileHandler.h"
#include "LinkedList.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define SIZE 100

void openProject(char* path, FrameNode** head)
{
    FILE* file = fopen(path, "r");
    if (file == NULL)
    {
        printf("File not found!\n");
        return NULL;
    }

    *head = NULL;
    FrameNode* newFrame;
    char* name = (char*)malloc(sizeof(char) * SIZE);
    char* path2 = (char*)malloc(sizeof(char) * SIZE);
    int duration = 0;

    rewind(file);
    while (fscanf(file, "%[^,],%d,%[^\n]\n", name, &duration, path2) != EOF)
    {
        newFrame = (FrameNode*)malloc(sizeof(FrameNode));
        newFrame->frame = (Frame*)malloc(sizeof(Frame));
        newFrame->frame->name = (char*)malloc((strlen(name) + 1) * sizeof(char));
        strcpy(newFrame->frame->name, name);
        newFrame->frame->duration = duration;
        newFrame->frame->path = (char*)malloc((strlen(path2) + 1) * sizeof(char));
        strcpy(newFrame->frame->path, path2);
        newFrame->next = *head;
        *head = newFrame;
    }

    fclose(file);
    free(name);
    free(path2);
}

bool saveInFile(FrameNode** head, char* path)
{
    FILE* file = fopen(path, "w");
    if (file == NULL)
    {
        printf("Failed to open file for writing.\n");
        return false;
    }

    FrameNode* current = *head;

    while (current != NULL)
    {
        fprintf(file, "%s,%d,%s\n", current->frame->name, current->frame->duration, current->frame->path);
        current = current->next;
    }

    fclose(file);
    return true;
}

int openOrcreateProject()
{
    int choice = 0;
    printf(" [0] Create a new project\n");
    printf(" [1] Open an existing project\n");
    scanf("%d", &choice);
    getchar();
    while (choice != 0 && choice != 1)
    {
        errorMsg();
        return openOrcreateProject();
    }
    return choice;
}

bool checkPath(char* path)
{
    FILE* file = fopen(path, "r");
    if (file)
    {
        fclose(file);
        return true;
    }
    return false;
}

void startProject()
{
    FrameNode* head = NULL;
    int num = -1;
    int openOrCreate = 0;
    printf("Welcome to the Magshimim Video Maker!\n");
    openOrCreate = openOrcreateProject();
    char* pathOfProject = (char*)malloc(SIZE * sizeof(char));

    if (openOrCreate == 1)
    {
        printf("Enter the path of the project (including project name):\n");
        insertString(pathOfProject);
        if (checkPath(pathOfProject))
        {
            openProject(pathOfProject, &head);
        }
        else
        {
            printf("Error! Can't open file, creating a new project\n");
            head = NULL;
            system("pause");
        }
    }

    while (num != 0)
    {
        system("cls");
        num = printMenu();
        navigate(num, &head);

    }

    free(pathOfProject);
}