#pragma once

#include "GLShape.h"
#include "GLShader.h"

#include <string>
using std::wstring;

#include <vector>
using std::vector;

#include "Math.h"
using namespace Math;

#define NODRAWTEXT //Prevents redefinition of DrawText

class GLText
{
#define NUM_SYMBOLS 256
	char		symbolOffset;
	Short2		imageSize;
	Short2		cellSize;
	char		widths[NUM_SYMBOLS];
	GLuint		texture;

	GLShape shape;
	vector<Vector2> positions;
	vector<Vector2> texCoords;

	public:
	GLText( );
	~GLText( );

	void AddText( wstring text, Vector2 position );
	void UpdateText( );
	void DrawText( bool update = true );
	void LoadFont( const char* file );

	private:
	static GLShader textShader;

	public:
	static void InitializeText( );

};
