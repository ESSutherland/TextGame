//Ethan Sutherland
//essutherland1@dmacc.edu
//Week 7 assignment

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

//gameItem structure
struct gameItem {
	string Name;
	string Desc;
	bool obtained;
	int X;
	int Y;
};

//gameEnemy structure
struct gameEnemy {
	string Name;
	int Damage;
	int Health;
	bool alive;
	int X;
	int Y;
};

struct gameRoom {
	string Name;
	string Desc;
	string Info;
	int X;
	int Y;
};

enum Direction {
	NORTH,
	SOUTH,
	EAST,
	WEST
};

//Game class
class Game{
public:
	void movePlayer(Direction);
	void roomDesc();
	void printInventory();
	void printInventoryItem(int);
	void checkChoice();
	void initItems();
	void initEnemies();
	void initRooms();
	void dropItem(gameItem&);
	void pickUpItem(gameItem&);
	void fightEnemy(gameEnemy&);
	bool unlockDoor();
	bool checkWin();
};
//Declare global variables
const int MAP_SIZE = 4;
bool won = false, cellDoor = false, endGame = false;
int choice;
string playerName;
int itemCount = 0, playerX = 0, playerY = 0, day = 1, moves = 0, healthPoints = 100, attackDamage;
vector<gameItem> inventory;
string wallText = "You cannot go that direction, there is a wall";
string lockedText = "This door is locked. Maybe there are keys somewhere.";
gameItem letter, fire, ice, earth, air, keys, cellKey, sword, healthPotion;
gameEnemy guard1, guard2;
Game game;
gameRoom map[MAP_SIZE][MAP_SIZE];
bool lockedDoors[8];



//function to print the inventory
void Game::printInventory() {
	int itemNum, itemChoice;
	//print items in inventory
	cout << "\n Inventory: " << endl;
	for (int i = 0; i < itemCount; i++) {
		cout << i + 1 << " - " << inventory[i].Name << endl;
	}
	//input for an item
	cout << "Choose and item: ";
	cin >> itemNum;
	if (itemNum > 0 && itemNum <= itemCount) {
		//input for drop or inspect
		if (inventory[itemNum - 1].Name != healthPotion.Name) {
			cout << "\n1 - Inspect" << endl;
			cout << "2 - Drop" << endl;
			cout << "What would you like to do: ";
			cin >> itemChoice;
		}
		else if (inventory[itemNum - 1].Name == healthPotion.Name) {
			cout << "\n1 - Inspect" << endl;
			cout << "2 - Drop" << endl;
			cout << "3 - Use" << endl;
			cout << "What would you like to do: ";
			cin >> itemChoice;
		}
		//print item description
		if (itemChoice == 1) {
			game.printInventoryItem(itemNum);
		}
		//drop an item
		else if (itemChoice == 2) {
			if (inventory[itemNum - 1].Name == letter.Name) {
				cout << "You cannot drop the letter." << endl;
			}
			else if (inventory[itemNum - 1].Name == keys.Name) {
				game.dropItem(keys);
				inventory.erase(inventory.begin() + (itemNum - 1));
			}
			else if (inventory[itemNum - 1].Name == cellKey.Name) {
				game.dropItem(cellKey);
				inventory.erase(inventory.begin() + (itemNum - 1));
			}
			else if (inventory[itemNum - 1].Name == fire.Name) {
				game.dropItem(fire);
				inventory.erase(inventory.begin() + (itemNum - 1));
			}
			else if (inventory[itemNum - 1].Name == ice.Name) {
				game.dropItem(ice);
				inventory.erase(inventory.begin() + (itemNum - 1));
			}
			else if (inventory[itemNum - 1].Name == earth.Name) {
				game.dropItem(earth);
				inventory.erase(inventory.begin() + (itemNum - 1));
			}
			else if (inventory[itemNum - 1].Name == air.Name) {
				dropItem(air);
				inventory.erase(inventory.begin() + (itemNum - 1));
			}
			else if (inventory[itemNum - 1].Name == sword.Name) {
				dropItem(sword);
				inventory.erase(inventory.begin() + (itemNum - 1));
			}
		}
		else if (inventory[itemNum - 1].Name == healthPotion.Name && itemChoice == 3) {		
			if (healthPoints == 100) {
				cout << "Your health is full. \n" << endl;
			}
			else {
				healthPoints += 40;
				inventory.erase(inventory.begin() + (itemNum - 1));
				cout << healthPotion.Name << " has been used." << endl;
				itemCount--;
				if (healthPoints > 100) {
					healthPoints = 100;
				}
			}			
		}
		else {
			cout << "Invalid option." << endl;
		}
	}
}

