#include <windows.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "consolestuff.h"
#include "matrixmath.h"

#define COUNT 500

void clearlines();
void fuck();

class gamepiece* tetri;

clock_t timer; // 

/*game control variables*/
int cycles = 1; //cycles increased every peice landed
bool gameIsOn = true; //game runs as long as this is true.
int tickTime = 200; //interval between game ticks. decays as cycles increases
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
	char board[boardsize] = {
							0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
							0, 5, 6, 7, 8, 1, 2, 3, 4, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 0, 5, 6, 7, 8, 1, 2, 3, 4, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29
	};

	char scoreboard[22] = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
							 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };

	char scoretxt[11] = {'s', 'c', 'o', 'r', 'e', ' ', '0', '0', '0', '0', '0'};

	int score = 0;

	int width = 74;
	int ascii = 219;
	int attrib = 27;

	//take in int and return ascii value to write
	char toAscii(int val) {
		char sascii = '#';
		
		switch (val) {
			case 0:
				sascii = '0';
				break;
			case 1:
				sascii = '1';
				break;
			case 2:
				sascii = '2';
				break;
			case 3:
				sascii = '3';
				break;
			case 4:
				sascii = '4';
				break;
			case 5:
				sascii = '5';
				break;
			case 6:
				sascii = '6';
				break;
			case 7:
				sascii = '7';
				break;
			case 8:
				sascii = '8';
				break;
			case 9:
				sascii = '9';
				break;
		}
		return sascii;
	}

	void draw() {
		
		//score updater
		int result = score;
		if ((result / 100) > 0) { //score has 3 digits
			int hundreds = (result / 100); // number of hundreds
			
			result = result - (hundreds * 100); // subtract hundreds from results
			scoretxt[8] = toAscii(hundreds);
		}
		if ((result / 10) > 0) { //score has tenths place
			int tenths = (result / 10); //number of tenths
			scoretxt[9] = toAscii(tenths);
			result = result - (tenths * 10);
		}
		if ((result / 1) > 0) { //score has one place
			int ones = (result / 1); //number of ones
			scoretxt[10] = toAscii(ones);
		}
		
		


		//draw scoreboard
		for (int x = 0; x < 11; x++) {

			int c = scoreboard[x];
			int d = scoreboard[x + 11];

			buffy[c + d * 22].Char.AsciiChar = scoretxt[x];
			buffy[c + d * 22].Attributes = attrib;

		}

		//draw gameboard
		for (int x = 0; x < width; x++) {

			int a = board[x];
			int b = board[x + width];


			buffy[a + b * WIDTH].Char.AsciiChar = ascii;
			buffy[a + b * WIDTH].Attributes = attrib;

		}

	}
	
};
gameboard gboard; 

//Game Object shape
class gamepiece {

public:

