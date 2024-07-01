#ifndef FILEHANDLER
#define FILEHANDLER

#include <stdbool.h>
#include <stdio.h>
#include "LinkedList.h"

bool checkPath(char *path);
bool saveInFile(FrameNode **head, char *path);
void openProject(char *path, FrameNode *head);
int openOrcreateProject();
void reverseOrder(FrameNode **head);
#endif