//function to print the description of an item
void Game::printInventoryItem(int num) {
	cout << "\n" << inventory[num - 1].Desc << endl;	
}

void Game::movePlayer(Direction dir) {
	if (dir == NORTH) {
		cout << "\n You go North." << endl;
		playerY--;
		moves++;
	}
	else if (dir == SOUTH) {
		cout << "\n You go South." << endl;
		playerY++;
		moves++;
	}
	else if (dir == WEST) {
		cout << "\n You go West." << endl;
		playerX--;
		moves++;
	}
	else if (dir == EAST) {
		cout << "\n You go East." << endl;
		playerX++;
		moves++;
	}
	else {

	}
}

//function to check the input from the user
void Game::checkChoice() {
	Direction dir;
	//prints inventory if 6 is input
	if (choice == 6){
		game.printInventory();
	}
	//prints player coordinates (for testing)
	else if (choice == 0) {
		cout << playerX << " - " <<  playerY << endl;
	}
	//if north is selected
	else if (choice == 1) {
		dir = NORTH;
		//checks if going north is out of bounds
		if (playerY - 1 < 0) {
			cout << wallText << endl;
		}
		//checks for hard coded walls
		else if ((playerX == 3 && playerY == 0) || (playerX == 0 && playerY == 2) || (playerX == 0 && playerY == 3) || (playerX == 3 && playerY == 1)) {
			cout << wallText << endl;
		}
		//checks for locked door
		else if ((playerX == 1 && playerY == 1) || (playerX == 2 && playerY == 3) || (playerX == 2 && playerY == 2) || (playerX == 3 && playerY == 2)) {
			if (playerX == 1 && playerY == 1 && !lockedDoors[0] && keys.obtained) {
				lockedDoors[0] = unlockDoor();
				if (lockedDoors[0]) {
					movePlayer(dir);
				}
			}
			else if (playerX == 1 && playerY == 1 && lockedDoors[0]) {
				movePlayer(dir);
			}
			else if (playerX == 1 && playerY == 1 && !lockedDoors[0] && !keys.obtained) {
				cout << lockedText << endl;
			}				
			else if (playerX == 2 && playerY == 3 && !lockedDoors[1] && keys.obtained) {
				lockedDoors[1] = unlockDoor();
				if (lockedDoors[1]) {
					movePlayer(dir);
				}
			}
			else if (playerX == 2 && playerY == 3 && lockedDoors[1]) {
				movePlayer(dir);
			}
			else if (playerX == 2 && playerY == 3 && !lockedDoors[1] && !keys.obtained) {
				cout << lockedText << endl;
			}				
			else if (playerX == 2 && playerY == 2 && !lockedDoors[2] && keys.obtained) {
				lockedDoors[2] = unlockDoor();
				if (lockedDoors[2]) {
					movePlayer(dir);
				}
			}
			else if (playerX == 2 && playerY == 2 && lockedDoors[2]) {
				movePlayer(dir);
			}
			else if (playerX == 2 && playerY == 2 && !lockedDoors[2] && !keys.obtained) {
				cout << lockedText << endl;
			}				
			else if (playerX == 3 && playerY == 2 && !lockedDoors[3] && keys.obtained) {
				lockedDoors[3] = unlockDoor();
				if (lockedDoors[3]) {
					movePlayer(dir);
				}
			}
			else if (playerX == 3 && playerY == 2 && lockedDoors[3]) {
				movePlayer(dir);
			}
			else if (playerX == 3 && playerY == 2 && !lockedDoors[3] && !keys.obtained) {
				cout << lockedText << endl;
			}
		}
		//goes north
		else {
			movePlayer(dir);
		}
	}
	//if south is selected
	else if (choice == 2) {
		dir = SOUTH;
		//checks if going south is out of bounds
		if (playerY+1 > 3) {
			cout << wallText << endl;
		}
		//checks for hard coded walls
		else if ((playerX == 3 && playerY == 0) || (playerX == 0 && playerY == 1) || (playerX == 0 && playerY == 2)) {
			cout << wallText << endl;
		}
		//checks for locked door
		else if ((playerX == 1 && playerY == 0) || (playerX == 2 && playerY == 1) || (playerX == 2 && playerY == 2) || (playerX == 3 && playerY == 1)) {
			if (playerX == 1 && playerY == 0 && !lockedDoors[0] && keys.obtained) {
				lockedDoors[0] = unlockDoor();
				if (lockedDoors[0]) {
					movePlayer(dir);
				}
			}
			else if (playerX == 1 && playerY == 0 && lockedDoors[0]) {
				movePlayer(dir);
			}
			else if (playerX == 1 && playerY == 0 && !lockedDoors[0] && !keys.obtained) {
				cout << lockedText << endl;
			}
			else if (playerX == 2 && playerY == 1 && !lockedDoors[2] && keys.obtained) {
				lockedDoors[2] = unlockDoor();
				if (lockedDoors[2]) {
					movePlayer(dir);
				}
			}
			else if (playerX == 2 && playerY == 1 && lockedDoors[2]) {
				movePlayer(dir);
			}
			else if (playerX == 2 && playerY == 1 && !lockedDoors[2] && !keys.obtained) {
				cout << lockedText << endl;
			}
			else if (playerX == 2 && playerY == 2 && !lockedDoors[1] && keys.obtained) {
				lockedDoors[1] = unlockDoor();
				if (lockedDoors[1]) {
					movePlayer(dir);
				}
			}
			else if (playerX == 2 && playerY == 2 && lockedDoors[1]) {
				movePlayer(dir);
			}
			else if (playerX == 2 && playerY == 2 && !lockedDoors[1] && !keys.obtained) {
				cout << lockedText << endl;
			}
			else if (playerX == 3 && playerY == 1 && !lockedDoors[3] && keys.obtained) {
				lockedDoors[3] = unlockDoor();
				if (lockedDoors[3]) {
					movePlayer(dir);
				}
			}
			else if (playerX == 3 && playerY == 1 && lockedDoors[3]) {
				movePlayer(dir);
			}
			else if (playerX == 3 && playerY == 1 && !lockedDoors[3] && !keys.obtained) {
				cout << lockedText << endl;
			}
		}
		//checks for locked cell door
		else if (playerX == 0 && playerY == 0 && !lockedDoors[7]) {
			if (cellKey.obtained) {
				lockedDoors[7] = unlockDoor();
				if (lockedDoors[7]) {
					movePlayer(dir);
				}
			}
			else {
				cout << lockedText << endl;
			}
		}
		//goes south
		else {
			movePlayer(dir);
		}
	}
	//if west is selected
	else if (choice == 4) {
		dir = WEST;
		//checks if going west is out of bounds
		if (playerX-1 < 0) {
			cout << wallText << endl;
		}
		//checks for hard coded walls
		else if ((playerX == 2 && playerY == 0) || (playerX == 2 && playerY == 2) || (playerX == 3 && playerY == 2)) {
			cout << wallText << endl;
		}
		//checks for locked door
		else if ((playerX == 3 && playerY == 0) || (playerX == 1 && playerY == 3) || (playerX == 3 && playerY == 1)) {
			if (playerX == 3 && playerY == 0 && !lockedDoors[4] && keys.obtained) {
				lockedDoors[4] = unlockDoor();
				if (lockedDoors[4]) {
					movePlayer(dir);
				}
			}
			else if (playerX == 3 && playerY == 0 && lockedDoors[4]) {
				movePlayer(dir);
			}
			else if (playerX == 3 && playerY == 0 && !lockedDoors[4] && !keys.obtained) {
				cout << lockedText << endl;
			}
			else if (playerX == 1 && playerY == 3 && !lockedDoors[5] && keys.obtained) {
				lockedDoors[5] = unlockDoor();
				if (lockedDoors[5]) {
					movePlayer(dir);
				}
			}
			else if (playerX == 1 && playerY == 3 && lockedDoors[5]) {
				movePlayer(dir);
			}
			else if (playerX == 1 && playerY == 3 && !lockedDoors[5] && !keys.obtained) {
				cout << lockedText << endl;
			}
			else if (playerX == 3 && playerY == 1 && !lockedDoors[6] && keys.obtained) {
				lockedDoors[6] = unlockDoor();
				if (lockedDoors[6]) {
					movePlayer(dir);
				}
			}
			else if (playerX == 3 && playerY == 1 && lockedDoors[6]) {
				movePlayer(dir);
			}
			else if (playerX == 3 && playerY == 1 && !lockedDoors[6] && !keys.obtained) {
				cout << lockedText << endl;
			}
		}
		//goes west
		else {
			movePlayer(dir);
		}
	}
	//if east is selected	
	else if (choice == 3) {
		dir = EAST;
		//checks if going east is out of bounds
		if (playerX+1 > 3) {
			cout << wallText << endl;
		}
		//checks for hard coded walls
		else if ((playerX == 1 && playerY == 0) || (playerX == 1 && playerY == 2) || (playerX == 2 && playerY == 2)) {
			cout << wallText << endl;
		}
		//checks for locked doors
		else if ((playerX == 2 && playerY == 0) || (playerX == 0 && playerY == 3) || (playerX == 2 && playerY == 1)) {
			if (playerX == 2 && playerY == 0 && !lockedDoors[4] && keys.obtained) {
				lockedDoors[4] = unlockDoor();
				if (lockedDoors[4]) {
					movePlayer(dir);
				}
			}
			else if (playerX == 2 && playerY == 0 && lockedDoors[4]) {
				movePlayer(dir);
			}
			else if (playerX == 2 && playerY == 0 && !lockedDoors[4] && !keys.obtained) {
				cout << lockedText << endl;
			}
			else if (playerX == 0 && playerY == 3 && !lockedDoors[5] && keys.obtained) {
				lockedDoors[5] = unlockDoor();
				if (lockedDoors[5]) {
					movePlayer(dir);
				}
			}
			else if (playerX == 0 && playerY == 3 && lockedDoors[5]) {
				movePlayer(dir);
			}
			else if (playerX == 0 && playerY == 3 && !lockedDoors[5] && !keys.obtained) {
				cout << lockedText << endl;
			}
			else if (playerX == 2 && playerY == 1 && !lockedDoors[6] && keys.obtained) {
				lockedDoors[6] = unlockDoor();
				if (lockedDoors[6]) {
					movePlayer(dir);
				}
			}
			else if (playerX == 2 && playerY == 1 && lockedDoors[6]) {
				movePlayer(dir);
			}
			else if (playerX == 2 && playerY == 1 && !lockedDoors[6] && !keys.obtained) {
				cout << lockedText << endl;
			}
		}
		//goes east
		else {
			movePlayer(dir);
		}
	}
	//if inspect room is selected
	else if (choice == 5) {
		cout << map[playerX][playerY].Info << endl;

		//output for battle with guard1
		if (playerX == guard1.X && playerY == guard1.Y && guard1.alive) {
			fightEnemy(guard1);
		}

		if (playerX == guard2.X && playerY == guard2.Y && guard2.alive) {
			fightEnemy(guard2);
		}

		//ouput if you are in the room with the fire gem
		if (playerX == fire.X && playerY == fire.Y && !fire.obtained) {
			cout << "# The room seems to be warmer than the other rooms. \n"
				<< "# You notice a glowing gem in the corner of the room. \n" << endl;
			pickUpItem(fire);
		}
		//output if you are in the room with the ice gem
		if (playerX == ice.X && playerY == ice.Y && !ice.obtained) {
			cout << "# The room seems to be colder than the other rooms. \n"
				<< "# You notice a glowing gem in the corner of the room. \n" << endl;
			pickUpItem(ice);
		}
		//output if you are in the room with the earth gem
		if (playerX == earth.X && playerY == earth.Y && !earth.obtained) {
			cout << "# The room seems to be filled with some sort of energy. \n"
				<< "# You notice a glowing gem in the corner of the room. \n" << endl;
			pickUpItem(earth);
		}
		//output if you are in room with air gem
		if (playerX == air.X && playerY == air.Y && !air.obtained) {
			cout << "# The room seems to be filled with a slight breeze. \n"
				<< "# You notice a glowing gem in the corner of the room. \n" << endl;
			pickUpItem(air);
		}
		//output if you are in room with master keys
		if (playerX == keys.X && playerY == keys.Y && !keys.obtained && !guard1.alive) {
			
			cout << "# You find a set of keys on the ground. \n" << endl;
			pickUpItem(keys);
		}
		//output if you are in room with cell key
		if (playerX == cellKey.X && playerY == cellKey.Y && !cellKey.obtained) {
			cout << "# You find a key on the ground that might unlock the cell. \n" << endl;
			pickUpItem(cellKey);
		}
		//output if you are in room with sword
		if (playerX == sword.X && playerY == sword.Y && !sword.obtained) {
			cout << "# You find a sword on the ground that you can use to fight. \n" << endl;
			pickUpItem(sword);
		}
		//output if you are in room with health potion
		if (playerX == healthPotion.X && playerY == healthPotion.Y && !healthPotion.obtained) {
			cout << "# You found a health potion on the ground. \n" << endl;
			pickUpItem(healthPotion);
		}
	}
	else {
		cout << "Invalid option." << endl;
	}
}