	int object[objectsize]; //array of object points
	int draw[objectsize]; //object translated to position if collision not detected
	int collide[objectsize]; //object translated into position to check for collisions
	int positionX = 7;
	int positionY = 1;
	int objectWidth;
	int ascii = 219;
	int attrib = 11;
	int accelY = 1;
	int accelX = 0;
	int accelRotate = 0;
	bool isActive = TRUE;
	bool drop = false;
	bool down = false;
	int status = 1; //1 initialized, 2 falling, 3 collided
	int location;

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
		else if (i == 3) { // SET OBJECT TO I SHAPE

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
		if (tick || drop || down) {
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

			else if (!isCollided()) { // if no collision, set positionY with accelY added. 
				positionY = positionY + accelY;
			}
			down = false; 
		}



		//if rotated, move and check for collision. if non write to draw[]. 
		if (accelRotate != 0) {

			for (int x = 0; x < objectWidth; x++) {
				//Copy object into collide X and Y
				collide[x] = object[x];
				collide[x + objectWidth] = object[x + objectWidth];
			}

			if (accelRotate > 0) { //rotate right
				//pass matrix multiplier a pointer to array of object. pass object width param
				matmul(collide, objectWidth);
			}

			if (accelRotate < 0) { //rotate left
				//pass matrix multiplier a pointer to array of object. pass object width param
				matmul(collide, objectWidth, -1);
			}

			for (int x = 0; x < objectWidth; x++) {
				//Copy object into collide X and Y and add x and Y
				collide[x] = collide[x] + positionX;
				collide[x + objectWidth] = collide[x + objectWidth] + positionY;
			}

			//if collision detected, 
			if (isCollided()) {
				//std::cout << "rotation collision detected" << std::endl;
			}

			else if (!isCollided()) { // if no collision, set positionY with accelY added. 
				for (int x = 0; x < objectWidth; x++) {
					draw[x] = collide[x];
					draw[x + objectWidth] = collide[x + objectWidth];
				}
			
				if (accelRotate > 0) { //rotate right
					matmul(object, objectWidth);
				}

				if (accelRotate < 0) { //rotate left
					matmul(object, objectWidth, -1);
				}
			
			}
			accelRotate = 0;

		}

		//positionX = positionX + accelX;			

		//if X move, move and check for collision. if non write to draw[]
		if (accelX != 0) {
			for (int x = 0; x < objectWidth; x++) {
				collide[x] = object[x] + positionX + accelX;
				collide[x + objectWidth] = object[x + objectWidth] + positionY;
			}

			if (isCollided()) {
				//std::cout << "collision on X: " << accelX << " ." << positionX << std::endl;
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


		
		if (!accelRotate) {
			//move to position
			for (int x = 0; x < objectWidth; x++) {
				collide[x] = object[x] + positionX;
				collide[x + objectWidth] = object[x + objectWidth] + positionY;
			}

			for (int x = 0; x < objectWidth; x++) {
				draw[x] = object[x] + positionX;
				draw[x + objectWidth] = object[x + objectWidth] + positionY;
			}
		}

		update();

	}
	/*
	void rotate() { //function to rotate object array points
		//pass matrix multiplier a pointer to array of object. pass object width param
		matmul(object, objectWidth);
	}

	void lrotate() { //function to rotate object array points
	//pass matrix multiplier a pointer to array of object. pass object width param
		matmul(object, objectWidth, -1);
	}

	*/
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
			if (location > (WIDTH * 29 - 1)) { //if cell is from pieces dumped on the border when line removed..
				buffy[location].Attributes = 27; //color match the border. There is prob a better way to do this lol. 
			}
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

	/*function takes in a row number. checks draw[] for matches in that row and deletes them. 
	For any point that is ABOVE the row in question. move Y point down by 1 to collapse down
	*/
	void deleteRow(int row) {
	
		for (int d = 0; d < objectWidth; d++) { //for elements in draw
			if (draw[d + objectWidth] == row) { //if point Y coord is = to row point
				//draw[d + objectWidth] = HEIGHT + 10; // 
				draw[d + objectWidth] = HEIGHT - 1;
				draw[d] = 0;
			}
			else if (draw[d + objectWidth] < row) {
				draw[d + objectWidth]++; //drow point by 1
			}
		}	
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
	int counter = 0; //counter to see if row matches is 100%
	
	for (int row = 0; row < HEIGHT; row++) { //for each row
		for (int col = 1; col < 16; col++) { //for each column in row starting at 2 ending at 15
			for (int obj = 0; obj < COUNT; obj++) { //for each object in object array
				if (tetri[obj].status == 3) {
					for (int i = 0; i < tetri[obj].objectWidth; i++) { // for each point in the object's draw array[]
						if ((tetri[obj].draw[i] == col) && (tetri[obj].draw[i + tetri[obj].objectWidth] == row)) { //if object draw[] contains match for current row and column.
							counter++; // increase counter by 1. 
						}
					}
				}
			}
		}

		//on current row
		if (counter == 14) { // if counter is greater than row elements
			gboard.score++; //increase score in gameboard
			for (int obj = 0; obj < COUNT; obj++) { //for every object in array
				if (tetri[obj].status == 3) { //if object status is landed
					tetri[obj].deleteRow(row); //run function to delete rows and move draw elements down by 1 on all objects.
				}
			}
		}
		counter = 0; // set counter back to 0 before checking next row
	}
}



int main() {

	//create gameboard object

	//Variables for the gametick function. 
	//Used to determine if loop cycle allows game object movement. (falling speed etc)
	timer = clock();
	bool tick = FALSE;
	srand(time(NULL));

	//Setup console screen and handles
	setupscreen();

	tetri = new gamepiece[500];



	// set 100 diff pieces in the object array
	for (int i = 0; i < 500; i++) {

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
			//draw gameboard
			gboard.draw();

			tick = gametick(timer); // see if current cycle is a gametick

			char key = checkInput();
			if (key == 97) {
				tetri[x].accelX = -1;
			}
			else if (key == 100) {
				tetri[x].accelX = 1;
			}
			else if ((key == 32) | (key == 46)) {
				tetri[x].accelRotate = -1;
			}
			else if (key == 44) {
				tetri[x].accelRotate = 1;
			}
			else if (key == 119) {
				tetri[x].drop = true;
			}
			else if (key == 115) {
				tetri[x].down = true;
			}
			else if (key == 27) {
				gameIsOn = false;
				break;
			}

			tetri[x].move(tick);

			drawLanded();
			clearlines();
			draw();
		}
		x++;
	}
	delete[] tetri;
	return 0;
}


