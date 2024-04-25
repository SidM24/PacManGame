#include "loadAndBindTextures.h"

#ifndef COURSEWORK_TEXTURES_H
#define COURSEWORK_TEXTURES_H

unsigned int maze_tex;
unsigned int pill_tex;
unsigned int bigPill_tex;

unsigned int pac_tex_mouth_closed;
unsigned int pac_tex_mouth_midopen;
unsigned int pac_tex_mouth_open;

//Ghost textures
unsigned int red_ghost_tex;
unsigned int blue_ghost_tex;
unsigned int pink_ghost_tex;
unsigned int yellow_ghost_tex;
unsigned int frightend_ghost_tex;
unsigned int semi_frighten_ghost_tex;

//Textures for Interface
unsigned int lives_tex;

//Ghost eyes texture
unsigned int eyes_up;
unsigned int eyes_down;
unsigned int eyes_left;
unsigned int eyes_right;

//background textures
unsigned int background_tex;


void loadandbindallTextures() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Binding the maze texture
	maze_tex = load_and_bind_texture("sprites/maze/maze.png");
	//Pill textures
	pill_tex = load_and_bind_texture("sprites/maze/p-0.png");
	bigPill_tex = load_and_bind_texture("sprites/maze/p-1.png");

	//loading the Pacman texture
	pac_tex_mouth_closed = load_and_bind_texture("sprites/pacman/2.png");
	pac_tex_mouth_midopen = load_and_bind_texture("sprites/pacman/1.png");
	pac_tex_mouth_open= load_and_bind_texture("sprites/pacman/0.png");

	//loading the different ghost textures
	red_ghost_tex = load_and_bind_texture("sprites/ghosts/r-1.png");
	blue_ghost_tex = load_and_bind_texture("sprites/ghosts/b-1.png");
	pink_ghost_tex = load_and_bind_texture("sprites/ghosts/p-1.png");
	yellow_ghost_tex = load_and_bind_texture("sprites/ghosts/y-1.png");
	frightend_ghost_tex = load_and_bind_texture("sprites/ghosts/frightened_ghost.png");
	semi_frighten_ghost_tex = load_and_bind_texture("sprites/ghosts/semi_frightened_ghost.png");

	//Interface Textures
	lives_tex = load_and_bind_texture("sprites/interface/lives.png");

	//eyes
	eyes_up = load_and_bind_texture("sprites/eyes/up.png");
	eyes_down = load_and_bind_texture("sprites/eyes/down.png");
	eyes_left = load_and_bind_texture("sprites/eyes/left.png");
	eyes_right = load_and_bind_texture("sprites/eyes/right.png");

	//Start page background texture
	background_tex = load_and_bind_texture("sprites/startpage/background.png");
}

void drawTexture(unsigned int texture,int length, int height,float angle) {

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);

	int widthCentre = length / 2;
	int verticalCentre = height / 2;

	//Rotating the texture if it is required by angle degree about its origin
	glTranslatef((float)widthCentre, (float)verticalCentre, 0.0f);
	glRotatef(angle, 0.0f, 0.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);      // Lower left corner
	glVertex2i(-widthCentre, -verticalCentre);
	glTexCoord2f(1.0f, 0.0f);      // Lower right corner
	glVertex2i(widthCentre, -verticalCentre);
	glTexCoord2f(1.0f, 1.0f);      // Upper right corner
	glVertex2i(widthCentre, verticalCentre);
	glTexCoord2f(0.0f, 1.0f);      // Upper left corner
	glVertex2i(-widthCentre, verticalCentre);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	// Pop matrix to ignore above transformations on future objects
	glPopMatrix();

}

#endif