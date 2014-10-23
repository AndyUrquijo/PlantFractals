#pragma once

#include "Plant.h"
#include "GLShader.h"

class PlantSystem
{

	float timeVal;
	std::vector<Plant> plantArray;
	GLShader updateShader; 
	GLShader drawShader; 
	GLShader drawLeavesShader; 
	
	public:

	std::vector<PlantVertex> vertexData;
	std::vector<int> parentIndexData;


	GLuint	staticDataBuffer;
	GLuint	dynamicDataBuffer;
	GLuint	dataBufferSize;
	
	GLuint	parentIndexBuffer;

	void Initialize( );
	void Update( );
	void Render();

};

