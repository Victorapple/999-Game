#include "rooms.h"
#include "items.h"

struct LLRoom* newLLRoom(struct Room* room, struct LLRoom* next) {
    struct LLRoom* newLLRoom = NULL;
    newLLRoom = (struct LLRoom*) malloc( sizeof(struct LLRoom));
    if (newRoom == NULL) {
        return NULL;
    }

    (*newLLRoom).room = room;
    (*newLLRoom).next = next;

    return newLLRoom;

};

//Initialize a room without filling in directions.
struct Room* newRoom(char* name, char* description, struct Item* items, struct Item* people) {
    struct Room* newRoom = NULL;
    newRoom = (struct Room*) malloc( sizeof(struct Room));
    if (newRoom == NULL) {
        return NULL;
    }

    (*newRoom).name = name;
    (*newRoom).description = description;
    (*newRoom).items = items;
    (*newRoom).people = people;
    (*newRoom).north = NULL;
    (*newRoom).south = NULL;
    (*newRoom).east = NULL;
    (*newRoom).west = NULL;
    (*newRoom).up = NULL;
    (*newRoom).down = NULL;
    (*newRoom).x = -1;
    (*newRoom).y = -1;
    (*newRoom).z = -1;
    (*newRoom).omens = 0;
    
}

//Assigns coordinate values to a room.
void cords(struct Room* room, int x, int y, int z) {
    (*room).x = x;
    (*room).y = y;
    (*room).z = z;
}

//Syncs a room with it's surrounding rooms if not already.
void syncRooms(struct Room* room, struct Room* map[3][3][3]) {

    //Checks for a room to the west, if it exists, links.
    if((*room).x != 0 && map[(*room).x - 1][(*room).y][(*room).z] != NULL) {
        (*room).west = map[(*room).x - 1][(*room).y][(*room).z];
        (*map[(*room).x - 1][(*room).y][(*room).z]).east = room;
    }

    //Checks for a room to the east, if it exists, links.
    if((*room).x != 2 && map[(*room).x + 1][(*room).y][(*room).z] != NULL) {
        (*room).east = map[(*room).x + 1][(*room).y][(*room).z];
        (*map[(*room).x + 1][(*room).y][(*room).z]).west = room;
    }

    //Checks for a room to the north, if it exists, links.
    if((*room).y != 2 && map[(*room).x][(*room).y + 1][(*room).z] != NULL) {
        (*room).north = map[(*room).x][(*room).y + 1][(*room).z];
        (*map[(*room).x][(*room).y + 1][(*room).z]).south = room;
    }

    //Checks for a room to the south, if it exists, links.
    if((*room).y != 0 && map[(*room).x][(*room).y - 1][(*room).z] != NULL) {
        (*room).south = map[(*room).x][(*room).y - 1][(*room).z];
        (*map[(*room).x][(*room).y - 1][(*room).z]).north = room;
    }

    //Checks for a room upwards, if it exists, links.
    if((*room).z != 2 && map[(*room).x][(*room).y][(*room).z + 1] != NULL) {
        (*room).up = map[(*room).x][(*room).y][(*room).z + 1];
        (*map[(*room).x][(*room).y][(*room).z + 1]).down = room;
    }

    //Checks for a room downwards, if it exists, links.
    if((*room).z != 0 && map[(*room).x][(*room).y][(*room).z - 1] != NULL) {
        (*room).down = map[(*room).x][(*room).y][(*room).z - 1];
        (*map[(*room).x][(*room).y][(*room).z - 1]).up = room;
    }

}

//Syncs the sum of values of people in the current room and sets it to magic cube.
void syncValues(struct Room* room, int magicCube[3][3][3]) {

    magicCube[(*room).x][(*room).y][(*room).z] = 0;
    struct Item* person = (*room).people;
    person = getNext(person);
    while (person != NULL) {
        magicCube[(*room).x][(*room).y][(*room).z] = magicCube[(*room).x][(*room).y][(*room).z] + getValue(person);  
        person = getNext(person);  
    }

}

void freeRoom(struct Room* room) {
    free(room);
}

void freeAllRooms(struct LLRoom* head) {

    struct LLRoom* curr = head;
    struct LLRoom* next = (*head).next;

    while(curr != NULL) {
        (free(curr));
        curr = next;
        
        if(curr != NULL) {
            next = (*curr).next;   
        }

    }
}