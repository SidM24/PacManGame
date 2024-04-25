#ifndef COURSEWORK_GHOST_H
#define COURSEWORK_GHOST_H

//Defining the different types of ghosts
typedef enum{RED,BLUE,PINK,YELLOW} colour;
typedef enum { SCATTER, CHASE, FRIGHTEN, LEAVE, DEATH, PEN } ghostStates;


extern unsigned int red_ghost_tex;
extern unsigned int blue_ghost_tex;
extern unsigned int pink_ghost_tex;
extern unsigned int yellow_ghost_tex;
extern unsigned int frightend_ghost_tex;
extern unsigned int semi_frighten_ghost_tex;


direction tempdir = UP;

extern Pacman pacman;
extern bool frighten;
extern int pills;
extern int gameTick;

class Ghost {
public:
	float ghostX;
	float ghostY;
	colour ghostColour;
	direction ghostCurrentDirection;
	direction ghostDirectionStore;
	//Ghost Speed
	float ghostSpeed = 0.0f;
	ghostStates ghostState;
	bool eaten;
	int eaten_time = 0;

	Ghost(float x,float y,colour ghost_colour) {
		ghostX = x;
		ghostY = y;
		ghostColour = ghost_colour;
		ghostCurrentDirection = LEFT;
		ghostDirectionStore = LEFT;
		eaten = false;
		eaten_time = 0;
		switch (ghostColour){
		case RED:
			ghostState = SCATTER;
			ghostSpeed = 0.003f;
			break;
		case PINK:
			ghostState = PEN;
			ghostSpeed = 0.007f;
			break;
		case BLUE:
			ghostState = PEN;
			ghostSpeed = 0.005f;
			break;
		case YELLOW:
			ghostState = PEN;
			ghostSpeed = 0.0085f;
			break;
		}
	}

	void draw() {
		glPushMatrix();

		//Setting the ghost Texture
		unsigned int ghost_tex;

		//Checking the color of the ghost
		if (ghostState==FRIGHTEN) {
			if(gameTick-frighten_time<=3500)
				ghost_tex = frightend_ghost_tex;
			else {
				//we will alternate the textures as follows
				int temp = gameTick % 200;
				if (temp < 100)
					ghost_tex = semi_frighten_ghost_tex;
				else
					ghost_tex = frightend_ghost_tex;
			}
		}
		else {
			switch (ghostColour) {
			case RED:
				ghost_tex = red_ghost_tex;
				break;
			case BLUE:
				ghost_tex = blue_ghost_tex;
				break;
			case PINK:
				ghost_tex = pink_ghost_tex;
				break;
			case YELLOW:
				ghost_tex = yellow_ghost_tex;
				break;
			}
		}
		//Setting the eyes to the current direction in which ghost is going
		unsigned int ghost_eyes_tex;
		switch (ghostCurrentDirection){
		case UP:
			ghost_eyes_tex = eyes_up;
			break;
		case DOWN:
			ghost_eyes_tex = eyes_down;
			break;
		case LEFT:
			ghost_eyes_tex = eyes_left;
			break;
		case RIGHT:
			ghost_eyes_tex = eyes_right;
			break;
		}

		translateBottomLeft();
		translateToMazeCoords(ghostX, ghostY);
		
		glTranslatef(-3.0f, -3.0f, 0.0f);

		drawTexture(ghost_tex, 14, 14, 0);
		drawTexture(ghost_eyes_tex, 14, 14, 0);
		glPopMatrix();
	}

	int getGhostX() {
		return (int)floor(ghostX);
	}
	
	int getGhostY() {
		return (int)floor(ghostY);
	}

	bool isAtCentre() {
		return (int)(ghostY * 10.0f) % 10 == 0 && (int)(ghostX * 10.0f) % 10 == 0;
	}

	tile getTile(float x, float y) {
		return maze[(int)floor(x)][(int)floor(y)];
	}

	tile getFollowingTile(direction dir) {
		switch (dir) {
		case LEFT:
			return getTile(getGhostX() - 1, getGhostY());
		case UP:
			return getTile(getGhostX(), getGhostY() + 1);
		case RIGHT:
			return getTile(getGhostX() + 1, getGhostY());
		case DOWN:
			return getTile(getGhostX(), getGhostY() - 1);
		default:
			return getTile(getGhostX(), getGhostY());
		}
	}

