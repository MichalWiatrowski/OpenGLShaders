// Scene class. Configures a basic 3D scene.
// Interfaces with the Input class to handle user input
// Calculates and outputs Frames Per Second (FPS) rendered.
// Important functions are the constructor (initialising the scene), 
// update (for process user input and updating scene objects) and render (renders scene).
#ifndef _SCENE_H
#define _SCENE_H

// Include GLUT, openGL, input.
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Input.h"
#include <stdio.h>
// Further includes should go here:
#include "SOIL.h"
#include <vector>
#include "camera.h"
#include "Model.h"
#include "ProceduralGeneration.h"
#include "Shadow.h"
#include <chrono>
class Scene{

public:
	Scene(Input *in, camera *cam);

	// Main render function
	void render();
	// Update function receives delta time from parent (used for frame independent updating).
	void update(float dt);
	// Resizes the OpenGL output based on new window size.
	void resize(int w, int h);

protected:
	// Renders text (x, y positions, RGB colour of text, string of text to be rendered)
	void displayText(float x, float y, float r, float g, float b, char* string);
	// A function to collate all text output in a single location
	void renderTextOutput();
	void calculateFPS();
	void loadTexture(char* filename);
	// draw primitive functions
	

	// For access to user input.
	Input* input;
	camera* Camera;
	Model model;
	ProceduralGeneration sphere1;
	ProceduralGeneration sphere2;
	ProceduralGeneration sphere3;
	ProceduralGeneration sphere4;
	ProceduralGeneration sphere5;
	ProceduralGeneration sphere6;

	
	ProceduralGeneration platform1;
	ProceduralGeneration platform1top;

	ProceduralGeneration platform2;
	ProceduralGeneration platform2top;

	ProceduralGeneration platform3;
	ProceduralGeneration platform3top;

	Shadow shadow1;
	// For Window and frustum calculation.
	int width, height;
	float fov, nearPlane, farPlane;

	// For FPS counter and mouse coordinate output.
	int frame = 0, time, timebase = 0;
	char fps[40];
	char mouseText[40];
	GLuint m_texture;
	float shadowMatrix[16];
private:
	bool wireframe = false; //boolean that controls the wireframe

	short int cameraX;
	short int cameraY;


	////////////////////////////////////////////////// changing light colour
	float lightTimer = 0.0f;
	short int lightNo = 1;
	float r =1.0f, g = 0.0f, b = 0.0f;
	////////////////////////////////////////////////  angle of the 3rd platform light (rotation)

	float x = 0.0f, z = 0.33f;
	bool xPointer = false, zPointer = false;
	/////////////////////////////////////////////// variable storing the rotation angle of the model
	int rotation = 0;
	//////////////////////////////////////////////// stores the x and z position of the 1st platform light
	float light1PositionX = 0.0f;
	float light1PositionZ = 0.0f;
	////////////////////////////////////////////////
	int currentCamera = 0;
	//////////////////////////////////////////////// stores x value for the scrolling camera
	float scrollingPositionX = 0.0f;
	float scrollingLookatX = 0.0f;
	bool scrolling = false;
	////////////////////////////////////////////////
};

#endif