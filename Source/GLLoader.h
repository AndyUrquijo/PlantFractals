#pragma once

#include "GLSharedTypes.h"

enum IMAGE_FORMAT { BMP_FORMAT, TGA_FORMAT };

struct GLLoader
{
	static void LoadObject( const char* filePath, VertexTN** vertices, uint* vSize, ushort** indices, uint* iSize ) {}
	static bool LoadTexture( const char* szFileName, IMAGE_FORMAT format, GLenum filter, GLenum wrap ) { return false; }
};