#include "items.h"

//Normal items are declared without a value. Only characters will have a value.
struct Item* newItem(char* name, char* description, struct Item* next) {

    struct Item* newItem = NULL;
    newItem = (struct Item*) malloc( sizeof(struct Item));
    if (newItem == NULL) {
        return NULL;
    }

    (*newItem).name = name;
    (*newItem).description = description;
    (*newItem).value = 0;
    (*newItem).next = next;

    return newItem;
}

char* getName(struct Item* item) {
    return (*item).name;
}

char* getDescription(struct Item* item) {
    return (*item).description;
}

struct Item* getNext(struct Item* item) {
    return (*item).next;
}

int getValue(struct Item* player) {
    return(*player).value;
}

//Adds an item to the front of the list, thanks to the buffer for convenience.
struct Item* addItem(struct Item* item, struct Item* head) {

    struct Item* temp = (*head).next;
    (*head).next = item;
    (*item).next = temp;
    
}

//Returns 1 if the linked list beginning at head contains the item name, else return 0.
int containsItem(char* name, struct Item* head) {

    head = (*head).next;

    while(head != NULL) {
        if(strcmp(name,(*head).name) == 0) {
            return 1;
        }
        head = (*head).next;
    }
    return 0;
}

//Get's an item from a list, but does not remove it.
struct Item* getItem(char* name, struct Item* head) {

     if(containsItem(name, head) == 0) {
        return NULL;
    }

    struct Item* prev = head;
    struct Item* curr = getNext(head);
    while (strcmp( getName(curr), name) != 0) {
        prev = getNext(prev);
        curr = getNext(curr);
        if (curr == NULL) {
            return NULL;
        }
    }

    return curr;
}

//Removes an item from a list. Thanks to the buffer as the head, we can even remove the first node.
struct Item* removeItem(char* name, struct Item* head) {

    if(containsItem(name, head) == 0) {
        return NULL;
    }

    struct Item* prev = head;
    struct Item* curr = getNext(head);
    while (strcmp( getName(curr), name) != 0) {
        prev = getNext(prev);
        curr = getNext(curr);
        if (curr == NULL) {
            return NULL;
        }
    }

    (*prev).next = getNext(curr);
    return curr;
}

//Frees the item, assuming it was removed first.
void freeItem(struct Item* item) {
    free(item);
}

//Frees all items in a list, given the head. 
void freeAllItems(struct Item* head) {
    struct Item* curr = head;
    struct Item* next = (*head).next;

    while(curr != NULL) {
        freeItem(curr);
        curr = next;
        if(curr != NULL) {
            next = (*curr).next; 
        } 
    }
}
