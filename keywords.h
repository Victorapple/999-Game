#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rooms.h"

#ifndef KEYWORDS_HEAD
#define KEYWORDS_HEAD

struct Command{
    char* name;
    char* description;
    void (*function) (char* argument);
    struct Command* next;
};

struct Command* newCommand(char* name, char* description, void* function, struct Command* next);

void addCommand(struct Command* command, struct Command* head);

struct Command* containsCommand(char* name, struct Command* head);

void executeCommand(struct Command* command, char* argument);

void iterateCommand(struct Command* head);

void freeAll(struct LLRoom* head);
#endif