//function to drop an item out of your inventory
void Game::dropItem(gameItem &item) {
	item.obtained = false;
	item.X = playerX;
	item.Y = playerY;
	itemCount--;
	cout << item.Name << " dropped." << endl;
}

void Game::pickUpItem(gameItem &item) {
	cout << "1 - Yes" << endl;
	cout << "2 - No" << endl;
	cout << "\n Pick up the " << item.Name << "?: ";
	cin >> choice;

	//add air gem to inventory
	if (choice == 1) {
		inventory.push_back(item);
		item.obtained = true;
		itemCount++;
		cout << "The " << item.Name << " has been added to your inventory \n" << endl;
	}
}

void Game::fightEnemy(gameEnemy &enemy) {
	cout << "# You encounter a guard in the room. \n" << endl;
	int run = 1;
	do {
		srand(time(NULL));
		cout << playerName << ": " << healthPoints << " HP \t \t"
			<< enemy.Name <<": " << enemy.Health << " HP \n" << endl;
		cout << "1 - Fight" << endl;
		cout << "2 - Run" << endl;
		cout << "\n What will you do?: ";
		cin >> choice;

		if (choice == 1 && sword.obtained) {
			attackDamage = (rand() % 6) + 7;
			enemy.Health -= attackDamage;
			cout << "You deal " << attackDamage << " damage. \n" << endl;

			enemy.Damage = (rand() % 6) + 3;
			healthPoints -= enemy.Damage;
			cout << "You take " << enemy.Damage << " damage. \n" << endl;
		}
		else if (choice == 2) {
			run = (rand() % 2) + 1;
			if (run == 1) {
				cout << "You failed to run away! \n" << endl;

				enemy.Damage = (rand() % 6) + 3;
				healthPoints -= enemy.Damage;
				cout << "You take " << enemy.Damage << " damage. \n" << endl;
			}
			else if (run == 2) {
				cout << "You successfully escape \n" << endl;
			}
		}
		else if (!sword.obtained) {
			cout << "You cannot fight without a weapon. \n" << endl;
		}
		else {
			cout << "Invalid option." << endl;
		}

		if (enemy.Health <= 0) {
			cout << enemy.Name << " was defeated. \n" << endl;
			enemy.alive = false;
		}
	} while (enemy.alive && run == 1 && healthPoints > 0);
}

