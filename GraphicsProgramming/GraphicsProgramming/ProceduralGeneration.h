#pragma once
#define _USE_MATH_DEFINES
#include <glut.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>
#include <vector>
#include "Vector3.h"
#include "SOIL.h"
using namespace std;
class ProceduralGeneration
{
public:
	ProceduralGeneration();
	~ProceduralGeneration();

	void generateSphere(int r, int seg, char* filename);
	void generateDisc(int r, int seg);
	void generateHalfSphere(int r, int seg, char* filename);
	void render();
	void render1();
private:
	void loadTexture(char* filename);
	float x, y, z;
	float radius, theta, delta;
	int segments;
	vector<float> vertex, normals, texCoords;
	int m_vertexCount;
	GLuint m_texture;
};

