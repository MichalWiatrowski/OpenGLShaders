#include "ProceduralGeneration.h"



ProceduralGeneration::ProceduralGeneration()
{
}


ProceduralGeneration::~ProceduralGeneration()
{
}



void ProceduralGeneration::render()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

//	glBindTexture(GL_TEXTURE_2D, m_texture);
	glVertexPointer(3, GL_FLOAT, 0, vertex.data());
	glNormalPointer(GL_FLOAT, 0, normals.data());
//	glTexCoordPointer(2, GL_FLOAT, 0, texCoords.data());

	glDrawArrays(GL_QUADS, 0, m_vertexCount);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//	glBindTexture(GL_TEXTURE_2D, NULL);
}
void ProceduralGeneration::render1()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//	glBindTexture(GL_TEXTURE_2D, m_texture);
	glVertexPointer(3, GL_FLOAT, 0, vertex.data());
	glNormalPointer(GL_FLOAT, 0, normals.data());
	//	glTexCoordPointer(2, GL_FLOAT, 0, texCoords.data());

	glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//	glBindTexture(GL_TEXTURE_2D, NULL);
}
void ProceduralGeneration::loadTexture(char* filename)
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

void ProceduralGeneration::generateSphere(int r, int seg, char* filename)
{

	segments = seg; //number of segemnts
	radius = r; //radius of the sphere 
	theta =  (2 * M_PI) / segments; //calculate theta
	delta = M_PI / segments; //calculate delta
	
	//intialise variables
	float angle_long = 0.0f;
	float angle_lati = 0.0f;
	float next_long = 0.0f;
	float next_lati = 0.0f;

	for (int longitude = 0; longitude < (segments + 1); longitude++)
	{
		
		for (int latitude = 0; latitude < (segments + 1); latitude++)
		{

			next_lati = angle_lati + theta; // calculate  next angle by adding theta to current angle
			vertex.push_back(radius * cos(angle_lati) * sin(angle_long)); vertex.push_back(radius * cos(angle_long)); vertex.push_back(radius * sin(angle_lati) * sin(angle_long));
			normals.push_back(radius * cos(angle_lati) * sin(angle_long)); normals.push_back(radius * cos(angle_long)); normals.push_back(radius * sin(angle_lati) * sin(angle_long));
			//texCoords.push_back(0.0f); texCoords.push_back(0.0f);


			vertex.push_back(radius * cos(next_lati) * sin(angle_long)); vertex.push_back(radius * cos(angle_long)); vertex.push_back(radius * sin(next_lati) * sin(angle_long));
			normals.push_back(radius * cos(next_lati) * sin(angle_long)); normals.push_back(radius * cos(angle_long)); normals.push_back(radius * sin(next_lati) * sin(angle_long));
			//texCoords.push_back(1.0f); texCoords.push_back(0.0f);

			vertex.push_back(radius * cos(next_lati) * sin(next_long)); vertex.push_back(radius * cos(next_long)); vertex.push_back(radius * sin(next_lati) * sin(next_long));
			normals.push_back(radius * cos(next_lati) * sin(next_long)); normals.push_back(radius * cos(next_long)); normals.push_back(radius * sin(next_lati) * sin(next_long));
			//texCoords.push_back(1.0f); texCoords.push_back(1.0f);

			vertex.push_back(radius * cos(angle_lati) * sin(next_long)); vertex.push_back(radius * cos(next_long)); vertex.push_back(radius * sin(angle_lati) * sin(next_long));
			normals.push_back(radius * cos(angle_lati) * sin(next_long)); normals.push_back(radius * cos(next_long)); normals.push_back(radius * sin(angle_lati) * sin(next_long));
			//texCoords.push_back(0.0f); texCoords.push_back(1.0f);
			
			angle_lati = next_lati; //calculates current angle by setting it to next angle (next angle is going to be current in the next iteration of the loop)

		}
		//increments current angle by setting it to the next angle
		//and then increments next angle by adding current angle with delta that
		//was calculated above
		angle_long = next_long; 
		next_long = angle_long + delta;
	}
	m_vertexCount = vertex.size() / 3; //calculate the amount of vertex for vertex arrays

	//loadTexture(filename);
	
}


