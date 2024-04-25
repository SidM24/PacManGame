#include <GL/glut.h>
#include<stdio.h>
#include<string.h>
#include<algorithm>
#include<iostream>
#include<stdlib.h>
#include <png.h>
#include<vector>
#include<fstream>
#include<sstream>
#include<math.h>

using namespace std;

#include "maze.h"
#include "textures.h"
#include "loadAndBindTextures.h"
#include "Interface.h"
#include "pacman.h"
#include "Ghost.h"

//Variables
int frighten_time = 0;
bool frighten = false;
int total_lives = 3;

//score variable would store the score at present moment of time
int score = 00;

//pills stores the total number of pills present on the screen
int pills = 244; //Initially there are 244 pills

//Custom variable to store the current state of the game
typedef enum { BEGIN, PLAY, DIE, OVER, START } gameState;
gameState current_game_state = START;

//Creating the pacman object of the Pacman class
Pacman pacman;

//To keep a record of the time-stamp the current game is in:
int gameTick = 0;

//ghosts
Ghost ghosts[4] = {
    Ghost(13.5f,19.0f,RED),
    Ghost(11.5f,16.0f,BLUE),
    Ghost(13.5f,16.0f,PINK),
    Ghost(15.5f,16.0f,YELLOW)
};

bool paused = true;


void detectGhost() {
    float pacX = pacman.getPacmanX();
    float pacY = pacman.getPacmanY();
    //Checking if any of the ghost has collided with the pacman
    for (int i = 0; i < 4; i++)
    {
        Ghost g = ghosts[i];
        float gX = g.getGhostX();
        float gY = g.getGhostY();
        if (pacX == gX && pacY == gY) {
            //Checking if any ghost is present at the staring position of the pacman, if yes , then we will reset his position
            if (g.ghostState == FRIGHTEN) {
                //If we have collided with a frightened ghost
                score += 200;
                ghosts[i].eaten = true;
                ghosts[i].eaten_time = gameTick;
                ghosts[i].sendGhostToPen();
            }
            else {
                for (int i = 0; i < 4; i++)
                {
                    if (ghosts[i].getGhostY() >= 6.0f 
                        && ghosts[i].getGhostY() <= 9.0f 
                        && ghosts[i].getGhostX() >= 5.0f 
                        && ghosts[i].getGhostX() <= 19.0f) {
                        ghosts[i].ghostX = 13.0f;
                        ghosts[i].ghostY = 19.0f;
                    }
                }
                current_game_state = DIE;
            }   
        }
    }
}

void detectPill()
{
    if (pacman.getTile(pacman.pacmanX, pacman.pacmanY) == O) {
        frighten = true;
        frighten_time = gameTick;
        for (int i=0; i< 4; i++)
        {
            if (ghosts[i].ghostState == SCATTER) {
                ghosts[i].eaten = false;
                ghosts[i].ghostState = FRIGHTEN;
                if (ghosts[i].ghostCurrentDirection == LEFT) { // Reverse their direction
                    ghosts[i].ghostCurrentDirection = RIGHT;
                }
                else if (ghosts[i].ghostCurrentDirection == UP) {
                    ghosts[i].ghostCurrentDirection = DOWN;
                }
                else if (ghosts[i].ghostCurrentDirection == RIGHT) {
                    ghosts[i].ghostCurrentDirection = LEFT;
                }
                else {
                    ghosts[i].ghostCurrentDirection = UP;
                }
            }
        }
    }
}

