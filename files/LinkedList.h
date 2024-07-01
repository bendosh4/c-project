#ifndef LINKEDLISTH
#define LINKEDLISTH

#include <stdbool.h>
#include <stdio.h>

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

void insertString(char* location);
int printMenu();
bool addFrame(FrameNode** head);
bool freeAllNode(FrameNode** head);
bool searchFrame(FrameNode* head, char* name);
bool deleteFrame(char* name, FrameNode** head);
bool changeFrameLocation(FrameNode** head, char* name, int newLocation);
bool changeFrameTime(char* name, FrameNode** head, int newLen);
void printFrames(FrameNode* head);
void navigate(int choice, FrameNode** head);
void errorMsg();
int lenOfNodeRecursive(FrameNode* head);
bool changeAllFrameDuration(FrameNode** head, int newLen);
void free_one_node(FrameNode** ToRemove);

#endif
