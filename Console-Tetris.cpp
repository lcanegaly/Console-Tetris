#include <windows.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "consolestuff.h"
#include "matrixmath.h"

#define COUNT 500


class gamepiece* tetri;

clock_t timer; // 

/*game control variables*/
int cycles = 1; //cycles increased every peice landed
bool gameIsOn = true; //game runs as long as this is true.
int tickTime = 400; //interval between game ticks. decays as cycles increases
int decay = 10; //amount to decay per 10 cycles
int decayFreq = 10; //the number of cycles per decay


//function to determine if time elapased is enough to move game objects
//used to determine fall speed of tetris peices. 
bool gametick(clock_t& timer) {
	bool tick;
	int x = (cycles % decayFreq); //cycles currently on modulo decayfreq, every 10 times decrease ticktime by decay time.
	if (x == 0) { // every 10 cycles

		tickTime = tickTime - decay; //decay 10 from tickTimer
		cycles = 1;

	}
	//check if tick TRUE (last cycle was a tick cycle). Set false and reset
	if (clock() - timer > tickTime) {
		tick = TRUE;
		timer = clock();

	}
	else {
		tick = FALSE;
	}
	return tick;
}


class gameboard {

public:
	int board[boardsize] = {
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
							0, 5, 6, 7, 8, 1, 2, 3, 4, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 0, 5, 6, 7, 8, 1, 2, 3, 4, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29
	};


	int width = 74;
	int ascii = 219;
	int attrib = 27;

	void draw() {

		for (int x = 0; x < width; x++) {

			int a = board[x];
			int b = board[x + width];

			buffy[a + b * WIDTH].Char.AsciiChar = ascii;
			buffy[a + b * WIDTH].Attributes = attrib;

		}

	}
	
}; gameboard gboard;

//Game Object shape
class gamepiece {

public:

	int object[objectsize]; //array of object points
	int draw[objectsize]; //object translated to position if collision not detected
	int collide[objectsize]; //object translated into position to check for collisions
	int positionX = 2;
	int positionY = 1;
	int objectWidth;
	int ascii = 219;
	int attrib = 11;
	int accelY = 1;
	int accelX = 0;
	bool isActive = TRUE;
	int status = 1; //1 initialized, 2 falling, 3 collided
	int location;
	int rowclear = 0;

	void setType(int i) {

		if (i == 0) { // SET OBJECT TO T SHAPE
			objectWidth = 4;
			attrib = 12;
			//T shape
			int tShape[] = { 0, 1, 0, 0,
							 0, 0, 1,-1 };

			for (int x = 0; x < 8; x++) {
				object[x] = tShape[x];
			}

		}
		else if (i == 1) { // SET OBJECT TO L SHAPE

			objectWidth = 4;
			attrib = 13;
			int lShape[] = { 0, 0, 0, 1,
							 1, 0,-1,-1 };

			for (int x = 0; x < 8; x++) {
				object[x] = lShape[x];
			}

		}
		else if (i == 2) { // SET OBJECT TO REVERSE L SHAPE

			objectWidth = 4;
			attrib = 14;
			int rlShape[] = { 0, 0, 0, -1,
							 1, 0,-1,-1 };

			for (int x = 0; x < 8; x++) {
				object[x] = rlShape[x];
			}

		}
		else if (i == 3) { // SET OBJECT TO REVERSE I SHAPE

			objectWidth = 4;
			attrib = 15;
			int iShape[] = { 0, 0, 0, 0,
							 1, 0,-1,-2 };

			for (int x = 0; x < 8; x++) {
				object[x] = iShape[x];
			}

		}
		else if (i == 4) { // SET OBJECT TO Z SHAPE

			objectWidth = 4;
			attrib = 11;
			int zShape[] = { 0, 0, 1, 1,
							 1, 0, 0,-1 };

			for (int x = 0; x < 8; x++) {
				object[x] = zShape[x];
			}

		}
		else if (i == 5) { // SET OBJECT TO REVERSE Z SHAPE

			objectWidth = 4;
			attrib = 10;
			int rzShape[] = { 0, 0,-1,-1,
							  1, 0, 0,-1 };

			for (int x = 0; x < 8; x++) {
				object[x] = rzShape[x];
			}

		}
		else if (i == 6) { // SET OBJECT TO SQUARE SHAPE

			objectWidth = 4;
			attrib = 18;
			int sqShape[] = { 0, 0, 1, 1,
							  1, 0, 0, 1 };

			for (int x = 0; x < 8; x++) {
				object[x] = sqShape[x];
			}
		}


	}

