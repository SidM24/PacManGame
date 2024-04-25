#ifndef COURSEWORK_PACMAN_H
#define COURSEWORK_PACMAN_H

extern unsigned int pac_tex_mouth_closed;
extern unsigned int pac_tex_mouth_midopen;
extern unsigned int pac_tex_mouth_open;
extern int score;
extern int pills;
extern int gameTick;
extern bool frighten;
extern int frighten_time;


int pac_tick = 0;
bool begin_temp = true;


//direction 
typedef enum{LEFT,RIGHT,UP,DOWN,NONE} direction;


class Pacman {

public:
	float pacmanX;
	float pacmanY;
	direction currentDirection;
	direction directionStore;
	float angle;

	//Constructor to initialize the pacman 
	Pacman(){
		pacmanX = 13.5f;
		pacmanY = 7.0f;
		currentDirection = LEFT;
		directionStore = LEFT;
		angle = 0.0f;
	}

	void checkTile() {
		switch (getTile(pacmanX,pacmanY)){
		case o:
			//current tile has small pill
			maze[getPacmanX()][getPacmanY()] = e;
			score += 10;
			pills--;
			break;
		case O:
			//current tile has Big pill
			//Setting the ghost state to frightened
			maze[getPacmanX()] [getPacmanY()] = E;
			score += 50;
			pills--;
			break;
		case P:
			//The pacman has entered reached to the portal
			if (getPacmanX() >= 26.5)
				pacmanX = 1.0f;
			else
				pacmanX = 26.5f;
			break;
		}
	}

	//Function to return the current x position of pacman
	int getPacmanX() {
		return (int)floor(pacmanX);
	}

	//Function to return the current y position of pacman
	int getPacmanY() {
		return (int)floor(pacmanY);
	}

	//Function to get the current tile on which the pacman is:
	tile getTile(float x,float y) {
		return maze[(int)floor(x)][(int)floor(y)];
	}

	tile getFollowingTile(direction dir) {
		switch (dir) {
		case LEFT:
			return getTile(getPacmanX() - 1, getPacmanY());
		case RIGHT:
			return getTile(getPacmanX() + 1, getPacmanY());
		case UP:
			return getTile(getPacmanX(), getPacmanY() + 1);
		case DOWN:
			return getTile(getPacmanX(), getPacmanY() - 1);
		default:
			return getTile(getPacmanX(), getPacmanY());
		}
	}

	//Function to draw the pacman on the screen
	//The pacman has 3 different states, mouth wide open,mid_open and closed, so according to game tick we can 
	//display the 3 different states alternatively.
	void draw() {
		glPushMatrix();
		
		unsigned int current_pacman_texture;
		int tick_remainder=pac_tick%300;

		if (gameTick <= 3850) {
			//Game is in BEGIN state so we will display the mid-open texture
			current_pacman_texture = pac_tex_mouth_closed;
		}
		else {
			if (tick_remainder < 100)
				current_pacman_texture = pac_tex_mouth_open;
			else if (tick_remainder < 200)
				current_pacman_texture = pac_tex_mouth_midopen;
			else
				current_pacman_texture = pac_tex_mouth_closed;
		}
			

		//Setting the angle of the sprite correct according to the direction of motion of the pacman
		switch (currentDirection) {
		case LEFT:
			angle = 0.0f;
			break;
		case RIGHT:
			angle = 180.0f;
			break;
		case UP:
			angle = 270.0f;
			break;
		case DOWN:
			angle = 90.0f;
			break;
		}

		translateBottomLeft();
		translateToMazeCoords(pacmanX, pacmanY);
		glTranslatef(-3.0f, -3.0f, 0.0f);
		//drawing the pacman 
		drawTexture(current_pacman_texture, 14, 14, angle);
		glPopMatrix();

		pac_tick++;
	}

	//Checking if the pacman is at the centre of the current tile
	bool isAtCentre() {
		return (int)(pacmanX*10.0f)%10 == 0 && (int)(pacmanY*10.0f)%10==0;
	}

	void setDirectionStore(direction PressedKey) {
		directionStore = PressedKey;
	}

	//Function to check if the next tile in the direction in which the pacman is moving is a wall/gate
	bool isWall(direction dir){
		if (getFollowingTile(dir) == W || getFollowingTile(dir) == G)
			return true;
		else
			return false;
	}

	void move() {
		//we will change the direction of pacman only if it is at the centre of the tile
		//we do this as we have to keep the position relative to the maze matrix to edit any changes in the maze
		if (isAtCentre()) {
			if (!isWall(directionStore))
				currentDirection = directionStore;
			else if(isWall(currentDirection))
				currentDirection = NONE;
		}

		//Initially we have set the pacman position to 13.5f and 7.0f which is not centre so at the starting
		//we have to move the pacman forcefully
		if (!isAtCentre() && (directionStore != LEFT || directionStore != RIGHT) && begin_temp && !isWall(directionStore)) {
			currentDirection = LEFT;
			begin_temp = false;
		}

		switch (currentDirection) {
		case LEFT:
			pacmanX -= 0.012f;
			pacmanY = round(pacmanY);
			break;
		case RIGHT:
			pacmanX += 0.012f;
			pacmanY = round(pacmanY);
			break;
		case UP:
			pacmanX = round(pacmanX);
			pacmanY += 0.012f;
			break;
		case DOWN:
			pacmanX = round(pacmanX);
			pacmanY -= 0.012f;
			break;
		default:
			if (!begin_temp) { // Do not round pacman if it is the very start as he begin between two tiles
				pacmanX = round(pacmanX);
				pacmanY = round(pacmanY);
			}
		}

	}
};

#endif