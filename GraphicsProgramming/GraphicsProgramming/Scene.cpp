#include "Scene.h"


Scene::Scene(Input *in, camera *cam)
{
	// Store pointer for input and camera class
	input = in;
	Camera = cam;
	
	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.39f, 0.58f, 93.0f, 1.0f);			// Cornflour Blue Background
	glEnable(GL_DEPTH_TEST);							// Enable Depth Test
	glClearDepth(1.0f);									// Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glClearStencil(0);									// Clear stencil buffer
	
	// Other OpenGL / render setting should be applied here.

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Blending Setup
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glutWarpPointer(400, 300); // keep the cursor in the middle of the screen
	glEnable(GL_TEXTURE_2D); // Enable Textures
	//load the skybox texture
	loadTexture("gfx/skybox.png");
	// setup texture parameters
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/////////////////////////// Initialise variables
	glEnable(GL_LIGHTING); // Emable Lighting 
	// setup light attenuation for light0
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);
	
	
	//generate 6 spheres with a radius of 1 and 50 segments, no texture
	sphere1.generateSphere(1, 50, "");
	sphere2.generateSphere(1, 50, "");
	sphere3.generateSphere(1, 50, "");
	sphere4.generateSphere(1, 50, "");
	sphere5.generateSphere(1, 50, "");
	sphere6.generateSphere(1, 50, "");

	//generate the 3 platforms, they consist of 3 half spheres and 3 discs
	//they all have a radius of 5 and are made of 50 segments, no textures
	platform1.generateHalfSphere(5, 50, "");
	platform1top.generateDisc(5, 50);

	platform2.generateHalfSphere(5, 50, "");
	platform2top.generateDisc(5, 50);

	platform3.generateHalfSphere(5, 50, "");
	platform3top.generateDisc(5, 50);

	
	//loads the spaceship model and its texture
	model.load("Models/spaceship.obj", "Models/spaceship.jpg");
	
}

