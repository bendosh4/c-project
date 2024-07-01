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

/*
this function gets a pointer to a string and inserts a string in it
input: a pointer to a string
output: none
runtime: O(1)
    */
void insertString(char* location)
{
    fgets(location, SIZE, stdin);
    location[strcspn(location, "\n")] = 0;
}

/*
this function prints the menu and gets the choice from the user
input: none
output: none
runtime: O(choice 1-8)
    */
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

/*
this functiom sreach for a frame in the linked list
input: the head of the linked list and the name of the frame
output: true if the frame is found, false if not
runtime: O(n) len of the linked list
*/
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

/*
this function adds a frame to the linked list
input: the head of the linked list
output: true if the frame is added, false if not
runtime: O(1)
    */
bool addFrame(FrameNode** head)
{
    FrameNode* current = NULL;
    FrameNode* newFrame = (FrameNode*)malloc(sizeof(FrameNode));
    newFrame->frame = (Frame*)malloc(sizeof(Frame)); // init the frame
    newFrame->frame->path = (char*)malloc(sizeof(char) * SIZE); // init the path
    newFrame->frame->name = (char*)malloc(sizeof(char) * SIZE); // init the name

    int duration = 0;

    printf("*** Creating a new Frame ***\n");
    printf("Enter frame path:\n");
    insertString(newFrame->frame->path);

    printf("Please insert frame duration (in milliseconds):\n");
    scanf("%d", &duration);
    getchar();

    printf("Enter frame name:\n");
    insertString(newFrame->frame->name);

    
    if (*head != NULL && searchFrame(*head, newFrame->frame->name)) // check if head points to NULL (empty list)
    {
        printf("Frame with this name already exists.\n");
        free_one_node(&newFrame);
        return false;
    }
    else if (!checkPath(newFrame->frame->path)) // check if the path is valid
    {
        printf("File is not exist!! check path\n");
        free_one_node(&newFrame);
        system("pause");
        return false;
    }
    
    newFrame->frame->duration = duration; // adding the frame duration to the linked list
    newFrame->next = NULL; // adjusting the next pointer

    // Insert new frame into the linked list
    if (*head == NULL) // if the list is empty then the new frame will be the head
    {
        *head = newFrame; // head points to the new frame
    }
    else // if the list is not empty then the new frame will be added to
    {
        current = *head; // current points to the head
        while (current->next != NULL) // finds the last node in the list
        {
            current = current->next; // current points to the next node
        }
        current->next = newFrame; // the last node points to the new frame
    }

    return true;
}

/*
this function free all the nodes in the linked list
input: the head of the linked list
output: none
runtime: O(n)
    */
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

/*
this function deletes a frame from the linked list
input: the name of the frame and the head of the linked
output: true if the frame is deleted, false if not
runtime: O(n)
    */
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

/*
this function changes the location of a frame in the linked list
input: the name of the frame and the head of the linked list
output: true if the frame is changed, false if not
runtime: O(n^2) 2 while loop
    */
bool changeFrameLocation(FrameNode** head, char* name, int newLocation)
{
    if (newLocation < 1) // check if the new location is valid
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
        if (strcmp(current->frame->name, name) == 0) // find the target node
        {
            targetNode = current;
            targetPrev = previous;
        }
        previous = current; // previous points to the current node
        current = current->next; // current points to the next node
    }

    if (targetNode == NULL) // check if the target node is found
    {
        return false;
    }

    if (targetPrev == NULL) // check if the target node is the head
    {
        *head = targetNode->next;
    }
    else // if the target node is not the head
    {
        targetPrev->next = targetNode->next;
    }

    if (newLocation == 1) // check if the new location is the head
    {
        targetNode->next = *head;
        *head = targetNode;
        return true;
    }

    current = *head;
    previous = NULL;
    index = 1;

    while (current != NULL && index < newLocation) // find the new location
    {
        previous = current;
        current = current->next;
        index++;
    }

    if (previous != NULL) // check if the new location is not the head
    {
        previous->next = targetNode;
        targetNode->next = current;
    }
    else // if the new location is the head
    {
        targetNode->next = *head;
        *head = targetNode;
    }

    return true;
}

/*
this function changes the duration of a frame in the linked list
input: the name of the frame and the head of the linked list
output: true if the frame is changed, false if not
runtime: O(n)
    */
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

/*
this function prints all the frames in the linked list
input: the head of the linked list
output: none
runtime: O(n)
    */
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

/*
this function prints a error message
input: none
output: none
runtime: O(1)
    */
void errorMsg()
{
    printf("Something went wrong, check parameters!!\n");
    system("pause");
}

/*
this function gets the length of the linked list
input: the current Frame of the linked list
output: the length of the linked list
runtime: O(n)
    */
int lenOfNodeRecursive(FrameNode* head)
{
    if (head == NULL)
    {
        return 0;
    }
    return 1 + lenOfNodeRecursive(head->next);
}

/*
this function changes the duration of all the frames in the linked list
input: the head of the linked list and the new duration
output: true if the duration is changed, false if not
    */
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

/*
this fumction navigates bettwen the cases in the menu
input: the head of the linked list
output: none
runtime: O(n)
    */
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

/*
this function free 1 node from the linked list
input: the node to free
output: none
runtime: O(1)
    */
void free_one_node(FrameNode** ToRemove)
{
    FrameNode* node = *ToRemove;
    free(node->frame->path);
    free(node->frame->name);
    free(node->frame);
    free(node);
}

/*
this function start the program
input: none
output: none
runtime: O(n)
    */
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
