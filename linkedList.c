#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS
#define FALSE 0
#define TRUE !FALSE

// Frame struct
typedef struct Frame
{
    char* name;
    unsigned int duration;
    char* path;
} Frame;

// Link (node) struct
typedef struct FrameNode
{
    Frame* frame;
    struct FrameNode* next;
} FrameNode;

void insertString(char* location)
{
    fgets(location, 100, stdin);
    location[strcspn(location, "\n")] = 0;
}

int printMenu()
{
    int choice = 0;

    printf("0. Exit\n");
    printf("1. Add Frame\n");
    printf("2. Remove Frame\n");
    printf("3. Change Frame Location\n");
    printf("4. Change Frame Duration\n");
    printf("5. Change All Frames Duration\n");
    printf("6. Print All Frames\n");
    printf("7. Play The Movie\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);
    while (choice < 0 || choice > 7)
    {
        printf("Invalid choice, try again: ");
        scanf("%d", &choice);
    }
    return choice;
}

bool searchFrame(FrameNode * head, char* name)
{
    FrameNode* current = head;

    while (current != NULL)
    {
        if (strcmp(current->frame->name, name) == 0)
        {
            return true;
        }
        current = current->next;
    }
    return false;
}

bool addFrame(FrameNode * *head)
{
    FrameNode* current = NULL;
    FrameNode* newFrame = (FrameNode*)malloc(sizeof(FrameNode));
    char* path = (char*)malloc(sizeof(char) * 100);
    char* name = (char*)malloc(sizeof(char) * 100);
    int duration = 0;

    printf("*** Creating a new Frame ***\n");
    printf("Enter frame path:\n");
    insertString(path);
    printf("Enter frame duration:\n");
    scanf("%d", &duration);
    getchar();
    printf("Enter frame name:\n");
    insertString(name);
    while (searchFrame(*head, name))
    {
        printf("Frame with this name already exists, try again: ");
        insertString(name);
    }

    newFrame->frame = (Frame*)malloc(sizeof(Frame));
    newFrame->frame->path = path;
    newFrame->frame->name = name;
    newFrame->frame->duration = duration;
    newFrame->next = NULL;

    if (*head == NULL)
    {
        *head = newFrame;
    }
    else
    {
        current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newFrame;
    }
    return TRUE;
}

bool freeAllNode(FrameNode * *head)
{
    FrameNode* current = NULL;
    FrameNode* next = NULL;

    current = *head;
    while (current != NULL)
    {
        next = current->next;
        free(current->frame->path);
        free(current->frame->name);
        free(current->frame);
        free(current);
        current = next;
    }
    *head = NULL;
    return TRUE;
}

bool saveInFile(FrameNode * *head, char* path)
{
    FILE* file = fopen(path, "w");
    FrameNode* current = *head;

    while (current != NULL)
    {
        fprintf(file, "%s\n", current->frame->name);
        fprintf(file, "%s\n", current->frame->path);
        fprintf(file, "%d\n", current->frame->duration);
        current = current->next;
    }
    fclose(file);
    return TRUE;
}

bool deleteFrame(char* name, FrameNode * *head)
{
    FrameNode* current = *head;
    FrameNode* previous = NULL;

    while (current != NULL)
    {
        if (strcmp(current->frame->name, name) == 0)
        {
            if (previous == NULL)
            { // The node to delete is the head
                *head = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            free(current->frame->path);
            free(current->frame->name);
            free(current->frame);
            free(current);
            return TRUE;
        }
        previous = current;
        current = current->next;
    }
    return FALSE;
}

bool changeFrameLocation(FrameNode * *head, char* name, int newLocation)
{
    FrameNode* current = NULL;
    FrameNode* previous = NULL;
    FrameNode* toMove = NULL;
    int i = 0;

    if (*head == NULL || newLocation < 0)
    {
        return FALSE;
    }

    current = *head;

    // Find the frame to move
    while (current != NULL)
    {
        if (strcmp(current->frame->name, name) == 0)
        {
            toMove = current;
            break;
        }
        previous = current;
        current = current->next;
    }

    if (toMove == NULL)
    {
        return FALSE;
    }

    // Unlink the frame to move
    if (previous == NULL)
    {
        *head = toMove->next;
    }
    else
    {
        previous->next = toMove->next;
    }

    if (newLocation == 0)
    { // Move to the start
        toMove->next = *head;
        *head = toMove;
        return TRUE;
    }

    // Find the new location
    current = *head;
    for (i = 0; i < newLocation - 1 && current != NULL; i++)
    {
        current = current->next;
    }

    if (current == NULL)
    { // Invalid new location
        return FALSE;
    }

    // Insert the frame at the new location
    toMove->next = current->next;
    current->next = toMove;

    return TRUE;
}

bool changeFrameTime(char* name, FrameNode * *head, int newLen)
{
    FrameNode* current = *head;

    while (current != NULL)
    {
        if (strcmp(current->frame->name, name) == 0)
        {
            current->frame->duration = newLen;
            return TRUE;
        }
        current = current->next;
    }
    return FALSE;
}

void printFrames(FrameNode * head)
{
    FrameNode* current = head;

    while (current != NULL)
    {
        printf("Frame name: %s\n", current->frame->name);
        printf("Frame path: %s\n", current->frame->path);
        printf("Frame duration: %d\n", current->frame->duration);
        current = current->next;
    }
}

FrameNode** openProject(char* path)
{
    // TODO: open the project and return the head of the linked list
}