//function to get room desctiptions
void Game::roomDesc() {
	cout << map[playerX][playerY].Name << endl;
	cout << map[playerX][playerY].Desc << endl;
}

//function to initialize enemy data
void Game::initEnemies() {
	//data for guard1
	guard1.Name = "Guard";
	guard1.Health = 50;
	guard1.alive = true;
	guard1.X = 0;
	guard1.Y = 2;

	//data for guard2
	guard2.Name = "Guard";
	guard2.Health = 50;
	guard2.alive = true;
	guard2.X = 2;
	guard2.Y = 0;

}

//function to initialize item data
void Game::initItems() {
	//data for the quest letter
	letter.Name = "Quest Letter";
	letter.Desc = "\t 'Hello " + playerName + ", there is a task that you must complete \n"
		+ "\t to escape this place. You must locate the four gems of \n"
		+ "\t life: the fire gem, the ice gem, the earth gem, and the air gem. You must take \n"
		+ "\t them to the altar located somewhere in this dungeon within two days.'\n";
	letter.obtained = false;

	//data for the master keys
	keys.Name = "Master Keys";
	keys.Desc = "\t A set of keys that seem to unlock any door in the dungeon \n";
	keys.obtained = false;
	keys.X = 0;
	keys.Y = 2;

	//data for the cell key
	cellKey.Name = "Cell Key";
	cellKey.Desc = "\t A key used to unlock the dungeon cell \n";
	cellKey.obtained = false;
	cellKey.X = 1;
	cellKey.Y = 0;

	//data for the sword
	sword.Name = "Sword";
	sword.Desc = "\t A dull broadsword that seems to have a lot of battle experience. \n";
	sword.obtained = false;
	sword.X = 0;
	sword.Y = 1;

	//data for the fire gem
	fire.Name = "Fire Gem";
	fire.Desc = "\t A glowing red gem that is warm to the touch. \n";
	fire.obtained = false;
	fire.X = 2;
	fire.Y = 2;

	//data for the ice gem
	ice.Name = "Ice Gem";
	ice.Desc = "\t A glowing light blue gem that is cool to the touch \n";
	ice.obtained = false;
	ice.X = 3;
	ice.Y = 1;

	//data for the earth gem
	earth.Name = "Earth Gem";
	earth.Desc = "\t A glowing green gem that seems to emit some sort of energy \n";
	earth.obtained = false;
	earth.X = 3;
	earth.Y = 0;

	//data for the air gem
	air.Name = "Air Gem";
	air.Desc = "\t A glowing white gem that seems to emit a small breeze \n";
	air.obtained = false;
	air.X = 0;
	air.Y = 3;

	//data for health potion
	healthPotion.Name = "Health Potion";
	healthPotion.Desc = "\t A potion that will restore up to 40 points of health \n";
	healthPotion.X = 1;
	healthPotion.Y = 2;
}