void Scene::update(float dt)
{
	
	//run the camera update function
	Camera->update();

	// Handle user input
/////////////////////////////////////////// toggle through cameras
	//if z or c is pressed, change camera
	if (input->isKeyDown('z'))
	{
		input->SetKeyUp('z');
		currentCamera--;
	
	}
	if (input->isKeyDown('c'))
	{
		input->SetKeyUp('c');
		currentCamera++;
	}
	//only have 3 cameras so if toggeled higher than 3, go back to the first one and vice versa
	if (currentCamera > 2)
	{
		currentCamera = 0;
	}
	if (currentCamera < 0)
	{
		currentCamera = 2;
	}
//////////////////////////////////////// Toggling of the wireframe
	//if p is pressed, enable or disable wireframe
	if (input->isKeyDown('p'))
	{
		input->SetKeyUp('p');
		wireframe = !wireframe;

	}
	if (wireframe) // if wireframe is true, enable wireframe
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (!wireframe) // if false, disable the wireframe
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
////////////////////////////////////// end of wireframe toggling 
//////////////////////////////////////--- Main Camera (0) Control
	//if current camera is 0 (or main) enable free moving controls
	if (currentCamera == 0)
	{
		
		if (input->isKeyDown('w')) // forward
		{
			Camera->moveForward(dt);
		}
		if (input->isKeyDown('s')) // backward
		{
			Camera->moveBackward(dt);
		}
		if (input->isKeyDown('d')) //right
		{
			Camera->moveRight(dt);
		}
		if (input->isKeyDown('a')) //left
		{
			Camera->moveLeft(dt);
		}
		if (input->isKeyDown('q')) //down
		{
			Camera->moveDown(dt);
		}
		if (input->isKeyDown('e')) //up
		{
			Camera->moveUp(dt);
		}
//////////////////////////////////////// update the yaw and pitch of the camera
		
			Camera->setYaw(input->getMouseX() - 400, dt);

			Camera->setPitch(input->getMouseY() - 300, dt);
		
	}
//////////////////////////////////////////// setup for fixed camera
	if (currentCamera == 1)
	{
		//if current camera is 1, enable fixed view camera at the position said below
		Camera->setPosX(0); Camera->setPosY(6); Camera->setPosZ(7); Camera->setLookX(0); Camera->setLookY(-1); Camera->setLookZ(0); Camera->setUpX(0); Camera->setUpY(1); Camera->setUpZ(0);
		// enables the movement of the 1st platform light to demonstrate light on high shininess spheres
		//if statements to keep the light from moving off the platform
		if (light1PositionZ > -4)
		if (input->isKeyDown('w')) //move the light forward
		{
			light1PositionZ -= 2 * dt;
		}
		if (light1PositionZ < 4)
		if (input->isKeyDown('s')) //move the light backward
		{
			light1PositionZ += 2 * dt;
		}
		if (light1PositionX < 4)
		if (input->isKeyDown('d')) //move the light right
		{
			light1PositionX += 2 * dt;
		}
		if (light1PositionX > -4)
		if (input->isKeyDown('a')) //move the light left
		{
			light1PositionX -= 2 * dt;
		}
		glutWarpPointer(400, 300); // keep the cursor in the middle of the screen
	}
//////////////////////////////////////////// setup for scrolling camera
	if (currentCamera == 2)
	{
		//keep scrolling right until it hits 45 or more on the x-axis and start scrolling left
		if (scrollingPositionX >= 45.0f)
		{
			scrolling = true;
			
		}
		//keep scrolling left until it hits 0 or less on the x-axis and start scrolling right
		if (scrollingPositionX < 0.0f)
		{
			scrolling = false;	
		}
		//increment the position and lookat by 0.5 times by delta time
		if (scrolling == false)
		{
			scrollingPositionX += 0.5 * dt;
			scrollingLookatX += 0.5 * dt;
		}
		//decrease the position and lookat by 0.5 times by delta time
		if (scrolling == true)
		{
			scrollingPositionX -= 0.5 * dt;
			scrollingLookatX -= 0.5 * dt;
		}
		//reset the pitch and yaw so the camera looks forward
		Camera->setPitch(0, dt); Camera->setYaw(0, dt);
		//update the camera position and lookat values
		Camera->setPosX(scrollingPositionX); Camera->setPosY(4); Camera->setPosZ(20); Camera->setLookX(scrollingLookatX);
		glutWarpPointer(400, 300); // keep the cursor in the middle of the screen
	}

////////////////////////////////////////
// update scene related variables
////////////////////////////////////////////////////////////// 3rd platform light colour
	//increment the variable with dt, acts as a timer
	lightTimer += dt;
	//if the variable exceeds 10, reset it to 0 and increment the variable that controls the colour
	if (lightTimer >= 10.0f)
	{
		lightNo++;
		lightTimer = 0.0f;
	}
	//if the colour variable exceeds 9, reset it to 1
	if (lightNo > 9)
	{
		lightNo = 1;
	}
	// if statements, everytime the lightNo variable increments, the r,g,b values will change (they are used in the diffuse in one of the lights)
	if (lightNo == 1)
	{
		r = 5.0f;
		g = 0.0f;
		b = 0.0f;
	}
	else if (lightNo == 2)
	{
		r = 5.0f;
		g = 5.0f;
		b = 0.0f;
	}
	else if (lightNo == 3)
	{
		r = 0.0f;
		g = 5.0f;
		b = 0.0f;
	}
	else if (lightNo == 4)
	{
		r = 0.0f;
		g = 5.0f;
		b = 5.0f;
	}
	else if (lightNo == 5)
	{
		r = 0.0f;
		g = 0.0f;
		b = 5.0f;
	}
	else if (lightNo == 6)
	{
		r = 5.0f;
		g = 0.0f;
		b = 5.0f;
	}
	else if (lightNo == 7)
	{
		r = 0.5f;
		g = 0.5f;
		b = 5.0f;
	}
	else if (lightNo == 8)
	{
		r = 0.5f;
		g = 5.0f;
		b = 0.5f;
	}
	else if (lightNo == 9)
	{
		r = 5.0f;
		g = 0.5f;
		b = 0.5f;
	}
/////////////////////////////////////////////////// end of light colour settings
/////////////////////////////////////////////////// 3rd platform light angle (animation)
	//if x is greater or equal to 0.33, set xPointer to true
	if (x >= 0.33f)
	{
		xPointer = true;
	}
	//and x is less or equal to -0.33, set xPointer to false
	else if (x <= -0.33f)
	{
		xPointer = false;
	}
	//and z is more or equal to 0.33, set zPointer to true
	if (z >= 0.33f)
	{
		zPointer = true;
	}
	//and z is less or equal to -0.33, set zPointer to false
	else if (z <= -0.33f)
	{
		zPointer = false;
	}

	//if zPointer is true and if zPointer is false, subtract and add 0.03f times delta time respectively
	if (zPointer == true)
	{
		z -= 0.03f * dt;
	}
	else if (zPointer == false)
	{
		z += 0.03f * dt;
	}
	//if xPointer is true and if xPointer is false, subtract and add 0.03f times delta time respectively
	if (xPointer == false)
	{
		x += 0.03f * dt;
	}
	else if (xPointer == true)
	{
		x -= 0.03f * dt;
	}
////////////////////////////////////////////// end of light angle settings
///////////////////////////////////////////// rotation angle for the model setup
	//increment the rotation angle by 6 times delta time
	rotation += 6 * dt;
	//if the rotation is 360 or more, reset it back to 0
	if (rotation >= 360)
	{
		rotation = 0;
	}
////////////////////////////////////////// end of model rotation setup
	// Calculate FPS for output
	calculateFPS();
}

