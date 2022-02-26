#include <stdio.h>
#include <stdlib.h>
#include "items.h"
#include "rooms.h"
#include "keywords.h"

#define HOURS(x) (x) / 60 
#define MINUTES(x) (x) % 60

//Information relating to the avatar.
struct Room* currentRoom;
struct Item* inventory; 
struct Item* people;

//Global game state trackers and commands.
int time = 60 * 9;
struct Command* commandsList;
struct Room* map[3][3][3];
int magicCube[3][3][3];
struct LLRoom* randomRooms;
int gameOver = 0;

//Omens, I guess.
int aprilQuest = 1;
int treyCandy = 1;

//A function that allows for the continuation of text progression (Only for the story). 
void cont() {
    getchar();
    return;
}

//A function that prints the time left for the game. Returns 1 if there's still time, else 0.
int checkTime() {
    if(time <= 0) {
        printf("You are out of time.\n\n");
        gameOver = 3;
        return 0;
    }

    if(MINUTES(time)) {
        printf("You have %i hours and %i minutes remaining. \n\n", HOURS(time), MINUTES(time));
    } else {
        printf("You have %i hours remaining. \n\n", HOURS(time));
    }    
    return 1;
}

//A function that prints out the name and a description of a room upon entering.
void enterRoom(struct Room* room) {
    printf("\n");
    printf("%s \n\n", (*room).name);
    printf("%s \n", (*room).description);
    printf("\n");
    if(strcmp((*room).name, "Staff Room") == 0) {
        if ((*room).omens == 1) {
            printf("The largest filing cabinet appear to be locked.\n\n");
        } else {
            printf("The largest filing cabinet is smashed open.\n\n");
        }     
    } 
    if(strcmp((*room).name, "Dormitories") == 0) {
        if ((*room).omens == 1) {
            printf("A safety deposit box is locked and placed on one of the beds.\n\n");
        } else {
            printf("The safety deposit box is unlocked.\n\n");
        }     
    }
    if(strcmp((*room).name, "Kitchen") == 0) {
        if ((*room).omens == 1) {
            printf("A repulsive stench comes from a strange box on top of a cabinet...\n\n");
        } 
    } 

    if(containsItem("Mystery Meat", (*room).items)) {
        printf("A repulsive stench fills the room...\n\n");
    } 

}

//A function that prints out in one line a list of people who are following you.
void listFollow() {

    //These static variables are for letting characters stop following you once you arrive in certain rooms for the first time.
    static int ninaStart = 1;
    static int ottoStart = 1;
    static int sevStart = 1;
    static int leoStart = 1;
    static int aprilStart = 1;  
    static int treyStart = 1;
    static int acezweiStart = 1;


    struct Item* followList = (*people).next;
    if(followList == NULL) {
        return;     
    } else {   
        printf("%s", getName(followList));
        followList = getNext(followList);

        while(followList != NULL) {
            printf(", %s", getName(followList));
            followList = getNext(followList);
        }
        printf(" followed you into the %s.\n\n", (*currentRoom).name);
    }

    //First time Nina enters the Workshop.
    if( containsItem("Nina", people) && ninaStart && strcmp( (*currentRoom).name, "Workshop") == 0) {
        addItem(removeItem("Nina", people), (*currentRoom).people);
        ninaStart = 0;
        printf("Nina has stopped following you.\n\n");
        printf("Nina: Wow! There's so much stuff here!\n\n");
    } 

    //First time Otto enters the Media Room.
    if( containsItem("Otto", people) && ottoStart && strcmp( (*currentRoom).name, "Media Lab") == 0) {
        addItem(removeItem("Otto", people), (*currentRoom).people);
        ottoStart = 0;
        printf("Otto has stopped following you.\n\n");
        printf("Otto: I'm gonna check this place out for a bit. Mind if you keep exploring without me?\n\n");
    } 

    //First time Sev enters the Dormitories.
    if( containsItem("Sev", people) && ottoStart && strcmp( (*currentRoom).name, "Dormitories") == 0) {
        addItem(removeItem("Sev", people), (*currentRoom).people);
        sevStart = 0;
        printf("Sev has stopped following you.\n\n");
        printf("Sev: My back...I'll take a rest here for now.\n\n");
    } 

    //April will move to and stay in the Library once you talk to her with the Book.
    if( containsItem("April", people) && aprilStart && strcmp( (*currentRoom).name, "Library") == 0) {
        addItem(removeItem("April", people), (*currentRoom).people);
        aprilStart = 0;
        printf("April and Leo have stopped following you.\n\n");
        printf("April: Thank...thank you very much for leading me here.\n");
        aprilQuest = 0;
    } 

    //Leo will move to and join April in the Library once you talk to April with the book.
    if( containsItem("Leo", people) && leoStart && strcmp( (*currentRoom).name, "Library") == 0) {
        addItem(removeItem("Leo", people), (*currentRoom).people);
        leoStart = 0;
        printf("Leo: Are you feeling better now April?\n");
        printf("April: Yeah! There's so many books here!\n\n");
    } 

    //First time Trey enters the Research Lab.
    if( containsItem("Trey", people) && treyStart && strcmp( (*currentRoom).name, "Research Lab") == 0) {
        addItem(removeItem("Trey", people), (*currentRoom).people);
        sevStart = 0;
        printf("Trey has stopped following you.\n\n");
        printf("Trey: Hmmm...\n\n");
    } 

    //First time Zwei and Ace enter the Staff Room.
    if( containsItem("Ace", people) && containsItem("Zwei", people) && acezweiStart && strcmp( (*currentRoom).name, "Staff Room") == 0) {
        addItem(removeItem("Ace", people), (*currentRoom).people);
        addItem(removeItem("Zwei", people), (*currentRoom).people);
        acezweiStart = 0;
        printf("Ace and Zwei has stopped following you.\n\n");
        printf("Ace: Perhaps we will start our search for clues here.\n");
        printf("Zwei: ...\n\n");
    } 

    syncValues(currentRoom, magicCube);

}

//A function that gets the available rooms in each direction if applicable.
void getDirections(struct Room* room) {
    printf("\nThere is a staircase leading both upwards and downwards, and there are four doors in four\ncardinal directions.\n\n");
    if((*room).north != NULL) {
        printf("The north door leads to %s.\n", (*(*room).north).name);
    }
    if((*room).west != NULL) {
        printf("The west door leads to %s.\n", (*(*room).west).name);
    }
    if((*room).east != NULL) {
        printf("The east door leads to %s.\n", (*(*room).east).name);
    }
    if((*room).up != NULL) {
        printf("Up the staircase leads to %s.\n", (*(*room).up).name);
    }
    if((*room).down != NULL) {
        printf("Down the staircase leads to %s.\n", (*(*room).down).name);
    }
    printf("The south door is locked.\n\n");
}

//Finds the nth room in the linked list, pops it and returns the room. Assumes n is less than than the total number of nodes.
struct Room* chooseRoom(int n) {

      struct LLRoom* prev = randomRooms;
      struct LLRoom* curr = (*randomRooms).next;

      if (n == 0) {
            return NULL;
      }

      for(int i = 1; i < n; i++) {
            prev = (*prev).next;
            curr = (*curr).next;
      }

      (*prev).next = (*curr).next;
      struct Room* newRoom = (*curr).room;
      free(curr);
      return newRoom;
}

//Verifies if a magic square was made or not by looking at magicCube. This function will only ever trigger when you enter the Reception Room.
int verify() {

    //Checks the possible magic square formed by the xy plane.
    if(magicCube[0][1][1] + magicCube[2][1][1] + 5 == 15 && magicCube[1][2][1] + magicCube[1][0][1] + 5 == 15) {
        //Then checks the diagonals
        if(magicCube[0][0][1] + magicCube[2][2][1] == 10 && magicCube[0][2][1] + magicCube[2][0][1] == 10) {
            //Before finally checking the two outer columns
            if(magicCube[0][2][1] + magicCube[0][1][1] + magicCube[0][0][1] == 15 && magicCube[2][2][1] + magicCube[2][1][1] + magicCube[2][0][1] == 15) {
                //And of course, the two outer rows
                if(magicCube[0][2][1] + magicCube[1][2][1] + magicCube[2][2][1] == 15 && magicCube[0][0][1] + magicCube[1][0][1] + magicCube[2][0][1] == 15) {
                    return 1;
                }
            }
        }
    }
    //Checks the possible magic square formed by the xz plane.
    if(magicCube[0][1][1] + magicCube[2][1][1] + 5 == 15 && magicCube[1][1][2] + magicCube[1][1][0] + 5 == 15) {
        //Then checks the diagonals
        if(magicCube[0][1][2] + magicCube[2][1][0] == 10 && magicCube[0][1][0] + magicCube[2][1][2] == 10) {
            //Before finally checking the two outer columns
            if(magicCube[0][1][2] + magicCube[0][1][1] + magicCube[0][1][0] == 15 && magicCube[2][1][2] + magicCube[2][1][1] + magicCube[2][1][0] == 15) {
                //And of course, the two outer rows
                if(magicCube[0][1][2] + magicCube[1][1][2] + magicCube[2][1][2] == 15 && magicCube[0][1][0] + magicCube[1][1][0] + magicCube[2][1][0] == 15) {
                    return 1;
                }
            }
        }
    }
    //Checks the possible magic square formed by the yz plane.
    if(magicCube[1][0][1] + magicCube[1][2][1] + 5 == 15 && magicCube[1][1][0] + magicCube[1][1][2] + 5 == 15) {
        //Then checks the diagonals
        if(magicCube[1][0][2] + magicCube[1][2][0] == 10 && magicCube[1][0][0] + magicCube[1][2][2] == 10) {
            //Before finally checking the two outer columns
            if(magicCube[1][0][0] + magicCube[1][0][1] + magicCube[1][0][2] == 15 && magicCube[1][2][0] + magicCube[1][2][1] + magicCube[1][2][2] == 15) {
                //And of course, the two outer rows
                if(magicCube[1][0][2] + magicCube[1][1][2] + magicCube[1][2][2] == 15 && magicCube[1][0][0] + magicCube[1][1][0] + magicCube[1][2][0] == 15) {
                    return 1;
                }
            }
        }
    }

    return 0;
}