//Function for the movement of the pacman character
void idle(){
    if (!paused) {
        switch (current_game_state) {
        case BEGIN:
            //after 2.5 seconds the game mode is set to play mode
            if (gameTick >= 3850)
                current_game_state = PLAY;
            break;
        case PLAY:
            if (pills == 0) {
                //reset all the pills in the maze, the pacman is set in the middle
                pills = 244;
                resetMaze();
            }
            detectPill();
            pacman.checkTile();
            detectGhost();
            pacman.move();
            detectGhost();
            for (int i = 0; i < 4; ++i) {
                ghosts[i].move(ghosts[0]);
            }
            detectGhost();
            if (frighten) {
                if (gameTick - frighten_time == 5000) {
                    frighten = false;
                    for (int i = 0; i < 4; i++){
                        if (ghosts[i].ghostState == FRIGHTEN) {
                            ghosts[i].eaten = false;
                            ghosts[i].ghostState = SCATTER;
                        }
                    }
                }
            }
            break;
        case DIE:
            cout << "Pacman Died" << endl;
            gameTick = 0;
            --total_lives;
            if (total_lives == 0) {
                current_game_state = OVER;
                break;
            }
            //Reset the coordinates of the pacman to the centre again 
            pacman.pacmanX = 13.5f;
            pacman.pacmanY = 7.0f;
            pacman.currentDirection = LEFT;
            pacman.directionStore = LEFT;
            current_game_state = BEGIN;
            break;
        }
        gameTick++; 
        glutPostRedisplay(); 
    }
}

void special(int key, int , int) {
    switch (key)
    {
    case GLUT_KEY_LEFT:
        pacman.setDirectionStore(LEFT);
        break;
    case GLUT_KEY_RIGHT:
        pacman.setDirectionStore(RIGHT);
        break;
    case GLUT_KEY_UP:
        pacman.setDirectionStore(UP);
        break;
    case GLUT_KEY_DOWN:
        pacman.setDirectionStore(DOWN);
        break;
    }
}
void resetGame() {
    score = 0;
    total_lives = 3;
    gameTick = 0;
    pills = 244;
    frighten = false;
    frighten_time = 0;
    //Reset the position of all ghosts
    for (int i = 0; i < 4; i++)
    {
        ghosts[i].resetAllGhosts();
    }
    //Reset the pacman 
    pacman.pacmanX = 13.5f;
    pacman.pacmanY = 7.0f;
    pacman.currentDirection = LEFT;
    pacman.directionStore = LEFT;
    current_game_state = BEGIN;
}
void keyboard(unsigned char key, int, int) {
    switch (tolower(key)) {
    case 'q':
        exit(1);
    case 'p':
        if (paused)
            paused = false;
        else
            paused = true;
        break;
    case 'r':
        //To resest the game
        if (current_game_state == OVER) {
            resetMaze();
            resetGame();
        }
        break;
    case ' ':
        //To start the game when the game is first opened
        if (current_game_state == START) {
            paused = false;
            current_game_state = BEGIN;
        }
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 300, 0, 300);
    loadandbindallTextures();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    switch (current_game_state)
    {
    case START:
        drawStartScreen();
        drawStart();
        break;
    case BEGIN:
        drawMaze();
        //Drawing the pacman
        pacman.draw();  
        //Drawing the ghosts
        for (int i = 0; i < 4; ++i) {
            ghosts[i].draw();
        }
        drawInterface();
        draw_Ready();
        break;
    case PLAY:
        drawMaze();
        pacman.draw();
        for (int i = 0; i < 4; ++i) {
            ghosts[i].draw();
        }
        drawInterface();
        draw_Ready();
        break;
    case DIE:
        drawMaze();
        pacman.draw();
        drawInterface(); 
        break;
    case OVER:
        setHighScore(score); 
        drawMaze();
        drawInterface();
        drawGameOver();
        drawPressToPlay();
        break;
    }

    glutSwapBuffers();
}



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB); // Single buffer and RGB color mode

    int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
    int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);
    int windowWidth = 512; // Set your desired window width
    int windowHeight = 512; // Set your desired window height
    int windowPosX = (screenWidth - windowWidth) / 2;
    int windowPosY = (screenHeight - windowHeight) / 2;
    glutInitWindowPosition(windowPosX, windowPosY);

    glutInitWindowSize(512, 512); // Set window size
    glutCreateWindow("PacMan"); // Create a window with the given title

    
    // Register display
    glutDisplayFunc(display); 

    //To change the game state
    glutKeyboardFunc(keyboard);
    //For movement we will use the special Function
    glutSpecialFunc(special);

    //For performing the background processing 
    glutIdleFunc(idle);

    init(); 
    glutMainLoop(); // Enter the GLUT event processing loop

    return 0;
}