void Scene::render() {

	glEnable(GL_LIGHTING); // enable lighting
	glDisable(GL_DEPTH_TEST); // disable the depth test
	glColor3f(1.0f, 1.0f, 1.0f);//reset colour
	// Clear Color, Depth and Stencil Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	// Reset transformations
	glLoadIdentity();

	// Set the camera
	//call the getters in the camera class to get all the values for gluLookAt
	gluLookAt(Camera->getPosX(), Camera->getPosY(), Camera->getPosZ(), Camera->getLookX(), Camera->getLookY(), Camera->getLookZ(), Camera->getUpX(), Camera->getUpY(), Camera->getUpZ());
	
	// Render geometry/scene here -------------------------------------
	
///////////////////////////////////////////////// ------ skybox rendering 
	glPushMatrix();	// push the matrix for the skybox
	glTranslatef(Camera->getPosX() - 0.5, Camera->getPosY()- 0.5, Camera->getPosZ()-0.5); /// translate the cube (skybox) to move with the camera
	glBindTexture(GL_TEXTURE_2D, m_texture); // bind the skybox texture that was setup in the constructor
	//////////////////// -- setup repeating texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//////////////////// -- draw the cube
	glBegin(GL_QUADS);
	//front
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(10.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(10.0f, 10.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 10.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	//left
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(10.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(10.0f, 10.0f);
	glVertex3f(0.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 10.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	//right
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glTexCoord2f(10.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(10.0f, 10.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 10.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	//top
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(10.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 1.0f);
	glTexCoord2f(10.0f, 10.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0f, 10.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	//bottom
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2f(10.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(10.0f, 10.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 10.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	//back
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glTexCoord2f(10.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(10.0f, 10.0f);
	glVertex3f(0.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 10.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd(); //// end of cube drawing
	glBindTexture(GL_TEXTURE_2D, NULL); // unbind opengl from the skybox texture
	glPopMatrix(); // pop and return to 'root'

/////////////////////////////////////-- end of skybox rendering
glColor3f(1.0f, 1.0f, 1.0f); // set the colour to default (white)
 /////////////////////////////////////-- light 0 setyup (spotlight)
	glPushMatrix(); // push the matrix for light0
	GLfloat Light_Ambient[] = { 0, 0, 0, 1.0f }; // set the ambient to black
	GLfloat Light_Diffuse[] = { 3.0f, 3.0f, 3.0f, 1.0f }; // set the diffuse colour to very bright white
	GLfloat Light_Position[] = { light1PositionX, 10.0f, light1PositionZ, 1.0f }; // set position
	GLfloat spot_Direction[] = { 0.0f, -1.0f, 0.0f }; // point downwards on the y-axis

	glLightfv(GL_LIGHT0, GL_AMBIENT, Light_Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, Light_Position);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 25.0f);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_Direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 50.0);
	glPopMatrix(); // pop the matrix
	
	glPushMatrix();// push the matrix for light1
	GLfloat Light_Ambient1[] = { 0, 0, 0, 1.0f };
	GLfloat Light_Diffuse1[] = { 4.0f, 4.0f, 4.0f, 1.0f };
	GLfloat Light_Position1[] = { 20.0f, 10.0f, 0.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, Light_Ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Light_Diffuse1);
	glLightfv(GL_LIGHT1, GL_POSITION, Light_Position1);
	glPopMatrix();

	glPushMatrix();// push the matrix for light2
	GLfloat Light_Ambient2[] = { 0, 0, 0, 1.0f };
	GLfloat Light_Diffuse2[] = { r, g, b, 1.0f };
	GLfloat Light_Position2[] = { 40.0f, 10.0f, 0.0f, 1.0f };
	GLfloat spot_Direction2[] = { x, -1.0f, z };

	glLightfv(GL_LIGHT2, GL_AMBIENT, Light_Ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, Light_Diffuse2);
	glLightfv(GL_LIGHT2, GL_POSITION, Light_Position2);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 25.0f);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_Direction2);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 50.0);
	glPopMatrix();// pop the matrix

	glPushMatrix();// push the matrix for light3
	GLfloat Light_Ambient3[] = { 0, 0, 0, 1.0f };
	GLfloat spot_Direction3[] = { 0.0f, -1.0f, 0.0f };
	GLfloat Light_Diffuse3[] = { 4.0f, 4.0f, 4.0f, 1.0f };
	GLfloat Light_Position3[] = { 20.0f, 10.0f, -10.0f, 1.0f };

	glLightfv(GL_LIGHT3, GL_AMBIENT, Light_Ambient3);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, Light_Diffuse3);
	glLightfv(GL_LIGHT3, GL_POSITION, Light_Position3);
	glPopMatrix();// pop the matrix

	glEnable(GL_DEPTH_TEST); //disable depth testing

////////////////////////////////////////////////////////////////////////////////////////////////////////-------------------------- 6 procedural spheres
	glEnable(GL_LIGHT0); //enable light0
	glPushMatrix(); /// push matrix to create a single control group for all the spheres
	glTranslatef(-4.0f, 0.7f, 1.0f); /// allows me to translate the entire group

	glEnable(GL_COLOR_MATERIAL); //enable color materials
	GLfloat shininess[] = { 40.0 }; //initialise the shininess variable to 40
	GLfloat highSpec[] = { 1.0, 1.0, 1.0, 1.0 }; // make the specular variable high
	//apply the variables into the material functions
	glMaterialfv(GL_FRONT, GL_SPECULAR, highSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	
	glPushMatrix(); //////// push to create control panel for sphere 1
	
	glColor3f(1.0f, 1.0f, 1.0f);  //set colour for the shape
	glTranslatef(1.5f, 0.0f, -2.0f);  /// move the shape so it doesnt overlap with others
	sphere1.render();  //run the render function to draw the shape
	glPopMatrix(); //// pop to return to main group control

	glPushMatrix(); ////// sphere 2
	glColor3f(1.0f, 1.0f, 0.0f); // set colour
	glTranslatef(4.0f, 0.0f, -2.0f); //translate
	sphere2.render(); //render
	glPopMatrix(); //pop matrix

	glPushMatrix(); /////sphere 3
	glTranslatef(6.5f, 0.0f, -2.0f);//translate
	glColor3f(0.0f, 1.0f, 0.0f);// set colour
	sphere3.render();//render
	glPopMatrix(); // pop matrix

	glPushMatrix(); /////// sphere 4
	glTranslatef(1.5f, 0.0f, 0.0f);//translate
	glColor3f(0.0f, 1.0f, 1.0f);// set colour
	sphere4.render();//render
	glPopMatrix(); // pop matrix

	glPushMatrix();  //////sphere 5
	glTranslatef(4.0f, 0.0f, 0.0f);//translate
	glColor3f(0.0f, 0.0f, 1.0f);// set colour
	sphere5.render();//render
	glPopMatrix();// pop matrix

	glPushMatrix(); /////sphere 6
	glTranslatef(6.5f, 0.0f, 0.0f);//translate
	glColor3f(1.0f, 0.0f, 1.0f);// set colour
	sphere6.render();//render
	glPopMatrix();// pop matrix

	glDisable(GL_COLOR_MATERIAL); //disable color materials
	glPopMatrix(); //pop matrix and return to 'root'
	
///////////////////////////////////////////////////////////////////////////////////////////////////  platform 1
	
	glPushMatrix(); //push the matrix (platform1)
	glEnable(GL_COLOR_MATERIAL); //enable color materials
	glColor3f(1.0f, 0.0f, 0.0f); //set the colour to red

	glPushMatrix(); //push the matrix for the halfsphere
	glRotatef(180, 1.0f, 0.0f, 0.0f); //rotate it up-side down
	platform1.render(); //redner the half sphere
	glPopMatrix(); // pop the matrix

	glPushMatrix(); //push the matrix for top of the platform
	glTranslatef(0.0f, -0.32f, 0.0f); //translate down in the y-axis so there is no gap
	glRotatef(-90, 1.0f, 0.0f, 0.0f); //rotate it on the x-axis
	platform1top.render1(); //render the disc
	glPopMatrix(); //pop the matrix

	glDisable(GL_COLOR_MATERIAL); //disable color material
	glPopMatrix(); //pop the matrix
	glDisable(GL_LIGHT0); //disable light 0
	///////////////////////////////////////////////////////////////////////////////////////////////////  platform 2
	glPushMatrix(); //push the matrix for the seconds platform
	glEnable(GL_LIGHT1); //enable light1
	glColor3f(0.0f, 1.0f, 0.0f); //set colour to green
	glEnable(GL_COLOR_MATERIAL); //enable color material
	
	glPushMatrix(); //push the matrix for the halfsphere
	glTranslatef(20.0f, 0.0f, 0.0f); //translate along the x-axis
	glRotatef(180, 1.0f, 0.0f, 0.0f); // rotate it up - side down
	platform2.render(); //render
	glPopMatrix(); //pop the matrix

	glPushMatrix();
	glTranslatef(20.0f, -0.32f, 0.0f); //translate down in the y-axis so there is no gap
	glRotatef(-90, 1.0f, 0.0f, 0.0f); //rotate it on the x-axis
	platform2top.render1(); //render the disc
	glPopMatrix();

	glDisable(GL_LIGHT1); //disable light1
	glDisable(GL_COLOR_MATERIAL); //disable color material
	glPopMatrix();
	
///////////////////////////////////////////////////////////////////////////////////////////////////  platform 3
	
	glPushMatrix(); // push the matrix for the third platform
	glEnable(GL_LIGHT2); //enable light 2
	glColor3f(1.0f, 1.0f, 1.0f); //set the colour to white
	glEnable(GL_COLOR_MATERIAL); // enable color material
	glPushMatrix();//push the matrix for the halfsphere
	glTranslatef(40.0f, 0.0f, 0.0f); //translate along the x-axis
	glRotatef(180, 1.0f, 0.0f, 0.0f); // rotate it up - side down
	platform3.render();
	glPopMatrix(); //pop the matrix

	glPushMatrix();
	glTranslatef(40.0f, -0.32f, 0.0f); //translate down in the y-axis so there is no gap
	glRotatef(-90, 1.0f, 0.0f, 0.0f); //rotate it on the x-axis
	platform3top.render1(); //render the disc
	glPopMatrix();

	glDisable(GL_LIGHT2);
	glDisable(GL_COLOR_MATERIAL);
	glPopMatrix(); //pop the matrix

///////////////////////////////////////////////////////////////////////////////////////////////////////	
	glDisable(GL_DEPTH_TEST); //disable depth testing
	glDisable(GL_LIGHTING); //disable lighting
	glDisable(GL_TEXTURE_2D); //disable textures
	glColor3f(0.1f, 0.1f, 0.1f); // Shadow's colour
	/////////////////////////////////////////////////////////shadows render
	glPushMatrix(); //push the matrix for the shadow
	float cordArray[12] = { 15, -0.32, -5, 15, -0.32, 5, 25, -0.32, 5, 25, -0.32, -5 }; //create fake plane coordinates for where the shadow is going to be rendered
	shadow1.generateShadowMatrix(shadowMatrix, Light_Position1, cordArray); //call the shadow matrix function 
	glMultMatrixf((GLfloat *)shadowMatrix); //run the shadow code
	//translate to floor and draw shadow, remember to match any transforms on the object
	glTranslatef(20.0f, -0.32f, 0.0f); //translate
	glScalef(3.0f, 3.0f, 3.0f); //scale
	glRotatef(rotation, 1, 0.5, -1); //rotate
	model.render(); //render model's shadow
	glPopMatrix(); //pop matrix

	glColor3f(1.0f, 1.0f, 1.0f); // Reset Colour
	glEnable(GL_DEPTH_TEST); //enable depth testing
	glEnable(GL_LIGHTING); //enable lighting
	glEnable(GL_TEXTURE_2D); //enable textures

	//////////////////////////////////////////model render
	glPushMatrix(); //push matrix
	glEnable(GL_LIGHT1); //enable light1
	glTranslatef(20.0f, 2.5f, 0.0f); //translate
	glScalef(3.0f, 3.0f, 3.0f); //scale
	glRotatef(rotation, 1, 0.5, -1); //rotate
	model.render(); //render model
	glDisable(GL_LIGHT1); //disable light1
	glPopMatrix(); //pop matrix
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////Setup reflection

glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);//turn off writing to the frame buffer
glEnable(GL_STENCIL_TEST);//enable the stencil test
glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
glStencilFunc(GL_ALWAYS, 1, 1); //set the stencil function to always pass
glDisable(GL_DEPTH_TEST);//disable depth testing

glPushMatrix(); //push the matrix
glTranslatef(15, -1, -5); //translate the wall behind the second platform

glEnable(GL_CULL_FACE); //enable face culling
glCullFace(GL_BACK); //cull the back face
glBegin(GL_QUADS); //beggin drawing the plane

glVertex3f(10, 0, 0);
glVertex3f(10, 10, 0);
glVertex3f(0, 10, 0);
glVertex3f(0, 0, 0);

glEnd(); //end drawing the plane
glDisable(GL_CULL_FACE); //disable face culling
glPopMatrix(); //pop the matrix

glEnable(GL_DEPTH_TEST); //enables the depth test
glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //turn on rendering to the frame buffer
glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);//set the stencil to keep all values
glStencilFunc(GL_EQUAL, 1, 1); //set stencil function to test if the value is 1
glEnable(GL_LIGHT3); //enable light3

glPushMatrix(); //push the matrix
glScalef(1.0, 1.0, -1.0);//flips the scale on z axis
glTranslatef(20.0f, 3.0f, 10.0f); //translate
glScalef(3.0f, 3.0f, 3.0f); //scale
glRotatef(rotation, 1, 0.5, -1); //rotate
model.render(); //render the reflection
glPopMatrix(); //pop the matrix

glPushMatrix(); //push matrix
glColor3f(0.0f, 1.0f, 0.0f); //set the colour to green
glScalef(1.0, 1.0, -1.0); //flip scale
glTranslatef(0, 0, 10); //translate
glEnable(GL_COLOR_MATERIAL); //enable color material

glPushMatrix(); //push matrix
glTranslatef(20.0f, 0.0f, 0.0f); //translate
glRotatef(180, 1.0f, 0.0f, 0.0f); //rotate
glRotatef(90, 0.0f, 1.0f, 0.0f); //rotate
platform2.render(); //render platform reflection
glPopMatrix(); //pop matrix

glPushMatrix(); //push matrix
glTranslatef(20.0f, -0.32f, 0.0f); //translate
glRotatef(-90, 1.0f, 0.0f, 0.0f); //rotate
platform2top.render1(); //render top of the platforms reflection
glPopMatrix(); //pop matrix

glDisable(GL_COLOR_MATERIAL); //color material
glPopMatrix(); //pop matrix
glDisable(GL_LIGHT3); //disable light3
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// render reflection shadow
glDisable(GL_DEPTH_TEST); //disable depth test
glDisable(GL_LIGHTING); //disable lighting
glDisable(GL_TEXTURE_2D); //disable texture
glColor3f(0.1f, 0.1f, 0.1f); // Shadow's colour
glPushMatrix(); //push matrix


glMultMatrixf((GLfloat *)shadowMatrix); //run shadow code
//translate to floor and draw shadow, remember to match any transforms on the object
glTranslatef(20.0f, -0.32f, 0.0f); //translate
glScalef(1.0, 1.0, -1.0); //flip scale 
glTranslatef(0, 0, 10); //translate
glScalef(3.0f, 3.0f, 3.0f); //scale
glRotatef(rotation, 1, 0.5, -1); //rotate

model.render(); //render shadow
glPopMatrix(); //pop matrix
glColor3f(1.0f, 1.0f, 1.0f); // reset colour
glEnable(GL_DEPTH_TEST); //enable depth testing
glEnable(GL_LIGHTING); //enable lighting
glEnable(GL_TEXTURE_2D); //enable textures
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

glDisable(GL_STENCIL_TEST); //disable stencil testing
glEnable(GL_BLEND); //enable blending
glDisable(GL_LIGHTING); //disable lighting
glColor4f(0.3f, 0.25f, 0.55f, 0.2f); //set the colour for the mirror

glPushMatrix(); //push the matrix
glTranslatef(15, -1, -5); //translate the mirror behind platform 2
glBegin(GL_QUADS); //draw the mirror

glVertex3f(10, 0, 0);
glVertex3f(10, 10, 0);
glVertex3f(0, 10, 0);
glVertex3f(0, 0, 0);

glEnd(); //end of draw
glPopMatrix(); //pop the matrix
glEnable(GL_LIGHTING); //enable lighting
glDisable(GL_BLEND); //enable blending

	// End render geometry --------------------------------------
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	// Render text, should be last object rendered.
	renderTextOutput();
	
	// Swap buffers, after all objects are rendered.
	glutSwapBuffers();

}

void Scene::loadTexture(char* filename)
{
	GLuint texture = SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y // Depending on texture file type some need inverted others don't.
	);

	//check for an error during the load process
	if (texture == 0)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}
	m_texture = texture;
}
// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h) 
{
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h;
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 100.0f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fov, ratio, nearPlane, farPlane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

	
}

// Calculates FPS
void Scene::calculateFPS()
{

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}



// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());

	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(1, 1, 1);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float)width/(float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}
