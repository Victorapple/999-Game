#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef ROOMS_HEAD
#define ROOMS_HEAD

//Structure for rooms.
struct Room {
    char* name;
    char* description;
    struct Item* items;
    struct Item* people;
    struct Room* north;
    struct Room* south;
    struct Room* east;
    struct Room* west;
    struct Room* up;
    struct Room* down;
    int x;
    int y;
    int z;
    int omens;
};

//Structure for a Linked List of Rooms
struct LLRoom {
    struct Room* room;
    struct LLRoom* next;
};

struct LLRoom* newLLRoom(struct Room* room, struct LLRoom* next);

struct Room* newRoom(char* name, char* description, struct Item* items, struct Item* people);

void cords(struct Room* room, int x, int y, int z);

void syncRooms(struct Room* room, struct Room* map[3][3][3]); 

void syncValues(struct Room* room, int magicCube[3][3][3]);

void freeRoom(struct Room* room);

void freeAllRooms(struct LLRoom* room);

#endif