//Function for the look command: Gives a detailed description of the current room if arg = NULL. Else, gives a description of arg.
void look(char* arg) { 

    if (arg == NULL) { //Basic look command.

        time = time - 5;
        enterRoom(currentRoom);
     
        struct Item* itemList = (*currentRoom).items;
        itemList = getNext(itemList);
        struct Item* peopleList = (*currentRoom).people;
        peopleList = getNext(peopleList);
        struct Item* followerList = people;
        followerList = getNext(followerList);

        if(itemList == NULL) {
            printf("There are no items found in the %s.\n",(*currentRoom).name);  
        } else {
            printf("Items found in the %s: \n\n",(*currentRoom).name);

            while(itemList != NULL) {
                printf("%s \n", getName(itemList));
                itemList = getNext(itemList);
            }
        }
        printf("\n");

        if(peopleList == NULL && followerList == NULL) {
            printf("There is no one but you in the %s.\n",(*currentRoom).name);   
        } else {
            printf("People in the %s: \n\n",(*currentRoom).name);

            while(peopleList != NULL) {
                printf("%s: Bracelet number %i\n", getName(peopleList), getValue(peopleList));
                peopleList = getNext(peopleList);
            }

            while(followerList != NULL) {
                printf("%s: Bracelet number %i\n", getName(followerList), getValue(followerList));
                followerList = getNext(followerList);
            }
            printf("You: Bracelet number 5\n");

        }

        getDirections(currentRoom);
        checkTime(); 

    } else { //Gets the description of the arg item and prints it./
        struct Item* item;
      
        if(containsItem(arg, inventory)) {
            item = getItem(arg, inventory);
        } else if(containsItem(arg, (*currentRoom).items)) {
            item = getItem(arg, (*currentRoom).items);
        } else if (containsItem(arg, people)) {
            item = getItem(arg, people);
        } else if(containsItem(arg, (*currentRoom).people)) {
            item = getItem(arg, (*currentRoom).people);
        } else {
            printf("\nThis item or person is currently not in the %s. \n\n",(*currentRoom).name);  
            return;       
        }
        printf("\n%s: %s\n\n", (*item).name, (*item).description);
    }

}

