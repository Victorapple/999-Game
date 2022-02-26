#include "keywords.h"
#include "items.h"

extern int time;
extern struct Room* currentRoom;
extern struct LLRoom* randomRooms;

#define HOURS(x) (x) / 60 
#define MINUTES(x) (x) % 60

//Creates a new command with a given pointer to a function and the next command in the linked list.
struct Command* newCommand(char* name, char* description, void* function, struct Command* next) {
      struct Command* newCommand = NULL;
      newCommand = (struct Command*) malloc( sizeof(struct Command));
      if (newCommand == NULL) {
        return NULL;
      }

      (*newCommand).name = name;
      (*newCommand).description = description;
      (*newCommand).function = function;
      (*newCommand).next = next;

      return newCommand;
}

//Checks if a linked list starting at head contains a certain command. Returns the command if true, else NULL.
struct Command* containsCommand(char* name, struct Command* head) {

    head = (*head).next;

    while(head != NULL) {
        if(strcmp(name, (*head).name) == 0) {
            return head;
        }
        head = (*head).next;
    }
    return NULL;
}

//Adds a command to a linked list of commands at the head.
void addCommand(struct Command* command, struct Command* head) {

    struct Command* temp = (*head).next;
    (*head).next = command;
    (*command).next = temp;
}

//Removes a command from a linked list. Returns said command, else NULL if does not exist.
struct Command* removeCommand(char* name, struct Command* head) {
      if (containsCommand(name, head) == NULL) {
            return NULL; 
      } 

      struct Command* prev = head;
      struct Command* curr = (*head).next;

      while (strcmp( (*curr).name, name) != 0) {
            prev = (*prev).next;
            curr = (*curr).next;
            if (curr == NULL) {
                  return NULL;
            }
      }

      (*prev).next = (*curr).next;
      return curr;

}

//Carries out the function of the selected command with the given argument.
void executeCommand(struct Command* command, char* argument) {
      (*command).function (argument);
}

//Goes through each command in the linked list and prints their name.
void iterateCommand(struct Command* head) {
      head = (*head).next;
      while(head != NULL) {
            printf("%s\n", (*head).name);
            head = (*head).next;
      }
      printf("\n");
}