	direction setGhostNextDirection(int pacmanX,int pacmanY) {
		//setting the next ghost move according the the position of pacman
		//Finding the horizontal and vertical distance between ghost and pacman
		int horizontal_distance = pacmanX - getGhostX();
		int vertical_distance =  pacmanY - getGhostY();

		if (abs(horizontal_distance) > abs(vertical_distance)) {
			// Move horizontally
			if (horizontal_distance > 0) {
				return RIGHT;
			}
			else if (horizontal_distance < 0) {
				return LEFT;
			}
		}
		else {
			// Move vertically
			if (vertical_distance > 0) {
				return UP;
			}
			else if (vertical_distance < 0) {
				return DOWN;
			}
		}

		//cout << "H:" << horizontal_distance << "V:" << vertical_distance <<"D:"<<ghostDirectionStore << endl;
	}

	bool isWall(direction dir) {
		if (getFollowingTile(dir) == W || getFollowingTile(dir) == G)
			return true;
		else
			return false;
	}

	direction nextMoveifStuck(direction currentDir) {
		switch (currentDir) {
		case LEFT: 
			if (!isWall(UP))
				return UP;
			else if (!isWall(DOWN))
				return DOWN;
			else
				return RIGHT;
		case RIGHT:
			if (!isWall(UP)) 
				return UP; 
			else if (!isWall(DOWN))
				return DOWN; 
			else
				return LEFT;  
		case UP:
			if (!isWall(LEFT))  
				return LEFT; 
			else if (!isWall(RIGHT))  
				return RIGHT;  
			else
				return DOWN;  
		case DOWN:
			if (!isWall(LEFT))
				return LEFT;
			else if (!isWall(RIGHT))
				return RIGHT;
			else
				return UP;
		}
	}

	//Common function to get the next movement direction for the particular ghost
	void ghostMovement(Ghost ghost) {
		//Setting the next direction of the ghost
		ghostDirectionStore = setGhostNextDirection(pacman.getPacmanX(), pacman.getPacmanY());
		if (ghostState == FRIGHTEN) {
			switch (ghostDirectionStore) {
			case UP:
				ghostDirectionStore = DOWN;
				break;
			case DOWN:
				ghostDirectionStore = UP;
				break;
			case LEFT:
				ghostDirectionStore = RIGHT;
				break;
			case RIGHT:
				ghostDirectionStore = LEFT;
				break;
			}
		}

		if (isAtCentre()) {
			if (!isWall(ghostDirectionStore))
				ghostCurrentDirection = ghostDirectionStore;
			else if (isWall(ghostCurrentDirection))
				ghostCurrentDirection = nextMoveifStuck(ghostCurrentDirection);
		}

		movement(ghost, ghostCurrentDirection);
	}

	void movement(Ghost ghost, direction dir) {
		switch (dir)
		{
		case LEFT:
			ghostX -= ghostSpeed;
			ghostY = round(ghostY);
			break;
		case UP:
			ghostY += ghostSpeed;
			ghostX = round(ghostX);
			break;
		case RIGHT:
			ghostX += ghostSpeed;
			ghostY = round(ghostY);
			break;
		case DOWN:
			ghostY -= ghostSpeed;
			ghostX = round(ghostX);
			break;
		default:
			break;
		}
	}

	void ghostLeavePen(Ghost ghost) {
		//First we will align the ghost parallel to the ghost pen gate
		//Correct X coordinates required between 13.45 and 13.55 13.50 to be precise
		//cout << ghostColour << ":" << ghostX << ":" << ghostY << endl;
		if (ghostX <= 13.55f && ghostX >= 13.45f) {
			if (ghostY < 19.0f)
			{
				ghostY += 0.010f;
			}
		}
		else {
			//First we will fix the x coordinates of the ghost
			if (ghostX < 13.50f)
				ghostX += 0.010f;
			else if (ghostX > 13.50f)
				ghostX -= 0.010f;
		}
		if (ghostY >= 19.0f) {
			ghostCurrentDirection = LEFT;
			ghostState = SCATTER;
		}
	}

	void penMovement(Ghost ghost) {
		//All the ghosts inside the pen will be moving up and down randomly between 16.5f and 15.5f
		if (ghostY <= 15.5f && tempdir == DOWN)
			tempdir = UP;
		else if (ghostY >= 16.5f && tempdir == UP)
			tempdir = DOWN;

		switch (tempdir)
		{
		case UP:
			ghostY += ghostSpeed; 
			break;
		case DOWN:
			ghostY -= ghostSpeed;
		}
	}