	void move(bool tick) {



		//if cycle is a gametick, move tick Y update first and check for collision
		if (tick) {
			for (int x = 0; x < objectWidth; x++) {
				//Copy object into collide X and Y and add x and Y + accelY positions
				collide[x] = object[x] + positionX;
				collide[x + objectWidth] = object[x + objectWidth] + positionY + accelY;
			}

			//if collision detected, 
			if (isCollided()) {
				accelY = 0; //no longer falling, set to 0 and do not add accel to Y position. 
				status = 3; //set status to collided
			}
			/*
			if (positionY > HEIGHT - 3) { // if at bottom of screen, set to collided
				status = 3;
			}
			*/
			else if (!isCollided()) { // if no collision, set positionY with accelY added. 
				positionY = positionY + accelY;
			}
		}



		//if rotated, move and check for collision. if non write to draw[]. 


		//positionX = positionX + accelX;			

		//if X move, move and check for collision. if non write to draw[]
		if (accelX != 0) {
			for (int x = 0; x < objectWidth; x++) {
				collide[x] = object[x] + positionX + accelX;
				collide[x + objectWidth] = object[x + objectWidth] + positionY;
			}

			if (isCollided()) {
				std::cout << "collision on X: " << accelX << " ." << positionX << std::endl;
				accelX = 0;
			}

			if (positionX > 20 || positionX < 1) { // if at left or right of screen, set to collided
				accelX = 0;
			}

			else if (!isCollided()) {
				positionX += accelX;
			}

			accelX = 0; // Reset acceleration for X movement.
		}

		for (int x = 0; x < objectWidth; x++) {
			collide[x] = object[x] + positionX;
			collide[x + objectWidth] = object[x + objectWidth] + positionY;
		}

		for (int x = 0; x < objectWidth; x++) {
			draw[x] = object[x] + positionX;
			draw[x + objectWidth] = object[x + objectWidth] + positionY;
		}

		update();

	}

	void rotate() { //function to rotate object array points
		//pass matrix multiplier a pointer to array of object. pass object width param
		matmul(object, objectWidth);
	}

	void lrotate() { //function to rotate object array points
	//pass matrix multiplier a pointer to array of object. pass object width param
		matmul(object, objectWidth, -1);
	}


	// draw array push into buffer. 
	void update() {

		for (int x = 0; x < objectWidth; x++) {
			/*
			int a = draw[x] = object[x] + positionX;
			int b = draw[x + objectWidth] = object[x + objectWidth] + positionY;
			*/

			int a = draw[x];
			int b = draw[x + objectWidth];

			location = a + b * WIDTH;

			buffy[location].Char.AsciiChar = ascii;
			buffy[location].Attributes = attrib;
		}
	}


	bool isCollided() {

		//loop to check current object against gameboard border
		for (int a = 0; a < objectWidth; a++) { // for each point in object array's width	
			for (int c = 0; c < gboard.width; c++) { // for each point in gboard board width
				if ((collide[a] == gboard.board[c]) && (collide[a + objectWidth] == gboard.board[c + gboard.width])) {
					return TRUE;
				}
			}
		}
		


		//Loop to check current object against all other landed objects
		for (int obj = 0; obj < 100; obj++) { // for each object in the object array
			if (tetri[obj].status == 3) { // if object status is collided (landed)
				for (int x = 0; x < objectWidth; x++) { //runs 7 times to iterate through x,ys of current object collide[]
					for (int i = 0; i < objectWidth; i++) { //for each current object collide[] x,y.. check each obj.collide[] for matches (collides)
						if ((collide[x] == tetri[obj].draw[i]) && (collide[x + objectWidth] == tetri[obj].draw[i + tetri[obj].objectWidth])) { //if X,Y matches, collision detectec
							return TRUE; //true, collision
						}
					}
				}
			}

		}
		return false; // false, no collision
	}

};
 




void drawLanded() {
	for (int obj = 0; obj < 100; obj++) { // for each object in the object array
		if (tetri[obj].status == 3) { // if object status is collided (landed)

			tetri[obj].update(); // write object draw array to buffer

		}

	}

}


void clearlines() {

	/*
	for each line of current object that has just landed.

	check each points row
		for every object
			check landed object draw array[] for a match to current x/y in base loop. 
				if a match is found. increment match counter ++. 
				add object index to match[match counter]
	if match counter == 14, 
		row match acheived.
		for every 14 points in match[]
			tetri[match[x]].deleteRow(row);
	else
		match counter = 0 reset match counter
		for each 14 points in match[]
			match[x] = 0; 	
	*/


}

int main() {

	//create gameboard object
	gameboard gboard;

	//Variables for the gametick function. 
	//Used to determine if loop cycle allows game object movement. (falling speed etc)
	timer = clock();
	bool tick = FALSE;
	srand(time(NULL));

	//Setup console screen and handles
	setupscreen();

	tetri = new gamepiece[500];



	// set 100 diff pieces in the object array
	for (int i = 0; i < 100; i++) {

		int type = rand() % 7;
		tetri[i].setType(type);

	}



	int x = 0;
	//Draw Loop
	while (gameIsOn) {
		if (x > 498) { gameIsOn = 0; }
		cycles = cycles + 1; //increases every time new piece drops
		tetri[x].status = 2; // update status of current object to falling. 

		while (tetri[x].status == 2) { //while object status is falling.

			//draw background
			background();
			gboard.draw();


			tick = gametick(timer); // see if current cycle is a gametick

			char key = checkInput();
			if (key == 97) {
				tetri[x].accelX = -1;
			}
			else if (key == 115) {
				tetri[x].accelX = 1;
			}
			else if ((key == 32) | (key == 46)) {
				tetri[x].lrotate();
			}
			else if (key == 44) {
				tetri[x].rotate();
			}
			else if (key == 27) {
				gameIsOn = false;
			}

			tetri[x].move(tick);

			drawLanded();
			clearlines();

			draw();
			//Sleep(10);

		}
		x++;
	}

	delete[] tetri;

	return 0;



}