void Game::initRooms() {
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			map[i][j].X = i;
			map[i][j].Y = j;
			//desc of room 0-0
			if (i == 0 && j == 0) {
				map[i][j].Name = "-- Dungeon Cell --";
				map[i][j].Desc = "# A small room with a tiny barred window looking outside. \n";
				map[i][j].Info = "# There is a door on the southern wall, and there is a hole that \n# you might be able to fit through on the eastern wall. \n";
			}
			else if (i == 1 && j == 0) {
				map[i][j].Name = "-- Dungeon Master's Room --";
				map[i][j].Desc = "# A room where the dungeon master resides. \n";
				map[i][j].Info = "# The room is dark, but seems to be empty. \n# There is a door on the southern wall, and there \n# is a hole on the western wall. \n";
			}
			//desc of room 2-0
			else if (i == 2 && j == 0) {
				map[i][j].Name = "-- Guard Room --";
				map[i][j].Desc = "# A room where guards are posted to guard the treasure room. \n";
				map[i][j].Info = "# There is a door on the southern and eastern walls. \n";
			}
			//desc of room 3-0
			else if (i == 3 && j == 0) {
				map[i][j].Name = "-- Treasure Room --";
				map[i][j].Desc = "# A room with many jewels and gold. \n";
				map[i][j].Info = "# A room where they store their valuables \n# there is a door on the western wall. \n";
			}
			//desc of room 0-1
			else if (i == 0 && j == 1) {
				map[i][j].Name = "-- Armory --";
				map[i][j].Desc = "# A room where weapons are kept. \n";
				map[i][j].Info = "# It's a very small room with broken weapons around. \n#  There is a door on the southern , northern, and the western walls. \n";
			}
			//desc of room 1-1
			else if (i == 1 && j == 1) {
				map[i][j].Name = "-- Dining Hall --";
				map[i][j].Desc = "# Where the guards eat. \n";
				map[i][j].Info = "# The room is filled with rows of tables and chairs. \n# There is a door on each wall. \n";
			}
			//desc of room 2-1
			else if (i == 2 && j == 1) {
				map[i][j].Name = "-- Pantry --";
				map[i][j].Desc = "# A room where food is stored. \n";
				map[i][j].Info = "# There is a door on each wall. \n";
			}
			//desc of room 3-1
			else if (i == 3 && j == 1) {
				map[i][j].Name = "-- Map Room --";
				map[i][j].Desc = "# A room with a map of the dungeon. \n";
				map[i][j].Info = "# The map shows the altar room is in the southwest corner. \n# There is a door on the western and southern walls. \n";
			}
			//desc of room 0-2
			else if (i == 0 && j == 2) {
				map[i][j].Name = "-- Barracks --";
				map[i][j].Desc = "# A room where the guards stay. \n";
				map[i][j].Info = "# The room is lined with beds along the walls. \n# There is a door on the eastern wall. \n";
			}
			//desc of room 1-2
			else if (i == 1 && j == 2) {
				map[i][j].Name = "-- Short Hall --";
				map[i][j].Desc = "# A short hallway. \n";
				map[i][j].Info = "# The hall is lit by candles on the walls. \n# A hall with a door on the north and east walls. \n";
			}
			//desc of room 2-2
			else if (i == 2 && j == 2) {
				map[i][j].Name = "-- Kitchen --";
				map[i][j].Desc = "# A kitchen that seems to be in poor shape. \n";
				map[i][j].Info = "# The room is covered in old food and broken dishware. \n# There is a door on northern and southern walls of the room. \n";
			}
			//desc of room 3-2
			else if (i == 3 && j == 2) {
				map[i][j].Name = "-- Long Hall --";
				map[i][j].Desc = "# A long hallway. \n";
				map[i][j].Info = "# The hall is damp and dark. \n# There is a door on the northern and southern walls. \n";
			}
			//desc of room 0-3
			else if (i == 0 && j == 3) {
				map[i][j].Name = "-- Storage Room --";
				map[i][j].Desc = "A dark and windowless room. \n";
				map[i][j].Info = "# The room is filled with crates and barrels. \n# There is a door on the eastern wall. \n";
			}
			//desc of room 1-3
			else if (i == 1 && j == 3) {
				map[i][j].Name = "-- Torture Room --";
				map[i][j].Desc = "# A room to interrogate prisoners. \n";
				map[i][j].Info = "# There are many torture devices scattered around. \n# There is a door on the eastern, western, and northern walls. \n";
			}
			//desc of room 2-3
			else if (i == 2 && j == 3) {
				map[i][j].Name = "-- Long Hall --";
				map[i][j].Desc = "# A long hallway. \n";;
				map[i][j].Info = "# The hall is damp and dark. \n# There is a door on the eastern, western, and northern walls. \n";
			}
			//desc of room 3-3
			else if (i == 3 && j == 3) {
				map[i][j].Name = "-- Altar Room --";
				map[i][j].Desc = "# A room with an odd altar in the center. \n";
				map[i][j].Info = "# You can drop the crystals in this room to activate the altar. \n#There is a door on the northern and western walls. \n";
			}
		}
	}
}

