#include "GLLoader.h"

#include "GLSharedTypes.h"
//#include <fstream>
//#include <string>
#include <sstream>
//using namespace std;

//#include "GLShape.h"

//#include "Math.h"
//using namespace Math;

bool operator==( const VertexTN& vertexA, const VertexTN& vertexB )
{
	if ( vertexA.position != vertexB.position )	return false;
	if ( vertexA.normal != vertexB.normal )		return false;
	if ( vertexA.texCoord != vertexB.texCoord )	return false;
	return true;
}



#if 0



// Loads a .obj file while assuming that it has textures and normals.
void GLLoader::LoadObject( const char * filePath )//, VertexTN** vertices, UINT* vSize, USHORT** indices, UINT* iSize )
{
	int i = 0;
	while (i < 10000);
	//ifstream ifs( filePath );
	//if ( ifs.fail( ) ) MessageBox( NULL, "Failed to open file.", NULL, MB_OK );
//	std::stringstream strstr;
	//ss << filePath;
	//*iSize = ss.tellg();

	ss << ifs.rdbuf( );

	string word;
	Vector2 vec2;
	Vector3 vec3;

	vector<Vector3> positions;
	vector<Vector3> normals;
	vector<Vector2> texCoords;

	vector<Vector3> combinations;

	Vector3 combination; // position, uv, normal
	stringstream ssface;
	stringstream sscomb;

	while ( !ss.eof( ) )
	{
		ss >> word;
		if ( word == "v" )
		{
			for ( int i = 0; i < 3; i++ )
				ss >> vec3.elm[i];
			positions.push_back( vec3 );
		}
		else if ( word == "vn" )
		{
			for ( int i = 0; i < 3; i++ )
				ss >> vec3.elm[i];
			normals.push_back( vec3 );
		}
		else if ( word == "vt" )
		{
			for ( int i = 0; i < 2; i++ )
				ss >> vec2.elm[i];
			//vec2.y = 1.0f - vec2.y;  // Invert v coord
			texCoords.push_back( vec2 );
		}
		else if ( word == "f" )
		{
			std::getline( ss, word, '\n' );
			ssface.clear( );
			ssface.str( word );
			std::getline( ssface, word, ' ' ); //get rid of first space

			int el = 0;
			while ( !ssface.eof( ) )
			{
				std::getline( ssface, word, ' ' );
				if ( word == "" ) break;

				sscomb.clear( );
				sscomb.str( word );

				int index = 0;
				std::getline( sscomb, word, '/' );
				combination.elm[index++] = atoi( word.c_str( ) );
				std::getline( sscomb, word, '/' );
				combination.elm[index++] = atoi( word.c_str( ) );
				std::getline( sscomb, word, '/' );
				combination.elm[index++] = atoi( word.c_str( ) );

				combinations.push_back( combination );
				if ( ++el == 4 ) //quad face
				{
					//form a new triangle with the previous vertex indices
					auto last = combinations.size( ) - 1;
					combinations.push_back( combinations[last - 3] );
					combinations.push_back( combinations[last - 1] );
				}
			}
		}
	}

	VertexTN vertex = { };
	USHORT index;

	USHORT indexCounter = 0;

	vector<VertexTN> vertexVector;
	vector<USHORT> indexVector;

	for ( UINT i = 0; i < combinations.size( ); i++ )
	{
		int index = 0;
		// Create a vertex by combining the indices (conversion is made from 1-based to 0-based)

		vertex.position = positions[combinations[i].elm[index++] - 1];
		vertex.texCoord = texCoords[combinations[i].elm[index++] - 1];
		vertex.normal	= normals[combinations[i].elm[index++] - 1];

		auto it = find( vertexVector.begin( ), vertexVector.end( ), vertex );
		if ( it == vertexVector.end( ) )
		{
			vertexVector.push_back( vertex );
			indexVector.push_back( indexCounter++ );
		}
		else
		{
			index = USHORT( it - vertexVector.begin( ) );
			indexVector.push_back( index );
		}
	}

	*vertices = new VertexTN[vertexVector.size()];
	*indices = new USHORT[indexVector.size()];

	memcpy(*vertices, &vertexVector[0], sizeof(VertexTN)*vertexVector.size() );
	memcpy(*indices, &indexVector[0],	sizeof(USHORT)*indexVector.size() );
}




bool GLLoader::LoadTexture( const char* szFileName, IMAGE_FORMAT format, GLenum filter, GLenum wrap )
{
	GLbyte* pBits;
	GLint nWidth, nHeight, iComponents;
	GLenum eFormat;
	bool success = true;

	switch ( format )
	{
	case BMP_FORMAT:
	pBits = gltReadBMPBits( szFileName, &nWidth, &nHeight );
	iComponents = GL_RGB;
	eFormat = GL_BGR;
	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
	break;
	case TGA_FORMAT:
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	pBits = gltReadTGABits( szFileName, &nWidth, &nHeight, &iComponents, &eFormat );
	break;
	default:
	success = false;
	break;
	}

	if ( pBits == NULL )
		success = false;

	if ( !success )
	{
		MessageBox( NULL, "Can't load texture.", szFileName, MB_OK );
		return false;
	}

	glTexImage2D( GL_TEXTURE_2D, 0, iComponents, nWidth, nHeight, 0,
				  eFormat, GL_UNSIGNED_BYTE, pBits );

	free( pBits );


	if ( filter != GL_LINEAR && filter != GL_NEAREST )
		glGenerateMipmap( GL_TEXTURE_2D );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap );




	return true;
}

#endif