	void sendGhostToPen() {
		ghostCurrentDirection = LEFT;
		ghostDirectionStore = LEFT;
		switch (ghostColour) { // Different settings depending on the colour of the ghost
		case RED:
			ghostX = 13.5f;
			ghostY = 16.0f;
			ghostState = PEN;
			break;
		case BLUE:
			ghostX = 11.5f;
			ghostY = 16.0f;
			ghostState = PEN;
			break;
		case YELLOW:
			ghostX = 15.5f;
			ghostY = 16.0f;
			ghostState = PEN;
			break;
		case PINK:
			ghostX = 13.5f;
			ghostY = 16.0f;
			ghostState = PEN;
			break;
		}
	}

	void resetAllGhosts() {
		ghostCurrentDirection = LEFT;
		ghostDirectionStore = LEFT;
		switch (ghostColour) { // Different settings depending on the colour of the ghost
		case RED:
			ghostX = 13.5f;
			ghostY = 19.0f;
			ghostState = SCATTER;
			break;
		case BLUE:
			ghostX = 11.5f;
			ghostY = 16.0f;
			ghostState = PEN;
			break;
		case YELLOW:
			ghostX = 15.5f;
			ghostY = 16.0f;
			ghostState = PEN;
			break;
		case PINK:
			ghostX = 13.5f;
			ghostY = 16.0f;
			ghostState = PEN;
			break;
		}
	}

	void redGhostMove(Ghost redGhost) {
		switch (ghostState) {
		case SCATTER:
			ghostMovement(redGhost);
			break;
		case FRIGHTEN:
			ghostMovement(redGhost);
			break;
		case PEN:
			if(gameTick-eaten_time == 2000){
				ghostState = LEAVE;
				eaten = false;
				eaten_time = 0;
			}
			else {
				penMovement(redGhost);
			}
			break;
		case LEAVE:
			ghostLeavePen(redGhost);
			break;
		}
	}
	void blueGhostMove(Ghost blueGhost) {
		if (pills <= 200 && ghostState==PEN && !eaten)
			ghostState = LEAVE;
		switch (ghostState){
		case SCATTER:
			ghostMovement(blueGhost);
			break;
		case LEAVE:
			ghostLeavePen(blueGhost);
			break;
		case PEN:
			if (gameTick - eaten_time == 2000) {
				eaten = false;
				eaten_time = 0;
				ghostState = LEAVE;
			}
			else{
				penMovement(blueGhost);
			}
			break;
		case FRIGHTEN: 
			ghostMovement(blueGhost);
			break;
		}
	}
	void pinkGhostMove(Ghost pinkGhost) {
		if (pills <= 150 && ghostState==PEN && !eaten)
			ghostState = LEAVE;
		switch (ghostState){
		case SCATTER:
			ghostMovement(pinkGhost);
			break;
		case LEAVE:
			ghostLeavePen(pinkGhost);
			break;
		case PEN:
			if (gameTick - eaten_time == 2000) {
				eaten = false;
				eaten_time = 0;
				ghostState = LEAVE;
			}
			else
				penMovement(pinkGhost);
			break;
		case FRIGHTEN:
			ghostMovement(pinkGhost);
			break;
		}
	}
	void yellowGhostMove(Ghost yellowGhost) {
		if (pills <= 90 && ghostState == PEN && !eaten)
			ghostState = LEAVE;
		switch (ghostState){
		case SCATTER:
			ghostMovement(yellowGhost);
			break;
		case LEAVE:
			ghostLeavePen(yellowGhost);
			break;
		case PEN:
			if (gameTick - eaten_time == 2000) {
				eaten = false;
				eaten_time = 0;
				ghostState = LEAVE;
			}
			else
				penMovement(yellowGhost);
			break;
		case FRIGHTEN:
			ghostMovement(yellowGhost);
			break;
		}
	
	}

	void move(Ghost ghost) {
		//Setting the direction of movement of every ghost
		//Making a different function for the movement of each ghost
		switch (ghostColour) {
		case RED:
			redGhostMove(ghost);
			break;
		case BLUE:
			blueGhostMove(ghost);
			break;
		case PINK:
			pinkGhostMove(ghost);
			break;
		case YELLOW:
			yellowGhostMove(ghost);
			break;
		}
	}
};

#endif