void ProceduralGeneration::generateHalfSphere(int r, int seg, char * filename) 
{
	segments = seg; //number of segemnts
	radius = r;//radius of the half sphere 
	theta = (2 * M_PI) / segments;//calculate theta
	delta = M_PI / segments;//calculate delta
	
	//intialise variables
	float angle_long = 0.0f;
	float angle_lati = 0.0f;
	float next_long = 0.0f;
	float next_lati = 0.0f;

	for (int longitude = 0; longitude < (segments + 1) / 2; longitude++) //generates a half sphere by dividing the longitude by 2
	{

		for (int latitude = 0; latitude < (segments + 1) ; latitude++)
		{

			next_lati = angle_lati + theta;// calculate  next angle by adding theta to current angle
			vertex.push_back(radius * cos(angle_lati) * sin(angle_long)); vertex.push_back(radius * cos(angle_long)); vertex.push_back(radius * sin(angle_lati) * sin(angle_long));
			normals.push_back(radius * cos(angle_lati) * sin(angle_long)); normals.push_back(radius * cos(angle_long)); normals.push_back(radius * sin(angle_lati) * sin(angle_long));
			texCoords.push_back(0.0f); texCoords.push_back(0.0f);


			vertex.push_back(radius * cos(next_lati) * sin(angle_long)); vertex.push_back(radius * cos(angle_long)); vertex.push_back(radius * sin(next_lati) * sin(angle_long));
			normals.push_back(radius * cos(next_lati) * sin(angle_long)); normals.push_back(radius * cos(angle_long)); normals.push_back(radius * sin(next_lati) * sin(angle_long));
			texCoords.push_back(1.0f); texCoords.push_back(0.0f);

			vertex.push_back(radius * cos(next_lati) * sin(next_long)); vertex.push_back(radius * cos(next_long)); vertex.push_back(radius * sin(next_lati) * sin(next_long));
			normals.push_back(radius * cos(next_lati) * sin(next_long)); normals.push_back(radius * cos(next_long)); normals.push_back(radius * sin(next_lati) * sin(next_long));
			texCoords.push_back(1.0f); texCoords.push_back(1.0f);

			vertex.push_back(radius * cos(angle_lati) * sin(next_long)); vertex.push_back(radius * cos(next_long)); vertex.push_back(radius * sin(angle_lati) * sin(next_long));
			normals.push_back(radius * cos(angle_lati) * sin(next_long)); normals.push_back(radius * cos(next_long)); normals.push_back(radius * sin(angle_lati) * sin(next_long));
			texCoords.push_back(0.0f); texCoords.push_back(1.0f);


			angle_lati = next_lati;//calculates current angle by setting it to next angle (next angle is going to be current in the next iteration of the loop)

		}
		//increments current angle by setting it to the next angle
		//and then increments next angle by adding current angle with delta that
		//was calculated above
		angle_long = next_long;
		next_long = angle_long + delta;
	}
	m_vertexCount = vertex.size() / 3; //calculate the amount of vertex for vertex arrays

	//loadTexture(filename);
}

void ProceduralGeneration::generateDisc(int r, int seg)
{
	theta = 0; //set theta to 0
	radius = r;//radius of the disc
	segments = seg; //number of segments
	
	float interval = (2 * M_PI) / segments; //calculate the interval
	for (int i = 0; i < segments; i++) //loop for the amount of segments
	{
		vertex.push_back(0); vertex.push_back(0); vertex.push_back(0);//middle of the disc, also first vertex
		normals.push_back(0); normals.push_back(0); normals.push_back(1); //normals for the first vertex
		//calculate x and y for the next vertex
		x = radius * cos(theta);
		y = radius * sin(theta);
		//push the next vertex and its normals
		vertex.push_back(x); vertex.push_back(y); vertex.push_back(0);
		normals.push_back(0); normals.push_back(0); normals.push_back(1);
		//calculate x and y for the next vertex
		x = radius * cos(theta + interval);
		y = radius * sin(theta + interval);
		//push the next vertex and its normals
		vertex.push_back(x); vertex.push_back(y); vertex.push_back(0);
		normals.push_back(0); normals.push_back(0); normals.push_back(1);
		
		
		theta += interval; //increment theta
	}
	m_vertexCount = vertex.size() / 3; //calculate the amount of vertex for vertex arrays
}




