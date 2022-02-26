9 Hours 9 People 9 Rooms Project 2 CS230 2020

I apologize in advance to whichever TA has to play my game. Playthrough to victory took me 6 minutes at best, so my video is of me losing after exploring most of the functionalities and features of my game.

There are two victory conditions: One is by forming a Magic Square by moving 8 other people around via follow and leave commands, and by generating the rooms so that they can form a square. This was ridiculously challenging due to how much things you need to keep track of, from room locations to people. Relying on look over and over burns up a lot of time, and while this is the "canon" victory, it's fairly unrealistic to achieve unless you 100% know what you're doing beforehand. Here are general features that are useful to know when going for this win condition,

1: Game space takes place in a 3x3x3 cube with the starting room (Reception Room) in the center. Events trigger when you go off the cube (too far from Reception Room).
2: Specific characters are more of a hassle to control than others. Zwei will not follow you unless Ace is following you, Trey won't follow you unless you talk to her with the Candy, April and Leo will not follow you until you give April the Book, in which they will follow you until you arrive at the Library. From there, April will stay permanently, while Leo can be moved.
3:Candy is found in the Staff Room after triggering the filing cabinet omen by using the Hammer found in the Workshop, Book is found in the Library.
4: Since you have bracelet number 5, going back to the center (Reception Room) once other people are in correct positions triggers the victory.

The second victory condition is hacking the system, and this is the required winning state where the player has found at least one omen, two items and is in the starting room. Specifically the two items needed are the Network PC and System Shutdown Codes, which are achieved through a couple omens. This is easier to achieve once you figure out the combinations via talking to specific people.

1: Take and drop Glass Beaker from Research Lab to obtain Glass Shards, combine Glass Shards with Glue from Workshop to get Glass Panel.
2: Use Hammer from Workshop in Staff Room to obtain Paper Clip (omen), talk to Nina with Paper Clip in inventory to obtain Lockpick.
3: Combine Hammer and Broken TV from Media Lab to obtain CRT. Combine CRT with Glass Panel to get LCD Panel. Combine LCD Panel with the Broken Laptop from Library to get Dead Laptop. Combine Dead Laptop with Battery from Media Lab to get Laptop. Use Laptop in Media Lab to obtain Network PC.
4: Use Lockpick in Dormitories to trigger the omen and get System Shutdown Codes. With both items in inventory, using the Network PC in the Reception Room triggers the winning state.

Losing states include:

-Creating more than 9 rooms
-Triggering an event when the Reception Room is already connected to 6 other rooms in all directions.
-Running out of time

As for the programming, since I wanted all my commands to be consistent so that they can be treated as structures and added to a linked list, all of them simply take a string as their arguments, received after parsing. Hence there are several global variables to manage game state modified by different functions, such as room locations relative to each other, inventory, people following you, or even if the game is over. For people, I repurposed the item structure to also take on an int value (For the magic square victory condition), and the rooms to take on an additional linked list of items called people. To select a random room when exploring, I made another structure with a pointer to a pointer of a room called LLRoom, which allows me to create a linked list of room pointers. A random number is generated and whichever room is at that point in the linked list is removed and used, and the max possible value of the random number is subtracted for later. Speaking of linked lists, all linked lists from rooms to items to people start with a "buffer" node, which contains no information and just has a pointer to the "first" element. The purpose of this buffer node is to make adding nodes to the list easier, as there becomes no need to modify the pointer. This buffer node still gets freed along with everything else malloced at the end of the game.

My game is definitely not playable in two minutes and I may have stretched the definition of omens and events to better suit my vision and goals for my game, along with severely overcomplicating what was already overcomplicated with the whole "can't go south" thing. Really grateful for the deadline extension Joe, and again, sorry for this ridiculously challenging game.


Youtube video: https://youtu.be/Ao9aqCQUpZg