//Function for the go command: Changes currentRoom and map if applicable and updates rooms.
void move(char* direction) {

    static int remainingRooms = 8;

    if (direction == NULL) {
        printf("\nPlease specify a direction to move in. For more info, type \"help go\".\n\n");
        return;
    }

    if (strcmp(direction, "north") == 0) {

        if((*currentRoom).north == NULL) {

            if(remainingRooms <= 0) { //Player tries to create a new room, but there is no more rooms, resulting in a loss.
                printf("\nYou move north and enter through the door, but what awaits you was a never ending pit.\n"); 
                printf("\nYou end up falling,\n");
                printf("\nfalling,\n"); 
                printf("\nfalling,\n");
                printf("\ninto oblivion.\n"); 
                gameOver = 2; 
                return;     
            } 
            struct Room* nextRoom;
            int num = rand() % (remainingRooms) + 1;
            nextRoom = chooseRoom(num);
            remainingRooms--; 

            (*nextRoom).x = (*currentRoom).x;
            (*nextRoom).y = (*currentRoom).y + 1;
            (*nextRoom).z = (*currentRoom).z;

            //Events will trigger whenecer the player chooses to go outside a 3x3x3 cube with the starting room in the center.
            if((*nextRoom).y > 2) {

                printf("\nThe ground beneath you trembles. Something has changed...\n");

                //Event occurs and shifts room to south, west, east, below, or above the starting room (1,1,1), in order of first availability.
                if(map[1][0][1] == NULL) { //south
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 0;
                    (*nextRoom).z = 1;
                    map[1][0][1] = nextRoom;
                    syncRooms(nextRoom, map);           
                } else if(map[0][1][1] == NULL) { //west
                    (*nextRoom).x = 0;
                    (*nextRoom).y = 1;
                    (*nextRoom).z = 1;
                    map[0][1][1] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else if(map[2][1][1] == NULL) { //east
                    (*nextRoom).x = 2;
                    (*nextRoom).y = 1;
                    (*nextRoom).z = 1;
                    map[2][1][1] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else if(map[1][1][2] == NULL) { //below
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 1;
                    (*nextRoom).z = 2;
                    map[1][1][2] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else if(map[1][1][0] == NULL) { //above
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 1;
                    (*nextRoom).z = 0;
                    map[1][1][0] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else { //If this happens, that means there is no way to connect back to the starting room, resulting in a loss.
                    printf("\nYou move north and enter through the door, but what awaits you was a never ending pit.\n"); 
                    printf("\nYou end up falling,\n");
                    printf("\nfalling,\n"); 
                    printf("\nfalling,\n");
                    printf("\ninto oblivion.\n");
                    gameOver = 1; 
                    return;                                                      
                }

                //Now assuming a successful reposition has been made.

            } else {
                map[(*nextRoom).x][(*nextRoom).y][(*nextRoom).z] = nextRoom;      
                (*currentRoom).north = nextRoom;
                (*nextRoom).south = currentRoom;
                syncRooms(nextRoom, map); 
            }

            currentRoom = nextRoom;

        } else {
            currentRoom = (*currentRoom).north;
        }

        time = time - 10;
        printf("\nYou move north and arrive at a new room.\n");      
        enterRoom(currentRoom);
        if(strcmp((*currentRoom).name, "Reception Room") == 0) {
            if(verify()) {
                gameOver = 8;
                return;
            }
        } 
        listFollow();
        checkTime();
        return;

    } else if (strcmp(direction, "east") == 0) {

        if((*currentRoom).east == NULL) {

            if(remainingRooms <= 0) {
                printf("\nYou move east and enter through the door, but what awaits you was a never ending pit.\n"); 
                printf("\nYou end up falling,\n");
                printf("\nfalling,\n"); 
                printf("\nfalling,\n");
                printf("\ninto oblivion.\n");
                gameOver = 2; 
                return;  
            } 
            struct Room* nextRoom;
            int num = rand() % (remainingRooms) + 1;
            nextRoom = chooseRoom(num);
            remainingRooms--; 

            (*nextRoom).x = (*currentRoom).x + 1;
            (*nextRoom).y = (*currentRoom).y;
            (*nextRoom).z = (*currentRoom).z;

            //Events will trigger whenecer the player chooses to go outside a 3x3x3 cube with the starting room in the center.
            if((*nextRoom).x > 2) {

                printf("\nThe ground beneath you trembles. Something has changed...\n");

                //Event occurs and shifts room to west, south, north, below, or above the starting room (1,1,1), in order of first availability.
                if(map[0][1][1] == NULL) { //west
                    (*nextRoom).x = 0;
                    (*nextRoom).y = 1;
                    (*nextRoom).z = 1;
                    map[0][1][1] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else if(map[1][0][1] == NULL) { //south
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 0;
                    (*nextRoom).z = 1;
                    map[1][0][1] = nextRoom;
                    syncRooms(nextRoom, map);           
                } else if(map[1][2][1] == NULL) { //north
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 2;
                    (*nextRoom).z = 1;
                    map[1][2][1] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else if(map[1][1][2] == NULL) { //below
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 1;
                    (*nextRoom).z = 2;
                    map[1][1][2] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else if(map[1][1][0] == NULL) { //above
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 1;
                    (*nextRoom).z = 0;
                    map[1][1][0] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else { //If this happens, that means there is no way to connect back to the starting room, resulting in a loss.
                    printf("\nYou move east and enter through the door, but what awaits you was a never ending pit.\n"); 
                    printf("\nYou end up falling,\n");
                    printf("\nfalling,\n"); 
                    printf("\nfalling,\n");
                    printf("\ninto oblivion.\n"); 
                    gameOver = 1;
                    return;                                                      
                }

                //Now assuming a successful reposition has been made.

            } else {

                map[(*nextRoom).x][(*nextRoom).y][(*nextRoom).z] = nextRoom; 
                (*currentRoom).east = nextRoom;
                (*nextRoom).west = currentRoom;
                syncRooms(nextRoom, map); 

            }

            currentRoom = nextRoom;

        } else {
            currentRoom = (*currentRoom).east;
        }
            
        time = time - 10;
        printf("\nYou move east and arrive at a new room.\n");      
        enterRoom(currentRoom);
        if(strcmp((*currentRoom).name, "Reception Room") == 0) {
            if(verify()) {
                gameOver = 8;
                return;
            }
        } 
        listFollow();
        checkTime();
        return;

    } else if (strcmp(direction, "west") == 0) {

        if((*currentRoom).west == NULL) {

            if(remainingRooms <= 0) {
                printf("\nYou move west and enter through the door, but what awaits you was a never ending pit.\n"); 
                printf("\nYou end up falling,\n");
                printf("\nfalling,\n"); 
                printf("\nfalling,\n");
                printf("\ninto oblivion.\n");
                gameOver = 2; 
                return; 
            } 
            struct Room* nextRoom;
            int num = rand() % (remainingRooms) + 1;
            nextRoom = chooseRoom(num);
            remainingRooms--; 

            (*nextRoom).x = (*currentRoom).x - 1;
            (*nextRoom).y = (*currentRoom).y;
            (*nextRoom).z = (*currentRoom).z;

            //Events will trigger whenecer the player chooses to go outside a 3x3x3 cube with the starting room in the center.
            if((*nextRoom).x < 0) {

                printf("\nThe ground beneath you trembles. Something has changed...\n");

                //Event occurs and shifts room to east, south, north, below, or above the starting room (1,1,1), in order of first availability.
                if(map[2][1][1] == NULL) { //east
                    (*nextRoom).x = 2;
                    (*nextRoom).y = 1;
                    (*nextRoom).z = 1;
                    map[2][1][1] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else if(map[1][0][1] == NULL) { //south
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 0;
                    (*nextRoom).z = 1;
                    map[1][0][1] = nextRoom;
                    syncRooms(nextRoom, map);           
                } else if(map[1][2][1] == NULL) { //north
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 2;
                    (*nextRoom).z = 1;
                    map[1][2][1] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else if(map[1][1][2] == NULL) { //below
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 1;
                    (*nextRoom).z = 2;
                    map[1][1][2] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else if(map[1][1][0] == NULL) { //above
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 1;
                    (*nextRoom).z = 0;
                    map[1][1][0] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else { //If this happens, that means there is no way to connect back to the starting room, resulting in a loss.
                    printf("\nYou move west and enter through the door, but what awaits you was a never ending pit.\n"); 
                    printf("\nYou end up falling,\n");
                    printf("\nfalling,\n"); 
                    printf("\nfalling,\n");
                    printf("\ninto oblivion.\n"); 
                    gameOver = 1;
                    return;                                                      
                }

                //Now assuming a successful reposition has been made.

            } else {

                map[(*nextRoom).x][(*nextRoom).y][(*nextRoom).z] = nextRoom; 
                (*currentRoom).west = nextRoom;
                (*nextRoom).east = currentRoom;
                syncRooms(nextRoom, map); 
            }

            currentRoom = nextRoom;

        } else {
            currentRoom = (*currentRoom).west;
        }
            
        time = time - 10;
        printf("\nYou move west and arrive at a new room.\n");      
        enterRoom(currentRoom);
        if(strcmp((*currentRoom).name, "Reception Room") == 0) {
            if(verify()) {
                gameOver = 8;
                return;
            }
        } 
        listFollow();
        checkTime();
        return;

    } else if (strcmp(direction, "up") == 0) {

        if((*currentRoom).up == NULL) {

            if(remainingRooms <= 0) {
                printf("\nYou go up the staircase, but what awaits you was a never ending pit.\n"); 
                printf("\nYou end up falling,\n");
                printf("\nfalling,\n"); 
                printf("\nfalling,\n");
                printf("\ninto oblivion.\n"); 
                gameOver = 2;
                return; 
            } 
            struct Room* nextRoom;
            int num = rand() % (remainingRooms) + 1;
            nextRoom = chooseRoom(num);
            remainingRooms--; 

            (*nextRoom).x = (*currentRoom).x;
            (*nextRoom).y = (*currentRoom).y;
            (*nextRoom).z = (*currentRoom).z + 1;

            //Events will trigger whenecer the player chooses to go outside a 3x3x3 cube with the starting room in the center.
            if((*nextRoom).z > 2) {

                printf("\nThe ground beneath you trembles. Something has changed...\n");

                //Event occurs and shifts room to below, east, south, north, or west of the starting room (1,1,1), in order of first availability.
                if(map[1][1][0] == NULL) { //below
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 1;
                    (*nextRoom).z = 0;
                    map[1][1][0] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else if(map[2][1][1] == NULL) { //east
                    (*nextRoom).x = 2;
                    (*nextRoom).y = 1;
                    (*nextRoom).z = 1;
                    map[2][1][1] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else if(map[1][0][1] == NULL) { //south
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 0;
                    (*nextRoom).z = 1;
                    map[1][0][1] = nextRoom;
                    syncRooms(nextRoom, map);           
                } else if(map[1][2][1] == NULL) { //north 
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 2;
                    (*nextRoom).z = 1;
                    map[1][2][1] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else if(map[0][1][1] == NULL) { //west
                    (*nextRoom).x = 0;
                    (*nextRoom).y = 1;
                    (*nextRoom).z = 1;
                    map[0][1][1] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else { //If this happens, that means there is no way to connect back to the starting room, resulting in a loss.
                    printf("\nYou go up the staircase, but what awaits you was a never ending pit.\n"); 
                    printf("\nYou end up falling,\n");
                    printf("\nfalling,\n"); 
                    printf("\nfalling,\n");
                    printf("\ninto oblivion.\n"); 
                    gameOver = 1;
                    return;                                                      
                }

                //Now assuming a successful reposition has been made.

            } else {
                        
                map[(*nextRoom).x][(*nextRoom).y][(*nextRoom).z] = nextRoom;
                (*currentRoom).up = nextRoom;
                (*nextRoom).down = currentRoom;
                syncRooms(nextRoom, map); 
                  
            } 

            currentRoom = nextRoom;

        } else {
            currentRoom = (*currentRoom).up;
        }
            
        time = time - 10;
        printf("\nYou move up a floor and arrive at a new room.\n");      
        enterRoom(currentRoom);
        if(strcmp((*currentRoom).name, "Reception Room") == 0) {
            if(verify()) {
                gameOver = 8;
                return;
            }
        } 
        listFollow();
        checkTime();
        return;

    } else if (strcmp(direction, "down") == 0) {

        if((*currentRoom).down == NULL) {

            if(remainingRooms <= 0) {
                printf("\nYou go down the staircase, but what awaits you was a never ending pit.\n"); 
                printf("\nYou end up falling,\n");
                printf("\nfalling,\n"); 
                printf("\nfalling,\n");
                printf("\ninto oblivion.\n");
                gameOver = 2; 
                return; 
            } 
            struct Room* nextRoom;
            int num = rand() % (remainingRooms) + 1;
            nextRoom = chooseRoom(num);
            remainingRooms--; 

            (*nextRoom).x = (*currentRoom).x;
            (*nextRoom).y = (*currentRoom).y;
            (*nextRoom).z = (*currentRoom).z - 1;

            //Events will trigger whenecer the player chooses to go outside a 3x3x3 cube with the starting room in the center.
            if((*nextRoom).z < 0) {

                printf("\nThe ground beneath you trembles. Something has changed...\n");

                //Event occurs and shifts room to above, east, south, north, or west of the starting room (1,1,1), in order of first availability.
                if(map[1][1][2] == NULL) { //above
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 1;
                    (*nextRoom).z = 2;
                    map[1][1][2] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else if(map[2][1][1] == NULL) { //east
                    (*nextRoom).x = 2;
                    (*nextRoom).y = 1;
                    (*nextRoom).z = 1;
                    map[2][1][1] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else if(map[1][0][1] == NULL) { //south
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 0;
                    (*nextRoom).z = 1;
                    map[1][0][1] = nextRoom;
                    syncRooms(nextRoom, map);           
                } else if(map[1][2][1] == NULL) { //north 
                    (*nextRoom).x = 1;
                    (*nextRoom).y = 2;
                    (*nextRoom).z = 1;
                    map[1][2][1] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else if(map[0][1][1] == NULL) { //west
                    (*nextRoom).x = 0;
                    (*nextRoom).y = 1;
                    (*nextRoom).z = 1;
                    map[0][1][1] = nextRoom;
                    syncRooms(nextRoom, map); 
                } else { //If this happens, that means there is no way to connect back to the starting room, resulting in a loss.
                    printf("\nYou go down the staircase, but what awaits you was a never ending pit.\n"); 
                    printf("\nYou end up falling,\n");
                    printf("\nfalling,\n"); 
                    printf("\nfalling,\n");
                    printf("\ninto oblivion.\n");
                    gameOver = 1; 
                    return;                                                      
                }

                //Now assuming a successful reposition has been made.

            } else {

                map[(*nextRoom).x][(*nextRoom).y][(*nextRoom).z] = nextRoom;
                (*currentRoom).down = nextRoom;
                (*nextRoom).up = currentRoom;
                syncRooms(nextRoom, map); 
            }

            currentRoom = nextRoom;


        } else {
            currentRoom = (*currentRoom).down;
        }
            
        time = time - 10;
        printf("\nYou move down a floor and arrive at a new room.\n");      
        enterRoom(currentRoom);
        if(strcmp((*currentRoom).name, "Reception Room") == 0) {
            if(verify()) {
                gameOver = 8;
                return;
            }
        } 
        listFollow();
        checkTime();
        return;

    } else if (strcmp(direction, "south") == 0) {
        printf("\nThe south door is locked.\n\n");     
        return; 
    } else {
        printf("\nInvalid direction. Use \"help go\" for more info on how to use this command.\n\n");
        return;      
    }

    return;

}

//Function for the take command: Takes an item from room if applicable and adds it to inventory.
void take(char* item) {
    if(item == NULL) {
        printf("\nPlease specify an item to take. For more info, type \"help take\".\n\n");
        return;
    }
    if(containsItem(item, (*currentRoom).items)) {
        addItem(removeItem(item, (*currentRoom).items), inventory);
        printf("\nYou have added the %s to your inventory.\n\n", item);
    } else {
        printf("\nThis item currently does not exist in this room.\n\n");
    }
}

//Function for the drop command: Drops an item from inventory if applicable into the current room.
void drop(char* item) {
    if(item == NULL) {
        printf("\nPlease specify an item to drop. For more info, type \"help drop\".\n\n");
        return;
    }
    if(containsItem(item, inventory)) {
        addItem(removeItem(item, inventory), (*currentRoom).items);
        printf("\nYou have dropped the %s from your inventory.\n\n", item);
        if( strcmp(item, "Glass Beaker") == 0) {
            printf("The Glass Beaker has shattered into Glass Shards.\n\n");
            addItem(newItem("Glass Shards", "Shattered like your hopes and dreams.", NULL),(*currentRoom).items);
            freeItem(removeItem("Glass Beaker",(*currentRoom).items));
        }

    } else {
        printf("\nYou currently don't have this item in your inventory.\n\n");
    }
}

//Function for the inventory command: Checks the player's inventory.
void checkInventory(char* arg) {

    if(arg != NULL) {
        printf("\nInvalid arguments. Type \"inventory\" to check your inventory.\n\n"); 
        return;         
    }

    struct Item* itr = inventory;
    itr = getNext(itr);

    if(itr == NULL) {
        printf("\nYou have no items in your inventory.\n\n"); 
        return;     
    }

    printf("\nItems in your inventory: \n\n");
 
    while(itr != NULL) {
        printf("%s \n", getName(itr));
        itr = getNext(itr);
    }
    printf("\n");

}

//Function for the help command: Gives list of commands and also individual commands.
void help(char* arg) {
    if (arg == NULL) {
        printf("\nList of commands for Nine Hours, Nine People, Nine Rooms: For a detailed description for a specific\ncommand, type \"help [name of command]\".\n\n");
        iterateCommand(commandsList);
    } else {
        if(containsCommand(arg, commandsList) != NULL) {
            printf("\n%s", (*containsCommand(arg, commandsList)).description);
        } else {
            printf("\nInvalid command. Type \"help\" for a list of commands.\n\n");
        }
    }
}

//Function for the use command: On specific usages at specific locations, certain omens trigger.
void use(char* arg) {

    if(arg == NULL) {
        printf("\nPlease specify an item to use. Type \"help use\" for more info.\n\n");  
        return;  
    }
    if(containsItem(arg, inventory) == 0) {
        printf("\nYou currently don't have this item!\n\n");  
        return;     
    }
    if(strcmp(arg, "Hammer") == 0 && strcmp((*currentRoom).name, "Staff Room") == 0 && (*currentRoom).omens == 1) {
        printf("\nYou used Hammer on the filing cabinet. With a resonating bang, the hinges fall off.\n\n");
        (*currentRoom).omens = 0;
        addItem(newItem("Paper Clip", "For keeping your papers together.", NULL), (*currentRoom).items);
        addItem(newItem("Candy", "It used to be Cigarettes, but we do not promote smoking.", NULL), (*currentRoom).items);

    } else if (strcmp(arg, "Laptop") == 0 && strcmp((*currentRoom).name, "Media Lab") == 0 && (*currentRoom).omens == 1) {
        printf("\nYou connect Laptop to the WiFi in the Media Lab to get Network PC.\n\n");
        (*currentRoom).omens = 0;
        addItem(newItem("Network PC", "With this, the secrets of Zero's game can be unlocked in the Reception Room.", NULL), inventory);
        freeItem(removeItem("Laptop", inventory));

    } else if (strcmp(arg, "Lockpick") == 0 && strcmp((*currentRoom).name, "Dormitories") == 0 && (*currentRoom).omens == 1) {
        printf("\nYou used Lockpick on the safety deposit box. It opens with a click.\n\n");
        (*currentRoom).omens = 0;
        addItem(newItem("System Shutdown Codes", "Key information on Zero's game.", NULL), (*currentRoom).items);

    } else if (strcmp(arg, "Budget Grappling Hook") == 0 && strcmp((*currentRoom).name, "Kitchen") == 0 && (*currentRoom).omens == 1) {
        printf("\nYou used Budget Grappling Hook to bring down the box on top of the cabinet. The source of the repulsive smell tumbles out.\n\n");
        (*currentRoom).omens = 0;
        addItem(newItem("Mystery Meat", "Would be a good source of protein if it wasn’t rotting. Or if the skin didn’t look human.", NULL), (*currentRoom).items);

    } else if (strcmp(arg, "Network PC") == 0 && strcmp((*currentRoom).name, "Reception Room") == 0 && containsItem("System Shutdown Codes", inventory)) {
        printf("\nBefore using the Network PC, you call out to the others until everyone is gathered in the Reception Room.\n");
        printf("One look at your face and everyone understands immediately. Grinning, Nina says,\"Well, what are you\nwaiting for?\"\n");
        printf("\nYou boot up the Network PC, and succesfully infiltrated the system behind Zero's Game with your C\nprogramming skills. ");
        printf("As you connect the System Shutdown Codes to the Network PC and execute the program, you hear\nZero's voice come to life over an intercom.\n\n");
        printf("\"Preposterous! I will not let you tear down the game I so cleverly...\"\n");
        printf("Whatever Zero planned to say was cut off by alarms, as the rumbling of hidden mechanisms echos throughout\nthe room. ");
        printf("You can feel the room moving, tremors on the ground almost cause you to lose your balance\nand fall over. ");
        printf("Nothing except for the blaring of the intercom and the growling of rooms shifting could\nbe heard, until finally...\n\n");
        printf("It stopped.\n\n");
        printf("Clunk. The Reception Room stops moving and locks in place. You hear a click, and the South door swings open.\n\n");
        printf("You look at everyone else. They look at each other and you. Then you all look towards the South door,\nat the blinding light outside.\n\n");
        printf("And together, you take all take a step forward.\n\n");
        printf("Away from Zero's game, and towards an unknown future.\n\n");
        gameOver = 9;

    } else {
        printf("\nAs Professor Oak says, there is a time and place for everything!\n\n");
    }
}

//Function for the combine command: If applicable, two items in your inventory can combine into a new item.
void combine(char* arg) {

    if (arg == NULL) {
        printf("\nYou haven't selected any items to combine. For more info, type \"help combine\".\n\n");    
        return;
    }
    char* first = strtok(arg, "&");
    if(first == NULL) {
        printf("\nYou haven't selected any items to combine. For more info, type \"help combine\".\n\n");  
        return;      
    }
    char* second = strtok(NULL, "&");
    if(second == NULL) {
        printf("\nYou haven't selected a second item to combine. For more info, type \"help combine\".\n\n");    
        return;
    }
    if(strtok(NULL, "&") != NULL) {
        printf("\nYou have selected more than two items to combine. For more info, type \"help combine\".\n\n"); 
        return;
    }
    if(containsItem(first, inventory) == 0 || containsItem(second, inventory) == 0) {
        printf("\nYou currently do not have one of these items in your inventory. For more info, type \"help combine\".\n\n"); 
        return;
    }

    //Combinable items appear below.
    if(strcmp(first, "Hammer") == 0 && strcmp(second, "Broken TV") == 0) {
        printf("\nYou have combined Hammer and Broken TV into CRT.\n\n");
        freeItem(removeItem("Broken TV", inventory));
        freeItem(removeItem("Hammer", inventory));
        addItem(newItem("CRT", "Cathode-ray tube. Nothing fancy.", NULL), inventory);
        return;
    } 
    if (strcmp(first, "Broken TV") == 0 && strcmp(second, "Hammer") == 0) {
        printf("\nYou have combined Hammer and Broken TV into CRT.\n\n");
        freeItem(removeItem("Broken TV", inventory));
        freeItem(removeItem("Hammer", inventory));
        addItem(newItem("CRT", "Cathode-ray tube. Nothing fancy.", NULL), inventory);
        return;
    }
    if (strcmp(first, "Glass Shards") == 0 && strcmp(second, "Glue") == 0) {
        printf("\nYou have combined Glass Shards and Glue into Glass Panel.\n\n");
        freeItem(removeItem("Glass Shards", inventory));
        freeItem(removeItem("Glue", inventory));
        addItem(newItem("Glass Panel", "The black mirror stares back.", NULL), inventory);
        return;
    }
    if (strcmp(first, "Glue") == 0 && strcmp(second, "Glass Shards") == 0) {
        printf("\nYou have combined Glass Shards and Glue into Glass Panel.\n\n");
        freeItem(removeItem("Glass Shards", inventory));
        freeItem(removeItem("Glue", inventory));
        addItem(newItem("Glass Panel", "The black mirror stares back.", NULL), inventory);
        return;
    }
    if (strcmp(first, "Glass Panel") == 0 && strcmp(second, "CRT") == 0) {
        printf("\nYou have combined Glass Panel and CRT into LCD Panel.\n\n");
        freeItem(removeItem("Glass Panel", inventory));
        freeItem(removeItem("CRT", inventory));
        addItem(newItem("LCD Panel", "With this you could finally play Pokemon with higher resolution.", NULL), inventory);
        return;
    }
    if (strcmp(first, "CRT") == 0 && strcmp(second, "Glass Panel") == 0) {
        printf("\nYou have combined Glass Panel and CRT into LCD Panel.\n\n");
        freeItem(removeItem("Glass Panel", inventory));
        freeItem(removeItem("CRT", inventory));
        addItem(newItem("LCD Panel", "With this you could finally play Pokemon with higher resolution.", NULL), inventory);
        return;
    }
    if (strcmp(first, "LCD Panel") == 0 && strcmp(second, "Broken Laptop") == 0) {
        printf("\nYou have combined LCD Panel and Broken Laptop into Dead Laptop.\n\n");
        freeItem(removeItem("LCD Panel", inventory));
        freeItem(removeItem("Broken Laptop", inventory));
        addItem(newItem("Dead Laptop", "Even my iPhone has longer battery life.", NULL), inventory);
        return;
    }
    if (strcmp(first, "Broken Laptop") == 0 && strcmp(second, "LCD Panel") == 0) {
        printf("\nYou have combined LCD Panel and Broken Laptop into Dead Laptop.\n\n");
        freeItem(removeItem("LCD Panel", inventory));
        freeItem(removeItem("Broken Laptop", inventory));
        addItem(newItem("Dead Laptop", "Even my iPhone has longer battery life.", NULL), inventory);
        return;
    }
    if (strcmp(first, "Dead Laptop") == 0 && strcmp(second, "Battery") == 0) {
        printf("\nYou have combined Dead Laptop and Battery into Laptop.\n\n");
        freeItem(removeItem("Dead Laptop", inventory));
        freeItem(removeItem("Battery", inventory));
        addItem(newItem("Laptop", "Your own state of the art gaming PC!", NULL), inventory);
        return;
    }
    if (strcmp(first, "Battery") == 0 && strcmp(second, "Dead Laptop") == 0) {
        printf("\nYou have combined Dead Laptop and Battery into Laptop.\n\n");
        freeItem(removeItem("Dead Laptop", inventory));
        freeItem(removeItem("Battery", inventory));
        addItem(newItem("Laptop", "Your own state of the art gaming PC!", NULL), inventory);
        return;
    }
    if (strcmp(first, "Rope") == 0 && strcmp(second, "Dumbbell") == 0) {
        printf("\nYou have combined Rope and Dumbbell into Budget Grappling Hook.\n\n");
        freeItem(removeItem("Rope", inventory));
        freeItem(removeItem("Dumbbell", inventory));
        addItem(newItem("Budget Grappling Hook", "A dumbbell attached to an end of a rope, Indiana Jones style.", NULL), inventory);
        return;
    }
    if (strcmp(first, "Dumbbell") == 0 && strcmp(second, "Rope") == 0) {
        printf("\nYou have combined Rope and Dumbbell into Budget Grappling Hook.\n\n");
        freeItem(removeItem("Rope", inventory));
        freeItem(removeItem("Dumbbell", inventory));
        addItem(newItem("Budget Grappling Hook", "A dumbbell attached to an end of a rope, Indiana Jones style.", NULL), inventory);
        return;
    }

    printf("\n%s and %s do not combine into anything.\n\n", first, second);
}

//Function for the talk command: Talk with others in room, triggers certain events if conditions are met.
void talk(char* arg) {

    if (arg == NULL) {
        printf("\nYou talk to yourself. If you want to talk to someone, enter \"talk [name]\".\n\n");
        return;
    }
    if (containsItem(arg, (*currentRoom).people) == 0 && containsItem(arg, people) == 0 ) {
        printf("\nThat person does not exist or is not currently in the room.\n\n");
        return;
    }
    //Specific interaction messages with Nina. 9
    if(strcmp(arg, "Nina") == 0) {
        if(containsItem("Paper Clip", inventory)) {
            printf("\nNina: Hey, is that a Paper Clip? Here, give it me, I can turn it into a Lockpick. It might be more\nuseful, don't you agree? Haha! Here you go!\n\n");
            removeItem("Paper Clip", inventory);
            addItem(newItem("Lockpick", "Opportunities await no one.", NULL), inventory);
            printf("Lockpick has been added to your inventory.\n\n");

        } else {

            int num = rand() % 4  + 1;    

            switch (num) {
                case 1:
                    printf("\nNina: Have you got the Hammer from the Workshop yet? I'm sure it'll be useful!\n\n");
                    break;
                case 2:
                    printf("\nNina: Man, I'm all for pranks and stuff, but this joke Zero's playing is way too far...\n\n");
                    break;
                case 3: 
                    printf("\nNina: These bracelets won't come off even with tools. Why is my number 9 and yours 5, I wonder...\n\n");
                    break;
                default:
                    printf("\nNina: Hey! What's up? Found anything that can come in handy?\n\n");
                    break;
            }
        }
    }

    //Specific interaction messages with Otto. 8
    if(strcmp(arg, "Otto") == 0) {
        if(containsItem("Laptop", inventory)) {
            printf("\nOtto: Dude, where did you get that Laptop? Connect it to the WiFi in the Media Lab bro!\n\n");
        } else {

            int num = rand() % 4  + 1;    

            switch (num) {
                case 1:
                    printf("\nOtto: Hmmm...to fix a broken laptop screen...you'd need an LCD Panel.\n\n");
                    break;
                case 2:
                    printf("\nOtto: Technology isn't completely foolproof...I'll find a way...eventually...\n\n");
                    break;
                case 3: 
                    printf("\nOtto: Most TV's have a CRT in them. You could make an LCD Panel with that.\n\n");
                    break;
                default:
                    printf("\nOtto: Ello, how you holding up?\n\n");
                    break;
            }
        }
    }

    //Specific interaction messages with Sev. 7
    if(strcmp(arg, "Sev") == 0) {

        int num = rand() % 4  + 1;    

        switch (num) {
            case 1:
                printf("\nSev: Bracelets numbered 1 to 9...strange indeed.\n\n");
                break;
            case 2:
                printf("\nSev: I'm too old to be solving riddles and playing games.\n\n");
                break;
            case 3: 
                printf("\nSev: Perhaps you should be weary of the shifting rooms. I don't like it...\n\n");
                break;
            default:
                printf("\nSev: Ah, you are that youngster from earlier. How can I help you?\n\n");
                break;
        }
        
    }

    //Specific interaction messages with Leo. 6
    if(strcmp(arg, "Leo") == 0) {
        if(aprilQuest) {
            printf("\nLeo: When April is scared like this, a good book can cheer her right up.\n\n");
        } else {
            int num = rand() % 4  + 1;    

            switch (num) {
                case 1:
                    printf("\nLeo: Hey, thanks for cheering up April. I really appreciate it.\n\n");
                    break;
                case 2:
                    printf("\nLeo: I don't if this will help you, but I noticed that the rooms only shift when you try to go too far from the Reception Room...\n\n");
                    break;
                case 3: 
                    printf("\nLeo: No matter how far you go in any one direction, you will loop back again. Strange.\n\n");
                    break;
                default:
                    printf("\nLeo: Hey.\n\n");
                    break;
            }
        }
    }

    //Specific interaction messages with April. 4
    if(strcmp(arg, "April") == 0) {
        if(aprilQuest) {
            if(containsItem("Book", inventory)) {
                printf("\n...\n\n");
                printf("April: Is that...A Colorless Sight? May I read it?\n\n");
                printf("You give the Book to April. Book removed from inventory.\n\n");
                removeItem("Book", inventory);
                printf("April: Eh?! You say...there's a library...here?\n\n");
                printf("April: Do you mind...showing me?\n\n");
                printf("Leo: Mind if I tag along too?\n\n");
                addItem(removeItem("April", (*currentRoom).people), people);
                addItem(removeItem("Leo", (*currentRoom).people), people);
                printf("April and Leo are now following you.\n\n");
            } else {
                printf("\n...\n\n");
                printf("April is too scared to be talked to right now.\n\n");
            }
          

        } else {
            int num = rand() % 4  + 1;    

            switch (num) {
                case 1:
                    printf("\nApril: Um...thank you for showing me the Library.\n\n");
                    break;
                case 2:
                    printf("\nApril: Um, I don't know if this would help but...this game reminds me of a math puzzle called the Magic Square...\n\n");
                    break;
                case 3: 
                    printf("\nApriL: You have the number 5 bracelet...the mean of the sum ofnumbers between 1 and 9...\n\n");
                    break;
                default:
                    printf("\nApril: ...Hello...\n\n");
                    break;
            }
        }
    }
    //Specific character interactions with Trey. 3
    if(strcmp(arg, "Trey") == 0) {


        if (treyCandy) {

            if (containsItem("Candy", inventory)) {
                printf("\nTrey: Is that candy? You're giving it to me? Okay...I guess we can talk.\n\n");
                removeItem("Candy", inventory);
                printf("\nYou give the Candy to Trey. Candy removed from inventory.\n\n");
                treyCandy = 0;
            } else {
                printf("\n...\n\n");
                printf("Trey: Leave me alone.\n\n");
            }
            
        } else {

            int num = rand() % 4  + 1;    

            switch (num) {
                case 1:
                    printf("\nTrey: Ace and Zwei are brothers. Zwei won't go anywhere without Ace.\n\n");
                    break;
                case 2:
                    printf("\nTrey: You'd have to help out April before Leo will listen to you.\n\n");
                    break;
                case 3: 
                    printf("\nTrey: People aren't going to like the smell of the Mystery Meat.\n\n");
                    break;
                default:
                    printf("\nTrey: Got any more candy?\n\n");
                    break;
            }
        }
    }

    //Specific interaction messages with Zwei. 2
    if(strcmp(arg, "Zwei") == 0) {
        if(containsItem("Ace", people) == 0) {
               printf("\n...\n\n");
            printf("Zwei ignores you.\n\n");
        } else {
            int num = rand() % 4  + 1;    

            switch (num) {
                case 1:
                    printf("\nZwei: Ace is really nice.\n\n");
                    break;
                case 2:
                    printf("\nZwei: Ace...I don't like this game.\n\n");
                    break;
                case 3: 
                    printf("\nZwei: If Ace trusts you...so do I.\n\n");
                    break;
                default:
                    printf("\nZwei: ...Hi.\n\n");
                    break;
            }
        }
    }

    //Specific interaction messages with Ace. 1
    if(strcmp(arg, "Ace") == 0) {

        int num = rand() % 4  + 1;    

        switch (num) {
            case 1:
                printf("\nAce: I'm worried about Trey...perhaps some Candy will help her open up.\n\n");
                break;
            case 2:
                printf("\nAce: I trust that you can figure out Zero's Game.\n\n");
                break;
            case 3: 
                printf("\nAce: If you have trouble, talking to others may help.\n\n");
                break;
            default:
                printf("\nAce: Hello. How can I help you?\n\n");
                break;
            
        }
    }
}

//Function for the follow command: Asks someone in the room to follow you if applicable.
void follow(char* arg) {
    
    if (arg == NULL) {
        printf("\nYou did not specify someone to follow you. Enter \"help follow\" for more info.\n\n");
        return;
    }
    if (containsItem(arg, people)) {
        printf("\n%s is already following you.\n\n", arg);
        return;    
    }
    if (containsItem(arg, (*currentRoom).people) == 0) {
        printf("\nThat person does not exist or is not currently in the room.\n\n");
        return;
    }

    //Zwei will only follow the player if Ace is already following.
    if(strcmp(arg, "Zwei") == 0) {
        if(containsItem("Ace", people) == 0) {
            printf("\nZwei: ...\n");
            printf("\nZwei refuses to follow you.\n\n");
            return;
        }
    }

    //Leo will not follow you until you help April.
    if(strcmp(arg, "Leo") == 0) {
        if(aprilQuest) {
            printf("\nLeo: Sorry...I'm going to stay with April until she feels better.\n");
            printf("\nLeo declines to follow you.\n\n");
            return;
        }
    }

    //April will not follow you whatsoever.
    if(strcmp(arg, "April") == 0) {
        if(aprilQuest) {
            printf("\nApril: ...\n\n");
            printf("\nApril is too scared to follow you.\n\n");
            return;
        } else {
            printf("\nApril: Sorry but...is it okay if I stay here?\n");
            printf("\nApril declines to follow you.\n\n");
            return;     
        }
    }

    //Trey will only follow you after you give her candy.
    if(strcmp(arg, "Trey") == 0) {
        if(treyCandy) {
            printf("\nTrey: What do you want?\n");
            printf("\nTrey does not want to follow you.\n\n");
            return;
        }
    }

    printf("\n%s: Sure, lead the way.\n", arg);
    addItem(removeItem(arg, (*currentRoom).people), people);
    printf("\n%s is now following you.\n\n", arg);
     syncValues(currentRoom, magicCube);
   
}

//Function for the leave command: Removes someone following you and adds them to the current room.
void leave(char* arg) {
    if (arg == NULL) {
        printf("\nYou did not specify someone to leave you. Enter \"help leave\" for more info.\n\n");
        return;
    }
    if (containsItem(arg, people) == 0) {
        printf("\nThat person does not exist or is currently not following you.\n\n");
        return;    
    }

    //Specific interaction, you can't leave April behind until she gets to the Library.
    if (strcmp(arg, "April") == 0) {
        if(aprilQuest) {
            printf("\nApril: Um...the library?\n");
            printf("\nYou cannot leave April until you bring her to the Library.\n\n");
            return;   
        }
    }
    //Specific interaciton, you can't elave Leo behind either until you bring April to the Library.
    if (strcmp(arg, "Leo") == 0) {
        if(aprilQuest) {
            printf("\nLeo: I want to take April to the Library first.\n");
            printf("\nYou cannot leave Leo until you bring April to the Library.\n\n");
            return;   
        }
    }

    addItem(removeItem(arg, people), (*currentRoom).people);
    printf("\n%s is no longer following you.\n\n", arg);
    syncValues(currentRoom, magicCube);

}

//Function for the quit command: Allows the user to terminate the game at any time.
void quit(char* arg) {
    gameOver = 4;
}


void main() {
    //Default items and rooms are declared here. Rooms are also added to a Linked List(for retrieving a random room).
    inventory = newItem(NULL, NULL, NULL);
    people = newItem(NULL, NULL, NULL);

    struct Item* itemSetStart = newItem(NULL, NULL, NULL);

    struct Item* itemSetLab = newItem(NULL, NULL, newItem("Glass Beaker",
        "Someone didn’t completely wash it properly. Faint red stains can still be seen.", NULL));

    struct Item* itemSetWorkshop = newItem(NULL, NULL, newItem("Hammer", "A blunt hammer. It’s a little rusty.",
                                   newItem("Rope", "Lightweight and durable, and perfect for hanging things.",
                                   newItem("Glue", "Elmer's Super Glue! Now only $1.99!", NULL) )));

    struct Item* itemSetLibrary = newItem(NULL, NULL, newItem("Book", "A Colorless Sight: An award winning romance novel about a blind girl and bullying.",
                                  newItem("Broken Laptop", "The screen is broken and the battery is missing...", NULL)));

    struct Item* itemSetStaff = newItem(NULL, NULL, NULL);

    struct Item* itemSetGym = newItem(NULL, NULL, newItem("Dumbbell", "Lighweight and blunt.", NULL));

    struct Item* itemSetMedia = newItem(NULL, NULL, newItem("Broken TV", "If it was working, you’d probably see a white mask with red cheeks on the screen.",
                                newItem("Battery", "Without the assault.", NULL)));

    struct Item* itemSetKitchen = newItem(NULL, NULL, NULL);

    struct Item* itemSetDorms = newItem(NULL, NULL, NULL);

    struct Room* ReceptionRoom = newRoom("Reception Room", "Nothing about this reception room is anything welcoming, with the heaviness of our fate etched\nout in the ticking of a digital clock on the wall. Besides an empty desk and a strange rubix cube\nwith numbers on it, the colorless atmosphere almost seems to make time stand still.", itemSetStart, newItem(NULL, NULL, NULL));
    cords(ReceptionRoom, 1, 1, 1);

    struct Room* ResearchLab = newRoom("Research Lab", "Microscopes and complicated machinery litter the countertops throughout the room, yet the lab\ncontains no traces of any use. The chill in the air is almost palpable as the only sound in the\nroom comes from the breeze of the ventilation vent in the ceiling.", itemSetLab, newItem(NULL, NULL, NULL));
    randomRooms = newLLRoom(ResearchLab, NULL);
    
    struct Room* Workshop = newRoom("Workshop", "Shavings litter the floor, and the smell of wood still lingers in the air. Several tools are lying\ncarefree on the tables, as if the previous people in the room didn’t have time to put them away.", itemSetWorkshop, newItem(NULL, NULL, NULL));
    randomRooms = newLLRoom(Workshop, randomRooms);

    struct Room* Library = newRoom("Library", "Bookshelves are packed to the brim with dusty reference books and other works of text. A\nposter slanting sideways on the wall says, \"Open a book, open a mind\". One could simply lose\ntrack of time in the world of fiction.", itemSetLibrary, newItem(NULL, NULL, NULL));
    randomRooms = newLLRoom(Library, randomRooms);

    struct Room* StaffRoom = newRoom("Staff Room", "A giant conference table lies in the center of the room, with chairs scattered all around. An\nempty water cooler lies in the corner, along with several filing cabinets. A minimalistic\nrepresentation of office life.", itemSetStaff, newItem(NULL, NULL, NULL));
    randomRooms = newLLRoom(StaffRoom, randomRooms);
    (*StaffRoom).omens = 1;

    struct Room* Gym = newRoom("Gym", "Multi-colored dumbbells and exercise mats bring life to the otherwise gray and dark room. For\nanother room in an underground shelter, the place is quite well packed with exercise equipment\nand weights.", itemSetGym, newItem(NULL, NULL, NULL));
    randomRooms = newLLRoom(Gym, randomRooms);

    struct Room* MediaLab = newRoom("Media Lab", "The audiovisual room is lined with desks and chairs, each featuring digital sound-editing\nhardware and headphones. Stereos line the walls, and a screen in the front depicts nothing\nbut emptiness.", itemSetMedia, newItem(NULL, NULL, NULL));
    randomRooms = newLLRoom(MediaLab, randomRooms);
    (*MediaLab).omens = 1;

    struct Room* Kitchen = newRoom("Kitchen", "Clearly worn down from prominent use, the kitchen is a remnant of its former glory among a\nhappy family. Racks of knives and other utensils lie on a counter, and the stovetop is charred\nfrom overheating. The sink is still working, and the sounds of a loose faucet dripping on the\nmetal echoes like a heartbeat.", itemSetKitchen, newItem(NULL, NULL, NULL));
    randomRooms = newLLRoom(Kitchen, randomRooms);
    (*Kitchen).omens = 1;

    struct Room* Dormitory = newRoom("Dormitories", "A pair of bunkbeds line two sides of the wall, and a futon is laid out in the middle. Every healthy\nhuman being should get at least 8 hours of rest, perhaps indefinite if the situation calls for it.", itemSetDorms, newItem(NULL, NULL, NULL));
    randomRooms = newLLRoom(Dormitory, randomRooms);
    (*Dormitory).omens = 1;

    //Dummy head node.
    randomRooms = newLLRoom(NULL, randomRooms);

    //Map is for documenting locations of rooms relative to each other. 
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                map[i][j][k] = NULL;
            }
        }
    }
    map[1][1][1] = ReceptionRoom;

    //Magic Cube stores information on the added values of people in each room.
     for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                magicCube[i][j][k] = 0;
            }
        }
    }
   
    //This section estblishes all the commands and adds them to a commands list.
    void (*look_ptr) = &look;
    void (*quit_ptr) = &quit;
    void (*move_ptr) = &move;
    void (*take_ptr) = &take;
    void (*drop_ptr) = &drop;
    void (*inventory_ptr) = &checkInventory;
    void (*help_ptr) = &help;
    void (*use_ptr) = &use;
    void (*combine_ptr) = &combine;
    void (*talk_ptr) = &talk;
    void (*follow_ptr) = &follow;
    void (*leave_ptr) = &leave;

    commandsList = newCommand(NULL, NULL, NULL, NULL);
    struct Command* lookCommand = newCommand("look", "Syntax: \"look\", \"look [item]\"\n\nGives more information for the current room, including items. Uses 5 minutes.\n\nGives more information for an item in inventory or room. Does not take up time.\n\n", look_ptr, NULL);
    struct Command* quitCommand = newCommand("quit", "Syntax: \"quit\"\n\nTerminates the game at any time.\n\n", quit_ptr, NULL);
    struct Command* moveCommand = newCommand("go", "Syntax: \"go [direction]\"\n\nMove in the target direction: north, west, east, up or down. Uses 10 minutes.\n\n", move_ptr, NULL);
    struct Command* takeCommand = newCommand("take", "Syntax: \"take [item]\"\n\nTakes an item from the current room and adds it to player's inventory if item exists in current\nroom. Does not take up any time.\n\n", take_ptr, NULL);
    struct Command* dropCommand = newCommand("drop", "Syntax: \"drop [item]\"\n\nDrops an item from player's inventory into the current room if item exists in inventory. Does not\ntake up any time.\n\n", drop_ptr, NULL);
    struct Command* inventoryCommand = newCommand("inventory", "Syntax: \"inventory\"\n\nGives a list of items in the player's inventory. Does not take up any time.\n\n", inventory_ptr, NULL);
    struct Command* helpCommand = newCommand("help", "Syntax: \"help\", \"help [command]\"\n\nGives a list of all commands in the game.\n\nGives description of a specific command.\n\n", help_ptr, NULL);
    struct Command* useCommand = newCommand("use", "Syntax: \"use [item]\"\n\nTriggers certain events in certain rooms with certain items. Does not take up any time.\n\n", use_ptr, NULL);
    struct Command* combineCommand = newCommand("combine", "Syntax: \"combine [item]&[item]\"\n\nCombines two items if applicable. Items must be in inventory, and are consumed once combined. Does\nnot take up any time.\n\n", combine_ptr, NULL);
    struct Command* talkCommand = newCommand("talk", "Syntax: \"talk [name]\"\n\nTalk to someone to gather information. Does not take up any time.\n\n", talk_ptr, NULL);
    struct Command* followCommand = newCommand("follow", "Syntax: \"follow [name]\"\n\nAsks someone to follow you. Does not take up any time.\n\n", follow_ptr, NULL);
    struct Command* leaveCommand = newCommand("leave", "Syntax: \"leave [name]\"\n\nRequest someone to stop following you. Does not take up any time.\n\n", leave_ptr, NULL);

    addCommand(quitCommand, commandsList);
    addCommand(leaveCommand, commandsList);
    addCommand(followCommand, commandsList);
    addCommand(talkCommand, commandsList);
    addCommand(combineCommand, commandsList);
    addCommand(useCommand, commandsList);
    addCommand(inventoryCommand, commandsList);
    addCommand(dropCommand, commandsList);
    addCommand(takeCommand, commandsList);
    addCommand(moveCommand, commandsList);
    addCommand(lookCommand, commandsList);
    addCommand(helpCommand, commandsList);
    

    //This section establishes the participants of Zero's game (excluding you, of course) adds them to your following list in the beginning.
    //People will eventually drift off to different rooms as you explore more.
    
    struct Item* Nina = newItem("Nina", "Holds the bracelet with the number 9. An outgoing mischievous prankster.", NULL);
    (*Nina).value = 9;
    struct Item* Otto = newItem("Otto", "Holds the bracelet with the number 8. A whiz with electronics and gadgets.", NULL);
    (*Otto).value = 8;
    struct Item* Sev = newItem("Sev", "Holds the bracelet with the number 7. An elderly man with a calm demeanor.", NULL);
    (*Sev).value = 7;
    struct Item* Leo = newItem("Leo", "Holds the bracelet with the number 6. An strong-willed youth. Friends with April.", NULL);
    (*Leo).value = 6;
    struct Item* April = newItem("April", "Holds the bracelet with the number 4. A timid and shy girl. Friends with Leo.", NULL);
    (*April).value = 4;
    struct Item* Trey = newItem("Trey", "Holds the bracelet with the number 3. A fierce hot-headed girl with determination.", NULL);
    (*Trey).value = 3;
    struct Item* Zwei = newItem("Zwei", "Holds the bracelet with the number 2. A quiet boy. Younger brother of Ace.", NULL);
    (*Zwei).value = 2;
    struct Item* Ace = newItem("Ace", "Holds the bracelet with the number 1. A resonable and charismatic young man. Older brother of Zwei.", NULL);
    (*Ace).value = 1;

    addItem(Nina, people);
    addItem(Otto, people);
    addItem(Sev, people);
 
    addItem(Trey, people);
    addItem(Zwei, people);
    addItem(Ace, people);

    //Additional variables for game logic. Naming should be intuitive.
    char input[50];
    currentRoom = ReceptionRoom;

    //Unlike other characters, Leo and April begin in the Reception Room and stay there instead of following the player initially.
    addItem(Leo, (*currentRoom).people);
    addItem(April, (*currentRoom).people);


    printf("===================================\n");
    printf("Nine Hours, Nine People, Nine Rooms\n");
    printf("===================================\n");
    printf("A text adventure game by Victor L.\n");
    printf("\n");
    printf("This game is a work of fiction. Any resemblance to actual people, living or dead, or actual\nevents is purely coincidental.\n");
    printf("\n");
    printf("Synopsis: A mysterious man named Zero has kidnapped you and eight other people in an underground\nshelter. Your task is to solve his game and escape before it's too late.\n\n");
    printf("Press ENTER to continue...\n");
    cont();

    printf("...\n\n");
    cont();

    printf("You open your eyes, and find yourself in an unfamiliar place.\n");
    cont();
    printf("Looking around what appears to be a windowless and lifeless room, you are shocked to find eight\nother people in the room waking up and groaning around you.\n");
    cont();
    printf("A girl's voice calls out: \"Ugh, my head...\" as she picks herself up off the ground. A man's\nvoice follows. \"Where am I?\"\n");
    cont();
    printf("As everyone else scrambles awake, confusion and chaos are imminent and the tension is almost palpable.\n");
    cont();
    printf("\"Who are you guys?\" an young girl asks pointedly. \"Why...can't I remember how I got here?\" a\nman grumbles.\n");
    cont();
    printf("You try to recall yourself, but found that your memory is blank too.\n");
    cont();
    printf("\"Okay, what's going on?\" a timid voice stammers, directed to you. You shake your head,\nunable to answer.\n");
    cont();
    printf("\"Whoever put us in here, it's not funny!\" a girl screams at the air, kicking against the wall.\n");
    cont();
    printf("\"As others voice their agreement, you look down and see something attached to your right arm\nlike a bracelet.\n");
    cont();
    printf("\"You look around, and see that the others have something similar on their wrists as well.\n");
    cont();
    printf("\"What the heck is this? Is this a number 3?\" a young youth mutters, tugging at the bracelet.\n");
    cont();
    printf("\"I wouldn't do that if I were you.\" a deep male voice echos across the room.\n");
    cont();
    printf("Suddenly, an intercom somewhere in the room blared to life with static, before the same voice\nrang out again.\n");
    cont();
    printf("\"Greetings, ladies and gentlemen. I hope you found your rest well and dreams pleasant.\"\n");
    cont();
    printf("\"My name is Zero, and you are all participates of a game.\"\n");
    cont();
    printf("\"A game...where you will put your life on the line.\"\n");
    cont();
    printf("Silence fills the room after \"Zero\"'s announcement. A game? Of life and death?.\n");
    cont();
    printf("A girl breaks the tension. \"Hold up, what do you mean by that? Participates? Game? Our\nlives on the line?\"\n");
    cont();
    printf("The voice continues. \"Yes...you heard me correctly. Right now you're in an underground\nshelter, unaccessible to the outside world. There is no escape except to beat my game.\"\n");
    cont();
    printf("\"You may have already noticed the bracelets. Those are necessary for solving my puzzle.\nPlease don't attempt to take them off...I'd rather not deal with any...unpleasantness.\"\n");
    cont();
    printf("\"Please try your best to work together. You have 9 hours to escape this shelter, otherwise...\"\n");
    cont();
    printf("\"Well, it is a game of life and death after all.\"\n");
    cont();
    printf("\"Good luck.\" Static again, and Zero's voice fades out.\n");
    cont();
    printf("...\n");
    cont();
    printf("\"That was it? What the heck was that?\" a girl grumbles.\n");
    cont();
    printf("Another guy echos agreement. \"That explained nothing. Except that we're going to die in 9 hours.\"\n");
    cont();
    printf("Hey now, it's too early to give up now. Let's first get to know each other first,\" a young man\ncalmly says.\n");
    cont();
    printf("\"You can't be serious. Why should we play this silly game of his?\" the girl from before argues.\n");
    cont();
    printf("\"Because...\" the young man says, with heavy emphasis. \"We're not exactly in a position to\nbargain.\"\n");
    cont();
    printf("Silence once again, but it seems like everyone, even the girl, has come to an understanding.\n");
    cont();
    printf("\"Let's go around sharing our names and our bracelet numbers, cause Zero said these are\nimportant to solving the game.\" the young man continues.\n");
    cont();
    printf("\"My name is Ace. My bracelet is the number 1. And this kid is my younger brother.\"\n");
    cont();
    printf("\"Go on, introduce yourself.\" Ace urges a young boy next to him.\n");
    cont();
    printf("\"My name...my name is Zwei.\" the kid stammers. \"Bracelet number 2.\"\n");
    cont();
    printf("The girl from before reluctantly goes next. \"Trey. 3.\" she says unfriendly.\n");
    cont();
    printf("\"At Ace's urging, a shivering girl meekly squeaks out her name. \"I'm...April...\"\n");
    cont();
    printf("Without missing a beat, the boy next to her chimes in. \"Her number is 4. The name's Leo, I\nhave the number 6 on my bracelet.\"\n");
    cont();
    printf("\"My name is Sev. Braclet number 7.\" an elderly man speaks up next.\n");
    cont();
    printf("\"Otto. Number 8.\" says the guy next to you as he continues to poke at his bracelet.\n");
    cont();
    printf("\"Nina! Bracelet number 9!\" a friendly girl proudly declares her number.\n");
    cont();
    printf("Finally, all eyes lay on you, as the last person left. You introduce yourself, and hold out\nyour arm for everyone to see. Bracelet number 5.\n");
    cont();
    printf("\"Okay, that only took up 10 minutes, but now we have more information than before.\" Ace\nwraps up the introductions.\n");
    time = time - 10;
    cont();
    printf("\"And what good comes out of this?\" Trey grumbles again.\n");
    cont();
    printf("Sev's voice calls out. \"One to nine...and a gamemaster named Zero...\"\n");
    cont();
    printf("\"Whatever these bracelets are, they're definitely not just cosmetics.\" Otto remarks.\n\"I think these can explode at any time.\"\n");
    cont();
    printf("\"Shouldn't you not mess around with it then...?\" Leo says, then turns to comfort April,\nwho starts crying.\n");
    cont();
    printf("\"I'm no good with riddles. Someone else figure this out.\" Nina yawns.\n");
    cont();
    printf("All these conversations are going nowhere. You begin to try to piece together the game, and\nwith just under 9 hours left, it looks like your life depends on it.\n");
    cont();
    printf("No...not just you...everyone. In this game of life and death.\n");
    cont();
    printf("=================================================================\n");
    printf("\n");
    printf("The game is now in your hands. Look around to gather clues, talk to others to figure out\nmore about the situation, and use your head to escape Zero's game. Commands can be found\nwith help, case sensitive.\n\n");
    checkTime();
    printf("\n");
    printf("\n");

    while(gameOver == 0) {

        printf("Enter a command: \n");
        fgets(input, 50, stdin);

        //This adds an extra space at the end of the input string to help parse if there is only one argument.
        int size = strlen(input);
        input[size - 1] = ' ';
        input[size] = '\0';

        //keyword is for the command.
        char* keyword = strtok(input, " ");
        if (keyword == NULL) {
            printf("Invalid command. Use \"help\" for a list of commands, or \"help command\" for a specific command.\n\n");
            continue;
        }

        //args is a combined string of additional arguements after the command.
        char* args = (char*) malloc(30);

        char* arg = strtok(NULL, " ");
        if(arg == NULL) {
            args = NULL;
        } else {
            strcat(args, arg);
            while (arg != NULL) {

                arg = strtok(NULL, " ");
                if (arg != NULL) {
                    strcat(args, " ");  
                    strcat(args, arg);     
                }
            }
        }

        //We now have two strings: keyword for the command, args for the arguments.

        struct Command* command = containsCommand(keyword, commandsList);
        if (command == NULL) {
            printf("\nInvalid command. Use \"help\" for a list of commands, or \"help command\" for a specific command.\n\n");
        } else {
            executeCommand(command, args);
        }
    }
    
    //Different outcomes.
    if(gameOver == 1) {
        printf("\nGAMEOVER: An event has triggered, but could not connect back to the starting room.");
    }

    if(gameOver == 2) {
        printf("\nGAME OVER: You attempted to create more than 9 rooms.\n\n");
    }

    if(gameOver == 3) {

        printf("DING DONG DING DONG\n\n");
        printf("Press ENTER to continue.\n");
        cont();
        printf("Zero's voice echos throughout the %s over an intercom. \"Hello ladies and gentlemen. The\n9 hours designated for you all to work together and escape...\"\n", (*currentRoom).name);
        cont();
        printf("Has regrettably come to an end.\"\n");
        cont();
        printf("Silence fills the room, as gravity of the situation comes to everyone's attention.\n"); 
        cont();  
        printf("There was no more hope. Only the sound of Zero's final remarks.\n");
        cont();
        printf("\"Unfortunately, it appears you all could not solve my puzzle in time. Such a shame, really, I had high\nhopes for each and everyone of you. I truly, truly believed. I truly, truly, believed, that the nine\nof you could have solved my puzzle and make it out alive. Together.\"\n");
        cont();
        printf("\"The magic of human interaction is truly a wonderful gift. Connections, added up, are all the same. But\nalas, such things can never be comprehended by you all. Not in this life, at least.\"\n\n\"Farewell.\"\n");

        printf("\nGAME OVER: You have run out of time.\n\n");
    }

    if(gameOver == 4) {
        printf("\nGAME OVER: You have terminated the game.\n\n");

    }
    if(gameOver == 8) {
        printf("When you enter the Reception Room, a mysterious alarm starts playing.\n\n");
        printf("Press ENTER to continue.\n");
        cont();
        printf("You look at the strange-looking rubix cube from before: it is now lit up, and filled with numbers.\n");
        cont();
        printf("You realize it's a magic square, with 5 at the center.\n");
        cont();
        printf("Zero's voice comes through the intercom. \"Congratulations!\"\n");
        cont();
        printf("\"You all have managed to work together and figure out my puzzle. With time to spare.\"\n");
        cont();
        printf("\"Now that the game is over, I'll answer any questions you have when you leave.\"\n");
        cont();
        printf("As everyone else gather in the Reception Room, rumbling can be heard as the room shift a final time.\n");
        cont();
        printf("Clunk. The Reception Room stops moving and locks in place. You hear a click, and the South door swings open.\n\n");
        cont();
        printf("You look at everyone else. They look at each other and you. Then you all look towards the South door,\nat the blinding light outside.\n");
        cont();
        printf("And together, you take all take a step forward.\n\n");
        
        printf("YOU HAVE ESCAPED: Solve the magic square puzzle of Zero's game.\n\n");
    }
    if(gameOver == 9) {
        printf("\nYOU HAVE ESCAPED: Hacked the system and escaped Zero's game.\n\n");
    }

    //garbage collection time!
    freeAllItems(itemSetStart);
    freeAllItems(itemSetLab);
    freeAllItems(itemSetWorkshop);
    freeAllItems(itemSetLibrary);
    freeAllItems(itemSetStaff);
    freeAllItems(itemSetDorms);
    freeAllItems(itemSetGym);
    freeAllItems(itemSetMedia);
    freeAllItems(itemSetKitchen);
    freeAllItems(inventory);
    //freeing all the people(cause they escaped or died)
    freeAllItems(people);
    freeAllItems((*ResearchLab).people);
    freeAllItems((*Workshop).people);
    freeAllItems((*Library).people);
    freeAllItems((*StaffRoom).people);
    freeAllItems((*Dormitory).people);
    freeAllItems((*Gym).people);
    freeAllItems((*MediaLab).people);
    freeAllItems((*Kitchen).people);
    //Freeing all the rooms cause Zero's game is over
    freeRoom(ReceptionRoom);
    freeRoom(ResearchLab);
    freeRoom(Workshop);
    freeRoom(Library);
    freeRoom(StaffRoom);
    freeRoom(Dormitory);
    freeRoom(Gym);
    freeRoom(MediaLab);
    freeRoom(Kitchen);
    //Freeing everything else I missed.
    freeAllRooms(randomRooms);
}
