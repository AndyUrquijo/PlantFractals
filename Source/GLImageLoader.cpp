#include "GLImageLoader.h"

#include <fstream>
using namespace std;

#include "WinApp.h"

#undef LoadImage

GLint LoadBMPImage( const char* file );

GLint GLImageLoader::LoadImage( const char* file, GLImageFormat format )
{
	switch ( format )
	{
	case IMAGE_FORMAT_BMP:
		return LoadBMPImage( file );
	default:
		return -1;
	}
}

GLint LoadBMPImage( const char* file )
{

	// Data read from the header of the BMP file
	unsigned char header[54]; // 54-bytes header
	unsigned int dataPos;     // Position of the image data
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;

	ifstream ifs( file, ios_base::in | ios_base::binary );
	if ( !ifs.is_open( ) ) WinApp::ShowErrorMessage( L"Could not open the bmp file." );

	ifs.read( (char*) header, 54 );

	if ( ifs.eof( ) )
		WinApp::ShowErrorMessage( L"There was a problem reading the file." );

	if ( header[0] != 'B' || header[1] != 'M' )
		WinApp::ShowErrorMessage( L"There was a problem reading the file." );

	// Read ints from the byte array
	dataPos = *(int*) &( header[0x0A] );
	imageSize = *(int*) &( header[0x22] );
	width = *(int*) &( header[0x12] );
	height = *(int*) &( header[0x16] );

	// Some BMP files are misformatted, guess missing information
	if ( imageSize == 0 )    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if ( dataPos == 0 )      dataPos = 54; // The BMP header is done that way


	// Create a buffer
	data = new unsigned char[imageSize];

	ifs.read( (char*) data, imageSize );
	ifs.close( );

	GLuint texture;
	glGenTextures( 1, &texture );

	glBindTexture( GL_TEXTURE_2D, texture );

	// Give the image to OpenGL
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	delete data;

	return texture;

}
