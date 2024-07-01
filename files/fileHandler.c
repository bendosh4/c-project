#define _CRT_SECURE_NO_WARNINGS
#include "fileHandler.h"
#include "LinkedList.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define SIZE 100

/*
this function open the project (file)
input: the path of the project, the head of the linked list
output: none
runtime: O(n)
    */
void openProject(char* path, FrameNode** head)
{
    FILE* file = fopen(path, "r");
    if (file == NULL)
    {
        printf("File not found!\n");
        system("pause");
        return;
    }

    *head = NULL;
    FrameNode* newFrame;
    char* name = (char*)malloc(sizeof(char) * SIZE);
    char* path2 = (char*)malloc(sizeof(char) * SIZE);
    int duration = 0;

    // read the file in a spical way 
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
    reverseOrder(head);
}

/*
this function saves the project (file)
input: the head of the linked list and the path of the project
output: true if the project is saved, false if not
runtime: O(n)
    */
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

/*
this function gets a choice opens or creates a project (file)
input: none
output: 1 if the user chose to open a project, 0 if the user chose to create a project
runtime: O(1)
    */
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

/*
this function checks if the path is valid
input: the path of the project
output: true if the path is valid, false if not
runtime: O(1)
    */
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

/*
this function reverses the order of the frames in the linked list
input: the head of the linked list
output: none
runtime: O(n)
    */
void reverseOrder(FrameNode** head)
{
    FrameNode* current = *head;
    FrameNode* prev = NULL;
    FrameNode* next = NULL;

    while (current != NULL)
    {
        next = current->next; // Store the next node
        current->next = prev; // Reverse the pointer
        prev = current; // Move to the next node
        current = next; // Move to the next node
    }
    *head = prev; // Update the head to the new reversed list
}
