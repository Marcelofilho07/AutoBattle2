
# AutoBattle2
## Introduction
This is an Auto Battler made in C++ for a recruitment process at Kokku. In this file I'll document all changes made to the original project and how each class is working now.

The game follow all rules from the requisite document, except for a need of a "Index" variable which I have found no use for it and it could be said it has an arbitrary decision by the fact that the turn order is decided by how the characters are created. Also, I don't folllow a class system. Each character can have their own customizible attributes. For the extra feature, I'm from 27/12/1997 so I've added an Invulnerable (take no damage) and an Empower (more damage) state to each character.

For variable and functions names I'm trying to follow the Unreal Engine coding [guidelines](https://docs.unrealengine.com/4.26/en-US/ProductionPipelines/DevelopmentSetup/CodingStandard/)
## AutoBattleC++.cpp

```
// AutoBattleC++.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//
#include "BattleField.h"
#include <iostream> 

int main()
{
    BattleField* battleField = new(BattleField);
    battleField->Setup();
}
```

First of all, there is no need to include <iostream> since we are not using it here.
We're creating a Battlefield pointer and instancing it's memory on the heap. There is no need for it.
We can just create a Battlefield variable on the stack and run it directly.

All those changes have been done in the final version
```
#include "Public/BattleField.h"

int main()
{
    BattleField battleField;
    battleField.Setup();
}
```

## BattleField.h

Once again, some unnecessary includes. First iostream for the same previous reason, we're not using it. "Grid.h" could be forward declared since we're only using a pointer to it here.
I like to avoid using "using namespace" so I will also be removed.

For the class itself, all members are declared as public. We're going to change this and expose as public only what is needed like the Setup function and some getters.
For the members we are going to make a lot of changes.
```
	Grid	* grid;                         
	Types::GridBox* PlayerCurrentLocation;  
	Types::GridBox* EnemyCurrentLocation;
	list<Character>* AllPlayers;
	shared_ptr<Character> PlayerCharacter;
	//Character* PlayerCharacter;
	shared_ptr<Character>* EnemyCharacter;
	int currentTurn;
	int numberOfPossibleTiles;
```
There is no need for this huge space between "Grid" and "*". Also when we get further in this document we'll see that we're no longer using a Grind class.

This "Types::Gridbox" definition seens very weird so we're changing it in the future.

A list<Character> is good option for a container to hold AllPlayer, but there is no need to a be pointer to a List. We're changing this and using a forward list since there is no need to use a double-linked list.

A shared_ptr to hold our players data can be good since we might share them with other classes, but we should't have a pointer to a shared_ptr like for the EnemyCharacter and we're also making other changes to this.

I believe those two last variables can alse be discarded, but there is no problem with them for now.

And of course, remove all unnecessary comments.

For functions I believe there isn't a lot of problems worth mentioning, besides making some of then const. There will be a lot of changes.

