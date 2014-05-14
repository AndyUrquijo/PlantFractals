#include "GLText.h"
#include "GLImageLoader.h"
using namespace GLImageLoader;
#include "GLFactory.h"
using namespace GLFactory;

#include <fstream>
using namespace std;

#include "WinApp.h"


GLText::GLText( )
{
	shape.primitiveType = GL_TRIANGLE_STRIP;
}

GLText::~GLText( )
{
}

void GLText::ClearText( )
{
	positions.clear( );
	texCoords.clear( );
}
void GLText::AddText( wstring text, Vector2 position )
{
	float dx = 0.1f;
	float dy = 0.1f;
	Vector2 dY = { 0, dy };

	positions.push_back( position );
	texCoords.push_back( { 0, 0 } );

	Vector2 factor;
	factor.x = (float) cellSize.x / imageSize.x;
	factor.y = (float) cellSize.y / imageSize.y;
	int numCols = imageSize.x / cellSize.x;



	int i = 0;
	while ( text[i] )
	{
		char charval = text[i] - symbolOffset;
		Vector2 texOffset;
		texOffset.x = ( charval % numCols )* factor.x;
		texOffset.y = ( charval / numCols )* factor.y;
		float widthRatio = (float) widths[text[i]] / cellSize.x;

		positions.push_back( position );
		positions.push_back( position - dY );
		texCoords.push_back( { texOffset.x, texOffset.y } );
		texCoords.push_back( { texOffset.x, texOffset.y + factor.y } );

		position.x += dx * widthRatio;

		positions.push_back( position );
		positions.push_back( position - dY );
		texCoords.push_back( { texOffset.x + factor.x*widthRatio, texOffset.y } );
		texCoords.push_back( { texOffset.x + factor.x*widthRatio, texOffset.y + factor.y } );


		i++;
	}

	positions.push_back( position + dY );
	positions.push_back( position + dY );
	texCoords.push_back( { 0, 0 } );
	texCoords.push_back( { 0, 0 } );


}

void GLText::UpdateText( )
{
	glBindVertexArray( shape.vertexArray );
	shape.vertexCount = (uint)positions.size( );
	CreateVertexBuffer( VERTEX_POSITION, 2, GL_FLOAT, &positions[0], sizeof(Vector2) * shape.vertexCount );
	CreateVertexBuffer( VERTEX_TEXTURE, 2, GL_FLOAT, &texCoords[0], sizeof(Vector2) * shape.vertexCount );
}

void GLText::DrawText( bool update )
{
	textShader.Use( );
	//glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	//glBlendFunc(GL_ONE, GL_SRC_ALPHA );
	glEnable( GL_BLEND );
	glBindTexture( GL_TEXTURE_2D, texture );
	if ( update ) UpdateText( );
	shape.Draw( );
}



void GLText::LoadFont( const char* file )
{
# pragma pack (1)
	struct {
		ushort	signature;
		int		imageWidth;
		int		imageHeight;
		int		cellWidth;
		int		cellHeight;
		char	bpp;// bits per pixel
		char	symbolOffset;
		char	widths[NUM_SYMBOLS];
	} header;
# pragma pack ()

	ifstream ifs( file, ios_base::in | ios_base::binary );
	if ( ifs.fail( ) ) WinApp::ShowErrorMessage( L"The font file could not be opened." );


	ifs.seekg( 0, ios_base::end );
	size_t imageDataSize = (size_t) ifs.tellg( ) - sizeof header;
	ifs.seekg( 0, ios_base::beg );
	char* imageData = new char[imageDataSize];



	ifs.read( (char*) &header, sizeof header );
	ifs.read( (char*) imageData, imageDataSize );
	ifs.close( );



	// Check ID is 'BFF2'
	if ( header.signature != 0xF2BF )
		WinApp::ShowErrorMessage( L"The font file does not have the right format." );

	// Check pixel format
	if ( header.bpp != 8 )
		WinApp::ShowErrorMessage( L"The font file has an unsupported pixel format." );

	// Check filesize
	if ( imageDataSize != header.imageHeight*header.imageWidth )
		WinApp::ShowErrorMessage( L"The font file is incorrectly formated or may be corrupted." );



	// Grab the rest of the header
	imageSize.x = header.imageWidth;
	imageSize.y = header.imageHeight;
	cellSize.x = header.cellWidth;
	cellSize.y = header.cellHeight;
	symbolOffset = header.symbolOffset;
	memcpy( widths, header.widths, NUM_SYMBOLS );



	// Create Texture
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	// Fonts should be rendered at native resolution so no need for texture filtering
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	// Stop chararcters from bleeding over edges
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, header.imageWidth, header.imageHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imageData );



	// Clean up
	delete[] imageData;
}


// Static members

GLShader GLText::textShader;

void GLText::InitializeText( )
{
	textShader.CreateProgram( );
	textShader.LoadShader( "Shaders/Text.vp", GL_VERTEX_SHADER );
	textShader.LoadShader( "Shaders/Text.fp", GL_FRAGMENT_SHADER );
	textShader.BindAttribute( VERTEX_POSITION, "_position" );
	textShader.BindAttribute( VERTEX_TEXTURE, "_texCoord" );
	textShader.CompileProgram( );
}