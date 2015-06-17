#version 430 core

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;


layout(std430, binding = 0) coherent buffer vertexBuffer
{
    vec4 colors[];
};

const ivec2 size = ivec2(31,31);

bool OutOfBounds( ivec2 index )
{
	return index.x >= size.x || index.y >= size.y || index.x < 0 || index.y < 0;
}

int GetIndex( ivec2 index )
{
	return index.x + index.y*size.x;
}

const vec4 colorSeq[3] = 
{
vec4(1,0,0,1),
vec4(0,1,0,1),
vec4(0,0,1,1),
};

void main()
{
	ivec2 currIndex = ivec2(gl_GlobalInvocationID.xy);
	int currID = GetIndex( currIndex );
	bool black = false;

	
	//for( int i = -1; i <= 1; i++)
	//{
	//	for( int j = -1; j <= 1; j++)
	//	{
	//		ivec2 index = ivec2( currIndex.x + i, currIndex.y + j);
	//		int neigId = GetIndex( index );
	//		if( !OutOfBounds(index) )
	//		{
	//			if( colors[neigId].r == 1 )
	//				colors[currID].ba = vec2(1,1); 
	//		}
	//	}
	//}

	ivec2 prevIndex = currIndex;
	prevIndex.x = currIndex.x -1;
	int prevID = GetIndex(prevIndex);
	if( currIndex.x == 0 )
		colors[currID] = colorSeq[0]; 
	int i = 0;
	for(; i < 3; i++ )
	{
		if(colors[prevID] == colorSeq[i] )
		{
			break;
		}
	}
	if( i == 3 )
		colors[currID] = vec4(1,1,1,1);
	else
	{
		colors[currID] = colorSeq[(i+1)%3];
		memoryBarrier();
	}
	

}