/*
.	Author:		Andres Urquijo
.	Filename:	PlantSystem.h
*/
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

	GLuint trunkTex;

	public:

	std::vector<PlantVertex> vertexData;
	std::vector<int> parentIndexData;


	GLuint	staticDataBuffer;
	GLuint	dynamicDataBuffer;
	GLuint	translatedDataBuffer;
	GLuint	dataBufferSize;
	
	GLuint	parentIndexBuffer;

	void Initialize( );
	void Update( );
	void Render();

	private:

	void InitializeShaders();
	void InitializePlants();
	void InitializeBuffers();
};

