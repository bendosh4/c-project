#ifndef LINKEDLISTH
#define LINKEDLISTH

#include <stdio.h>
#include <stdbool.h>
#define FALSE 0
#define TRUE !FALSE

// Frame struct
typedef struct Frame
{
    char* name;
    unsigned int	duration;
    char* path;
} Frame;


// Link (node) struct
typedef struct FrameNode
{
    Frame* frame;
    struct FrameNode* next;
} FrameNode;


void insertString(char* location);
int printMenu();
bool addFrame(FrameNode** head);
bool freeAllNode(FrameNode** head);
bool searchFrame(FrameNode* head, char* name);
bool saveInFile(FrameNode** head, char* path);
bool deleteFrame(char* name, FrameNode** head);
bool changeFrameLocation(FrameNode** head, char* name, int newLocation);
bool changeFrameTime(char* name, FrameNode** head, int newLen);
void printFrames(FrameNode* head);
FrameNode** openProject(char* path);


#endif
