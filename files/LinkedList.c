#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "view.h"
#include "LinkedList.h"
#include "fileHandler.h"

#pragma warning(disable : 4996)
#define SIZE 100

#define EXIT 0 
#define ADD_FRAME 1
#define REMOVE_FRAME 2
#define CHANGE_FRAME_LOCATION 3
#define CHANGE_FRAME_DURATION 4
#define CHANGE_ALL_FRAME_DURATION 5 
#define PRINT_ALL_FRAMES 6
#define PLAY_THE_MOVIE 7
#define SAVE_THE_PROJECT 8

void insertString(char* location)
{
    fgets(location, SIZE, stdin);
    location[strcspn(location, "\n")] = 0;
}

int printMenu()
{
    int choice = 0;
    printf("What would you like to do\n");
    printf(" 0. Exit\n");
    printf(" 1. Add Frame\n");
    printf(" 2. Remove Frame\n");
    printf(" 3. Change Frame Location\n");
    printf(" 4. Change Frame Duration\n");
    printf(" 5. Change All Frames Duration\n");
    printf(" 6. Print All Frames\n");
    printf(" 7. Play The Movie\n");
    printf(" 8. Save the File\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar();
    while (choice < EXIT || choice > SAVE_THE_PROJECT)
    {
        printf("Invalid choice, try again: ");
        scanf("%d", &choice);
        getchar();
    }
    printf("\n\n");
    return choice;
}

bool searchFrame(FrameNode* head, char* name)
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

bool addFrame(FrameNode** head)
{
    FrameNode* current = NULL;
    FrameNode* newFrame = (FrameNode*)malloc(sizeof(FrameNode));
    newFrame->frame = (Frame*)malloc(sizeof(Frame));
    newFrame->frame->path = (char*)malloc(sizeof(char) * SIZE); // Adjust size as needed
    newFrame->frame->name = (char*)malloc(sizeof(char) * SIZE); // Adjust size as needed

    int duration = 0;

    printf("*** Creating a new Frame ***\n");
    printf("Enter frame path:\n");
    insertString(newFrame->frame->path);

    printf("Please insert frame duration (in milliseconds):\n");
    scanf("%d", &duration);
    getchar();

    printf("Enter frame name:\n");
    insertString(newFrame->frame->name);

    // Check if head points to NULL (empty list)
    if (*head != NULL && searchFrame(*head, newFrame->frame->name))
    {
        printf("Frame with this name already exists.\n");
        free_one_node(&newFrame);
        return false;
    }
    else if (!checkPath(newFrame->frame->path))
    {
        printf("File is not exist!! check path\n");
        free_one_node(&newFrame);
        system("pause");
        return false;
    }

    newFrame->frame->duration = duration;
    newFrame->next = NULL;

    // Insert new frame into the linked list
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

    return true;
}


bool freeAllNode(FrameNode** head)
{
    FrameNode* current = *head;
    FrameNode* next = NULL;

    while (current != NULL)
    {
        next = current->next;
        free_one_node(&current);
        current = next;
    }
    *head = NULL;
    return true;
}

bool deleteFrame(char* name, FrameNode** head)
{
    FrameNode* current = *head;
    FrameNode* previous = NULL;

    while (current != NULL)
    {
        if (strcmp(current->frame->name, name) == 0)
        {
            if (previous == NULL)
            {
                *head = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            free_one_node(&current);
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}

bool changeFrameLocation(FrameNode** head, char* name, int newLocation)
{
    if (newLocation < 1)
    {
        return false;
    }

    FrameNode* current = *head;
    FrameNode* previous = NULL;
    FrameNode* targetNode = NULL;
    FrameNode* targetPrev = NULL;
    int index = 0;

    while (current != NULL)
    {
        if (strcmp(current->frame->name, name) == 0)
        {
            targetNode = current;
            targetPrev = previous;
        }
        previous = current;
        current = current->next;
    }

    if (targetNode == NULL)
    {
        return false;
    }

    if (targetPrev == NULL)
    {
        *head = targetNode->next;
    }
    else
    {
        targetPrev->next = targetNode->next;
    }

    if (newLocation == 1)
    {
        targetNode->next = *head;
        *head = targetNode;
        return true;
    }

    current = *head;
    previous = NULL;
    index = 1;

    while (current != NULL && index < newLocation)
    {
        previous = current;
        current = current->next;
        index++;
    }

    if (previous != NULL)
    {
        previous->next = targetNode;
        targetNode->next = current;
    }
    else
    {
        targetNode->next = *head;
        *head = targetNode;
    }

    return true;
}

bool changeFrameTime(char* name, FrameNode** head, int newLen)
{
    FrameNode* current = *head;

    while (current != NULL)
    {
        if (strcmp(current->frame->name, name) == 0)
        {
            current->frame->duration = newLen;
            return true;
        }
        current = current->next;
    }
    return false;
}

void printFrames(FrameNode* head)
{
    FrameNode* current = head;
    printf("\t\tName\tTime\tPath\n");
    while (current != NULL)
    {
        printf("\t\t%s", current->frame->name);
        printf("\t%d ms", current->frame->duration);
        printf("\t%s", current->frame->path);
        printf("\n");
        current = current->next;
    }
    system("pause");
}

void errorMsg()
{
    printf("Something went wrong, check parameters!!\n");
    system("pause");
}

int lenOfNodeRecursive(FrameNode* head)
{
    if (head == NULL)
    {
        return 0;
    }
    return 1 + lenOfNodeRecursive(head->next);
}

bool changeAllFrameDuration(FrameNode** head, int newLen)
{
    FrameNode* current = *head;
    while (current != NULL)
    {
        current->frame->duration = newLen;
        current = current->next;
    }
    return true;
}

void navigate(int choice, FrameNode** head)
{
    char* name = (char*)malloc(sizeof(char) * SIZE);
    int newLocation = 0;
    int newLen = 0;

    switch (choice)
    {
    case EXIT:
        freeAllNode(head);
        printf("Exiting...\n");
        break;
    case ADD_FRAME:
        addFrame(head);
        break;
    case REMOVE_FRAME:
        printf("Enter the frame name to delete: ");
        insertString(name);
        if (deleteFrame(name, head))
        {
            printf("Frame deleted successfully.\n");
        }
        else
        {
            errorMsg();
        }
        system("pause");
        break;
    case CHANGE_FRAME_LOCATION:
        printf("Enter the frame name to change location: ");
        insertString(name);
        printf("Enter the new location: ");
        scanf("%d", &newLocation);
        getchar();
        if (changeFrameLocation(head, name, newLocation))
        {
            printf("Frame location changed successfully.\n");
        }
        else if (lenOfNodeRecursive(*head) < newLocation)
        {
            printf("Invalid new location.\n");
        }
        else
        {
            errorMsg();
        }
        system("pause");
        break;

    case CHANGE_FRAME_DURATION:
        printf("Enter the frame name to change duration: ");
        insertString(name);
        printf("Enter the new duration: ");
        scanf("%d", &newLen);
        getchar();
        if (changeFrameTime(name, head, newLen))
        {
            printf("Frame duration changed successfully.\n");
        }
        else
        {
            errorMsg();
        }
        system("pause");
        break;

    case CHANGE_ALL_FRAME_DURATION:
        printf("Enter the new duration for all frames: ");
        scanf("%d", &newLen);
        getchar();
        if (changeAllFrameDuration(head, newLen))
        {
            printf("All frames duration changed successfully.\n");
        }
        else
        {
            errorMsg();
        }
        system("pause");
        break;

    case PRINT_ALL_FRAMES:
        printFrames(*head);
        break;

    case PLAY_THE_MOVIE:
        play(*head);
        break;

    case SAVE_THE_PROJECT:
        printf("Where to save the project? enter a full path and file name\n");
        insertString(name);
        saveInFile(head, name);
        system("pause");
        break;

    default:
        printf("Pick a number between 1-8:\n");
        system("pause");
        break;
    }
    free(name);
}

void free_one_node(FrameNode** ToRemove)
{
    FrameNode* node = *ToRemove;
    free(node->frame->path);
    free(node->frame->name);
    free(node->frame);
    free(node);
}