//function to check win
bool Game::checkWin() {
	if ((fire.X == 3 && fire.Y == 3) &&
		(ice.X == 3 && ice.Y == 3) &&
		(earth.X == 3 && earth.Y == 3) &&
		(air.X == 3 && air.Y == 3)) {
		return true;
	}
	else {
		return false;
	}
}

bool Game::unlockDoor() {
	cout << "This door is locked. \n" << endl;
	cout << "1 - Yes" << endl;
	cout << "2 - No" << endl;
	cout << "Unlock the door?: ";
	cin >> choice;

	if (choice == 1) {
		return true;
	}
	else if (choice == 2) {
		return false;
	}
	else {
		cout << "Invalid option." << endl;
		return false;
	}
}

//main function
int main() {
	//player name entry
	cout << "Enter your name: ";
	cin >> playerName;
	system("pause");
	system("CLS");
	game.initItems();
	game.initEnemies();
	game.initRooms();
	//announce setting
	cout << "# You awaken in a small cell that seems to be a dungeon of some sort. \n"
		<< "# Next to you, you find a letter. The letter reads: \n" << endl;
	//print letter
	cout << letter.Desc << endl;
	letter.obtained = true;
	inventory.push_back(letter);
	itemCount++;

	cout << "# The letter has been added to your inventory. \n" << endl;
	system("pause");
	//start game loop
	do {
		system("CLS");
		won = game.checkWin();
		cout << "Day " << day << "\t HP: " << healthPoints << endl;
		if (day < 3 && !won && healthPoints > 0) {
			game.roomDesc();

			if (moves < 20) {
				cout << "1 - Go North" << endl;
				cout << "2 - Go South" << endl;
				cout << "3 - Go East" << endl;
				cout << "4 - Go West" << endl;
				cout << "5 - Inspect Room" << endl;
				cout << "6 - Inventory" << endl;

				cout << "\n What will you do?: ";
				cin >> choice;
				game.checkChoice();
			}
			else {
				cout << "You have made the maximum number of moves for one day and must rest." << endl;
				moves = 0;
				day++;
			}
			system("pause");
		}
		else if (won) {
			cout << "Congratulations! You won!" << endl;
			endGame = true;
		}
		else if (healthPoints <= 0) {
			healthPoints = 0;
			cout << "You died! GAME OVER." << endl;
			endGame = true;
		}
		else {
			cout << "You didn't complete the task in time! GAME OVER." << endl;
			endGame = true;
		}		
	} while (!endGame);
}