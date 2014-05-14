#include "MatrixStack.h"


MatrixStack::MatrixStack( )
{
	current = Matrix44::MakeIdentity();
}


MatrixStack::~MatrixStack( )
{
}

void MatrixStack::PushMatrix( )
{
	matrices.push( current );
}

void MatrixStack::PopMatrix( )
{
	current = matrices.top();
	matrices.pop();
}

void MatrixStack::MultMatrix( const Matrix44& matrix )
{
	//Matrix44 back = matrices.back();
	//back *= matrix;
	//
	//matrix.pop();
	//matrix.push(back);
	current *= matrix;
}

Matrix44 MatrixStack::GetMatrix( ) const
{
	return current;
}
