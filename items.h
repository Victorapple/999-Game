#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef ITEMS_HEAD
#define ITEMS_HEAD

//Structure for items.

struct Item {
    char* name;
    char* description;
    int value;
    struct Item* next;
};


struct Item* newItem(char* name, char* description, struct Item* next);

char* getName(struct Item* item);

char* getDescription(struct Item* item);

int getValue(struct Item* item);

struct Item* getNext(struct Item* item);

struct Item* addItem(struct Item* item, struct Item* head);

int containsItem(char* name, struct Item* head);

struct Item* getItem(char* name, struct Item* head);

struct Item* removeItem(char* name, struct Item* head);

void freeItem(struct Item* item);

void freeAllItems(struct Item* head);

#endif