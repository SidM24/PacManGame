#ifndef COURSEWORK_INTERFACE_H
#define COURSEWORK_INTERFACE_H

string score_string = "Score: ";
extern int score;
extern int gameTick;
extern bool paused;

extern unsigned int lives_tex;
extern int total_lives;

extern unsigned int background_tex;


//Function to display the ready text on the screen


void display_Text(float x,float y, string s) {
	glPushMatrix(); 
	glRasterPos2f(x, y);
	int len = (int)s.length();
	for (int i = 0; i < len; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
	}
	glPopMatrix();
}

void drawStart() {
	string str = "Press SPACE to start";
	display_Text(100, 90, str);
}

void drawGameOver() {
	string str = "GAME OVER";
	display_Text(118,130, str);
}

void drawPressToPlay() {
	string str = "Press 'r' to play again";
	display_Text(100, 10, str);
}

void draw_Ready() {
	string str = "";
	float temp_x=130.0f;
	if (gameTick <= 500) {
		str = "READY";
		temp_x = 130.0f;
	}
	else if (gameTick <= 1500) {
		str = "3";
		temp_x=150.0f;
	}
	else if (gameTick <= 2500) {
		str = "2";
		temp_x=150.0f;
	}
	else if (gameTick <= 3500) {
		str = "1";
		temp_x=150.0f;
	}
	else if (gameTick > 3500 && gameTick<=3850) {
		str = "GO!!";
		temp_x=140.0f;
	}
	
	if (paused) {
		str = "PAUSED";
		temp_x = 130.0f;
	}

	display_Text(temp_x,130.0f,str);
}

//Function to draw the score board onto the game screen
void drawScore(string str){
	glPushMatrix();
	translateBottomLeft();
	translateToMazeCoords(15.0, 32.5);
	display_Text(50.0f,0.0f, str);
	glPopMatrix();
}

void drawLives(int liveCount)
{
	glPushMatrix();
	translateBottomLeft();
	translateToMazeCoords(2, -2); // Translate to the correct area of the screen ready for drawing
	for (int i = 0; i < liveCount; i++) { // Draw as many as icons as pacman has lives left
		drawTexture(lives_tex, 14, 14, 0);
		translateToMazeCoords(2, 0); // After drawing translate to the right ready for more potential icons to be drawn
	}
	glPopMatrix();
}

int getHighScore() {
	int highScore;
	fstream scoreFile("highScore.txt");
	//Checking if the file exist
	if (scoreFile.good()) {
		scoreFile >> highScore;
		scoreFile.close();
		return highScore;
	}
	else {
		//if the file does not exits then we will create a file
		ofstream highScoreFile("highScore.txt");
		highScoreFile << 0; 
		scoreFile.close();
		highScoreFile.close();
	}
	return 0;
}

//Function to set the high score if current score is greater than high score
void setHighScore(int currentScore) {
	int currentHighScore = getHighScore();
	if (currentHighScore < currentScore) {
		//Open the high Score File and update it
		ofstream highScoreFile("highScore.txt");
		highScoreFile.clear();
		highScoreFile << currentScore;
		highScoreFile.close();
	}
}

void drawHighScore() {
	string str = "High Score: "+ to_string(getHighScore());
	glPushMatrix();
	translateBottomLeft();
	translateToMazeCoords(0.0f, 32.5f);
	display_Text(0.0f, 0.0f, str);
	glPopMatrix();
}

void drawInterface() {
	string display_score = score_string + to_string(score);
	drawHighScore();
	drawScore(display_score);
	drawLives(total_lives);
}

void drawBackground() {
	glPushMatrix();
	drawTexture(background_tex, 300, 300, 0);
	glPopMatrix();
}

void drawStartScreen() {
	drawBackground();
}

#endif