Our [final interation](https://github.com/Marcelofilho07/AutoBattle2/blob/master/AutoBattleC%2B%2B/Public/BattleField.h) is much cleaner and have only what we need.
We have a clear separation between private and public members. No unnecessary includes.

```
	std::vector<std::vector<GridNode>> Grid;
	std::forward_list<std::shared_ptr<Character>> CharacterList;
	int NumberOfCharacters = 0;
	int NumberOfCharactersDead = 0;
	bool bIsGameRunning = true;
	int GridHeight = 0;
	int GridWidth = 0;
```
For our variables:

Our Grid no longer is a class, but a Vector of a Vector of GridNodes. We are using a vector here since we can have direct acess in O[1] and we can resize the container at our will.

Our Players and "AllPlayers" variables have been merged in a single forward_list of shared_ptr of Character. We're using shared_ptr since we're sharing ownership of this pointer between the list itself, other characters and the GridNode it is staying.

At last we have some support variables.

I'll venture more in depth of our new functions when I start explaining the final .cpp file in the next section.

## BattleField.cpp

This code is a mess and doesn't work at all. Again some unnecessary includes, but let us go through each function.

### BattleField()

Our default constructor initialize some variables, allocate memory on the heap and runs "Setup()".
In this case we could use a list-initialization or member-initialization, avoid using the new operator and remove the "Setup()" call, since we're calling it in main().
We'll be initializing our variables using member-initialization from C++11.

### Setup()

Here it's calling only "GetPlayerChoice()" and has a weird amount of blank spaces. We'll be removing everything and adding our main gameloop here.
Here our player will be prompted the size of the grid and if they want to add any new characters. We'll leave this loop only when the game ends.

### GetPlayerChoice()

This function has a lot of problem. First it doesn't do anything. It is using old C-style printing instead with printf.
```
    std::getline(std::cin, choice);
    
    cin >> choice;
```
Here, it's trying to get a input twice and it's using "std::" even having declared "using namespace std" before.

It uses a redundant switch statement to check the user input where a simple if would solve, for example:
```
if(choice > 1 && choice < 5)
{
    CreatePlayerCharacter(choice);
}
```
But choice is a string and CreatePlayerCharacter recieves an integer as parameter, so there is a wrong type problem.
Lastly, the default case calls itself where can lead to stackoverflow problem. A simple loop would solve this problem.

In our final interation this function has been deleted and it's functionality merged into CreateCharacter().

### CreatePlayerCharacter(int classIndex)

First, this function try to do a old C-Style cast from a integer to a pointer.
After, it tries to contruct a shared pointer of type Character using a pointer intead of the right type.
At last it calls CreateEnemyCharacter().

In our final interation this function has been deleted and it's functionality merged into CreateCharacter().

### CreateEnemyCharacter()

This functions calls another fuction(GetRandomInt) triggering a infinite function call loop
creating a stackoverflow error. Again, it uses a old C-Style cast. After it tries to initialize a
a raw pointer to shared_ptr using a raw pointer to Character. It rewrites the other player data and
at last it calls StartGame(). Calling another function inside a function recursivile is a repeating pattern in this code
and also a bad practice, because it doens't let the memory be freed and is very confusing to read.

In our final interation this function has been deleted and it's functionality merged into CreateCharacter().

### CreateCharacter() [NEW FUNCTION]

This is our interpretation of how those previous classes should be implemented. In short it handles player input to customize a character, alocate it to a space on the grid, then adds it to our forward_list through a shared_ptr. 

### StartGame()

Here things doens't work because of previous wrong types. It calls AlocatePlayers() and StartTurn.

In our final interation, this class is only resposible to call the turns of the game and ask if the player wants to play again.

### StartTurn()

Besides an empty if and calling another function in a chain of functions, this function is ok.

In our final interation this function has been deleted and it's functionality merged into HandleTurn().

### HandleTurn()

At the end of this function there is a recursive call to StartTurn that can cause a stack overflow.

In our final interation this function returns a bool to handle if our game has ended. It goes through every character and makes them execute their turn.

### GetRandomInt()

Broken infinite recursive call. Deprecated in final version.

### AlocatePlayers()

Redundant function only to call another function. Deprecated in final version.

### AlocatePlayerCharacter()

Broken, convoluted and unnecessary function. Again, recursive call of itself. Deprecated in final version.

### AlocateEnemyCharacter()

Broken, convoluted and unnecessary function. Again, recursive call of itself. Deprecated in final version.

### ClearGame() [NEW FUNCTION]

Responsible to cleaning up containers and resetting variables.

### DrawGrid() [NEW FUNCTION]

Draws the grid.

### ShowMsgReceiveInput(T& Input, const std::string& InputMsg, const std::string& FailMsg)() [NEW FUNCTION]

Properly treat input from player. Uses template.

## Character.h

Going forward, I belive the errors gonna repeat it self, so I'll be more direct on them.

For this header, again wrong includes. Adding "Character.h" to itself. Missing "vector" include.
All members are public, exposing unnecessary data. Many unnecessary comments.

Our final interation has proper includes, visibiliy and only necessary functions.

## Character.cpp

### Character(Types::CharacterClass charcaterClass)

This Parameterized Constructor is missing to iniatlize variables.

In our final interation, we use a proper list initialization.

### StartTurn(Grid* battlefield)

This class is supposed to handle target detection, but it has many redundant if else statements and return calls in the middle of code execution.

### Other Functions

All other functions in this class are empty.

### Final Interation

In our final interation we have a proper Parameterized Constructor, a function to better handle target detection, movement, attack and taking damage.

## Grid.h

Same problem of and all members beeing public, also doesn't proper initialize variables.

This class has been deleted in our final interation. We use a vector of vectors to represent a grid.

## Grid.cpp

### Grid(int Lines, int Columns)

Constructor using the heap and doesn't keep track of pointers to free memory afterwards, innevitable memory leak.
Also uses insert method instead of push_back(better performance).

### drawBattlefield(int Lines, int Columns)

This doesn't work. Just print an empty grid. Also causes memory leak.

## Types.h

This class contains a Struct and a Enum. Both of those data structeres doesn't make sense to be stored togheter inside a class. They should be declared somewhere else, for example in the Battlefield class.

In our final interation we don't use this class. It has been deleted.

# Conlcusion

The original project has a lot of problems from memory, efficiency and doesn't work at all.
Bad includes, stack overflow problems, memory leaks, wrong usage of types. All those problems were solved in our final interation and the game should run properly.
There is a minor know issue regarding input: Inputting 2 or more chars of different type may cause the buffer to overflow with the inputs after the first one is read.
I hope you enjoy my project and I looking forward a feedback.

